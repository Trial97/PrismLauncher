// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (c) 2026 Trial97 <alexandru.tripon97@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "VersionRange.h"

#include <QRegularExpression>
#include <QStringList>
#include <cstdint>
#include <vector>
#include "Version.h"

namespace {

std::vector<VersionRange::IsVersionCompatible> parseMavenVersion(QString spec)
{
    spec = spec.trimmed();

    if (spec.isEmpty() || spec == "*" || spec.compare("ANY", Qt::CaseInsensitive) == 0) {
        return { [](const Version&) { return true; } };
    }

    // Maven "soft" requirement: "1.2" means [1.2,)
    if (spec.front().isDigit()) {
        if (spec.contains(',')) {
            return {};
        }

        const Version version(spec);
        return { [version](const Version& v) { return v >= version; } };
    }

    std::vector<VersionRange::IsVersionCompatible> result;

    while (!spec.isEmpty()) {
        const QChar open = spec.front();
        if (open != '[' && open != '(') {
            return {};
        }

        const bool lowerInclusive = open == '[';

        // Find the matching closing delimiter.
        const auto close = [&spec] {
            const auto bracket = spec.indexOf(']');
            const auto paren = spec.indexOf(')');

            if (bracket < 0) {
                return paren;
            }
            if (paren < 0) {
                return bracket;
            }
            return std::min(bracket, paren);
        }();

        if (close <= 0) {
            return {};
        }

        const QChar closeChar = spec.at(close);
        const bool upperInclusive = closeChar == ']';

        const QString body = spec.sliced(1, close - 1).trimmed();
        spec = spec.sliced(close + 1).trimmed();

        const qsizetype comma = body.indexOf(',');

        // [1.0] — exact version
        if (comma < 0) {
            if (!lowerInclusive || !upperInclusive || body.isEmpty()) {
                return {};
            }

            const Version version(body);
            result.emplace_back([version](const Version& v) { return v == version; });
        } else {
            // Maven range: [1.0,2.0), (1.0,), (,2.0]
            const QString lower = body.left(comma).trimmed();
            const QString upper = body.sliced(comma + 1).trimmed();

            if (lower.isEmpty() && upper.isEmpty()) {
                return {};
            }

            std::optional<Version> lowerVersion;
            std::optional<Version> upperVersion;

            if (!lower.isEmpty()) {
                lowerVersion.emplace(lower);
            }

            if (!upper.isEmpty()) {
                upperVersion.emplace(upper);
            }

            result.emplace_back([lowerVersion = std::move(lowerVersion), upperVersion = std::move(upperVersion), lowerInclusive,
                                 upperInclusive](const Version& v) {
                if (lowerVersion) {
                    if (lowerInclusive ? v < *lowerVersion : v <= *lowerVersion) {
                        return false;
                    }
                }

                if (upperVersion) {
                    if (upperInclusive ? v > *upperVersion : v >= *upperVersion) {
                        return false;
                    }
                }

                return true;
            });
        }

        // Multiple ranges are OR-ed: [1.0,2.0],[3.0,4.0]
        if (spec.isEmpty()) {
            break;
        }

        if (!spec.startsWith(',')) {
            return {};
        }

        spec = spec.sliced(1).trimmed();

        if (spec.isEmpty()) {
            return {};
        }
    }

    return result;
}

enum class Op : std::uint8_t { EQ, GE, GT, LE, LT, TILDE, CARET };

// Consumes a leading comparison operator and returns the remaining version text.
Op consumeOperator(QString& str)
{
    auto consume = [&str](const QString& prefix) {
        if (str.startsWith(prefix)) {
            str.remove(0, prefix.size());
            return true;
        }
        return false;
    };

    if (consume(">=")) {
        return Op::GE;
    }
    if (consume("<=")) {
        return Op::LE;
    }
    if (consume(">")) {
        return Op::GT;
    }
    if (consume("<")) {
        return Op::LT;
    }
    if (consume("=")) {
        return Op::EQ;
    }
    if (consume("^")) {
        return Op::CARET;
    }
    if (consume("~")) {
        return Op::TILDE;
    }
    return Op::EQ;
}

// Returns how many leading numeric components are fixed for an X-range, or -1
// if the string is not an X-range (e.g. "1.x", "1.2.x", "1.*").
int countFixedComponents(const QString& str)
{
    auto parts = str.split('.');
    if (parts.size() < 2) {
        return -1;
    }

    QString last = parts.last().trimmed();
    if (last != "x" && last != "X" && last != "-" && last != "*") {
        // a trailing "*" with no dot is also a wildcard, e.g. "1.2*"
        if (!str.endsWith('*')) {
            return -1;
        }
    }

    int fixed = 0;
    for (int i = 0; i < parts.size() - 1; ++i) {
        bool ok = false;
        parts.at(i).toInt(&ok);
        if (!ok) {
            // allow fixed numeric components only
            return -1;
        }
        fixed = i + 1;
    }
    return fixed;
}

std::vector<VersionRange::IsVersionCompatible> parseSemverVersion(QString spec)
{
    spec = spec.trimmed();
    if (spec.isEmpty() || spec == "*" || spec == "ANY") {
        return { [](const Version&) { return true; } };
    }

    // Fabric/Quilt often use ||, &&, or space for multiple ranges:
    //  - "||" between parts is an OR relationship
    //  - "&&" or space within one string is an AND relationship
    if (spec.contains("||")) {
        std::vector<VersionRange::IsVersionCompatible> result;
        for (const auto& part : spec.split("||")) {
            auto rules = parseSemverVersion(part);
            for (auto& rule : rules) {
                result.emplace_back(std::move(rule));
            }
        }
        return result;
    }
    if (spec.contains("&&") || spec.contains(' ')) {
        static const QRegularExpression s_splitRegex("&&|\\s+");
        auto parts = spec.split(s_splitRegex, Qt::SkipEmptyParts);
        std::vector<VersionRange::IsVersionCompatible> subRules;
        for (const auto& part : parts) {
            for (auto& rule : parseSemverVersion(part)) {
                subRules.emplace_back(std::move(rule));
            }
        }
        if (subRules.empty()) {
            return {};
        }
        return { [subRules](const Version& v) {
            for (const auto& rule : subRules) {
                if (!rule(v)) {
                    return false;
                }
            }
            return true;
        } };
    }

    QString operandStr = spec;
    const Op op = consumeOperator(operandStr);
    if (operandStr.isEmpty()) {
        return {};
    }
    // X-ranges: "1.x", "1.2.x", "1.*" -> a bound over the fixed prefix.
    if (op == Op::EQ) {
        const int fixed = countFixedComponents(operandStr);
        if (fixed == 1) {
            // "1.x" -> >= 1.0 and < 2.0
            const int major = operandStr.section('.', 0, 0).toInt();
            const Version lower(QString::number(major));
            const Version upper(QString::number(major + 1));
            return { [lower, upper](const Version& v) { return v >= lower && v < upper; } };
        }
        if (fixed > 1) {
            // "1.2.x" -> >= 1.2 and < 1.3
            const QString prefix = operandStr.section('.', 0, fixed - 1);
            QStringList nextParts = prefix.split('.');
            nextParts.last() = QString::number(nextParts.last().toInt() + 1);
            const Version lower(prefix);
            const Version upper(nextParts.join('.'));
            return { [lower, upper](const Version& v) { return v >= lower && v < upper; } };
        }
    }

    const Version operand(operandStr);

    switch (op) {
        case Op::EQ:
            return { [operand](const Version& v) { return v == operand; } };
        case Op::GE:
            return { [operand](const Version& v) { return v >= operand; } };
        case Op::LE:
            return { [operand](const Version& v) { return v <= operand; } };
        case Op::GT:
            return { [operand](const Version& v) { return v > operand; } };
        case Op::LT:
            return { [operand](const Version& v) { return v < operand; } };
        case Op::TILDE: {
            // ~a.b.c -> >= a.b.c and < a.(b+1).0
            const auto parts = operand.toString().split('.');
            if (parts.size() < 2) {
                return { [operand](const Version& v) { return v >= operand; } };
            }
            auto upperParts = parts;
            upperParts[1] = QString::number(upperParts.at(1).toInt() + 1);
            upperParts.remove(2, upperParts.size() - 2);
            upperParts.append(QStringLiteral("0"));
            const Version upper(upperParts.join('.'));
            return { [operand, upper](const Version& v) { return v >= operand && v < upper; } };
        }
        case Op::CARET: {
            // ^a.b.c -> >= a.b.c and < (a+1).0.0
            const auto parts = operand.toString().split('.');
            if (parts.isEmpty()) {
                return { [operand](const Version& v) { return v >= operand; } };
            }
            const Version upper(QStringLiteral("%1.0.0").arg(parts.at(0).toInt() + 1));
            return { [operand, upper](const Version& v) { return v >= operand && v < upper; } };
        }
    }

    return {};
}

}  // namespace

VersionRange VersionRange::fromMaven(QString spec)
{
    auto rules = parseMavenVersion(spec);
    return { std::move(spec), std::move(rules) };
}

VersionRange VersionRange::fromSemver(QString spec)
{
    auto rules = parseSemverVersion(spec);
    return { std::move(spec), std::move(rules) };
}

VersionRange::VersionRange(QString spec, std::vector<IsVersionCompatible> rules) : m_spec(std::move(spec)), m_rules(std::move(rules))
{
    m_isValid = !m_rules.empty();
}

bool VersionRange::contains(const Version& version) const
{
    return m_isValid && std::ranges::any_of(m_rules, [&version](const IsVersionCompatible& rule) { return rule(version); });
}

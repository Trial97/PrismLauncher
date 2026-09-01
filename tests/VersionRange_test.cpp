/* Copyright 2013-2021 MultiMC Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QTest>

#include <VersionRange.h>

class VersionRangeTest : public QObject {
    Q_OBJECT

    void addDataColumns()
    {
        QTest::addColumn<QString>("spec");
        QTest::addColumn<QString>("version");
        QTest::addColumn<bool>("contains");
    }

    void addRow(const char* name, const char* spec, const char* version, bool contains)
    {
        QTest::newRow(name) << QString(spec) << QString(version) << contains;
    }

    void setupMavenRanges()
    {
        addDataColumns();

        // "*" / "ANY" / empty match everything
        addRow("any-star", "*", "1.0", true);
        addRow("any-word", "ANY", "12.34", true);
        addRow("any-empty", "", "1.0", true);

        // soft requirement "1.0" -> >= 1.0
        addRow("soft-equal", "1.0", "1.0", true);
        addRow("soft-above", "1.0", "1.5", true);
        addRow("soft-below", "1.0", "0.9", false);

        // exact "[1.0]"
        addRow("exact-hit", "[1.0]", "1.0", true);
        addRow("exact-below", "[1.0]", "0.9", false);
        addRow("exact-above", "[1.0]", "1.0.1", false);

        // "[1.0,2.0)" inclusive lower, exclusive upper
        addRow("closed-open-boundary", "[1.0,2.0)", "1.0", true);
        addRow("closed-open-upper-excluded", "[1.0,2.0)", "2.0", false);
        addRow("closed-open-inside", "[1.0,2.0)", "1.5", true);
        addRow("closed-open-below", "[1.0,2.0)", "0.5", false);

        // "(1.0,2.0]" exclusive lower, inclusive upper
        addRow("open-closed-lower-excluded", "(1.0,2.0]", "1.0", false);
        addRow("open-closed-upper-boundary", "(1.0,2.0]", "2.0", true);
        addRow("open-closed-inside", "(1.0,2.0]", "1.5", true);

        // "[1.5,)" open-ended lower
        addRow("lower-open-boundary", "[1.5,)", "1.5", true);
        addRow("lower-open-above", "[1.5,)", "2.0", true);
        addRow("lower-open-below", "[1.5,)", "1.4", false);

        // "(,1.0]" open-ended upper
        addRow("upper-open-boundary", "(,1.0]", "1.0", true);
        addRow("upper-open-below", "(,1.0]", "0.5", true);
        addRow("upper-open-above", "(,1.0]", "1.5", false);

        // "(,1.0],[1.2,)" multiple sets -> OR
        addRow("multi-first-set", "(,1.0],[1.2,)", "0.5", true);
        addRow("multi-gap", "(,1.0],[1.2,)", "1.1", false);
        addRow("multi-second-set", "(,1.0],[1.2,)", "2.0", true);

        // "(,1.1),(1.1,)" excludes 1.1
        addRow("exclude-below", "(,1.1),(1.1,)", "1.0", true);
        addRow("exclude-hit", "(,1.1),(1.1,)", "1.1", false);
        addRow("exclude-above", "(,1.1),(1.1,)", "1.2", true);
    }

    void setupSemverRanges()
    {
        addDataColumns();

        // "*" / "ANY" / empty match everything
        addRow("any-star", "*", "1.0", true);
        addRow("any-word", "ANY", "12.34", true);
        addRow("any-empty", "", "1.0", true);

        // bare version = exact match
        addRow("bare-exact", "1.0.0", "1.0.0", true);
        addRow("bare-below", "1.0.0", "0.9.9", false);
        addRow("bare-above", "1.0.0", "1.0.1", false);

        // = >= > <= <
        addRow("eq-hit", "=1.2.3", "1.2.3", true);
        addRow("eq-miss", "=1.2.3", "1.2.4", false);
        addRow("ge-hit", ">=1.0.0", "1.0.0", true);
        addRow("ge-miss", ">=1.0.0", "0.9.0", false);
        addRow("gt-hit", ">1.0.0", "1.0.1", true);
        addRow("gt-miss", ">1.0.0", "1.0.0", false);
        addRow("le-hit", "<=1.0.0", "1.0.0", true);
        addRow("le-miss", "<=1.0.0", "1.0.1", false);
        addRow("lt-hit", "<1.0.0", "0.9.0", true);
        addRow("lt-miss", "<1.0.0", "1.0.0", false);

        // ^ caret: >= a.b.c within same major (< (a+1).0.0)
        addRow("caret-hit", "^1.2.3", "1.2.3", true);
        addRow("caret-inside", "^1.2.3", "1.99.0", true);
        addRow("caret-next-major", "^1.2.3", "2.0.0", false);
        addRow("caret-below", "^1.2.3", "1.2.2", false);

        // ~ tilde: >= a.b.c within same minor (< a.(b+1).0)
        addRow("tilde-hit", "~1.2.3", "1.2.3", true);
        addRow("tilde-inside", "~1.2.3", "1.2.99", true);
        addRow("tilde-next-minor", "~1.2.3", "1.3.0", false);
        addRow("tilde-below", "~1.2.3", "1.1.9", false);

        // X-ranges
        addRow("xrange-major-boundary", "1.x", "1.5.0", true);
        addRow("xrange-major-above", "1.x", "2.0.0", false);
        addRow("xrange-major-below", "1.x", "0.9.0", false);
        addRow("xrange-minor-inside", "1.2.x", "1.2.9", true);
        addRow("xrange-minor-above", "1.2.x", "1.3.0", false);
        addRow("xrange-minor-at-boundary", "1.2.x", "1.2.0", true);
        addRow("xrange-star", "1.*", "1.0.0", true);
        addRow("xrange-star-above", "1.*", "2.0.0", false);
        addRow("xrange-upper-X", "1.X", "1.9.9", true);
    }

   private slots:
    void test_maven_contains_data() { setupMavenRanges(); }

    void test_maven_contains()
    {
        QFETCH(QString, spec);
        QFETCH(QString, version);
        QFETCH(bool, contains);

        const VersionRange range = VersionRange::fromMaven(spec);
        qDebug() << "spec:" << range.toString() << "version:" << version;

        QCOMPARE(range.isValid(), true);
        QCOMPARE(range.contains(Version(version)), contains);
    }

    void test_semver_contains_data() { setupSemverRanges(); }

    void test_semver_contains()
    {
        QFETCH(QString, spec);
        QFETCH(QString, version);
        QFETCH(bool, contains);

        const VersionRange range = VersionRange::fromSemver(spec);
        qDebug() << "spec:" << range.toString() << "version:" << version;

        QCOMPARE(range.isValid(), true);
        QCOMPARE(range.contains(Version(version)), contains);
    }
};

QTEST_GUILESS_MAIN(VersionRangeTest)

#include "VersionRange_test.moc"

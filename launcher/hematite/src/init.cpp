#include <QtGlobal>

// For versions less than Qt 6 we need to manually register the std numerics
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#include <QtCore/QMetaType>

#include <cstdint>
#include <mutex>

extern "C" bool
init_hematite_core()
{
  static std::once_flag flag;
  std::call_once(flag, []() {
    // If we are using Qt 5 then register std numbers as a type for use in QML.
    //
    // See also:
    // https://github.com/rust-lang/rust/issues/108081
    // https://github.com/KDAB/cxx-qt/pull/598
    
    // call things like qRegisterMetaType<T>();

  });

  return true;
}

#else

extern "C" bool
init_hematite_core()
{
  // Only needed for Qt5
  return true;
}

#endif

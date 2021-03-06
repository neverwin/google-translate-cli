set(CROSS_COMPILE_TOOLCHAIN_PATH "/usr/bin/")
set(CROSS_COMPILE_TOOLCHAIN_PREFIX "i686-w64-mingw32")
set(CROSS_COMPILE_SYSROOT "/usr/i686-w64-mingw32/")

SET(CMAKE_SYSTEM_NAME Windows)

SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER i686-mingw32-windres)
SET(CMAKE_CXX_LINK_EXECUTABLE  i686-w64-mingw32-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_EXE_LINKER_FLAGS_INIT "-static")
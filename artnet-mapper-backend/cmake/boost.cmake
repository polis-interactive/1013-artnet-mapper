
if (AM_PLATFORM STREQUAL EMBEDDED_LINUX)
    set(BOOST_ROOT "/home/polis/build/boost_1_81_0")
elseif (AM_PLATFORM STREQUAL EMBEDDED_LINUX_CC)
    set(BOOST_ROOT "${TARGET_ROOTFS}/home/polis/build/boost_1_81_0")
elseif (AM_PLATFORM STREQUAL X86_LINUX)
    set(BOOST_ROOT "/usr/local/lib/boost_1_81_0")
endif ()

set(BOOST_INCLUDEDIR  "${BOOST_ROOT}")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
set(Boost_Version 1.81.0)

set(Boost_NO_WARN_NEW_VERSIONS 1)
find_package(Boost ${Boost_Version} REQUIRED COMPONENTS system regex)
include_directories(${BOOST_INCLUDEDIR})
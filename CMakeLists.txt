cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(illex VERSION 0.1.1 LANGUAGES CXX)

find_package(Arrow 3.0.0 CONFIG REQUIRED)

include(FetchContent)

FetchContent_Declare(cmake-modules
  GIT_REPOSITORY  https://github.com/abs-tudelft/cmake-modules.git
  GIT_TAG         master
)
FetchContent_MakeAvailable(cmake-modules)
include(CompileUnits)

# rapidjson
FetchContent_Declare(rapidjson
  GIT_REPOSITORY  https://github.com/Tencent/rapidjson.git
  GIT_TAG         v1.1.0
)
FetchContent_GetProperties(rapidjson)
if (NOT rapidjson_POPULATED)
  FetchContent_Populate(rapidjson)
  include_directories("${rapidjson_SOURCE_DIR}/include")
endif ()

# CLI11
FetchContent_Declare(CLI11
  GIT_REPOSITORY  https://github.com/CLIUtils/CLI11.git
  GIT_TAG         v1.9.1
)
FetchContent_MakeAvailable(CLI11)

# spdlog
FetchContent_Declare(spdlog
  GIT_REPOSITORY  https://github.com/gabime/spdlog
  GIT_TAG         v1.7.0
)
FetchContent_MakeAvailable(spdlog)

# concurrentqueue
FetchContent_Declare(concurrentqueue
  GIT_REPOSITORY  https://github.com/cameron314/concurrentqueue.git
  GIT_TAG         v1.0.2
)
FetchContent_GetProperties(concurrentqueue)
if (NOT concurrentqueue_POPULATED)
  FetchContent_Populate(concurrentqueue)
endif ()
include_directories("${concurrentqueue_SOURCE_DIR}")

# kissnet
FetchContent_Declare(kissnet
  GIT_REPOSITORY  https://github.com/Ybalrid/kissnet
  GIT_TAG         a1c624499315093d99c5bf37f5ceb048c3b671cc
)
FetchContent_MakeAvailable(kissnet)

# putong
FetchContent_Declare(putong
  GIT_REPOSITORY  https://github.com/abs-tudelft/putong.git
  GIT_TAG         4a2bf644f899bb46dd8512edf95235280dc484bc
)
FetchContent_MakeAvailable(putong)

add_compile_unit(
  NAME illex::static
  TYPE OBJECT
  PRPS
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
  SRCS
    src/illex/client_queueing.cpp
    src/illex/client_buffering.cpp
    src/illex/client.cpp
    src/illex/document.cpp
    src/illex/arrow.cpp
    src/illex/value.cpp
  DEPS
    arrow_shared
    kissnet
    putong
    spdlog::spdlog
)

add_compile_unit(
  NAME illex::obj
  TYPE OBJECT
  PRPS
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
  SRCS
    src/illex/cli.cpp
    src/illex/file.cpp
    src/illex/producer.cpp
    src/illex/server.cpp
    src/illex/stream.cpp
  TSTS
    test/illex/test_arrow.cpp
    test/illex/test_gen.cpp
    test/illex/test_client.cpp
    test/illex/test_producer.cpp
    test/illex/test_file.cpp
  DEPS
    kissnet
    illex::static
    arrow_shared
    CLI11::CLI11
    spdlog::spdlog
    putong
)

add_compile_unit(
  NAME illex
  TYPE EXECUTABLE
  PRPS
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
  SRCS
    src/illex/main.cpp
  DEPS
    illex::obj
)

compile_units()

execute_process (
    COMMAND bash -c "awk -F= '/^ID=/{print $2}' /etc/os-release |tr -d '\n' | tr -d '\"'"
    OUTPUT_VARIABLE OS_NAME
)

execute_process (
    COMMAND bash -c "awk -F= '/^VERSION_ID=/{print $2}' /etc/os-release |tr -d '\n' | tr -d '\"'"
    OUTPUT_VARIABLE OS_VERSION
)

if(OS_NAME MATCHES "ubuntu")
  set(CPACK_DEBIAN_PACKAGE_RELEASE "ubuntu${OS_VERSION}")
  set(CPACK_GENERATOR "DEB")
elseif(OS_NAME MATCHES "centos")
  set(CPACK_RPM_PACKAGE_RELEASE_DIST "el${OS_VERSION}")
  if(OS_VERSION MATCHES "7")
    set(CPACK_GENERATOR "RPM;TGZ")
  else()
    set(CPACK_GENERATOR "RPM")
  endif()
endif()

set(CPACK_PACKAGE_VENDOR "Teratide")
set(CPACK_PACKAGE_VERSION_MAJOR "${illex_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${illex_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${illex_VERSION_PATCH}")
set(CPACK_PACKAGE_RELOCATABLE ON)

set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libarrow-dev (>= 3.0.0), libarrow-dev (<< 4.0.0)")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR}")

set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_RPM_FILE_NAME "RPM-DEFAULT")
set(CPACK_RPM_PACKAGE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION}")
set(CPACK_RPM_PACKAGE_LICENSE "ASL 2.0")
set(CPACK_RPM_PACKAGE_REQUIRES "arrow-libs >= 3.0.0, arrow-libs < 4.0.0")

set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_ARCHIVE_FILE_NAME "${CMAKE_PROJECT_NAME}-${illex_VERSION}-${CMAKE_SYSTEM_NAME}")

include(CPack)

cmake_minimum_required(VERSION 3.25)
project(memly LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(Qt6 REQUIRED COMPONENTS Quick Qml Network Core)

# Root main
file(GLOB SRC_MAIN CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/main.cpp")

# Qt
file(GLOB_RECURSE APP_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Qt/*.cpp")
file(GLOB_RECURSE APP_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Qt/*.[Hh]*")

# Utility
file(GLOB_RECURSE UTIL_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Utility/*.cpp")
file(GLOB_RECURSE UTIL_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Utility/*.[Hh]*")

# QML files
set(QML_DIR "${CMAKE_SOURCE_DIR}/src/Qml")
file(GLOB_RECURSE QT_QML CONFIGURE_DEPENDS "${QML_DIR}/*.qml")
foreach(_qml IN LISTS QT_QML)
    file(RELATIVE_PATH _rel "${QML_DIR}" "${_qml}")
    set_source_files_properties("${_qml}" PROPERTIES QT_RESOURCE_ALIAS "${_rel}")
endforeach()

# Database
file(GLOB_RECURSE DB_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Database/*.cpp")
file(GLOB_RECURSE DB_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Database/*.[Hh]*")

# Sql
file(GLOB SQL_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/Database/Sql/*.sql"
    "${CMAKE_SOURCE_DIR}/src/TransactionScripts/Sql/*.sql")

# Core
file(GLOB_RECURSE CORE_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Core/*.cpp")
file(GLOB_RECURSE CORE_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Core/*.[Hh]*")

# TransactionScripts
file(GLOB_RECURSE TS_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/TransactionScripts/*.cpp")
file(GLOB_RECURSE TS_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/TransactionScripts/*.[Hh]*")

set(ALL_SOURCES
    ${SRC_MAIN}
    ${APP_CPP} ${APP_HDR}
    ${UTIL_CPP} ${UTIL_HDR}
    ${DB_CPP} ${DB_HDR}
    ${TS_CPP} ${TS_HDR}
    ${CORE_CPP} ${CORE_HDR}
)

include(FetchContent)

# ====================== DuckDB: platform-specific binaries ======================
set(DUCKDB_VERSION "v1.4.1")

# Select correct DuckDB archive and hash based on host OS + architecture
if(APPLE)
    # macOS universal binary (keep existing settings)
    set(DUCKDB_URL "https://github.com/duckdb/duckdb/releases/download/${DUCKDB_VERSION}/libduckdb-osx-universal.zip")
    set(DUCKDB_SHA "a82bde325dcbb6d1011e1014673eb288e8c1fd5f5b755d47fd245e8b37194817")
elseif(WIN32)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^[Aa][Mm][Dd]64|^[Xx]86_64")
        # Windows amd64
        set(DUCKDB_URL "https://install.duckdb.org/${DUCKDB_VERSION}/libduckdb-windows-amd64.zip")
        set(DUCKDB_SHA "704877e85b556640b0f20a0f07a1c1f404320210b245f51b6da0087a1c92bb76")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^[Aa][Rr][Mm]64")
        # Windows arm64
        set(DUCKDB_URL "https://install.duckdb.org/${DUCKDB_VERSION}/libduckdb-windows-arm64.zip")
        set(DUCKDB_SHA "851f0b89659f9ebfdecc99a9371942117fe4bc9fd9521af82fbe52be073534b6")
    else()
        message(FATAL_ERROR "Unsupported Windows architecture for DuckDB: ${CMAKE_SYSTEM_PROCESSOR}")
    endif()
elseif(UNIX)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^[Aa][Mm][Dd]64|^[Xx]86_64")
        # Linux amd64
        set(DUCKDB_URL "https://install.duckdb.org/${DUCKDB_VERSION}/libduckdb-linux-amd64.zip")
        set(DUCKDB_SHA "ce859962fe96ca952d53571964dafa0168644283a76a2c669b3f73120d710edb")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^[Aa][Rr][Mm]64|^[Aa][Aa][Rr][Cc][Hh]64")
        # Linux arm64
        set(DUCKDB_URL "https://install.duckdb.org/${DUCKDB_VERSION}/libduckdb-linux-arm64.zip")
        set(DUCKDB_SHA "4aa05a74956b1d57f05a139623943231caeee36286c9f42ff10dc278b6df0b6e")
    else()
        message(FATAL_ERROR "Unsupported Linux architecture for DuckDB: ${CMAKE_SYSTEM_PROCESSOR}")
    endif()
else()
    message(FATAL_ERROR "Unsupported platform for DuckDB")
endif()

FetchContent_Declare(
    libduckdb
    URL "${DUCKDB_URL}"
    URL_HASH "SHA256=${DUCKDB_SHA}"
)
FetchContent_MakeAvailable(libduckdb)

# Determine DuckDB library filename and location per-platform
if(APPLE)
    set(_duckdb_lib_name "libduckdb.dylib")
elseif(WIN32)
    set(_duckdb_lib_name "duckdb.dll")
else()
    # Assume generic Unix (Linux, etc.)
    set(_duckdb_lib_name "libduckdb.so")
endif()

set(_duckdb_lib "${libduckdb_SOURCE_DIR}/${_duckdb_lib_name}")
if(NOT EXISTS "${_duckdb_lib}")
    set(_duckdb_lib "${libduckdb_SOURCE_DIR}/lib/${_duckdb_lib_name}")
endif()

set(_duckdb_inc "${libduckdb_SOURCE_DIR}")
if(NOT EXISTS "${_duckdb_inc}/duckdb.hpp")
    set(_duckdb_inc "${libduckdb_SOURCE_DIR}/include")
endif()

if(NOT EXISTS "${_duckdb_lib}")
    message(FATAL_ERROR "DuckDB dylib not found in ${libduckdb_SOURCE_DIR}")
endif()
if(NOT EXISTS "${_duckdb_inc}/duckdb.hpp")
    message(FATAL_ERROR "duckdb.hpp not found in ${libduckdb_SOURCE_DIR}")
endif()

add_library(duckdb SHARED IMPORTED GLOBAL)
set_target_properties(duckdb PROPERTIES
    IMPORTED_LOCATION "${_duckdb_lib}"
    INTERFACE_INCLUDE_DIRECTORIES "${_duckdb_inc}"
)

get_filename_component(_duckdb_libdir "${_duckdb_lib}" DIRECTORY)
set(_duckdb_rpath_dirs "${_duckdb_libdir}")

qt_add_executable(memly
    MANUAL_FINALIZATION
    ${ALL_SOURCES}
)

if(COMMAND qt_policy)
    qt_policy(SET QTP0004 NEW)
endif()
qt_add_qml_module(memly
    URI Memly
    VERSION 1.0
    RESOURCE_PREFIX "/"
    QML_FILES ${QT_QML}
)
qt_add_resources(memly "Sql"
    PREFIX "/Memly"
    BASE "${CMAKE_SOURCE_DIR}/src"
    FILES ${SQL_FILES}
)

target_include_directories(memly PRIVATE "${CMAKE_SOURCE_DIR}/src")

target_compile_features(memly PUBLIC cxx_std_23)
set_target_properties(memly PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_options(memly PRIVATE
    -Wall -Wextra -Wunused -pedantic -Werror
    $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer>
)

target_link_options(memly PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address>
)

target_link_libraries(memly PRIVATE
    Qt6::Quick Qt6::Qml Qt6::Network Qt6::Core duckdb
)
set_target_properties(memly PROPERTIES BUILD_RPATH "${_duckdb_rpath_dirs}")

qt_import_qml_plugins(memly)
qt_finalize_executable(memly)

# ======================== Tests: CTest + GoogleTest ===========================

include(CTest)
find_package(GTest REQUIRED)
include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/test/*.cpp")

set(CORE_SOURCES
    ${APP_CPP}
    ${UTIL_CPP}
    ${DB_CPP}
    ${CORE_CPP}
    ${TS_CPP}
)

add_executable(test-memly
    ${CORE_SOURCES}
    ${TEST_SOURCES}
)

target_compile_definitions(test-memly PRIVATE
    CMAKE_GENERATED_PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}"
)

target_include_directories(test-memly PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_compile_features(test-memly PUBLIC cxx_std_23)
target_compile_options(test-memly PRIVATE
    -Wall -Wextra -Wunused -pedantic -Werror
    $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer>
)

target_link_options(test-memly PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address>
)

target_link_libraries(test-memly PRIVATE
    GTest::gtest_main
    Qt6::Quick Qt6::Qml Qt6::Network Qt6::Core duckdb
)
set_target_properties(test-memly PROPERTIES BUILD_RPATH "${_duckdb_rpath_dirs}")

gtest_discover_tests(test-memly)

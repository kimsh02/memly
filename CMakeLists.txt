cmake_minimum_required(VERSION 3.25)
project(memly LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(Qt6 REQUIRED COMPONENTS Quick Qml Network Core)

# Root main
file(GLOB SRC_MAIN CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/main.cpp")

# QtGUI
file(GLOB_RECURSE GUI_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtGUI/*.cpp")
file(GLOB_RECURSE GUI_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtGUI/*.[Hh]*")

# QML files
set(QML_DIR "${CMAKE_SOURCE_DIR}/src/QtGUI/QML")
file(GLOB_RECURSE QT_QML CONFIGURE_DEPENDS "${QML_DIR}/*.qml")
foreach(_qml IN LISTS QT_QML)
    file(RELATIVE_PATH _rel "${QML_DIR}" "${_qml}")
    set_source_files_properties("${_qml}" PROPERTIES QT_RESOURCE_ALIAS "${_rel}")
endforeach()
file(GLOB_RECURSE QT_ASSETS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtGUI/Assets/*")

# DuckDB
file(GLOB_RECURSE DB_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/DuckDB/*.cpp")
file(GLOB_RECURSE DB_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/DuckDB/*.[Hh]*")

# SQL
file(GLOB SQL_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/DuckDB/SQL/*.sql"
    "${CMAKE_SOURCE_DIR}/src/TransactionScripts/SQL/*.sql")

# C++
file(GLOB_RECURSE COMMON_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Common/*.[Hh]*")
file(GLOB_RECURSE COMMON_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Common/*.cpp")
file(GLOB_RECURSE CORE_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Core/*.cpp")
file(GLOB_RECURSE CORE_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Core/*.[Hh]*")
file(GLOB_RECURSE TS_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/TransactionScripts/*.cpp")
file(GLOB_RECURSE TS_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/TransactionScripts/*.[Hh]*")

set(ALL_SOURCES
    ${SRC_MAIN}
    ${GUI_CPP} ${GUI_HDR}
    ${DB_CPP} ${DB_HDR}
    ${COMMON_CPP} ${COMMON_HDR}
    ${TS_CPP} ${TS_HDR}
    ${CORE_CPP} ${CORE_HDR}
)

include(FetchContent)

FetchContent_Declare(
    libduckdb
    URL https://github.com/duckdb/duckdb/releases/download/v1.4.1/libduckdb-osx-universal.zip
    URL_HASH SHA256=a82bde325dcbb6d1011e1014673eb288e8c1fd5f5b755d47fd245e8b37194817
)
FetchContent_MakeAvailable(libduckdb)

set(_duckdb_lib "${libduckdb_SOURCE_DIR}/libduckdb.dylib")
if(NOT EXISTS "${_duckdb_lib}")
    set(_duckdb_lib "${libduckdb_SOURCE_DIR}/lib/libduckdb.dylib")
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

if(APPLE)
    set_target_properties(memly PROPERTIES
        MACOSX_BUNDLE TRUE
        OUTPUT_NAME "Memly"
        MACOSX_BUNDLE_BUNDLE_NAME "Memly"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.kimsh02.memly"
    )
endif()

qt_policy(SET QTP0004 NEW)
qt_add_qml_module(memly
    URI memly.QML
    VERSION 1.0
    RESOURCE_PREFIX "/"
    QML_FILES ${QT_QML}
)
qt_add_resources(memly "Assets"
    PREFIX "/memly"
    BASE "${CMAKE_SOURCE_DIR}/src"
    FILES ${QT_ASSETS}
)
qt_add_resources(memly "SQL"
    PREFIX "/memly"
    BASE "${CMAKE_SOURCE_DIR}/src"
    FILES ${SQL_FILES}
)

target_include_directories(memly PRIVATE "${CMAKE_SOURCE_DIR}/src")

target_compile_features(memly PUBLIC cxx_std_23)
set_target_properties(memly PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_options(memly PRIVATE -Wall -Wextra -Wunused -pedantic -Werror)

target_link_libraries(memly PRIVATE
    Qt6::Quick Qt6::Qml Qt6::Network Qt6::Core duckdb
)
set_target_properties(memly PROPERTIES BUILD_RPATH "${_duckdb_rpath_dirs}")

qt_import_qml_plugins(memly)
qt_finalize_executable(memly)

# ======================== Tests: CTest + GoogleTest ===========================
include(CTest)
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/tests/*.cpp")

set(CORE_SOURCES
    ${COMMON_CPP}
    ${GUI_CPP}
    ${DB_CPP}
    ${CORE_CPP}
    ${TS_CPP}
)

add_executable(test-memly
    ${CORE_SOURCES}
    ${TEST_SOURCES}
)

target_include_directories(test-memly PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_compile_features(test-memly PUBLIC cxx_std_23)
target_compile_options(test-memly PRIVATE -Wall -Wextra -Wunused -pedantic -Werror)

target_link_libraries(test-memly PRIVATE
    GTest::gtest_main
    Qt6::Quick Qt6::Qml Qt6::Network Qt6::Core duckdb
)
set_target_properties(test-memly PROPERTIES BUILD_RPATH "${_duckdb_rpath_dirs}")

gtest_discover_tests(test-memly)

cmake_minimum_required(VERSION 3.25)
project(memly LANGUAGES C CXX)

# ------------------------ Compile DB ------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

# ------------------------ Qt (system-wide) ------------------------------------
set(QT_NO_PRIVATE_MODULE_WARNING ON)
find_package(Qt6 REQUIRED COMPONENTS Quick Qml Network Core Sql)

# ------------------------ Sources ---------------------------------------------
# Root main
file(GLOB SRC_MAIN CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/main.cpp")

# QtGUI
file(GLOB_RECURSE GUI_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtGUI/*.cpp")
file(GLOB_RECURSE GUI_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtGUI/*.[Hh]*")
file(GLOB_RECURSE QT_QML CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtGUI/Qml/*.qml")
file(GLOB_RECURSE QT_ASSETS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtGUI/Assets/*")

# QtDatabase
# file(GLOB_RECURSE DB_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtDatabase/*.cpp")
# file(GLOB_RECURSE DB_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtDatabase/*.[Hh]*")

# DuckDB
file(GLOB_RECURSE DB_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/DuckDB/*.cpp")
file(GLOB_RECURSE DB_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/DuckDB/*.[Hh]*")

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
    ${COMMON_CPP}
    ${COMMON_HDR}
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

# ---- Import prebuilt DuckDB dylib + headers ----
# Try both possible layouts: files at root or under lib/ and include/
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

# Compute lib dir for runtime lookup from build tree
get_filename_component(_duckdb_libdir "${_duckdb_lib}" DIRECTORY)
set(_duckdb_rpath_dirs "${_duckdb_libdir}")

# ------------------------ App Target ------------------------------------------
qt_add_executable(memly
    MANUAL_FINALIZATION
    ${ALL_SOURCES}
)

# QML and Assets
qt_add_resources(memly "Qml"
    PREFIX "/Qml"
    BASE "${CMAKE_SOURCE_DIR}/src/QtGUI/Qml"
    FILES ${QT_QML}
)
qt_add_resources(memly "Assets"
    PREFIX "/Assets"
    BASE "${CMAKE_SOURCE_DIR}/src/QtGUI/Assets"
    FILES ${QT_ASSETS}
)

# Include root one level up (src)
target_include_directories(memly PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
)

# ------------------------ C++ standard ----------------------------------------
target_compile_features(memly PUBLIC cxx_std_23)
set_target_properties(memly PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# ------------------------ Warnings (global) -----------------------------------
target_compile_options(memly PRIVATE -Wall -Wextra -Wunused -pedantic -Werror)

# ------------------------ Link ------------------------------------------------
target_link_libraries(memly PRIVATE
    Qt6::Quick Qt6::Qml Qt6::Network Qt6::Core Qt6::Sql duckdb
)
set_target_properties(memly PROPERTIES BUILD_RPATH "${_duckdb_rpath_dirs}")

qt_import_qml_plugins(memly)
qt_finalize_executable(memly)

# ======================== Tests: CTest + GoogleTest ===========================
include(CTest)

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# Collect test sources
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/tests/*.cpp")

# Reuse core sources but exclude main.cpp for tests
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

target_include_directories(test-memly PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
)

target_compile_features(test-memly PUBLIC cxx_std_23)
target_compile_options(test-memly PRIVATE -Wall -Wextra -Wunused -pedantic -Werror)

target_link_libraries(test-memly PRIVATE
    GTest::gtest_main
    Qt6::Quick Qt6::Qml Qt6::Network Qt6::Core Qt6::Sql duckdb
)
set_target_properties(test-memly PROPERTIES BUILD_RPATH "${_duckdb_rpath_dirs}")

# Auto-discover TEST() cases and register with CTest
gtest_discover_tests(test-memly)

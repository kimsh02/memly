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

# QtGui
file(GLOB_RECURSE GUI_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtGui/*.cpp")
file(GLOB_RECURSE GUI_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtGui/*.[Hh]*")
file(GLOB_RECURSE QT_QML CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtGui/Qml/*.qml")
file(GLOB_RECURSE QT_ASSETS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtGui/Assets/*")

# QtDatabase
file(GLOB_RECURSE DB_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtDatabase/*.cpp")
file(GLOB_RECURSE DB_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/QtDatabase/*.[Hh]*")

# C++
file(GLOB_RECURSE COMMON_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Common/*.[Hh]*")
file(GLOB_RECURSE CORE_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Core/*.cpp")
file(GLOB_RECURSE CORE_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Core/*.[Hh]*")
file(GLOB_RECURSE TS_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/TransactionScripts/*.cpp")
file(GLOB_RECURSE TS_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/TransactionScripts/*.[Hh]*")

set(ALL_SOURCES
  ${SRC_MAIN}
  ${GUI_CPP} ${GUI_HDR}
  ${DB_CPP} ${DB_HDR}
  ${COMMON_HDR}
  ${TS_CPP} ${TS_HDR}
  ${CORE_CPP} ${CORE_HDR}
)

# ------------------------ App Target ------------------------------------------
qt_add_executable(memly
  MANUAL_FINALIZATION
  ${ALL_SOURCES}
)

# QML and Assets
qt_add_resources(memly "Qml"
  PREFIX "/Qml"
  BASE "${CMAKE_SOURCE_DIR}/src/QtGui/Qml"
  FILES ${QT_QML}
)
qt_add_resources(memly "Assets"
  PREFIX "/Assets"
  BASE "${CMAKE_SOURCE_DIR}/src/QtGui/Assets"
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
  Qt6::Quick Qt6::Qml Qt6::Network Qt6::Core Qt6::Sql
)

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
  Qt6::Quick Qt6::Qml Qt6::Network Qt6::Core Qt6::Sql
)

# Auto-discover TEST() cases and register with CTest
gtest_discover_tests(test-memly)

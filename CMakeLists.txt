cmake_minimum_required(VERSION 3.25)
project(repetition-ai LANGUAGES C CXX)

# Prefer config packages (vcpkg)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

# ------------------------ Compile DB ------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

# ------------------------ Dependencies (via vcpkg) ----------------------------
set(QT_NO_PRIVATE_MODULE_WARNING ON)
find_package(Qt6 REQUIRED COMPONENTS Quick Qml Network Core Sql)

find_package(google_cloud_cpp_translate REQUIRED CONFIG)
find_package(google_cloud_cpp_texttospeech REQUIRED CONFIG)

# ------------------------ Sources ---------------------------------------------
# Root main
file(GLOB SRC_MAIN CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/main.cpp")

# GUI
file(GLOB_RECURSE GUI_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/gui/*.cpp")
file(GLOB_RECURSE GUI_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/gui/*.[Hh]*")
file(GLOB_RECURSE QML_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/gui/qml/*.qml")
file(GLOB_RECURSE ASSETS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/gui/assets/*")

# Database (moved to database_qt)
file(GLOB_RECURSE DB_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/database_qt/*.cpp")
file(GLOB_RECURSE DB_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/database_qt/*.[Hh]*")

# Models / Repository / Services / Common / Wrappers
file(GLOB_RECURSE MODEL_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/models/*.[Hh]*")
file(GLOB_RECURSE REPO_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/repository/*.cpp")
file(GLOB_RECURSE REPO_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/repository/*.[Hh]*")
file(GLOB_RECURSE SERVICES_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/services/*.cpp")
file(GLOB_RECURSE SERVICES_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/services/*.[Hh]*")
file(GLOB_RECURSE COMMON_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/common/*.[Hh]*")
file(GLOB_RECURSE UTIL_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/util/*.[Hh]*")
file(GLOB_RECURSE WRAPPER_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/service_wrappers/*.[Hh]*")

set(ALL_SOURCES
  ${SRC_MAIN}
  ${GUI_CPP} ${GUI_HDR}
  ${DB_CPP} ${DB_HDR}
  ${MODEL_HDR}
  ${REPO_CPP} ${REPO_HDR}
  ${SERVICES_CPP} ${SERVICES_HDR}
  ${COMMON_HDR} ${UTIL_HDR} ${WRAPPER_HDR}
)

# ------------------------ App Target ------------------------------------------
qt_add_executable(repetition-ai
  MANUAL_FINALIZATION
  ${ALL_SOURCES}
)

# QML and assets -> qrc:/qml and qrc:/assets
qt_add_resources(repetition-ai "qml"
  PREFIX "/qml"
  BASE "${CMAKE_SOURCE_DIR}/src/gui/qml"
  FILES ${QML_FILES}
)
qt_add_resources(repetition-ai "assets"
  PREFIX "/assets"
  BASE "${CMAKE_SOURCE_DIR}/src/gui/assets"
  FILES ${ASSETS}
)

# Include root one level up (src)
target_include_directories(repetition-ai PRIVATE
  "${CMAKE_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/src/gui"
  "${CMAKE_SOURCE_DIR}/src/database_qt"
  "${CMAKE_SOURCE_DIR}/src/models"
  "${CMAKE_SOURCE_DIR}/src/repository"
  "${CMAKE_SOURCE_DIR}/src/services"
  "${CMAKE_SOURCE_DIR}/src/common"
  "${CMAKE_SOURCE_DIR}/src/util"
  "${CMAKE_SOURCE_DIR}/src/service_wrappers"
)

# ------------------------ C++ standard ----------------------------------------
target_compile_features(repetition-ai PUBLIC cxx_std_26)
set_target_properties(repetition-ai PROPERTIES
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

# ------------------------ Warnings (global) -----------------------------------
target_compile_options(repetition-ai PRIVATE -Wall -Wextra -Wunused -pedantic -Werror)

# ------------------------ Link ------------------------------------------------
target_link_libraries(repetition-ai PRIVATE
  Qt6::Quick Qt6::Qml Qt6::Network Qt6::Core Qt6::Sql
  google-cloud-cpp::translate
  google-cloud-cpp::texttospeech
)

qt_import_qml_plugins(repetition-ai)
qt_finalize_executable(repetition-ai)

# ======================== Tests: CTest + GoogleTest ===========================
include(CTest) # defines BUILD_TESTING and enables testing

if(BUILD_TESTING)
  # Homebrew provides GTest config package: GTest::gtest, GTest::gtest_main
  find_package(GTest CONFIG REQUIRED)

  include(GoogleTest) # gtest_discover_tests()

  # Collect test sources
  file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/tests/*.cpp")

  # Reuse core sources but exclude main.cpp for tests
  set(CORE_SOURCES
    ${GUI_CPP}
    ${DB_CPP}
    ${REPO_CPP}
    ${SERVICES_CPP}
  )

  add_executable(repetition-ai-tests
    ${CORE_SOURCES}
    ${TEST_SOURCES}
  )

  target_include_directories(repetition-ai-tests PRIVATE
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/src"
  )

  target_compile_features(repetition-ai-tests PUBLIC cxx_std_26)
  target_compile_options(repetition-ai-tests PRIVATE -Wall -Wextra -Wunused -pedantic)

  target_link_libraries(repetition-ai-tests PRIVATE
    GTest::gtest_main
    Qt6::Quick Qt6::Qml Qt6::Network Qt6::Core Qt6::Sql
    google-cloud-cpp::translate
    google-cloud-cpp::texttospeech
  )

  # Auto-discover TEST() cases and register with CTest
  gtest_discover_tests(repetition-ai-tests)
endif()

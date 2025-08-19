cmake_minimum_required(VERSION 3.25)
project(repetition-ai LANGUAGES C CXX)

# set(QT_QML_GENERATE_QMLLS_INI ON)  # generate .qmlls.ini for QML imports/tooling

# ------------------------ Multi-configs ------------------------
set(CMAKE_CONFIGURATION_TYPES memory thread release
    CACHE STRING "Available build configs" FORCE)

# ------------------------ Compile DB ---------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------ Qt 6 (no fallback) -------------------
# If Qt isn't found, set CMAKE_PREFIX_PATH to your Qt prefix (e.g., $(brew --prefix qt))
find_package(Qt6 REQUIRED COMPONENTS Quick Qml)

# ------------------------ Sources ------------------------------
# Use CONFIGURE_DEPENDS so CMake regenerates when files are added/removed.
file(GLOB_RECURSE APP_CPP   CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/app/*.cpp")
file(GLOB_RECURSE APP_HDR   CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/app/*.[Hh]*")
file(GLOB_RECURSE QML_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/qml/*.qml")
file(GLOB_RECURSE ASSETS    CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/assets/*")

set(ALL_SOURCES
  ${APP_CPP} ${APP_HDR}
)

# ------------------------ Target -------------------------------
# qt_add_executable sets up AUTOMOC/AUTOUIC/AUTORCC for Qt6.
qt_add_executable(repetition-ai
  MANUAL_FINALIZATION
  ${ALL_SOURCES}
)

# qt_import_qml_plugins(repetition-ai)  # ensure QtQuick/QML plugins are discoverable at runtime

# Package QML and assets into the binary at qrc:/qml and qrc:/assets
qt_add_resources(repetition-ai "qml"
  PREFIX "/qml"
  BASE "${CMAKE_SOURCE_DIR}/src/qml"
  FILES ${QML_FILES}
)

qt_add_resources(repetition-ai "assets"
  PREFIX "/assets"
  BASE "${CMAKE_SOURCE_DIR}/src/assets"
  FILES ${ASSETS}
)

target_include_directories(repetition-ai PRIVATE
  "${CMAKE_SOURCE_DIR}/src/app"
  "${CMAKE_SOURCE_DIR}"
)

# ------------------------ C++ standard -------------------------
target_compile_features(repetition-ai PUBLIC cxx_std_26)
set_target_properties(repetition-ai PROPERTIES
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

# ------------------------ Warnings -----------------------------
target_compile_options(repetition-ai PRIVATE -Wall -Wextra -Wunused -pedantic -Werror)

# ------------------------ Sanitizers & Opt ---------------------
target_compile_options(repetition-ai PRIVATE
  $<$<CONFIG:memory>:-g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls>
  $<$<CONFIG:thread>:-g -fsanitize=thread,undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls>
  $<$<CONFIG:release>:-O3>
)
target_link_options(repetition-ai PRIVATE
  $<$<CONFIG:memory>:-fsanitize=address,undefined>
  $<$<CONFIG:thread>:-fsanitize=thread,undefined>
)
target_compile_definitions(repetition-ai PRIVATE
  $<$<CONFIG:release>:NDEBUG>
)

# ------------------------ Coverage (optional) ------------------
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
if (ENABLE_COVERAGE)
  message(STATUS "Building with coverage flags…")
  target_compile_options(repetition-ai PRIVATE -fprofile-instr-generate -fcoverage-mapping)
  if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_options(repetition-ai PRIVATE -lclang_rt.profile_osx)
  else()
    target_link_options(repetition-ai PRIVATE -lclang_rt.profile)
  endif()
endif()

# ------------------------ Link Qt 6 ----------------------------
target_link_libraries(repetition-ai PRIVATE Qt6::Quick Qt6::Qml)

# ------------------------ Finalize Qt 6 ------------------------
qt_finalize_executable(repetition-ai)

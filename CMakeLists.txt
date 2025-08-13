cmake_minimum_required(VERSION 3.25)
project(repetition-ai LANGUAGES C CXX)

# ------------------------ Multi-configs ------------------------
set(CMAKE_CONFIGURATION_TYPES memory thread release
    CACHE STRING "Available build configs" FORCE)

# ------------------------ Compile DB ---------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------ Qt 6 (no fallback) -------------------
# If Qt isn't found, set CMAKE_PREFIX_PATH to your Qt prefix (e.g., $(brew --prefix qt))
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ------------------------ Sources ------------------------------
# Use CONFIGURE_DEPENDS so CMake regenerates when files are added/removed.
file(GLOB_RECURSE CORE_SOURCES       CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/core/src/*.cpp")
file(GLOB_RECURSE CORE_HEADERS       CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/core/include/*.[Hh]*")
file(GLOB_RECURSE PLATFORM_QT_SRC    CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/platform/qt/src/*.cpp")
file(GLOB_RECURSE PLATFORM_QT_HDR    CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/platform/qt/include/*.[Hh]*")
file(GLOB            UI_CPP          CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/ui_qt/src/*.cpp")
file(GLOB            UI_HDR          CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/ui_qt/src/*.[Hh]*")
file(GLOB            UI_FORMS        CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/ui_qt/src/*.ui")
file(GLOB_RECURSE    UI_RESOURCES    CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/ui_qt/src/resources/*.qrc")

set(ALL_SOURCES
    ${CORE_SOURCES} ${CORE_HEADERS}
    ${PLATFORM_QT_SRC} ${PLATFORM_QT_HDR}
    ${UI_CPP} ${UI_HDR} ${UI_FORMS} ${UI_RESOURCES}
)

# ------------------------ Target -------------------------------
# qt_add_executable sets up AUTOMOC/AUTOUIC/AUTORCC for Qt6.
qt_add_executable(repetition-ai
    MANUAL_FINALIZATION
    ${ALL_SOURCES}
)

target_include_directories(repetition-ai PRIVATE
    "${CMAKE_SOURCE_DIR}/core/include"
    "${CMAKE_SOURCE_DIR}/platform/qt/include"
    "${CMAKE_SOURCE_DIR}/ui_qt/src"      # headers next to .ui/.cpp
    "${CMAKE_SOURCE_DIR}"                # if you keep any top-level headers
)

# ------------------------ C++ standard -------------------------
target_compile_features(repetition-ai PUBLIC cxx_std_26)
set_target_properties(repetition-ai PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# ------------------------ Warnings -----------------------------
target_compile_options(repetition-ai PRIVATE -Wall -Wextra -pedantic -Werror)

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
target_link_libraries(repetition-ai PRIVATE Qt6::Widgets)

# If you add more Qt modules, list them here and in find_package, e.g.:
# find_package(Qt6 REQUIRED COMPONENTS Widgets Network Sql)
# target_link_libraries(repetition-ai PRIVATE Qt6::Widgets Qt6::Network Qt6::Sql)

# ------------------------ Finalize Qt 6 ------------------------
qt_finalize_executable(repetition-ai)

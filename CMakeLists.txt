cmake_minimum_required(VERSION 3.25)
project(repetition-ai LANGUAGES C CXX)

# Silence FindBoost policy warning
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

# ------------------------ Multi-configs ------------------------
set(CMAKE_CONFIGURATION_TYPES memory thread release
  CACHE STRING "Available build configs" FORCE)

# ------------------------ Compile DB ---------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

# ------------------------ Qt 6 ------------------------
find_package(Qt6 REQUIRED COMPONENTS Quick Qml)

# ------------------------ Boost (uuid via headers) ------------------------
find_package(Boost REQUIRED) # use Boost::headers

# ------------------------ Sources ------------------------------
file(GLOB SRC_MAIN CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/main.cpp")
file(GLOB_RECURSE GUI_CPP CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/gui/*.cpp")
file(GLOB_RECURSE GUI_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/gui/*.[Hh]*")
file(GLOB_RECURSE MODEL_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/models/*.[Hh]*")
file(GLOB_RECURSE SERVICE_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/services/*.[Hh]*")
file(GLOB_RECURSE DB_HDR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/database/*.[Hh]*")
file(GLOB_RECURSE QML_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/gui/qml/*.qml")
file(GLOB_RECURSE ASSETS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/gui/assets/*")

set(ALL_SOURCES
  ${SRC_MAIN}
  ${GUI_CPP} ${GUI_HDR}
  ${MODEL_HDR} ${SERVICE_HDR} ${DB_HDR}
)

# ------------------------ Target -------------------------------
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

# include root one level up (src)
target_include_directories(repetition-ai PRIVATE
  "${CMAKE_SOURCE_DIR}/src"
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
if(ENABLE_COVERAGE)
  message(STATUS "Building with coverage flags…")
  target_compile_options(repetition-ai PRIVATE -fprofile-instr-generate -fcoverage-mapping)
  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_options(repetition-ai PRIVATE -lclang_rt.profile_osx)
  else()
    target_link_options(repetition-ai PRIVATE -lclang_rt.profile)
  endif()
endif()

# ------------------------ Link ----------------------------
target_link_libraries(repetition-ai PRIVATE
  Qt6::Quick Qt6::Qml
  Boost::headers
)
qt_import_qml_plugins(repetition-ai)
qt_finalize_executable(repetition-ai)

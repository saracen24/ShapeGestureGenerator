cmake_minimum_required (VERSION 3.12 FATAL_ERROR)

project (ShapeGestureGenerator
  VERSION 0.1.0.0
  DESCRIPTION "Shape gesture generator."
  LANGUAGES CXX
)

add_executable (${PROJECT_NAME})

target_compile_definitions (${PROJECT_NAME}
  PRIVATE
    PROJECT_NAME="${PROJECT_NAME}"
    PROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}"
    PROJECT_VERSION="${PROJECT_VERSION}"
)

find_package (OpenCV 4 REQUIRED
  COMPONENTS
    highgui
)

target_include_directories (${PROJECT_NAME}
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${OpenCV_INCLUDE_DIRS}"
)

target_link_libraries (${PROJECT_NAME}
  PRIVATE
    ${OpenCV_LIBS}
)

target_compile_options (${PROJECT_NAME}
  PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
      -Wall
      -Wextra
      -Wpedantic
      -pedantic-errors
    >
    $<$<CXX_COMPILER_ID:MSVC>:
      /Wall
      /EHsc
      /MP
    >
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:
      /Zi
      /MDd
    >
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:
      /MD
    >
)

set (SOURCE_FILES
  "./src/main.cpp"
)

source_group (TREE "${CMAKE_CURRENT_SOURCE_DIR}"
  FILES
    ${SOURCE_FILES}
)

target_sources (${PROJECT_NAME}
  PRIVATE
    ${SOURCE_FILES}
)

set_target_properties (${PROJECT_NAME}
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    USE_FOLDERS ON
)

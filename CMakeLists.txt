cmake_minimum_required(VERSION 3.12)
project(QCodeEditor)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_EXAMPLE "Example building required" OFF)

# Generate compile_commands.json in build/ for analyzers like clang-tidy.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${BUILD_EXAMPLE})
  message(STATUS "QCodeEditor example will be built.")
  add_subdirectory(example)
endif()

# Defautling to Qt5 if nothing found yet.
if(NOT ${QT_VERSION_MAJOR} OR ${QT_VERSION_MAJOR} EQUAL 5)
  set(QT "Qt5")
  qt5_add_resources(
    QRCS "${CMAKE_CURRENT_SOURCE_DIR}/resources/qcodeeditor_resources.qrc")
else()
  set(QT "Qt${QT_VERSION_MAJOR}")
  qt_add_resources(
    QRCS "${CMAKE_CURRENT_SOURCE_DIR}/resources/qcodeeditor_resources.qrc")
endif()

set(INCLUDE_FILES
    include/QHighlightRule
    include/QHighlightBlockRule
    include/QCodeEditor
    include/QCXXHighlighter
    include/QLineNumberArea
    include/QSearchWidget
    include/QStyleSyntaxHighlighter
    include/QSyntaxStyle
    include/QGLSLCompleter
    include/QGLSLHighlighter
    include/QJavaHighlighter
    include/QJSCompleter
    include/QJSHighlighter
    include/QLanguage
    include/QXMLHighlighter
    include/QJSONHighlighter
    include/QLuaCompleter
    include/QLuaHighlighter
    include/QPythonHighlighter
    include/QFaustCompleter
    include/QFaustHighlighter
    include/internal/QHighlightRule.hpp
    include/internal/QHighlightBlockRule.hpp
    include/internal/QCodeEditor.hpp
    include/internal/QCXXHighlighter.hpp
    include/internal/QJavaHighlighter.hpp
    include/internal/QJSCompleter.hpp
    include/internal/QJSHighlighter.hpp
    include/internal/QLineNumberArea.hpp
    include/internal/QSearchWidget.hpp
    include/internal/QStyleSyntaxHighlighter.hpp
    include/internal/QSyntaxStyle.hpp
    include/internal/QGLSLCompleter.hpp
    include/internal/QGLSLHighlighter.hpp
    include/internal/QLanguage.hpp
    include/internal/QXMLHighlighter.hpp
    include/internal/QJSONHighlighter.hpp
    include/internal/QLuaCompleter.hpp
    include/internal/QLuaHighlighter.hpp
    include/internal/QPythonCompleter.hpp
    include/internal/QPythonHighlighter.hpp
    include/internal/QFaustCompleter.hpp
    include/internal/QFaustHighlighter.hpp)

set(SOURCE_FILES
    src/internal/QCodeEditor.cpp
    src/internal/QLineNumberArea.cpp
    src/internal/QSearchWidget.cpp
    src/internal/QCXXHighlighter.cpp
    src/internal/QSyntaxStyle.cpp
    src/internal/QStyleSyntaxHighlighter.cpp
    src/internal/QGLSLCompleter.cpp
    src/internal/QGLSLHighlighter.cpp
    src/internal/QJavaHighlighter.cpp
    src/internal/QJSCompleter.cpp
    src/internal/QJSHighlighter.cpp
    src/internal/QLanguage.cpp
    src/internal/QXMLHighlighter.cpp
    src/internal/QJSONHighlighter.cpp
    src/internal/QLuaCompleter.cpp
    src/internal/QLuaHighlighter.cpp
    src/internal/QPythonCompleter.cpp
    src/internal/QPythonHighlighter.cpp
    src/internal/QFaustCompleter.cpp
    src/internal/QFaustHighlighter.cpp)

find_package(${QT} REQUIRED COMPONENTS Core Gui Widgets)

add_library(${PROJECT_NAME} STATIC ${QRCS} ${SOURCE_FILES} ${INCLUDE_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<INSTALL_INTERFACE:include>
                         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

# This is required to compile when library is attached via CMP with using global
# cache. Probably CMP with download per project would work.
if(NOT WIN32)
  execute_process(
    COMMAND id -u
    OUTPUT_VARIABLE USER_ID
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(SAFE_AUTOGEN_DIR "/tmp/cmake_autogen_${PROJECT_NAME}_${USER_ID}")
else()
  file(TO_CMAKE_PATH "$ENV{TEMP}" TEMP_PATH)
  set(SAFE_AUTOGEN_DIR "${TEMP_PATH}/cmake_autogen_${PROJECT_NAME}")
endif()

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES AUTOMOC ON
             AUTOGEN_BUILD_DIR "${SAFE_AUTOGEN_DIR}"
             AUTOUIC ON
             AUTORCC ON)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(
    ${PROJECT_NAME} PRIVATE -pedantic -Wall -Wextra -Woverloaded-virtual
                            -Winit-self -Wunreachable-code)
endif(CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(${PROJECT_NAME} ${QT}::Core ${QT}::Widgets ${QT}::Gui)

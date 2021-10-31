cmake_minimum_required(VERSION 3.10)

project(MiniExcelCmake_test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package(antlr4-runtime REQUIRED)
find_package(antlr4-generator REQUIRED)

# Set path to generator
set(ANTLR4_JAR_LOCATION ../third/antlr4/antlr-4.9.2-complete.jar)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

# generate combined grammar
antlr4_generate(
        cellExpressionGrammar
        /home/sviat/CLionProjects/MiniExcel/antlr4/CellExpression.g4
        BOTH                                # combined grammar
        FALSE                               # do not generate listener
        TRUE                                # generate visitor
        "antlr4"                            # namespace
)

set(SRC_DIR ../src)
set(INCLUDE_DIR ../include)

set(PROJECT_SOURCES
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${SRC_DIR}/Cell.cpp
        ${INCLUDE_DIR}/Cell.h
        ${SRC_DIR}/Window.cpp
        ${INCLUDE_DIR}/Window.h
        ${SRC_DIR}/TableModel.cpp
        ${INCLUDE_DIR}/TableModel.h
        ../resourses.qrc
        ${ANTLR4_SRC_FILES_cellExpressionGrammar}
        ${SRC_DIR}/CellFormulaVisitor.cpp
        ${INCLUDE_DIR}/CellFormulaVisitor.h
        ${SRC_DIR}/CellFormulaInterpreter.cpp
        ${INCLUDE_DIR}/CellFormulaInterpreter.h
        ${SRC_DIR}/ExceptionCellExpressionListener.cpp
        ${INCLUDE_DIR}/ExceptionCellExpressionListener.h
        ${SRC_DIR}/MainToolBar/MainToolBar.cpp
        ${INCLUDE_DIR}/MainToolBar/MainToolBar.h
        ${SRC_DIR}/InputToolBar/InputToolBar.cpp
        ${INCLUDE_DIR}/InputToolBar/InputToolBar.h)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MiniExcelCmake_test
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MiniExcelCmake APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MiniExcelCmake_test SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MiniExcelCmake_test
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# add required runtime library
add_dependencies(MiniExcelCmake_test antlr4_shared )

target_link_libraries(MiniExcelCmake_test PRIVATE Qt${QT_VERSION_MAJOR}::Widgets gtest_main antlr4_shared)

target_include_directories(MiniExcelCmake_test PUBLIC ../include ${ANTLR4_INCLUDE_DIR} ${ANTLR4_INCLUDE_DIR_cellExpressionGrammar})

enable_testing()

include(GoogleTest)
gtest_discover_tests(MiniExcelCmake_test)

set_target_properties(MiniExcelCmake_test PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MiniExcelCmake_test)
endif()

cmake_minimum_required(VERSION 3.10)

project(MiniExcelCmake VERSION 0.1 LANGUAGES CXX)

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
set(ANTLR4_JAR_LOCATION ${PROJECT_SOURCE_DIR}/third/antlr4/antlr-4.9.2-complete.jar)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# generate combined grammar
antlr4_generate(
        cellExpressionGrammar
        ${SRC_DIR}/antlr4/CellExpression.g4
        BOTH                                # combined grammar
        FALSE                               # do not generate listener
        TRUE                                # generate visitor
        "antlr4"                            # namespace
)

add_subdirectory(third/googletest-master)
add_subdirectory(test)

set(PROJECT_SOURCES
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/cell.cpp
        ${INCLUDE_DIR}/cell.h
        ${SRC_DIR}/window.cpp
        ${INCLUDE_DIR}/window.h
        ${SRC_DIR}/tablemodel.cpp
        ${INCLUDE_DIR}/tablemodel.h
        ${PROJECT_SOURCE_DIR}/resourses.qrc
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
    qt_add_executable(MiniExcelCmake
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MiniExcelCmake APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MiniExcelCmake SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MiniExcelCmake
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(MiniExcelCmake PUBLIC include ${ANTLR4_INCLUDE_DIR} ${ANTLR4_INCLUDE_DIR_cellExpressionGrammar})

# add required runtime library
add_dependencies(MiniExcelCmake antlr4_shared )

target_link_libraries(MiniExcelCmake PRIVATE Qt${QT_VERSION_MAJOR}::Widgets antlr4_shared)

set_target_properties(MiniExcelCmake PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MiniExcelCmake)
endif()

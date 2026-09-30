cmake_minimum_required(VERSION 3.16)

project(McpServerTest LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_subdirectory(../mcpserverlib subproject/mcpserverlib)

add_executable(McpServerTest
  main.cpp
  testcmd.h testcmd.cpp
)
target_link_libraries(McpServerTest PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  mcpserverlib
)

include(GNUInstallDirs)
install(TARGETS McpServerTest
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

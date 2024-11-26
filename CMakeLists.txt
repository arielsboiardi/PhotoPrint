cmake_minimum_required(VERSION 3.5.0)

project(PhotoPrint VERSION 0.1.0 LANGUAGES C CXX)

# Remove MSVC warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_WARNINGS")

# Windows app icon
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	enable_language("RC")
	set (WIN32_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/windows/PhotoPrint.rc)
	set(CMAKE_AUTORCC ON)
endif()

# wxWidgets libraries
set(wxBUILD_SHARED OFF)
add_subdirectory(wxWidgets-3.2.6)

# Asio libraries
include_directories(asio-1.30.2/include)

# Device library
include_directories(devices)

add_library(BX3SSUlib STATIC devices/BX3SSU.hpp)
set_target_properties(BX3SSUlib PROPERTIES LINKER_LANGUAGE CXX)

add_library(Arduinolib STATIC devices/Arduino.hpp)
set_target_properties(Arduinolib PROPERTIES LINKER_LANGUAGE CXX)

add_library(Seriallib STATIC devices/Serial.hpp)
set_target_properties(Seriallib PROPERTIES LINKER_LANGUAGE CXX)

# Main executable
add_executable(${PROJECT_NAME} WIN32 PhotoPrint.cpp ${WIN32_RESOURCES})

target_link_libraries(${PROJECT_NAME} wx::net wx::core wx::base)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DPI_AWARE "PerMonitor")

# Test serial 
add_executable(TestSerial SerialTest.cpp)
target_link_libraries(TestSerial)

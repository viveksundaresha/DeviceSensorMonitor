cmake_minimum_required(VERSION 3.16)
project(DeviceMonitorDashboard)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Charts REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/SensorData.h
    src/SensorData.cpp
    src/SensorSimulator.h
    src/SensorSimulator.cpp
)

target_link_libraries(${PROJECT_NAME} 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets
    Qt6::Charts
)
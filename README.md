Device Monitor Dashboard

A real time environmental sensor monitoring desktop application built with C++17 and Qt6.
The application simulates environmental sensor data and visualizes it in a graphical dashboard with live updates, configurable thresholds, and interactive charts.

Techstack Choice

Selected Option: Option C

Language: C++17
Framework: Qt 6 (Widgets)
Charting Library: Qt Charts

Reason for Choosing Option C

I chose C++ with Qt because it provides a robust framework for building desktop applications with real time UI updates. Qt also provides powerful features such as signals and slots, timers, and built in charting libraries which make it well suited for building interactive dashboards.

Using C++ also allowed me to demonstrate object oriented design and proper separation between the data layer, simulation logic, and user interface.

Application Features

The application simulates three environmental sensors:

• Temperature (°C)
• Humidity (%)
• Pressure (hPa)

Key functionality includes:

• Real time sensor updates every second
• Chart visualization showing historical data (last 30 samples)
• Threshold based warning indicators
• Start / Stop simulation controls
• Interactive UI with sensor selection and threshold adjustment

How to Build and Run
Prerequisites

• Qt 6.10 or newer
• CMake 3.18 or newer
• A C++17 compatible compiler (MinGW, GCC, or MSVC)

Qt Charts module must also be installed.

Build Instructions (Windows – MinGW)

From the root project directory:

mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=<path_to_qt>
cmake --build .

Run the application:

DeviceMonitorDashboard.exe
Build Instructions (Linux / macOS)
mkdir build
cd build
cmake ..
make
./DeviceMonitorDashboard
Code Architecture

The project follows a layered architecture that separates the user interface from the data and simulation logic.

This keeps the code modular and easier to extend.

Main Components
1. MainWindow (UI Layer)

Responsible for:

• Rendering the graphical user interface
• Displaying current sensor readings
• Displaying charts
• Handling user interactions such as buttons and sliders

MainWindow does not generate data. It only displays it.

2. SensorSimulator (Logic Layer)

Responsible for:

• Generating simulated sensor values
• Managing the update timer (1 second interval)
• Updating sensor data objects
• Emitting signals when new data is available

This class acts as the engine of the application.

3. SensorData (Data Layer)

Each sensor is represented by a SensorData object.

Responsibilities:

• Store current sensor value
• Maintain historical values for charting (last 30 samples)
• Store threshold values
• Detect threshold violations

Three instances are created:

• Temperature sensor
• Humidity sensor
• Pressure sensor

Architecture Diagram

<img width="1024" height="1536" alt="Design" src="https://github.com/user-attachments/assets/5a3f25cd-fec0-4c08-949e-2d32f5714cbd" />

Design Patterns Used
Observer Pattern

Qt signals and slots implement the Observer pattern.
SensorSimulator emits a signal when data updates, and MainWindow listens and updates the UI.

Model–View Separation (similar to MVC)

SensorData and SensorSimulator represent the model and logic, while MainWindow acts as the view/controller responsible for displaying data and handling user interaction.

Known Issues

• Sensor data is simulated and not connected to real hardware.
• Chart redraws every update instead of incrementally updating data points.
• No storage for sensor history.

Possible Improvements

If given more time, I would add the following improvements:

• Add unit tests for the sensor logic layer
• Improve chart performance by appending points instead of recreating the chart
• Add theme support (dark mode)
• Support additional sensors through a configurable sensor factory

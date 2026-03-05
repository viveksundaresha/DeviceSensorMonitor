A real-time environmental sensor monitoring application built with C++ and Qt.



\## Features



- Real-time temperature, humidity, and pressure monitoring

- Interactive chart visualization of sensor data

- Configurable alarm thresholds

- Start/stop simulation controls

- Responsive UI with live updates every second



## Building



Requirements:

- Qt 6.10+

- CMake 3.18+

- MinGW (Windows) or GCC (Linux/Mac)



### On Windows (MinGW)

```bash

mkdir build

cd build

cmake .. -G "MinGW Makefiles" -DCMAKE\_PREFIX\_PATH=<Qt Path>

cmake --build .

DeviceMonitorDashboard.exe

```


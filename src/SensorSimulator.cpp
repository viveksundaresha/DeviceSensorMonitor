#include "SensorSimulator.h"
#include <cmath>
#include <QRandomGenerator>

SensorSimulator::SensorSimulator()
    : m_updateCount(0)
    , m_isRunning(false)
{
    connect(&m_updateTimer, &QTimer::timeout, this, &SensorSimulator::updateSensors);
    m_updateTimer.setInterval(1000);
}

SensorSimulator::~SensorSimulator()
{
    if (m_isRunning)
    {
        m_updateTimer.stop();
    }
    qDeleteAll(m_sensors);
    m_sensors.clear();
}

void SensorSimulator::initialize()
{
    qDeleteAll(m_sensors);
    m_sensors.clear();
    
    m_sensors.append(new SensorData("Temperature", "°C"));
    m_sensors.append(new SensorData("Humidity", "%"));
    m_sensors.append(new SensorData("Pressure", "hPa"));
    
    m_sensors[0]->setThreshold(40.0);
    m_sensors[1]->setThreshold(85.0);
    m_sensors[2]->setThreshold(1030.0);
    
    updateSensors();
}

void SensorSimulator::start()
{
    if (!m_isRunning)
    {
        m_isRunning = true;
        m_updateTimer.start();
        emit simulationStateChanged(true);
    }
}

void SensorSimulator::stop()
{
    if (m_isRunning)
    {
        m_isRunning = false;
        m_updateTimer.stop();
        emit simulationStateChanged(false);
    }
}

bool SensorSimulator::isRunning() const
{
    return m_isRunning;
}

SensorData* SensorSimulator::getSensor(int index)
{
    if (index >= 0 && index < m_sensors.size())
    {
        return m_sensors[index];
    }
    return nullptr;
}

int SensorSimulator::getSensorCount() const
{
    return m_sensors.size();
}

void SensorSimulator::updateSensors()
{
    if (m_sensors.isEmpty())
        return;
    
    double temp = generateTemperature();
    double humidity = generateHumidity();
    double pressure = generatePressure();
    
    m_sensors[0]->setValue(temp);
    m_sensors[0]->addDataPoint(temp);
    
    m_sensors[1]->setValue(humidity);
    m_sensors[1]->addDataPoint(humidity);
    
    m_sensors[2]->setValue(pressure);
    m_sensors[2]->addDataPoint(pressure);
    
    m_updateCount++;
    
    emit sensorsUpdated();
}

double SensorSimulator::generateTemperature()
{
    double sine = std::sin(m_updateCount * 2.0 * M_PI / 100.0);
    double noise = (QRandomGenerator::global()->generate() % 100 - 50) / 100.0 * 2.0;
    return 25.0 + sine * 10.0 + noise;
}

double SensorSimulator::generateHumidity()
{
    double sine = std::sin(m_updateCount * 2.0 * M_PI / 120.0 + 1.0);
    double noise = (QRandomGenerator::global()->generate() % 100 - 50) / 100.0 * 2.0;
    double value = 55.0 + sine * 20.0 + noise;
    if (value < 0.0) value = 0.0;
    if (value > 100.0) value = 100.0;
    return value;
}

double SensorSimulator::generatePressure()
{
    double sine = std::sin(m_updateCount * 2.0 * M_PI / 150.0 + 2.0);
    double noise = (QRandomGenerator::global()->generate() % 100 - 50) / 100.0 * 1.0;
    return 1013.0 + sine * 10.0 + noise;
}

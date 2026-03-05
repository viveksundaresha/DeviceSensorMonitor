#include "SensorData.h"

SensorData::SensorData(const QString& name, const QString& unit)
    : m_name(name)
    , m_unit(unit)
    , m_value(20.0)  // Initialize with reasonable default
    , m_threshold(100.0)
{
    m_history.reserve(HISTORY_SIZE);
}

QString SensorData::getName() const
{
    return m_name;
}

QString SensorData::getUnit() const
{
    return m_unit;
}

double SensorData::getValue() const
{
    return m_value;
}

const QVector<double>& SensorData::getHistory() const
{
    return m_history;
}

double SensorData::getThreshold() const
{
    return m_threshold;
}

bool SensorData::isAboveThreshold() const
{
    return m_value > m_threshold;
}

void SensorData::setValue(double value)
{
    m_value = value;
}

void SensorData::setThreshold(double threshold)
{
    m_threshold = threshold;
}

void SensorData::addDataPoint(double value)
{
    m_history.append(value);
    if (m_history.size() > HISTORY_SIZE)
    {
        m_history.removeFirst();
    }
}

void SensorData::clearHistory()
{
    m_history.clear();
}

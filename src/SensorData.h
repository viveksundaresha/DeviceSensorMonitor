#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QString>
#include <QVector>

/**
 * @class SensorData
 * @brief Represents a single sensor's current and historical data
 * 
 * Encapsulates the Model layer for a single sensor. Each SensorData instance
 * maintains its own state independently, allowing multiple sensors to coexist
 * with different thresholds and histories.
 * 
 * Responsibilities:
 * - Store current sensor reading
 * - Maintain circular history of last N readings
 * - Manage threshold for alarm conditions
 * - Provide immutable access to data for UI layer
 * 
 * Thread-Safety: Not thread-safe. Should only be accessed from Qt's main thread.
 */
class SensorData
{
public:
    /**
     * Constructor
     * @param name Sensor name (e.g., "Temperature")
     * @param unit Measurement unit (e.g., "°C")
     */
    SensorData(const QString& name, const QString& unit);
    
    // ===== Query Methods =====
    
    /// @return Sensor's display name
    QString getName() const;
    
    /// @return Measurement unit (e.g., "°C", "%", "hPa")
    QString getUnit() const;
    
    /// @return Most recent sensor reading
    double getValue() const;
    
    /// @return Historical data points (oldest to newest)
    const QVector<double>& getHistory() const;
    
    /// @return Current alarm threshold value
    double getThreshold() const;
    
    /// @return true if current value exceeds threshold
    bool isAboveThreshold() const;
    
    // ===== Modification Methods =====
    
    /// Update the current sensor reading (doesn't add to history)
    void setValue(double value);
    
    /// Change the alarm threshold for this sensor
    void setThreshold(double threshold);
    
    /// Add a new measurement to history, automatically maintains size limit
    void addDataPoint(double value);
    
    /// Clear all historical data
    void clearHistory();
    
    // ===== Constants =====
    
    /// Maximum number of historical points to keep in memory
    static constexpr int HISTORY_SIZE = 30;

private:
    // Member variables follow m_ prefix convention
    QString m_name;           ///< Sensor name
    QString m_unit;           ///< Unit of measurement
    double m_value;           ///< Current reading
    double m_threshold;       ///< Alarm threshold
    QVector<double> m_history; ///< Time-series data for charting
};

#endif // SENSORDATA_H

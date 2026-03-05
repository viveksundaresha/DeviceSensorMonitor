#ifndef SENSORSIMULATOR_H
#define SENSORSIMULATOR_H

#include "SensorData.h"
#include <QObject>
#include <QTimer>
#include <QVector>

/**
 * @class SensorSimulator
 * @brief Manages realistic simulation of environmental sensors
 * 
 * This class is the core of the Model layer. It:
 * - Owns and manages all SensorData instances
 * - Generates realistic readings using sine waves + random noise
 * - Controls the simulation via QTimer (1-second intervals)
 * - Emits signals to notify View of changes (Observer pattern)
 * - Handles pause/resume functionality
 * 
 * Data Generation Strategy:
 * - Each sensor uses a sine wave with different phase
 * - Random noise added for realistic variation
 * - Ranges tuned for realistic environmental values
 * - 100-150 second oscillation period = natural variation
 * 
 * Thread-Safety: Not thread-safe. Designed for Qt's main thread.
 * If multi-threaded access needed, use QThread-based approach.
 */
class SensorSimulator : public QObject
{
    Q_OBJECT

public:
    /// Constructor - initializes but doesn't start simulation
    SensorSimulator();
    
    /// Destructor - cleans up sensors and stops timer
    ~SensorSimulator();
    
    // ===== Lifecycle Methods =====
    
    /**
     * Initialize sensors with default values and thresholds
     * Must be called once before use
     */
    void initialize();
    
    /**
     * Start the simulation timer
     * Begins emitting sensorsUpdated() signals every 1 second
     */
    void start();
    
    /**
     * Stop the simulation timer
     * Stops updates but preserves current data and history
     */
    void stop();
    
    // ===== Query Methods =====
    
    /// @return true if timer is active and generating data
    bool isRunning() const;
    
    /**
     * Get reference to a sensor by index
     * @param index 0=Temperature, 1=Humidity, 2=Pressure
     * @return nullptr if index out of range
     */
    SensorData* getSensor(int index);
    
    /// @return Number of sensors (always 3)
    int getSensorCount() const;

signals:
    /**
     * Emitted whenever sensor data is updated
     * Connected to MainWindow to refresh UI
     */
    void sensorsUpdated();
    
    /**
     * Emitted when simulation state changes
     * Allows MainWindow to update status indicator
     */
    void simulationStateChanged(bool running);

private slots:
    /**
     * Called by QTimer at regular intervals
     * Generates new sensor values and updates history
     */
    void updateSensors();

private:
    // ===== Data Generation Methods =====
    
    /**
     * Generate realistic temperature reading
     * Base: 25°C, Amplitude: ±10°C, Noise: ±2°C
     * Oscillation period: ~100 seconds
     */
    double generateTemperature();
    
    /**
     * Generate realistic humidity reading
     * Base: 55%, Amplitude: ±25%, Noise: ±5%
     * Oscillation period: ~120 seconds
     */
    double generateHumidity();
    
    /**
     * Generate realistic pressure reading
     * Base: 1013 hPa, Amplitude: ±10 hPa, Noise: ±2 hPa
     * Oscillation period: ~150 seconds
     */
    double generatePressure();

private:
    // ===== Member Variables =====
    
    QTimer m_updateTimer;                              ///< Drives data generation at 1Hz
    QVector<SensorData*> m_sensors;                   ///< Sensor instances (owned by this)
    int m_updateCount;                                 ///< Counter for sine wave phase
    bool m_isRunning;                                  ///< Tracks simulation state
    
    // ===== Parameters for Sine Wave Generation =====
    
    static constexpr double TEMPERATURE_BASE = 25.0;
    static constexpr double TEMPERATURE_AMPLITUDE = 10.0;
    static constexpr double TEMPERATURE_NOISE = 2.0;
    
    static constexpr double HUMIDITY_BASE = 55.0;
    static constexpr double HUMIDITY_AMPLITUDE = 25.0;
    static constexpr double HUMIDITY_NOISE = 5.0;
    
    static constexpr double PRESSURE_BASE = 1013.0;
    static constexpr double PRESSURE_AMPLITUDE = 10.0;
    static constexpr double PRESSURE_NOISE = 2.0;
};

#endif // SENSORSIMULATOR_H

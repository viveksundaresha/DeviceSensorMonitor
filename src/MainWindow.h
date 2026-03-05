#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SensorSimulator.h"
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <memory>

class QChart;
class QChartView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onSensorsUpdated();
    void onStartStopClicked();
    void onSensorSelected(int index);
    void onThresholdChanged(int value);
    void onSimulationStateChanged(bool running);

private:
    void setupUI();
    void createChart();
    void updateChart();
    void updateSensorDisplay();
    void updateStatusIndicators();
    QString getWarningStyle(bool aboveThreshold) const;

    std::unique_ptr<SensorSimulator> m_simulator;
    int m_selectedSensorIndex;
    
    QLabel* m_tempLabel;
    QLabel* m_humidityLabel;
    QLabel* m_pressureLabel;
    QLabel* m_tempWarning;
    QLabel* m_humidityWarning;
    QLabel* m_pressureWarning;
    
    QChart* m_chart;
    QChartView* m_chartView;
    
    QPushButton* m_startStopButton;
    QSlider* m_thresholdSlider;
    QComboBox* m_sensorSelector;
    QLabel* m_statusIndicator;
    QLabel* m_statusText;
};

#endif

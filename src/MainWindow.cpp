#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QApplication>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QPainter>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_selectedSensorIndex(0), m_chart(nullptr), m_chartView(nullptr),
      m_tempLabel(nullptr), m_humidityLabel(nullptr), m_pressureLabel(nullptr),
      m_tempWarning(nullptr), m_humidityWarning(nullptr), m_pressureWarning(nullptr),
      m_startStopButton(nullptr), m_thresholdSlider(nullptr), m_sensorSelector(nullptr),
      m_statusIndicator(nullptr), m_statusText(nullptr)
{
    setWindowTitle("Device Monitor Dashboard");
    setGeometry(100, 100, 1200, 700);
    
    m_simulator = std::make_unique<SensorSimulator>();
    m_simulator->initialize();
    
    connect(m_simulator.get(), &SensorSimulator::sensorsUpdated, this, &MainWindow::onSensorsUpdated);
    connect(m_simulator.get(), &SensorSimulator::simulationStateChanged, this, &MainWindow::onSimulationStateChanged);
    
    setupUI();
    createChart();
    updateChart();
    updateSensorDisplay();
    updateStatusIndicators();
    
    m_simulator->start();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    
    QVBoxLayout* leftLayout = new QVBoxLayout();
    
    QLabel* titleLabel = new QLabel("Device Monitor");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    leftLayout->addWidget(titleLabel);
    
    QGroupBox* statusGroup = new QGroupBox("Status", this);
    QVBoxLayout* statusLayout = new QVBoxLayout(statusGroup);
    m_statusIndicator = new QLabel("●");
    m_statusIndicator->setStyleSheet("color: green; font-size: 20px;");
    m_statusText = new QLabel("Running");
    m_statusText->setStyleSheet("font-weight: bold;");
    statusLayout->addWidget(m_statusIndicator);
    statusLayout->addWidget(m_statusText);
    leftLayout->addWidget(statusGroup);
    
    QGroupBox* readingsGroup = new QGroupBox("Current Readings", this);
    QVBoxLayout* readingsLayout = new QVBoxLayout(readingsGroup);
    
    QHBoxLayout* tempLayout = new QHBoxLayout();
    tempLayout->addWidget(new QLabel("Temperature:"));
    m_tempLabel = new QLabel("-- °C");
    m_tempLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    m_tempWarning = new QLabel("");
    tempLayout->addWidget(m_tempLabel);
    tempLayout->addWidget(m_tempWarning);
    tempLayout->addStretch();
    readingsLayout->addLayout(tempLayout);
    
    QHBoxLayout* humLayout = new QHBoxLayout();
    humLayout->addWidget(new QLabel("Humidity:"));
    m_humidityLabel = new QLabel("-- %");
    m_humidityLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    m_humidityWarning = new QLabel("");
    humLayout->addWidget(m_humidityLabel);
    humLayout->addWidget(m_humidityWarning);
    humLayout->addStretch();
    readingsLayout->addLayout(humLayout);
    
    QHBoxLayout* presLayout = new QHBoxLayout();
    presLayout->addWidget(new QLabel("Pressure:"));
    m_pressureLabel = new QLabel("-- hPa");
    m_pressureLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    m_pressureWarning = new QLabel("");
    presLayout->addWidget(m_pressureLabel);
    presLayout->addWidget(m_pressureWarning);
    presLayout->addStretch();
    readingsLayout->addLayout(presLayout);
    
    leftLayout->addWidget(readingsGroup);
    
    QGroupBox* controlsGroup = new QGroupBox("Controls", this);
    QVBoxLayout* controlsLayout = new QVBoxLayout(controlsGroup);
    
    m_startStopButton = new QPushButton("Stop", this);
    m_startStopButton->setMinimumHeight(40);
    connect(m_startStopButton, &QPushButton::clicked, this, &MainWindow::onStartStopClicked);
    controlsLayout->addWidget(m_startStopButton);
    
    controlsLayout->addWidget(new QLabel("Select Sensor for Chart:"));
    m_sensorSelector = new QComboBox(this);
    m_sensorSelector->addItem("Temperature");
    m_sensorSelector->addItem("Humidity");
    m_sensorSelector->addItem("Pressure");
    connect(m_sensorSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onSensorSelected);
    controlsLayout->addWidget(m_sensorSelector);
    
    controlsLayout->addWidget(new QLabel("Adjust Threshold:"));
    m_thresholdSlider = new QSlider(Qt::Horizontal, this);
    m_thresholdSlider->setMinimum(0);
    m_thresholdSlider->setMaximum(100);
    m_thresholdSlider->setValue(50);
    m_thresholdSlider->setTickPosition(QSlider::TicksBelow);
    m_thresholdSlider->setTickInterval(10);
    connect(m_thresholdSlider, &QSlider::valueChanged, this, &MainWindow::onThresholdChanged);
    controlsLayout->addWidget(m_thresholdSlider);
    
    controlsLayout->addStretch();
    leftLayout->addWidget(controlsGroup);
    leftLayout->addStretch();
    
    QWidget* leftPanel = new QWidget();
    leftPanel->setLayout(leftLayout);
    leftPanel->setMaximumWidth(300);
    mainLayout->addWidget(leftPanel);
    
    m_chartView = new QChartView(this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(m_chartView, 1);
    
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
}

void MainWindow::createChart()
{
    m_chart = new QChart();
    m_chart->setTitle("Sensor Data Over Time");
    m_chart->setAnimationOptions(QChart::SeriesAnimations);
    m_chartView->setChart(m_chart);
}

void MainWindow::updateChart()
{
    if (!m_chart) return;
    
    m_chart->removeAllSeries();
    const auto& axes = m_chart->axes();
    for (auto axis : axes) {
        m_chart->removeAxis(axis);
        delete axis;
    }
    
    SensorData* sensor = m_simulator->getSensor(m_selectedSensorIndex);
    if (!sensor) return;
    
    auto series = new QLineSeries();
    series->setName(sensor->getName());
    const auto& history = sensor->getHistory();
    for (int i = 0; i < history.size(); ++i)
        series->append(i, history[i]);
    
    m_chart->addSeries(series);
    
    auto axisX = new QValueAxis();
    axisX->setTitleText("Time (seconds)");
    axisX->setRange(0, SensorData::HISTORY_SIZE);
    m_chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    auto axisY = new QValueAxis();
    axisY->setTitleText(QString("%1 (%2)").arg(sensor->getName(), sensor->getUnit()));
    if (m_selectedSensorIndex == 0) axisY->setRange(5, 45);
    else if (m_selectedSensorIndex == 1) axisY->setRange(0, 100);
    else if (m_selectedSensorIndex == 2) axisY->setRange(990, 1035);
    
    m_chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
}

void MainWindow::updateSensorDisplay()
{
    if (m_tempLabel && m_simulator->getSensor(0))
        m_tempLabel->setText(QString::number(m_simulator->getSensor(0)->getValue(), 'f', 1) + " °C");
    if (m_humidityLabel && m_simulator->getSensor(1))
        m_humidityLabel->setText(QString::number(m_simulator->getSensor(1)->getValue(), 'f', 1) + " %");
    if (m_pressureLabel && m_simulator->getSensor(2))
        m_pressureLabel->setText(QString::number(m_simulator->getSensor(2)->getValue(), 'f', 1) + " hPa");
}

void MainWindow::updateStatusIndicators()
{
    if (m_tempWarning && m_simulator->getSensor(0)) {
        m_tempWarning->setText(m_simulator->getSensor(0)->isAboveThreshold() ? "⚠" : "");
        m_tempWarning->setStyleSheet(getWarningStyle(m_simulator->getSensor(0)->isAboveThreshold()));
    }
    if (m_humidityWarning && m_simulator->getSensor(1)) {
        m_humidityWarning->setText(m_simulator->getSensor(1)->isAboveThreshold() ? "⚠" : "");
        m_humidityWarning->setStyleSheet(getWarningStyle(m_simulator->getSensor(1)->isAboveThreshold()));
    }
    if (m_pressureWarning && m_simulator->getSensor(2)) {
        m_pressureWarning->setText(m_simulator->getSensor(2)->isAboveThreshold() ? "⚠" : "");
        m_pressureWarning->setStyleSheet(getWarningStyle(m_simulator->getSensor(2)->isAboveThreshold()));
    }
}

QString MainWindow::getWarningStyle(bool aboveThreshold) const
{
    return aboveThreshold ? "color: red; font-size: 16px; font-weight: bold;" : "color: green; font-size: 16px;";
}

void MainWindow::onSensorsUpdated()
{
    updateSensorDisplay();
    updateStatusIndicators();
    updateChart();
}

void MainWindow::onStartStopClicked()
{
    if (m_simulator->isRunning()) m_simulator->stop();
    else m_simulator->start();
}

void MainWindow::onSensorSelected(int index)
{
    m_selectedSensorIndex = index;
    updateChart();
}

void MainWindow::onThresholdChanged(int value)
{
    double threshold = 0.0;
    if (m_selectedSensorIndex == 0) threshold = 10.0 + (value / 100.0) * 40.0;
    else if (m_selectedSensorIndex == 1) threshold = (value / 100.0) * 100.0;
    else if (m_selectedSensorIndex == 2) threshold = 990.0 + (value / 100.0) * 45.0;
    
    SensorData* sensor = m_simulator->getSensor(m_selectedSensorIndex);
    if (sensor) {
        sensor->setThreshold(threshold);
        updateStatusIndicators();
    }
}

void MainWindow::onSimulationStateChanged(bool running)
{
    if (running) {
        m_statusIndicator->setText("●");
        m_statusIndicator->setStyleSheet("color: green; font-size: 20px;");
        m_statusText->setText("Running");
        m_startStopButton->setText("Stop");
    } else {
        m_statusIndicator->setText("●");
        m_statusIndicator->setStyleSheet("color: red; font-size: 20px;");
        m_statusText->setText("Stopped");
        m_startStopButton->setText("Start");
    }
}

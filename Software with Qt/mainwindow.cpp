#include "mpvwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "arduinohelper.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ArduinoHelper arduino = ArduinoHelper();

    auto comports = arduino.ListCOMPorts();

    for(int i = 0; i < comports.count(); i++)
    {
        ui->COMPort_selector->addItem(comports[i]);
    }



    rtsp_url = ui->rtsp_input->text();
    connect(ui->VideoWidget, SIGNAL(addPlotPoint(float)), ui->liveLinePlot, SLOT(addPoint(float)));
    //connect(m_slider, SIGNAL(sliderMoved(int)), SLOT(seek(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_plotbutton_clicked()
{
    ui->liveLinePlot->addPoint(rand());
}

void MainWindow::on_rtsp_input_textChanged(const QString &arg1)
{
    rtsp_url = arg1;
}

void MainWindow::on_rtsp_connect_clicked()
{
    rtsp_connect_button_state = !rtsp_connect_button_state;

    if (rtsp_connect_button_state){
        qInfo() << "Connection to RTSP Stream: " << rtsp_url;
        ui->VideoWidget->command(QStringList() << "loadfile" << rtsp_url);
        ui->rtsp_connect->setText(QString("Disconnect"));
    }
    else {
        qInfo() << "Disconnection from RTSP Stream";
        ui->VideoWidget->setProperty("pause", true);
    }
}

/*
void MainWindow::openMedia()
{
    //QString file = QFileDialog::getOpenFileName(0, "Open a video");
    //if (file.isEmpty())
    //    return;
    m_mpv->command(QStringList() << "loadfile" << "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov");
}

void MainWindow::seek(int pos)
{
    m_mpv->command(QVariantList() << "seek" << pos << "absolute");
}

void MainWindow::pauseResume()
{
    const bool paused = m_mpv->getProperty("pause").toBool();
    m_mpv->setProperty("pause", !paused);
}

void MainWindow::setSliderRange(int duration)
{
    //m_slider->setRange(0, duration);
}
*/


void MainWindow::on_arduino_connect_clicked()
{

}

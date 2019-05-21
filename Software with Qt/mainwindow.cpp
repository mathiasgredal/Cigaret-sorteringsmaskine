#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProgressDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    arduino = ArduinoHelper();

    auto comports = arduino.ListCOMPorts();

    for(int i = 0; i < comports.count(); i++)
    {
        ui->COMPort_selector->addItem(comports[i]);
    }

    rtsp_url = ui->rtsp_input->text();
    ui->Neural_Network_ButtonLayout->setAlignment(Qt::AlignTop);
    connect(ui->VideoWidget, SIGNAL(addPlotPoint(float)), ui->liveLinePlot, SLOT(addPoint(float)));
    connect(ui->Neural_Network_Output, SIGNAL(Image_Processed(QString)),this, SLOT(ImageFinishedProcessing(QString)));
    this->repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_rtsp_input_textChanged(const QString &arg1)
{
    rtsp_url = arg1;
}

void MainWindow::on_rtsp_connect_clicked()
{
    this->repaint();

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

void MainWindow::on_arduino_connect_clicked()
{
    qInfo() << "Connecting to arduino";
    ArduinoHelper::ConnectionErrors errors = arduino.ArduinoConnect(ui->BaudRate_selector->currentText().toInt(), ui->COMPort_selector->currentText());

    switch (errors) {
        case ArduinoHelper::ConnectionErrors::PORT_NOT_FOUND:
            qInfo() << "Couldn't find port";
            break;
        case ArduinoHelper::ConnectionErrors::UNABLE_TO_OPEN_PORT:
            qInfo() << "Couldn't open port";
            break;
        case ArduinoHelper::ConnectionErrors::SUCCES:
            qInfo() << "Succesfully opened port";
            break;
    }
}

void MainWindow::on_Arduino_Start_clicked()
{
    arduino.SendString("e");
}

void MainWindow::on_Arduino_Stop_clicked()
{
    arduino.SendString("r");
}

void MainWindow::on_Arduino_Push_clicked()
{
    arduino.SendString("t");
}

void MainWindow::on_refLineSlider_valueChanged(int value)
{
    ui->liveLinePlot->SetRefLine(value);
    ui->refLineInput->setText(QString::number(value));
}

void MainWindow::on_Start_Neural_Network_clicked()
{
    if(!ui->Neural_Network_Output->isRunning)
    {
        ui->Neural_Network_Output->startServer();
        ui->Start_Neural_Network->setText("Stop Neural Engine");
    }
    else{
        ui->Neural_Network_Output->stopServer();
        ui->Start_Neural_Network->setText("Start Neural Engine");
    }
}

void MainWindow::on_Scan_Image_clicked()
{
    ui->Neural_Network_Output->append("Scanning Image...");
    if (ui->Neural_Network_Output->isRunning){
        image_preview = ui->VideoWidget->screenCapture().copy();

        ui->Neural_Network_Output->ProcessImage(image_preview);
    }
    else {
        ui->Neural_Network_Output->append("Neural Engine not running");
    }
}

void MainWindow::on_Draw_Preview_clicked()
{
    //ui->Neural_Networ_Output_Preview->DrawPreview(QImage("/Users/mathiasgredal/Desktop/sorter firmware/NNServer/temp.JPG"), "{\"boundingboxes\": [[489, 482, 755, 963]], \"confidence\": [0.9996272325515747]}");
    image_preview = ui->VideoWidget->screenCapture().copy();

    ui->Neural_Networ_Output_Preview->DrawPreview(image_preview,"");
}

void MainWindow::ImageFinishedProcessing(QString ret)
{
    ui->Neural_Networ_Output_Preview->DrawPreview(image_preview,ret);
    qInfo() << "This worked " << ret;
}

void MainWindow::Run_Automatic_Sorting(){
    msgBox->setText("Running...");
    //msgBox->close();
    qInfo() << "Scanning Conveyor";
}

bool MainWindow::CheckNeuralEngine()
{
    return true;
}

bool MainWindow::CheckCameraStream()
{
    return true;
}

bool MainWindow::CheckArduino()
{
    return true;
}

void MainWindow::DisableUI()
{

}

void MainWindow::on_Sorting_Start_Button_clicked()
{
    // Open QMessageBox
    msgBox = new QMessageBox(this);

    msgBox->setText("Checking Neural Engine...");
    if(!CheckNeuralEngine()){
        msgBox->setText("Neural Engine Not Online!");
        QPushButton *abortButton = msgBox->addButton(tr("Abort"), QMessageBox::ActionRole);
        if(msgBox->clickedButton() == abortButton)
            return;
    } else if(!CheckCameraStream())
    {
        msgBox->setText("Camera Not Connected!");
        QPushButton *abortButton = msgBox->addButton(tr("Abort"), QMessageBox::ActionRole);
        if(msgBox->clickedButton() == abortButton)
            return;
    } else if(!CheckArduino())
    {
        msgBox->setText("Arduino Not Connected!");
        QPushButton *abortButton = msgBox->addButton(tr("Abort"), QMessageBox::ActionRole);
        if(msgBox->clickedButton() == abortButton)
            return;
    }
    else {
        // Disable all UI, such that the user doesn't break machine, mid operation
        DisableUI();

        // Create the final button

        //msgBox->setText("Press button to activate automatic conveyor scanning");
        QPushButton *connectButton = msgBox->addButton(tr("ACTIVATE AUTOMATIC SCANNING"), QMessageBox::ActionRole);

        //this->update();
        // Prevent button from closing to modal
        connectButton->disconnect();

        // Create signal for buttonpress
        connect(connectButton, SIGNAL(clicked()), this, SLOT(Run_Automatic_Sorting()));
    }


    msgBox->exec();
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

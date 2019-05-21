#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "arduinohelper.h"
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();    

private slots:
    void on_rtsp_input_textChanged(const QString &arg1);

    void on_rtsp_connect_clicked();

    void on_arduino_connect_clicked();

    void on_Arduino_Start_clicked();

    void on_Arduino_Stop_clicked();

    void on_Arduino_Push_clicked();

    void on_refLineSlider_valueChanged(int value);

    void on_Start_Neural_Network_clicked();

    void on_Scan_Image_clicked();

    void on_Draw_Preview_clicked();

    void ImageFinishedProcessing(QString ret);

    void on_Sorting_Start_Button_clicked();

    void Run_Automatic_Sorting();

private:
    Ui::MainWindow *ui;

    QMessageBox* msgBox;
    bool CheckNeuralEngine();
    bool CheckCameraStream();
    bool CheckArduino();
    void DisableUI();

    QString rtsp_url;
    bool rtsp_connect_button_state = false;

    ArduinoHelper arduino;

    QImage image_preview;
};

#endif // MAINWINDOW_H

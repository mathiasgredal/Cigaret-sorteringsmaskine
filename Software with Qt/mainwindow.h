#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include "mpvwidget.h"
#include "qcustomplot.h"

class MpvWidget;
class QCustomPlot;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    /*
public Q_SLOTS:
    void openMedia();
    void seek(int pos);
    void pauseResume();

    void on_pushButton_pressed();
private Q_SLOTS:
    void setSliderRange(int duration);
    void on_pushButton_clicked();
*/

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_plotbutton_clicked();

    void on_rtsp_input_textChanged(const QString &arg1);

    void on_rtsp_connect_clicked();

    void on_arduino_connect_clicked();

private:
    Ui::MainWindow *ui;
    QString rtsp_url;
    bool rtsp_connect_button_state = false;
    //MpvWidget *m_mpv;
};

#endif // MAINWINDOW_H

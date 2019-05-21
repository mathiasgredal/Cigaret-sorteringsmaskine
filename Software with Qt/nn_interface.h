#ifndef NN_INTERFACE_H
#define NN_INTERFACE_H
#include <QtCore>
#include <QTextBrowser>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class NN_Interface : public QTextBrowser
{
    Q_OBJECT
public:
    explicit NN_Interface(QWidget *parent = nullptr);
    void startServer();
    void ProcessImage(QImage image);
    void stopServer();
    bool isRunning = false;

private:
    QProcess *cmd;
    QNetworkAccessManager *manager;

signals:
    void proccessFinished(int exitCode, QProcess::ExitStatus status);
    void Image_Processed(QString ret);

public slots:
    void readCommand();
    void stopCommand(int exitCode, QProcess::ExitStatus exitStatus);

private slots:
    void error(QProcess::ProcessError error);
    void stateChanged(QProcess::ProcessState state);
};

#endif // NN_INTERFACE_H

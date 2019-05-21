#include "nn_interface.h"

NN_Interface::NN_Interface(QWidget *parent) : QTextBrowser(parent)
{
    manager = new QNetworkAccessManager(this);

    cmd = new QProcess(this);
    auto appPath = QDir(qApp->applicationDirPath());
    // Go 3 directories up
    appPath.cdUp(); appPath.cdUp(); appPath.cdUp();

    cmd->setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    cmd->setWorkingDirectory(appPath.path());
    cmd->setProcessChannelMode(QProcess::MergedChannels);
    // process has some data to read
    connect(cmd, SIGNAL (readyReadStandardOutput()), this, SLOT (readCommand()));
    //process finished
    connect(cmd, SIGNAL (finished(int, QProcess::ExitStatus)), this, SLOT (stopCommand(int, QProcess::ExitStatus)));
}

void NN_Interface::startServer()
{
    this->append("Starting Neural Engine...");
    auto appPath = QDir(qApp->applicationDirPath());
    // Go 3 directories up
    appPath.cdUp(); appPath.cdUp(); appPath.cdUp();

    QString graphPath = appPath.path() + "/mask_rcnn_balloon_0030.h5";
    QString serverPath = appPath.path() + "/server.py";


    cmd->start("bash -c \"/usr/local/bin/python3 server.py mask_rcnn_balloon_0030.h5\"");
    this->isRunning = true;
    //cmd->start("echo $PATH");
    // ??? the best way to continue here???
}

void NN_Interface::stopServer(){
    cmd->kill();
    this->isRunning = false;
}

void NN_Interface::readCommand(){
    this->moveCursor(QTextCursor::End);
    this->textCursor().insertText(cmd->readAll());
}
void NN_Interface::stopCommand(int exitCode, QProcess::ExitStatus exitStatus){
    this->append(cmd->readAll());
    this->append("cmd finished");
    this->append(QString::number(exitCode));
    this->isRunning = false;
}

void NN_Interface::error(QProcess::ProcessError error)
{
    qInfo() <<"Error" << error;
}

void NN_Interface::stateChanged(QProcess::ProcessState state)
{
    qInfo() << "Process::stateChanged" << state;
}

void NN_Interface::ProcessImage(QImage image)
{

    qInfo() << "Reqesting file";

    // Write image to disk
    auto appPath = QDir(qApp->applicationDirPath()); // Generate path to our project, so the server knows where to find it
    appPath.cdUp(); appPath.cdUp(); appPath.cdUp(); // Go 3 directories up, this might be exclusive to mac os, bcs. bundles
    image.save(appPath.path()+"/temp.jpg","JPG");

    // Send request to server
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl("http://127.0.0.1:5000/scanImage")));

    // When we are done processing image, this lambda function will get called
    reply->connect(reply,&QNetworkReply::finished,[=](){
        if (reply->error())
        {
            qDebug() << reply->errorString();
            return;
        }

        // Our answer is a JSON, we need to process it
        QString answer = reply->readAll();
        emit Image_Processed(answer);
        //qInfo() << answer;

        reply->deleteLater();
    });
}


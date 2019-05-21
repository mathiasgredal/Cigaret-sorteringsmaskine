#ifndef OUTPUT_NEURAL_NETWORK_H
#define OUTPUT_NEURAL_NETWORK_H

#include <QWidget>
#include <QTextBrowser>

class Output_Neural_Network : public QTextBrowser
{
    Q_OBJECT
public:
    explicit Output_Neural_Network(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // OUTPUT_NEURAL_NETWORK_H

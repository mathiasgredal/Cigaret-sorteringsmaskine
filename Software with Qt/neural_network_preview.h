#ifndef NEURAL_NETWORK_PREVIEW_H
#define NEURAL_NETWORK_PREVIEW_H

#include <QWidget>
#include <QPainter>
#include <QtCore>


class Neural_Network_Preview : public QWidget
{
    Q_OBJECT
public:
    explicit Neural_Network_Preview(QWidget *parent = nullptr);
    void DrawPreview(QImage imagePreview, QString drawingData);
    QImage backgroundImage;

private:
    QVector<QRect> boxes;
    QVector<float> confidenceValues;
    float map(float x, float in_min, float in_max, float out_min, float out_max);
protected:
    void paintEvent(QPaintEvent *event);
signals:

public slots:
};

#endif // NEURAL_NETWORK_PREVIEW_H

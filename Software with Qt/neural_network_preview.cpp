#include "neural_network_preview.h"

Neural_Network_Preview::Neural_Network_Preview(QWidget *parent) : QWidget(parent)
{

}

float Neural_Network_Preview::map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Neural_Network_Preview::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(!backgroundImage.isNull()){
        painter.drawImage(0,0,backgroundImage);
    }

    for(int i = 0; i < boxes.length(); i++)
    {
        painter.setPen(Qt::white);
        painter.drawRect(boxes[i]);
        painter.drawText(boxes[i].topLeft()+QPoint(0,-3), QString::number(confidenceValues[i]));
    }
}

void Neural_Network_Preview::DrawPreview(QImage imagePreview, QString drawingData)
{
    int imageHeight = imagePreview.height();
    int imageWidth = imagePreview.width();
    backgroundImage = imagePreview.scaled(this->size().width()*2,this->size().height()*2);

    boxes.clear();
    confidenceValues.clear();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(drawingData.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonArray boundingBoxes = jsonObject["boundingboxes"].toArray();
    QJsonArray confidenceVals = jsonObject["confidence"].toArray();

    foreach(const QJsonValue & value, confidenceVals)
    {
        confidenceValues.append(value.toDouble());
    }

    foreach (const QJsonValue & boundingBox, boundingBoxes)
    {
        auto boundingBoxArr = boundingBox.toArray();
        int y1 = boundingBoxArr[0].toInt();
        int x1 = boundingBoxArr[1].toInt();
        int y2 = boundingBoxArr[2].toInt();
        int x2 = boundingBoxArr[3].toInt();


        y1 = map(y1,0,imageHeight,0,this->size().height());
        x1 = map(x1,0,imageWidth,0,this->size().width());
        y2 = map(y2,0,imageHeight,0,this->size().height());
        x2 = map(x2,0,imageWidth,0,this->size().width());

        boxes.append(QRect(QPoint(x1, y1), QPoint(x2, y2)));
    }



    this->repaint();
}

#include "avgimgplot.h"

AvgImgPlot::AvgImgPlot(QWidget *parent)
  : QWidget(parent)
{

    starttime = (QDateTime::currentMSecsSinceEpoch()*0.01);
    dataPoints.append(QPointF(0,0));
}

AvgImgPlot::~AvgImgPlot()
{
    // deletion

}

float AvgImgPlot::map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void AvgImgPlot::paintEvent(QPaintEvent *event)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);

    //create a white pen that has solid line
    //and the width is 2.
    QPen myPen(Qt::white, 1, Qt::SolidLine);
    painter.setPen(myPen);

    painter.drawLines(dataPoints);

    QPen refPen(Qt::green, 1, Qt::SolidLine);
    painter.setPen(refPen);
    painter.drawLine(0, map(refLine,0,255,0,this->height()), this->width(), map(refLine,0,255,0,this->height()));
}
void AvgImgPlot::SetRefLine(float refColor){
    refLine = refColor;
}


void AvgImgPlot::addPoint(float meanColor){
    float timeSinceStart = (QDateTime::currentMSecsSinceEpoch()*0.01)-starttime;

    float xPos = map(timeSinceStart, timeSinceStart-(xAxisSpan*0.5), timeSinceStart+(xAxisSpan*0.5), 0,this->width());
    float yPos = map(meanColor, 0,yAxisSpan,0,this->height());

    float xPosDelta = (timeSinceStart-lasTimeSinceStart)*graphSpeed;

    for (int i = 0; i < dataPoints.length(); i++) {
        dataPoints[i].setX(dataPoints[i].x() - xPosDelta);
    }

    dataPoints.append(QPointF(xPos, yPos));
    dataPoints.append(QPointF(xPos, yPos));


    this->update();

    lasTimeSinceStart = timeSinceStart;
}

void AvgImgPlot::clearGraph(){
    /*
    this->graph(0)->data().clear();
    this->replot();
    */
}


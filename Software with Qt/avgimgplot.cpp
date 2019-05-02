#include "avgimgplot.h"


AvgImgPlot::AvgImgPlot(QWidget *parent)
  : QCustomPlot(parent)
{
    this->setOpenGl(true, 2);
    this->addGraph();
    this->graph(0)->setPen(QPen(QColor(255, 50, 0)));
    // give the axes some labels:
    this->xAxis->setLabel("Tid (sek)");
    this->yAxis->setLabel("Lysstyrke af billede (0-255)");

    // Add time axis
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s");
    this->xAxis->setTicker(timeTicker);
    this->axisRect()->setupFullAxesBox();
    this->yAxis->setRange(0, 255);

    // Draw
    this->replot();
}

AvgImgPlot::~AvgImgPlot()
{
    // deletion

}

void AvgImgPlot::addPoint(float meanColor){

    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.05) // at most add point every 50 ms
    {
      // add data to lines:
      this->graph(0)->addData(key, meanColor);
      lastPointKey = key;
      this->xAxis->setRange(key, 8, Qt::AlignRight);
      this->replot();
    }


    //qInfo() << meanColor;
}

void AvgImgPlot::clearGraph(){
    this->graph(0)->data().clear();
    this->replot();
}


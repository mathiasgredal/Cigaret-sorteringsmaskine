#ifndef AVGIMGPLOT_H
#define AVGIMGPLOT_H

#include "qcustomplot.h"

class AvgImgPlot : public QCustomPlot
{
    Q_OBJECT
public:
    explicit AvgImgPlot(QWidget* parent = 0);
    ~AvgImgPlot();
    void clearGraph();

public slots:
    void addPoint(float meanColor);

private:
    QVector<float> x_times;
    QVector<float> y_colors;

};

#endif // AVGIMGPLOT_H

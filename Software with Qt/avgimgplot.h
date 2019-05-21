#ifndef AVGIMGPLOT_H
#define AVGIMGPLOT_H

#include <QPainter>
#include <QWidget>
#include <QDateTime>

class AvgImgPlot : public QWidget
{
    Q_OBJECT
public:
    explicit AvgImgPlot(QWidget* parent = nullptr);
    ~AvgImgPlot();
    void clearGraph();

public slots:
    void addPoint(float meanColor);
    void SetRefLine(float refColor);

protected:
    void paintEvent(QPaintEvent *event);

private:
    float map(float x, float in_min, float in_max, float out_min, float out_max);
    QVector<QPointF> dataPoints;
    qint64 starttime;
    const float graphSpeed = 2;
    const float xAxisSpan = 20;
    const float yAxisSpan = 255;
    QPen outlinePen;
    float refLine = 100;
    float lasTimeSinceStart;
};

#endif // AVGIMGPLOT_H

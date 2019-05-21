#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QtWidgets/QOpenGLWidget>
#include <mpv/client.h>
#include <mpv/opengl_cb.h>
#include <mpv/qthelper.hpp>

class MpvWidget Q_DECL_FINAL: public QOpenGLWidget
{
    Q_OBJECT
public:
    int framenum = 0;
    int videoWidth = 0;
    int videoHeight = 0;

    MpvWidget(QWidget *parent = nullptr, Qt::WindowFlags f = nullptr);
    ~MpvWidget() override;
    void command(const QVariant& params);
    void setProperty(const QString& name, const QVariant& value);
    QVariant getProperty(const QString& name) const;
    QSize sizeHint() const override { return QSize(480, 270);}
    void stopStream();
    QImage screenCapture();

private:
    void handle_mpv_event(mpv_event *event);
    static void on_update(void *ctx);
    mpv::qt::Handle mpv;
    mpv_opengl_cb_context *mpv_gl;

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

signals:
    void durationChanged(int value);
    void positionChanged(int value);
    void addPlotPoint(float meanColor);

private slots:
    void swapped();
    void on_mpv_events();
    void maybeUpdate();
};



#endif // PLAYERWINDOW_H

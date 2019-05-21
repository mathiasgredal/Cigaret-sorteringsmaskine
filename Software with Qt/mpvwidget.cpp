#include "mpvwidget.h"
#include <QtGui/QOpenGLContext>



static void wakeup(void *ctx)
{
    QMetaObject::invokeMethod(static_cast<MpvWidget*>(ctx), "on_mpv_events", Qt::QueuedConnection);
}

static void *get_proc_address(void *ctx, const char *name) {
    Q_UNUSED(ctx);
    QOpenGLContext *glctx = QOpenGLContext::currentContext();
    if (!glctx)
        return nullptr;
    return (void *)glctx->getProcAddress(QByteArray(name));
}

MpvWidget::MpvWidget( QWidget *parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f)
{
    mpv = mpv::qt::Handle::FromRawHandle(mpv_create());
    if (!mpv)
        throw std::runtime_error("could not create mpv context");

    //mpv_set_option_string(mpv, "terminal", "yes");
    //mpv_set_option_string(mpv, "msg-level", "all=v");
    if (mpv_initialize(mpv) < 0)
        throw std::runtime_error("could not initialize mpv context");

    // Make use of the MPV_SUB_API_OPENGL_CB API.
    mpv::qt::set_option_variant(mpv, "vo", "opengl-cb");

    mpv::qt::set_option_variant(mpv, "hwdec", "auto");
    //mpv::qt::set_option_variant(mpv, "profile", "low-latency");
    //mpv::qt::set_option_variant(mpv, "opengl-glfinish", "yes");
    //mpv::qt::set_option_variant(mpv, "opengl-swapinterval", "0");
    //mpv::qt::set_option_variant(mpv, "fps", "30");
    //mpv::qt::set_option_variant(mpv, "untimed", "");

    mpv_gl = static_cast<mpv_opengl_cb_context *>(mpv_get_sub_api(mpv, MPV_SUB_API_OPENGL_CB));
    if (!mpv_gl)
        throw std::runtime_error("OpenGL not compiled in");
    mpv_opengl_cb_set_update_callback(mpv_gl, MpvWidget::on_update, static_cast<void *>(this));
    connect(this, SIGNAL(frameSwapped()), SLOT(swapped()));

    mpv_set_wakeup_callback(mpv, wakeup, this);
}

MpvWidget::~MpvWidget()
{
    makeCurrent();
    if (mpv_gl)
        mpv_opengl_cb_set_update_callback(mpv_gl, nullptr, nullptr);
    // Until this call is done, we need to make sure the player remains
    // alive. This is done implicitly with the mpv::qt::Handle instance
    // in this class.
    mpv_opengl_cb_uninit_gl(mpv_gl);
}

QImage MpvWidget::screenCapture()
{
    mpv_opengl_cb_draw(mpv_gl, defaultFramebufferObject(),640.0*2 ,-480*2);

    return QOpenGLWidget::grabFramebuffer().convertToFormat(QImage::Format_RGB888);
}

void MpvWidget::resizeEvent(QResizeEvent *event)
{
    event->accept();

    //float aspectRatio = 640.0/480.0;
    //QWidget::resize(event->size().height()*aspectRatio,event->size().height());
    QWidget::resize(640,480);
}

void MpvWidget::command(const QVariant& params)
{
    mpv::qt::command_variant(mpv, params);
}

void MpvWidget::setProperty(const QString& name, const QVariant& value)
{
    mpv::qt::set_property_variant(mpv, name, value);
}

QVariant MpvWidget::getProperty(const QString &name) const
{
    return mpv::qt::get_property_variant(mpv, name);
}

void MpvWidget::initializeGL()
{
    int r = mpv_opengl_cb_init_gl(mpv_gl, nullptr, get_proc_address, nullptr);
    if (r < 0)
        throw std::runtime_error("could not initialize OpenGL");
}

void MpvWidget::stopStream(){
    makeCurrent();
    if (mpv_gl)
        mpv_opengl_cb_set_update_callback(mpv_gl, nullptr, nullptr);

    mpv_opengl_cb_uninit_gl(mpv_gl);
}

void MpvWidget::paintGL()
{
    mpv_opengl_cb_draw(mpv_gl, defaultFramebufferObject(),640.0*2 ,-480*2);

    framenum++;

    if(framenum > 3){
        framenum = 0;
        // Calculate mean color
        auto frame = QOpenGLWidget::grabFramebuffer();
        auto pixelData = frame.convertToFormat(QImage::Format_RGB888);
        long int avg = 0;
        videoWidth = pixelData.width();
        videoHeight = pixelData.height();

        // TODO: stop using scanlines, find a way to use pixeldata.constBits()
        for (int i = 0; i < frame.height(); i += 1)
        {

            const uchar *scanLine = pixelData.constScanLine(i);
            avg += static_cast<int>(scanLine[0]);
            avg += static_cast<int>(scanLine[1]);
            avg += static_cast<int>(scanLine[2]);
        }
        auto avgFinal = static_cast<float>(avg)/(frame.height() * 3);


        Q_EMIT addPlotPoint(avgFinal);
        //qInfo() << ((float)avg)/(frame.height() * 3);
    }
}

void MpvWidget::swapped()
{
    mpv_opengl_cb_report_flip(mpv_gl, 0);
}

void MpvWidget::on_mpv_events()
{
    // Process all events, until the event queue is empty.
    while (mpv) {
        mpv_event *event = mpv_wait_event(mpv, 0);
        if (event->event_id == MPV_EVENT_NONE) {
            break;
        }
        handle_mpv_event(event);
    }
}

void MpvWidget::handle_mpv_event(mpv_event *event)
{
    switch (event->event_id) {
    default: ;
        // Ignore uninteresting or unknown events.
    }

}

// Make Qt invoke mpv_opengl_cb_draw() to draw a new/updated video frame.
void MpvWidget::maybeUpdate()
{
    // If the Qt window is not visible, Qt's update() will just skip rendering.
    // This confuses mpv's opengl-cb API, and may lead to small occasional
    // freezes due to video rendering timing out.
    // Handle this by manually redrawing.
    // Note: Qt doesn't seem to provide a way to query whether update() will
    //       be skipped, and the following code still fails when e.g. switching
    //       to a different workspace with a reparenting window manager.
    if (window()->isMinimized()) {
        makeCurrent();
        paintGL();
        context()->swapBuffers(context()->surface());
        swapped();
        doneCurrent();
    } else {
        update();
    }
}

void MpvWidget::on_update(void *ctx)
{
    QMetaObject::invokeMethod(static_cast<MpvWidget*>(ctx), "maybeUpdate");
}

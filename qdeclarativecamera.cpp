#include <QGraphicsVideoItem>
#include <QtCore/qmath.h>

#include "qdeclarativecamera.h"

QDeclarativeCamera::QDeclarativeCamera(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    camera_(0),
    viewfinder_(0)
{
    firstCamera = true;
    recorded = false;
    viewfinder_ = new QGraphicsVideoItem(this);
    connect(viewfinder_, SIGNAL(nativeSizeChanged(QSizeF)), this, SLOT(viewfinderSizeChanged(QSizeF)));
    //connect(mediaRecorder_, SIGNAL(durationChanged(qint64)), this, SLOT(durationChangedFunc(qint64)));
    viewfinder_->setSize(geometry.size());
    //toggleCamera();
}

QDeclarativeCamera::~QDeclarativeCamera()
{
    camera_->unload();
    delete viewfinder_;
    delete camera_;
}

void QDeclarativeCamera::launchMainCamera()
{
    toggleCamera(true);
}

void QDeclarativeCamera::launchFrontCamera()
{
    toggleCamera(false);
}

void QDeclarativeCamera::toggleCamera(bool mainCamera)
{
    delete camera_;
    delete capture;

    if(mainCamera)
        camera_ = new QCamera("primary");
    else
        camera_ = new QCamera("secondary");

    camera_->setViewfinder(viewfinder_);
    focus_ = camera_->focus();
    firstCamera = false;
    capture = new QCameraImageCapture(camera_);
    camera_->setCaptureMode(QCamera::CaptureStillImage);
    QImageEncoderSettings cameraSettings = capture->encodingSettings();
    capture->setEncodingSettings(cameraSettings);
    setPointModeFocus();
    qDebug() << "SUP: " << focus_->isFocusPointModeSupported(QCameraFocus::FocusPointCustom);

    connect(camera_, SIGNAL(lockStatusChanged(QCamera::LockStatus,QCamera::LockChangeReason)), this, SLOT(takePhotoFull(QCamera::LockStatus,QCamera::LockChangeReason)));
    connect(focus_, SIGNAL(focusZonesChanged()), this, SLOT(signalFocusGained()));

#ifdef DEBUG
    qDebug() << "ERROR: " << camera_->errorString();
#endif
}


void QDeclarativeCamera::viewfinderSizeChanged(const QSizeF& size)
{
    setImplicitWidth(size.width());
    setImplicitHeight(size.height());
}

void QDeclarativeCamera::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    geometry = newGeometry;
    viewfinder_->setSize(geometry.size());
    QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);
}

void QDeclarativeCamera::setAspectRatio(const Qt::AspectRatioMode &aspectRatio)
{
    if(aspectRatio == Qt::IgnoreAspectRatio)
        viewfinder_->setAspectRatioMode(aspectRatio);
    else if(aspectRatio == Qt::KeepAspectRatio)
        viewfinder_->setAspectRatioMode(aspectRatio);
    else if(aspectRatio == Qt::KeepAspectRatioByExpanding)
        viewfinder_->setAspectRatioMode(aspectRatio);
    else
    {
    }
}

Qt::AspectRatioMode QDeclarativeCamera::aspectRatio() const
{
    return aspectRatio_;
}

void QDeclarativeCamera::takeFocus()
{
    camera_->searchAndLock(QCamera::LockFocus);
    bool ready = capture->isReadyForCapture();

#ifdef DEBUG
    qDebug() << "State:" << ready;
#endif

#ifdef DEBUG
    qDebug() << "nativeSize:" << viewfinder_->nativeSize().width() << "x" << viewfinder_->nativeSize().height();
    qDebug() << "size:" << viewfinder_->size().width() << "x" << viewfinder_->size().height();
#endif

    //return ready;
}

void QDeclarativeCamera::unlock()
{
    camera_->unlock();
}

void QDeclarativeCamera::takePhoto()
{
    //camera_->searchAndLock();
    capture->capture();
    camera_->unlock();
}

void QDeclarativeCamera::takePhotoFull(QCamera::LockStatus status, QCamera::LockChangeReason reason)
{
#ifdef DEBUG
    qDebug() << "Reason:" << reason << "Status:" << status;
#endif

    if(status == QCamera::Unlocked) {
#ifdef DEBUG
        qDebug() << "Error: unlocked";
#endif
        return;
    }

    takePhoto();
    unlock();
}

int QDeclarativeCamera::getMPix()
{
    float result;

#ifdef DEBUG
    qDebug() << "Current width: " << getCurrentResolutionWidth() << "; Current height: " << getCurrentResolutionHeight();
#endif

    result = getCurrentResolutionWidth() * getCurrentResolutionHeight();
    result = result / 1000000;

    return result;
}

int QDeclarativeCamera::getCurrentResolutionWidth()
{
    if( ((resolutions.length() - 1) < current_resolution) || (current_resolution < 0) ) {
#ifdef DEBUG
        qDebug() << "[Error]: improper current_resolution!" << current_resolution << "where resolutions lenght is" << resolutions.length();
#endif
        return 0;
    }
    else
        //return resolutions.at(current_resolution).width();
        return capture->encodingSettings().resolution().width();
}

int QDeclarativeCamera::getCurrentResolutionHeight()
{
    if( ((resolutions.length() - 1) < current_resolution) || (current_resolution < 0) ) {
#ifdef DEBUG
        qDebug() << "[Error]: improper current_resolution!" << current_resolution << "where resolutions lenght is" << resolutions.length();
#endif
        return 0;
    }
    else
        return capture->encodingSettings().resolution().height();
}

void QDeclarativeCamera::setResolutionNumber(int resolution_number)
{
    if( ((resolutions.length() - 1) < resolution_number) || (resolution_number < 0) ) {
#ifdef DEBUG
        qDebug() << "[Error]: improper resolution_number!" << resolution_number << "where resolutions lenght is" << resolutions.length();
#endif
        return;
    }

    QImageEncoderSettings cameraSettings = capture->encodingSettings();
    //cameraSettings.setResolution(resolutions.at(resolution_number).width(), resolutions.at(resolution_number).height());
    cameraSettings.setResolution(2448, 4000);
    capture->setEncodingSettings(cameraSettings);
    this->current_resolution = resolution_number;

    int mpix = getMPix();
    emit this->mpixChanged(mpix);
}

void QDeclarativeCamera::setHighestResolution()
{
    if(resolutions.length() < 1) {
#ifdef DEBUG
        qDebug() << "[Error]: resolutions list is empty!";
#endif
        return;
    }

    setResolutionNumber(0);
}

void QDeclarativeCamera::parseAndStoreResolutions()
{
    bool cont;
    QList<QSize> list = capture->supportedResolutions(capture->encodingSettings(), &cont);
    qDebug() << "isContinous: " << cont;
    for(int i = 0; i < list.length() && i < 5; i++)
        resolutions.append(list.at(list.length() - 1 - i));

#ifdef DEBUG
    /* Debug info */
    for (int i = 0; i < resolutions.length(); i++)
        qDebug() << "Supported resolutions [" << i << "]:" << resolutions.at(i).width() << "x" << resolutions.at(i).height();
#endif

}

void QDeclarativeCamera::setPointModeFocus()
{
    focus_->setFocusMode(QCameraFocus::ContinuousFocus);

    //focus_->setFocusPointMode(QCameraFocus::FocusPointCenter);
    /* If face detection enabled */
    focus_->setFocusPointMode(QCameraFocus::FocusPointFaceDetection);
}

void QDeclarativeCamera::setPointModeFocusToCenter()
{
#ifdef DEBUG
    qDebug() << "setPointModeFocusToCenter";
#endif
    focus_->setFocusPointMode(QCameraFocus::FocusPointCenter);
}

void QDeclarativeCamera::setPointModeFocusToFaceDetection()
{
#ifdef DEBUG
    qDebug() << "setPointModeFocusToFaceDetection";
#endif
    focus_->setFocusPointMode(QCameraFocus::FocusPointFaceDetection);
}

void QDeclarativeCamera::setPointModeFocusToCustom()
{
#ifdef DEBUG
    qDebug() << "setPointModeFocusToCustom";
#endif
    focus_->setFocusPointMode(QCameraFocus::FocusPointCustom);
}

void QDeclarativeCamera::setFocusPoint(int x, int y)
{
    qreal x_val = (qreal)x / getViewfinderNativeWidth();
    qreal y_val = (qreal)y / getViewfinderNativeHeight();
    QPointF point;

    point.setX(x_val);
    point.setY(y_val);
    focus_->setCustomFocusPoint(point);

#ifdef DEBUG
    qDebug() << "x:" << x << " y:" << y;
    qDebug() << "x_val:" << x_val << " y_val:" << y_val;
#endif
}

void QDeclarativeCamera::signalFocusGained()
{
#ifdef DEBUG
    qDebug() << "focus gained";
#endif
    emit this->focusGained();
}

int QDeclarativeCamera::getViewfinderNativeWidth()
{
    return viewfinder_->nativeSize().width();
}

int QDeclarativeCamera::getViewfinderNativeHeight()
{
    return viewfinder_->nativeSize().height();
}

void QDeclarativeCamera::start()
{
    camera_->start();

    if(resolutions.length() < 1) {
        QImageEncoderSettings cameraSettings = capture->encodingSettings();
        capture->setEncodingSettings(cameraSettings);
        parseAndStoreResolutions();
        setHighestResolution();
    }
    isRecording = true;
}

void QDeclarativeCamera::stop()
{
    isRecording = false;
    camera_->stop();
}

void QDeclarativeCamera::unload()
{
    isRecording = false;

    camera_->unload();
}


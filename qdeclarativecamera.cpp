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

    connect(camera_, SIGNAL(lockStatusChanged(QCamera::LockStatus,QCamera::LockChangeReason)), this, SLOT(focusChangedSlot(QCamera::LockStatus,QCamera::LockChangeReason)));
    connect(focus_, SIGNAL(focusZonesChanged()), this, SLOT(focusZonesChanged()));

#ifdef DEBUG
    qDebug() << "ERROR: " << camera_->errorString();
#endif
}

void QDeclarativeCamera::isSupported()
{
    qDebug() << "Exposure manual: " << camera_->exposure()->isExposureModeSupported(QCameraExposure::ExposureManual);
    qDebug() << "ExposureAuto: " << camera_->exposure()->isExposureModeSupported(QCameraExposure::ExposureAuto);
    qDebug() << "ExposureNight: " << camera_->exposure()->isExposureModeSupported(QCameraExposure::ExposureNight);
    qDebug() << "ExposureBacklight: " << camera_->exposure()->isExposureModeSupported(QCameraExposure::ExposureBacklight);
    qDebug() << "ExposureSpotlight: " << camera_->exposure()->isExposureModeSupported(QCameraExposure::ExposureSpotlight);
    qDebug() << "ExposureSports: " << camera_->exposure()->isExposureModeSupported(QCameraExposure::ExposureSports);
    qDebug() << "ExposureSnow: " << camera_->exposure()->isExposureModeSupported(QCameraExposure::ExposureSnow);
    qDebug() << "ExposureBeach: " << camera_->exposure()->isExposureModeSupported(QCameraExposure::ExposureBeach);
    qDebug() << "ExposureLargeAperture: " << camera_->exposure()->isExposureModeSupported(QCameraExposure::ExposureLargeAperture);
    qDebug() << "ExposureSmallAperture: " << camera_->exposure()->isExposureModeSupported(QCameraExposure::ExposureSmallAperture);
    qDebug() << "ExposurePortrait: " << camera_->exposure()->isExposureModeSupported(QCameraExposure::ExposurePortrait);
    qDebug() << "ExposureModeVendor: " << camera_->exposure()->isExposureModeSupported(QCameraExposure::ExposureModeVendor);
    qDebug() << "";
    qDebug() << "FlashOff" << camera_->exposure()->isFlashModeSupported(QCameraExposure::FlashOff);
    qDebug() << "FlashOn" << camera_->exposure()->isFlashModeSupported(QCameraExposure::FlashOn);
    qDebug() << "FlashAuto" << camera_->exposure()->isFlashModeSupported(QCameraExposure::FlashAuto);
    qDebug() << "FlashRedEyeReduction" << camera_->exposure()->isFlashModeSupported(QCameraExposure::FlashRedEyeReduction);
    qDebug() << "FlashFill" << camera_->exposure()->isFlashModeSupported(QCameraExposure::FlashFill);
    qDebug() << "FlashTorch" << camera_->exposure()->isFlashModeSupported(QCameraExposure::FlashTorch);
    qDebug() << "FlashSlowSyncFrontCurtain" << camera_->exposure()->isFlashModeSupported(QCameraExposure::FlashSlowSyncFrontCurtain);
    qDebug() << "FlashSlowSyncRearCurtain" << camera_->exposure()->isFlashModeSupported(QCameraExposure::FlashSlowSyncRearCurtain);
    qDebug() << "FlashManual" << camera_->exposure()->isFlashModeSupported(QCameraExposure::FlashManual);
    qDebug() << "";
    qDebug() << "MeteringAverage" << camera_->exposure()->isMeteringModeSupported(QCameraExposure::MeteringAverage);
    qDebug() << "MeteringSpot" << camera_->exposure()->isMeteringModeSupported(QCameraExposure::MeteringSpot);
    qDebug() << "MeteringMatrix" << camera_->exposure()->isMeteringModeSupported(QCameraExposure::MeteringMatrix);
    qDebug() << "";
    bool ap = false;
    for (int i = 0; i < camera_->exposure()->supportedApertures(&ap).length(); i++)
        qDebug() << "aperture: " << camera_->exposure()->supportedApertures().at(i);
    qDebug() << "aperture cont: " << ap;

    ap = false;
    for (int i = 0; i < camera_->exposure()->supportedIsoSensitivities(&ap).length(); i++)
        qDebug() << "iso: " << camera_->exposure()->supportedIsoSensitivities().at(i);
    qDebug() << "iso cont: " << ap;

    ap = false;
    for (int i = 0; i < camera_->exposure()->supportedShutterSpeeds(&ap).length(); i++)
        qDebug() << "shutter: " << camera_->exposure()->supportedShutterSpeeds().at(i);
    qDebug() << "shutter cont: " << ap;

    qDebug() << "";
    qDebug() << "denoising:" << camera_->imageProcessing()->isDenoisingSupported();
    qDebug() << "sharpening:" << camera_->imageProcessing()->isSharpeningSupported();

    qDebug() << "WhiteBalanceManual:" << camera_->imageProcessing()->isWhiteBalanceModeSupported(QCameraImageProcessing::WhiteBalanceManual);
    qDebug() << "WhiteBalanceAuto:" << camera_->imageProcessing()->isWhiteBalanceModeSupported(QCameraImageProcessing::WhiteBalanceAuto);
    qDebug() << "WhiteBalanceSunlight:" << camera_->imageProcessing()->isWhiteBalanceModeSupported(QCameraImageProcessing::WhiteBalanceSunlight);
    qDebug() << "WhiteBalanceCloudy:" << camera_->imageProcessing()->isWhiteBalanceModeSupported(QCameraImageProcessing::WhiteBalanceCloudy);
    qDebug() << "WhiteBalanceShade:" << camera_->imageProcessing()->isWhiteBalanceModeSupported(QCameraImageProcessing::WhiteBalanceShade);
    qDebug() << "WhiteBalanceTungsten:" << camera_->imageProcessing()->isWhiteBalanceModeSupported(QCameraImageProcessing::WhiteBalanceTungsten);
    qDebug() << "WhiteBalanceFluorescent:" << camera_->imageProcessing()->isWhiteBalanceModeSupported(QCameraImageProcessing::WhiteBalanceFluorescent);
    qDebug() << "WhiteBalanceIncandescent:" << camera_->imageProcessing()->isWhiteBalanceModeSupported(QCameraImageProcessing::WhiteBalanceIncandescent);
    qDebug() << "WhiteBalanceFlash:" << camera_->imageProcessing()->isWhiteBalanceModeSupported(QCameraImageProcessing::WhiteBalanceFlash);
    qDebug() << "WhiteBalanceSunset:" << camera_->imageProcessing()->isWhiteBalanceModeSupported(QCameraImageProcessing::WhiteBalanceSunset);
    qDebug() << "WhiteBalanceVendor:" << camera_->imageProcessing()->isWhiteBalanceModeSupported(QCameraImageProcessing::WhiteBalanceVendor);
    qDebug() << "";
    qDebug() << "ManualFocus" << camera_->focus()->isFocusModeSupported(QCameraFocus::ManualFocus);
    qDebug() << "AutoFocus" << camera_->focus()->isFocusModeSupported(QCameraFocus::AutoFocus);
    qDebug() << "ContinuousFocus" << camera_->focus()->isFocusModeSupported(QCameraFocus::ContinuousFocus);
    qDebug() << "InfinityFocus" << camera_->focus()->isFocusModeSupported(QCameraFocus::InfinityFocus);
    qDebug() << "HyperfocalFocus" << camera_->focus()->isFocusModeSupported(QCameraFocus::HyperfocalFocus);
    qDebug() << "MacroFocus" << camera_->focus()->isFocusModeSupported(QCameraFocus::MacroFocus);
    qDebug() << "";
    qDebug() << "FocusPointAuto" << camera_->focus()->isFocusPointModeSupported(QCameraFocus::FocusPointAuto);
    qDebug() << "FocusPointCenter" << camera_->focus()->isFocusPointModeSupported(QCameraFocus::FocusPointCenter);
    qDebug() << "FocusPointFaceDetection" << camera_->focus()->isFocusPointModeSupported(QCameraFocus::FocusPointFaceDetection);
    qDebug() << "FocusPointCustom" << camera_->focus()->isFocusPointModeSupported(QCameraFocus::FocusPointCustom);
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
    camera_->searchAndLock();
    capture->capture();
    camera_->unlock();
}

void QDeclarativeCamera::focusZonesChanged()
{
#ifdef DEBUG
    qDebug() << "Info: focus zones length:" << focus_->focusZones().length();
    qDebug() << "Info: focus zone X:" << focus_->focusZones().last().area().x();
    qDebug() << "Info: focus zone Y:" << focus_->focusZones().last().area().y();
    qDebug() << "Info: focus zone WIDTH:" << focus_->focusZones().last().area().width();
    qDebug() << "Info: focus zone HEIGHT:" << focus_->focusZones().last().area().height();
#endif
}

void QDeclarativeCamera::focusChangedSlot(QCamera::LockStatus status, QCamera::LockChangeReason reason)
{
    /* Let's assume that the camera is locked. */
    bool value = true;

#ifdef DEBUG
    qDebug() << "Reason:" << reason << "Status:" << status;
#endif

    if(status != QCamera::Locked) {
#ifdef DEBUG
        qDebug() << "Info: unlocked or searching";
#endif
        value = false;
    }
   // else
    //    camera_->unlock();

    emit this->focusChanged(value);
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
    cameraSettings.setResolution(4000, 2448);
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
    //focus_->setFocusPointMode(QCameraFocus::FocusPointCenter);
    /* If face detection enabled */
    focus_->setFocusPointMode(QCameraFocus::FocusPointFaceDetection);
    focus_->setFocusMode(QCameraFocus::AutoFocus);
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
    isSupported();
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


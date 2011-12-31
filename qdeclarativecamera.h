#ifndef QDeclarativeCamera_H
#define QDeclarativeCamera_H

#include <QDeclarativeItem>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QMediaRecorder>

#define DEBUG

class QGraphicsVideoItem;
class QDeclarativeCamera : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(Qt::AspectRatioMode aspectRatio READ aspectRatio WRITE setAspectRatio)

public:
    explicit QDeclarativeCamera(QDeclarativeItem *parent = 0);
    ~QDeclarativeCamera();

    Qt::AspectRatioMode aspectRatio() const;
    void setAspectRatio(const Qt::AspectRatioMode& aspectRatio);
    void setResolutionNumber(int resolution_number);

public slots:
    void viewfinderSizeChanged(const QSizeF& size);
    void launchMainCamera();
    void launchFrontCamera();
    void parseAndStoreResolutions();

    int getCurrentResolutionWidth();
    int getCurrentResolutionHeight();
    int getMPix();
    int getViewfinderNativeWidth();
    int getViewfinderNativeHeight();

    void start();
    void stop();
    void unload();
    void takeFocus();
    void takePhoto();
    void unlock();

    void setPointModeFocus();
    void setPointModeFocusToCenter();
    void setPointModeFocusToFaceDetection();
    void setPointModeFocusToCustom();
    void setFocusPoint(int x, int y);

private slots:
    void focusChangedSlot(QCamera::LockStatus status, QCamera::LockChangeReason reason);
    void focusZonesChanged();

protected slots:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

signals:
    void mpixChanged(int mpix);
    void focusChanged(bool gained);

private:
    void toggleCamera(bool mainCamera);
    void setHighestResolution();
    void isSupported();

    QCamera* camera_;
    QCameraFocus *focus_;
    QGraphicsVideoItem* viewfinder_;
    Qt::AspectRatioMode aspectRatio_;
    QMediaRecorder* mediaRecorder_;
    QCameraImageCapture *capture;
    QRectF geometry;
    bool firstCamera;
    QTimer *timer;
    bool isRecording;
    bool isRecordingInParts;
    int videoPartNumber;
    bool recorded;
    QList<QSize> resolutions;
    int current_resolution; // indicates number of the element from "resolutions" list
};

#endif // QDeclarativeCamera_H

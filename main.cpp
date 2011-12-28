#include <QtGui/QApplication>
#include <QGLWidget>
#include <QDeclarativeEngine>

#include "qmlapplicationviewer.h"
#include "qdeclarativecamera.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    qmlRegisterType<QDeclarativeCamera>("Camera", 1, 0, "Camera");
    QmlApplicationViewer viewer;
    viewer.setViewport(new QGLWidget());
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockLandscape);
    viewer.setMainQmlFile(QLatin1String("qml/photo/main.qml"));
    viewer.showExpanded();

    return app->exec();
}

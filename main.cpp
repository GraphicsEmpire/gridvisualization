#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include "BeforeRenderingItem.h"
#include "GridEqualizer.h"


int main(int argc, char *argv[])
{
    qmlRegisterType<GridEqualizer>("mbrdna.qml.components", 1, 0, "GridEqualizer");
    qmlRegisterType<BeforeRenderItem>("mbrdna.qml.components", 1, 0, "BeforeRenderItem");

    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    viewer.setSurfaceType(QWindow::OpenGLSurface);

    QSurfaceFormat format;
    format.setSamples(8);
    viewer.setFormat(format);
    viewer.setMainQmlFile(QStringLiteral("qml/GridVisualization/main.qml"));
    viewer.showExpanded();

    //Connect Before RenderItem
    QObject::connect(&viewer, SIGNAL(beforeRendering()), BeforeRenderGlobal::Instance(), SLOT(handlerBefore()), Qt::DirectConnection);
    QObject::connect(&viewer, SIGNAL(afterRendering()), BeforeRenderGlobal::Instance(), SLOT(handlerAfter()), Qt::DirectConnection);

    return app.exec();
}

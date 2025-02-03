#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include "Backend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    Backend *backend = new Backend(&app);

    engine.rootContext()->setContextProperty("Backend", backend);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("SetOwnIcon", "Main");

    return app.exec();
}

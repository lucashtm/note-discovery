#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "startupcontroller.h"
#include <QIcon>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon("C:/Users/lucas/Documents/audio_input/music.png"));
    StartupController* controller = new StartupController(nullptr);
    qmlRegisterSingletonInstance<StartupController>("app.controllers", 1, 0, "StartupController", controller);
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

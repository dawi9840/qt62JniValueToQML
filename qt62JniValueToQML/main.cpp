#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QQmlContext>
#include <QScreen>

#include <qtandroidservice.h>
#include <clustertool.h>

int main(int argc, char *argv[])
{
    // Creating instance of ClusterTool
    ClusterTool tool;
    // Env setting.
    //tool.configureDpiSettings();
    //tool.configureDisplaySettingsForAVD(); // 0.8 scale
    tool.configureDisplaySettingsForEVM();

    QGuiApplication app(argc, argv);
    // Fonts format chesk and setting.
    int localFont = tool.CheckLoadFontsIsExist();
    QFontDatabase::applicationFontFamilies(localFont);
    QFont font("Myriad Pro", 14);
    app.setFont(font);

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/test.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.load(url);

    QtAndroidService *qtAndroidService = new QtAndroidService(&app);
    engine.rootContext()->setContextProperty(QLatin1String("qtAndroidService"), qtAndroidService);

    return app.exec();
}



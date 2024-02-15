#ifndef QTANDROIDSERVICE_H
#define QTANDROIDSERVICE_H

#include <QObject>
#include <QCoreApplication>
#include <QJniEnvironment>
#include <QJniObject>
#include <QCoreApplication>


class QtAndroidService : public QObject
{
    Q_OBJECT

public:
    QtAndroidService(QObject *parent = nullptr);

    static QtAndroidService *instance() { return m_instance; }
    Q_INVOKABLE void sendToService(const QString &name);
    QString converToQstring(JNIEnv *env, jstring inputJString);

signals:
    void messageFromService(const QString &message);

private:
    bool registerQJniEnv(const JNINativeMethod methods[], int totalMethods);
    static QtAndroidService *m_instance; // Static pointer to the instance of the QtAndroidService
};

#endif // QTANDROIDSERVICE_H

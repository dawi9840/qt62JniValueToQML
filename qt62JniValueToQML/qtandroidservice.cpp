#include "qtandroidservice.h"
#include "private/qandroidextras_p.h"

// Initialize the singleton instance of QtAndroidService
QtAndroidService *QtAndroidService::m_instance = nullptr;

static void receivedFromAndroidService(JNIEnv *env, jobject /*thiz*/, jstring value)
{
    qDebug() << "cpp_receivedFromAndroidService";

    const QString valueQString = QtAndroidService::instance()->converToQstring(env, value);

    // Emit a signal to the QtAndroidService instance
    if (QtAndroidService::instance() != nullptr) {

        qDebug() << "Before emit";
        emit QtAndroidService::instance()->messageFromService(valueQString);
        qDebug() << "invoke messageFromService()";
        qDebug() << "After emit";
    }
}

// QtAndroidService class constructor
QtAndroidService::QtAndroidService(QObject *parent) : QObject(parent)
{
    m_instance = this;
    JNINativeMethod methods[] {
        {"sendToQt", "(Ljava/lang/String;)V", reinterpret_cast<void *>(receivedFromAndroidService)},
    };
    int totalMethods = sizeof(methods)/sizeof(methods[0]);
    bool registerQJniEnvStatus = QtAndroidService::registerQJniEnv(methods, totalMethods);
    if(registerQJniEnvStatus == true){
        m_instance->sendToService("Qt");
    }
}

/**
 * @brief Return the register QJNI env status for mapping Java methods to C++ methods
 *
 * @param methods The array of JNINativeMethod containing Java methods to map to C++ methods
 * @param totalMethods The total count of methods in the methods array
 * @return bool indicating the status of JNI method registration
 *
 * This function registers JNI methods to associate Java methods with corresponding C++ methods.
 */
bool QtAndroidService::registerQJniEnv(const JNINativeMethod methods[], int totalMethods){
    // Create a QJniEnvironment to registe the registerNativeMethods.
    QJniEnvironment env;
    jclass clazz = env.findClass("com/foxconn/QtAndroidService");
    jint ret = env.registerNativeMethods(clazz, methods, totalMethods);

    // Check the Register QJNI and return the status.
    //qDebug()<< "ret: " << ret;

    if(ret > 0){
        qDebug()<< "registerQJniEnv(): Register success!\n----------\n";
        // Return the ret value after registering the JNI method
        return true;
    }else{
        qDebug()<< "registerQJniEnv(): Register failed.\n----------\n";
        return false;
    }
}

// Start service to invoke Android activity Internet
void QtAndroidService::sendToService(const QString &name)
{
    qDebug()<< "sendToService(): Start service to invoke Android activity Internet";
    Q_UNUSED(name);

    // 創建一個 Android Intent 物件以啟動指定的 Android 服務
    // Start Android service through JNI and deliver specific Intent
    auto activity = QJniObject(QNativeInterface::QAndroidApplication::context());
    QAndroidIntent serviceIntent(activity.object(), "com/foxconn/QtAndroidService");

    // 使用 Android 活動物件呼叫 Java 中的 startService 方法，
    // 並將 Intent 物件作為參數傳遞，以啟動指定的 Android 服務
    QJniObject result = activity.callObjectMethod(
        "startService",
        "(Landroid/content/Intent;)Landroid/content/ComponentName;",
        serviceIntent.handle().object());
}

// To convert the JString to the QString type, and return the QString variable.
QString QtAndroidService::converToQstring(JNIEnv *env, jstring inputJString){
    // Jstring type
    const char *valueJStr = env->GetStringUTFChars(inputJString, nullptr);

    // Convert it to QString type
    const QString valueQString = QString::fromUtf8(valueJStr);

    //qDebug() << "dawi_valueJStr: " << valueJStr << ", Type: " << typeid(valueJStr).name();
    //qDebug() << "dawi_valueQStr: " << valueQString << ", Type: " << typeid(valueQString).name() << "\n";

    // Release memory
    env->ReleaseStringUTFChars(inputJString, valueJStr);
    return valueQString;
}

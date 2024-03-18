/*******************************************************************************
 *
 * libcontemporary - Common libraries
 * Copyright (C) 2022 Victor Tran
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#include "tapplication.h"
#include "private/translatorproxy.h"
#include "tlogger.h"
#include <QDir>
#include <QFileOpenEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QPainter>
#include <QProcess>
#include <QSvgRenderer>
#include <QSysInfo>
#include <QThread>
#include <QTranslator>

#ifdef T_OS_UNIX_NOT_MAC
    #include <signal.h>
#endif

#ifdef Q_OS_MAC
    #include "private/tapplicationmacprivate.h"
#endif

#ifdef Q_OS_WIN
    #include "tnotification/tnotification-win.h"
    #include <Windows.h>

    #include <DbgHelp.h>
    #include <appmodel.h>
#endif

#if QT_CONFIG(sharedmemory)
    #include <QSharedMemory>
#endif

#ifdef HAVE_LIBUNWIND
    #define UNW_LOCAL_ONLY
    #include <libunwind.h>
#endif

#if defined(__GNUG__)
    #include <cxxabi.h>
#endif

struct tApplicationPrivate {
        QTranslator translator;
        QStringList pluginTranslators;
        QStringList libraryTranslators;
        QList<TranslatorProxy*> applicationTranslators;
        tApplication* applicationInstance;

        bool crashHandlingEnabled = false;
        QIcon applicationIcon;

        QString applicationShareDir;
        QString genericName;
        QPixmap aboutDialogSplashGraphic;
        QList<QPair<QString, QString>> versions;
        QStringList copyrightLines;
        tApplication::KnownLicenses license = tApplication::Other;
        QString copyrightHolder, copyrightYear;
        QMap<tApplication::UrlType, QUrl> applicationUrls;

#if QT_CONFIG(sharedmemory)
        QSharedMemory* singleInstanceMemory = nullptr;
        QLocalServer* singleInstanceServer = nullptr;
#endif

        static bool isInitialised;

#ifdef T_OS_UNIX_NOT_MAC
        static void crashTrapHandler(int sig);
#elif defined(Q_OS_WIN)
        static LONG WINAPI crashTrapHandler(PEXCEPTION_POINTERS exceptionInfo);
        PCONTEXT crashCtx = nullptr;

        bool hasCheckedIsRunningAsUwp = false;
        bool isRunningAsUwp = false;

        QString winClassId;
#else
        static void crashTrapHandler();
#endif

#ifdef Q_OS_MAC
        tApplicationMacPrivate* privateProxy;
#endif

        static void qtMessageHandler(QtMsgType messageType, const QMessageLogContext& context, const QString& message) {
            tLogger::log(messageType, "QMessageLogger", message, context.file, context.line, context.function);
            //        tApplication::d->oldMessageHandler(messageType, context, message);
        }

        QtMessageHandler oldMessageHandler;
};

tApplicationPrivate* tApplication::d = nullptr;
bool tApplicationPrivate::isInitialised = false;

tApplication::tApplication(int& argc, char** argv) :
    QApplication(argc, argv) {
    d = new tApplicationPrivate();
    d->applicationInstance = this;

#ifdef Q_OS_MAC
    d->privateProxy = new tApplicationMacPrivate(this);
#endif

    d->oldMessageHandler = qInstallMessageHandler(&tApplicationPrivate::qtMessageHandler);

    // Mark some strings for translation
    if (false) {
        // macOS Application Menu
        (void) QT_TRANSLATE_NOOP("MAC_APPLICATION_MENU", "Services");
        (void) QT_TRANSLATE_NOOP("MAC_APPLICATION_MENU", "Hide %1");
        (void) QT_TRANSLATE_NOOP("MAC_APPLICATION_MENU", "Hide Others");
        (void) QT_TRANSLATE_NOOP("MAC_APPLICATION_MENU", "Show All");
        (void) QT_TRANSLATE_NOOP("MAC_APPLICATION_MENU", "Preferences...");
        (void) QT_TRANSLATE_NOOP("MAC_APPLICATION_MENU", "About %1");
        (void) QT_TRANSLATE_NOOP("MAC_APPLICATION_MENU", "Quit %1");

        // QCommandLineParser
        (void) QT_TRANSLATE_NOOP("QCommandLineParser", "Displays version information.");
        (void) QT_TRANSLATE_NOOP("QCommandLineParser", "Displays this help.");
        (void) QT_TRANSLATE_NOOP("QCommandLineParser", "Unknown option '%1'.");
        (void) QT_TRANSLATE_NOOP("QCommandLineParser", "Unknown options: %1.");
        (void) QT_TRANSLATE_NOOP("QCommandLineParser", "Missing value after '%1'.");
        (void) QT_TRANSLATE_NOOP("QCommandLineParser", "Unexpected value after '%1'.");
        (void) QT_TRANSLATE_NOOP("QCommandLineParser", "[options]");
        (void) QT_TRANSLATE_NOOP("QCommandLineParser", "Usage: %1");
        (void) QT_TRANSLATE_NOOP("QCommandLineParser", "Options:");
        (void) QT_TRANSLATE_NOOP("QCommandLineParser", "Arguments:");
    }

    Q_INIT_RESOURCE(thelibs_translations);
    Q_INIT_RESOURCE(thelibs_icons);

    QLocale locale;
    // macOS gives weird language/region combinations sometimes so extra logic might be required
    if (!d->translator.load(locale, "", "", ":/the-libs/translations/")) {
        Q_UNUSED(d->translator.load(locale.name(), ":/the-libs/translations/"));
    }
    installTranslator(new TranslatorProxy(&d->translator));

#ifdef Q_OS_MAC
    this->setAttribute(Qt::AA_DontShowIconsInMenus, true);
#endif

    if (this->currentPlatform() == Flatpak) this->addLibraryPath("/app/plugins");

    d->versions.append({"libContemporary", QStringLiteral("%1 (API %2)").arg(THE_LIBS_VERSION).arg(THE_LIBS_API_VERSION)});
    d->versions.append({"Qt", QString(qVersion())});

    QString platformString;
    switch (this->currentPlatform()) {
        case TheDesk:
            platformString = tr("Linux");
            break;
        case Flatpak:
            platformString = tr("Flatpak");
            break;
        case Windows:
            platformString = tr("Microsoft Windows");
            break;
        case WindowsAppPackage:
            platformString = tr("Microsoft Windows Store");
            break;
        case MacOS:
            platformString = tr("macOS");
            break;
        case OtherPlatform:
            platformString = tr("Unknown");
            break;
    }
    d->versions.append({tr("Platform"), platformString});

#if defined(T_ARCH_X86_64)
    d->versions.append({tr("Architecture"), QStringLiteral("x86_64")});
#elif defined(T_ARCH_AARCH64)
    d->versions.append({tr("Architecture"), QStringLiteral("aarch64")});
#else
    d->versions.append({tr("Architecture"), tr("Unknown")});
#endif

    d->applicationIcon = QIcon(":/libcontemporary-appassets/appicon.svg");
    d->isInitialised = true;
}

bool tApplication::event(QEvent* event) {
    if (event->type() == QEvent::FileOpen) {
        QFileOpenEvent* openEvent = (QFileOpenEvent*) event;
        emit openFile(openEvent->file());
    } else if (event->type() == QEvent::LocaleChange) {
        // Reinstall the-libs translator
        Q_UNUSED(d->translator.load(QLocale().name(), ":/the-libs/translations/"));

        // Reinstall the application translators
        installTranslators();

        // Tell everyone the translations have changed
        emit updateTranslators();
    } else if (event->type() == QEvent::ApplicationActivate) {
#ifdef Q_OS_MAC
        emit dockIconClicked();
#endif
    }

    return QApplication::event(event);
}

QStringList tApplication::exportBacktrace(void* data) {
    QStringList backtrace;
#ifdef HAVE_LIBUNWIND
    unw_cursor_t cur;
    unw_context_t ctx;

    unw_getcontext(&ctx);
    unw_init_local(&cur, &ctx);

    // Start unwinding
    while (unw_step(&cur) > 0) {
        unw_word_t offset;
        unw_word_t pc;
        unw_get_reg(&cur, UNW_REG_IP, &pc);
        if (pc == 0) break;

        char sym[256];
        if (unw_get_proc_name(&cur, sym, sizeof(sym), &offset) == 0) {
            // Demangle the name depending on the current compiler
            QString functionName;
    #if defined(__GNUG__)
            // We're using a gcc compiler
            int status;
            char* demangled = abi::__cxa_demangle(sym, nullptr, nullptr, &status);
            if (status == 0) functionName = QString::fromLocal8Bit(demangled);
            std::free(demangled);
    #elif defined(_MSC_VER)
                // We're using msvc
    #endif

            if (functionName == "") {
                functionName = QString::fromLocal8Bit(sym);
            }

            backtrace.append(QString("0x" + QString::number(pc, 16) + ": " + functionName));
        } else {
            backtrace.append(QString("0x" + QString::number(pc, 16) + ": ??\n").toUtf8());
        }
    }
#endif

#ifdef Q_OS_WIN
    #ifdef T_ARCH_X86
    const int maxNameLen = 255;
    HANDLE proc = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();
    DWORD64 displacement;

    CONTEXT ctx;
    if (data == nullptr) {
        ctx.ContextFlags = CONTEXT_FULL;
        RtlCaptureContext(&ctx);
    } else {
        ctx = *reinterpret_cast<PCONTEXT>(data);
    }

    SymInitialize(proc, NULL, TRUE);
    SymSetOptions(SYMOPT_LOAD_LINES);

    STACKFRAME64 frame;
    frame.AddrPC.Offset = ctx.Rip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = ctx.Rbp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = ctx.Rsp;
    frame.AddrStack.Mode = AddrModeFlat;

    // Start walking
    while (StackWalk(IMAGE_FILE_MACHINE_AMD64, proc, thread, &frame, &ctx, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL)) {
        char buffer[sizeof(SYMBOL_INFO) + maxNameLen * sizeof(TCHAR)];
        PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(buffer);
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = maxNameLen;
        SymFromAddr(proc, frame.AddrPC.Offset, &displacement, symbol);

        // Unmangle the name
        char name[maxNameLen];
        if (!UnDecorateSymbolName(symbol->Name, reinterpret_cast<PSTR>(name), maxNameLen, UNDNAME_COMPLETE)) {
            memcpy(name, symbol->Name, maxNameLen);
        }

        backtrace.append(QString("0x" + QString::number(frame.AddrPC.Offset, 16) + ": " + QString::fromLatin1(name)));
    }

    SymCleanup(proc);
    #endif
    #ifdef T_ARCH_ARM

    #endif
#endif
    return backtrace;
}

tApplication::~tApplication() {
#if QT_CONFIG(sharedmemory)
    if (d->singleInstanceMemory) {
        d->singleInstanceMemory->detach();
        delete d->singleInstanceMemory;
    }
    if (d->singleInstanceServer) {
        d->singleInstanceServer->close();

        // There's a race condition here...
        QThread::sleep(1);
    }
#endif
}

void writeCrashSysInfo(QStringList& bt) {
    bt.append("Application name:\t" + tApplication::applicationName());
    bt.append("Application PID:\t" + QString::number(tApplication::applicationPid()));
    bt.append("Application Command:\t" + tApplication::applicationFilePath());

    bt.append(" ");

    bt.append("System Name:\t" + QSysInfo::prettyProductName());
    bt.append("System Architecture:\t" + QSysInfo::currentCpuArchitecture());
    bt.append("System Kernel:\t" + QSysInfo::kernelType());
    bt.append("System Type:\t" + QSysInfo::productType());
    bt.append("System Version:\t" + QSysInfo::productVersion());

    bt.append(" ");
}

#ifdef T_OS_UNIX_NOT_MAC
void tApplicationPrivate::crashTrapHandler(int sig) {
    signal(sig, SIG_DFL);
    // Attempt to start Bonkers to tell the user that the app has crashed
    QStringList args = {
        "--appname", "\"" + tApplication::applicationName() + "\"",
        "--apppath", "\"" + tApplication::applicationFilePath() + "\"",
        "--bt"};

    QProcess* process = new QProcess();
    process->setEnvironment(QProcess::systemEnvironment());
    process->start(QStringLiteral(SYSTEM_LIBRARY_DIRECTORY).append("/bonkers"), args, QProcess::Unbuffered | QProcess::WriteOnly);

    // Write out crash information
    QStringList bt;
    writeCrashSysInfo(bt);

    QString signalName;
    switch (sig) {
        case SIGSEGV:
            signalName = QStringLiteral("SIGSEGV");
            break;
        case SIGBUS:
            signalName = QStringLiteral("SIGBUS");
            break;
        case SIGFPE:
            signalName = QStringLiteral("SIGFPE");
            break;
        case SIGILL:
            signalName = QStringLiteral("SIGILL");
            break;
        case SIGABRT:
            signalName = QStringLiteral("SIGABRT");
            break;
        default:
            signalName = QStringLiteral("SIG") + QString::number(sig);
    }
    char* signalText = strsignal(sig);
    if (signalText == nullptr) {
        bt.append("Received signal:\t" + signalName);
    } else {
        bt.append("Received signal:\t" + signalName + " (" + QString::fromLocal8Bit(signalText) + ")");
    }

    bt.append(" ");
    bt.append("Backtrace:");

    // Get a backtrace
    bt.append(tApplication::exportBacktrace());
    process->write(bt.join("\n").toUtf8());
    process->closeWriteChannel();
    process->waitForBytesWritten();

    // Reset the signal and re-raise it
    raise(sig);
}
#endif

#ifdef Q_OS_WIN
LONG WINAPI tApplicationPrivate::crashTrapHandler(PEXCEPTION_POINTERS exceptionInfo) {
    // Attempt to start Bonkers to tell the user that the app has crashed
    QStringList args = {
        "--appname", "\"" + tApplication::applicationName() + "\"",
        "--apppath", "\"" + tApplication::applicationFilePath() + "\"",
        "--bt"};

    QProcess* process = new QProcess();
    process->setEnvironment(QProcess::systemEnvironment());
    process->start(QDir(tApplication::applicationDirPath()).absoluteFilePath("bonkers.exe"), args, QProcess::Unbuffered | QProcess::WriteOnly);

    // Write out crash information
    QStringList bt;
    writeCrashSysInfo(bt);

    bt.append("Windows Exception " + QString("0x" + QString::number(exceptionInfo->ExceptionRecord->ExceptionCode, 16)));

    bt.append(" ");
    bt.append("Backtrace:");

    // Get a backtrace
    bt.append(tApplication::exportBacktrace(exceptionInfo->ContextRecord));
    process->write(bt.join("\n").toUtf8());
    process->closeWriteChannel();
    process->waitForBytesWritten();

    // Reset the signal and re-raise it

    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

void tApplication::registerCrashTrap() {
    // macOS has its own crash trap handler so we don't do anything for macOS

#ifdef T_OS_UNIX_NOT_MAC
    // Check that the crash handler exists
    if (QFile(QStringLiteral(SYSTEM_LIBRARY_DIRECTORY).append("/bonkers")).exists()) {
        // Enable the crash trap
        d->crashHandlingEnabled = true;

        signal(SIGSEGV, tApplicationPrivate::crashTrapHandler);
        signal(SIGBUS, tApplicationPrivate::crashTrapHandler);
        signal(SIGFPE, tApplicationPrivate::crashTrapHandler);
        signal(SIGILL, tApplicationPrivate::crashTrapHandler);
        signal(SIGABRT, tApplicationPrivate::crashTrapHandler);
    }
#endif

#ifdef Q_OS_WIN
    // Check that the crash handler exists
    if (QFile(QDir(this->applicationDirPath()).absoluteFilePath("bonkers.exe")).exists()) {
        // Enable the crash trap
        d->crashHandlingEnabled = true;

        SetUnhandledExceptionFilter(tApplicationPrivate::crashTrapHandler);
    }
#endif
}

QStringList tApplication::exportBacktrace() {
    return tApplication::exportBacktrace(nullptr);
}

QIcon tApplication::applicationIcon() {
    return d->applicationIcon;
}

QStringList tApplication::shareDirs() {
    QStringList list;
    for (auto dir : systemShareDirs()) {
        list.append(QDir(dir).absoluteFilePath(d->applicationShareDir));
    }
    return list;
}

QStringList tApplication::systemShareDirs() {
    return qEnvironmentVariable("XDG_DATA_DIRS", "/usr/local/share/:/usr/share/").split(":");
}

void tApplication::setApplicationIcon(QIcon icon) {
    d->applicationIcon = icon;
}

void tApplication::setApplicationShareDir(QString shareDir) {
    d->applicationShareDir = shareDir;
}

void tApplication::installTranslators() {
    for (TranslatorProxy* translator : d->applicationTranslators) {
        removeTranslator(translator);
        translator->deleteLater();
    }
    d->applicationTranslators.clear();

    QLocale locale;
    auto languages = locale.uiLanguages();
    tDebug("tApplication") << languages;

    QTranslator* localTranslator = new QTranslator();
#if defined(Q_OS_MAC)
    QString translationsPath = QDir(macOSBundlePath()).absoluteFilePath(QStringLiteral("Contents/Resources/translations/"));

    // macOS gives weird language/region combinations sometimes so extra logic might be required
    if (!localTranslator->load(locale, "", "", translationsPath)) {
        localTranslator->load(locale.name(), translationsPath);
    }
#elif defined(Q_OS_LINUX)
    for (auto dir : shareDirs()) {
        if (localTranslator->load(locale, "", "", QDir(dir).absoluteFilePath("translations"))) break;
    }
#elif defined(Q_OS_WIN)
    Q_UNUSED(localTranslator->load(locale, "", "", this->applicationDirPath() + "\\translations"));
#endif
    TranslatorProxy* localTranslatorProxy = new TranslatorProxy(localTranslator);
    this->installTranslator(localTranslatorProxy);
    d->applicationTranslators.append(localTranslatorProxy);

    for (QString library : d->libraryTranslators) {
        QTranslator* translator = new QTranslator();
#if defined(Q_OS_MAC)
        auto libraryName = library.split("/").last();
        QString translationsPath = QDir(macOSBundlePath()).absoluteFilePath(QStringLiteral("Contents/Frameworks/%1.framework/Resources/translations").arg(libraryName));

        // macOS gives weird language/region combinations sometimes so extra logic might be required
        if (!translator->load(locale, "", "", translationsPath)) {
            translator->load(locale.name(), translationsPath);
        }
#else
        for (auto dir : systemShareDirs()) {
            if (translator->load(locale, "", "", QDir(QDir(dir).absoluteFilePath(library)).absoluteFilePath("translations"))) break;
        }
#endif
        TranslatorProxy* translatorProxy = new TranslatorProxy(translator);
        installTranslator(translatorProxy);
        d->applicationTranslators.append(translatorProxy);
    }

    for (QString plugin : d->pluginTranslators) {
        QTranslator* translator = new QTranslator();
#if defined(Q_OS_MAC)
        QString translationsPath = macOSBundlePath() + "/Contents/Resources/Plugins/" + plugin + "/translations/";

        // macOS gives weird language/region combinations sometimes so extra logic might be required
        if (!translator->load(locale, "", "", translationsPath)) {
            translator->load(locale.name(), translationsPath);
        }
#elif defined(Q_OS_LINUX)
        for (auto dir : shareDirs()) {
            auto pluginPath = QDir(QDir(QDir(dir).absoluteFilePath("plugins")).absoluteFilePath(plugin)).absoluteFilePath("translations");
            if (translator->load(locale, "", "", pluginPath)) break;
        }
#elif defined(Q_OS_WIN)
        Q_UNUSED(translator->load(locale, "", "", this->applicationDirPath() + "\\plugins\\" + plugin + "\\translations"));
#endif
        TranslatorProxy* translatorProxy = new TranslatorProxy(translator);
        installTranslator(translatorProxy);
        d->applicationTranslators.append(translatorProxy);
    }

    if (qEnvironmentVariableIsSet("THELIBS_TEXT_DIRECTION")) {
        if (qEnvironmentVariable("THELIBS_TEXT_DIRECTION").toUpper() == "RTL") {
            this->setLayoutDirection(Qt::RightToLeft);
        } else {
            this->setLayoutDirection(Qt::LeftToRight);
        }
    } else {
        if (!languages.isEmpty()) {
            this->setLayoutDirection(QLocale(languages.first()).textDirection());
        }
    }
}

void tApplication::addPluginTranslator(QString pluginName) {
    d->pluginTranslators.append(pluginName);
    d->applicationInstance->installTranslators();
}

void tApplication::removePluginTranslator(QString pluginName) {
    d->pluginTranslators.removeOne(pluginName);
    d->applicationInstance->installTranslators();
}

void tApplication::addLibraryTranslator(QString sharePath) {
    d->libraryTranslators.append(sharePath);
    d->applicationInstance->installTranslators();
}

QString tApplication::macOSBundlePath() {
#ifdef Q_OS_MAC
    return d->privateProxy->macOSBundlePath();
#else
    return "";
#endif
}

QString tApplication::macOSBundlePath(QString bundleIdentifier) {
#ifdef Q_OS_MAC
    return d->privateProxy->macOSBundlePath(bundleIdentifier);
#else
    return "";
#endif
}

void tApplication::setGenericName(QString genericName) {
    d->genericName = genericName;
}

void tApplication::setAboutDialogSplashGraphic(QPixmap aboutDialogSplashGraphic) {
    d->aboutDialogSplashGraphic = aboutDialogSplashGraphic;
}

void tApplication::addLibraryVersion(QString libraryName, QString version) {
    d->versions.append({libraryName, version});
}

void tApplication::addCopyrightLine(QString copyrightLine) {
    d->copyrightLines.append(copyrightLine);
}

void tApplication::setCopyrightHolder(QString copyrightHolder) {
    d->copyrightHolder = copyrightHolder;
}

void tApplication::setCopyrightYear(QString copyrightYear) {
    d->copyrightYear = copyrightYear;
}

void tApplication::setApplicationLicense(tApplication::KnownLicenses license) {
    d->license = license;
}

void tApplication::setApplicationUrl(tApplication::UrlType type, QUrl url) {
    d->applicationUrls.insert(type, url);
}

#ifdef Q_OS_WIN
void tApplication::setWinApplicationClassId(QString classId) {
    d->winClassId = classId;
    tNotificationWindows::initialise(classId);
}
#endif

QPixmap tApplication::aboutDialogSplashGraphicFromSvg(QString svgFile) {
    QImage image(QSize(100, 340), QImage::Format_ARGB32);
    QPainter painter(&image);
    QSvgRenderer renderer(svgFile);
    renderer.render(&painter, QRectF(QPointF(0, 0), QSizeF(100, 340)));
    return QPixmap::fromImage(image);
}

QString tApplication::genericName() {
    return d->genericName;
}

QPixmap tApplication::aboutDialogSplashGraphic() {
    return d->aboutDialogSplashGraphic;
}

QList<QPair<QString, QString>> tApplication::versions() {
    QList<QPair<QString, QString>> versions;
    versions.append({tApplication::applicationName(), tApplication::applicationVersion()});
    versions.append(d->versions);
    return versions;
}

QStringList tApplication::copyrightLines() {
    QStringList copyrightLines = d->copyrightLines;

    auto wrapLicense = [](QString text) {
        return QStringLiteral(R"(<a href="license://">%1</a>)").arg(text);
    };

    switch (d->license) {
        case Gpl3:
            copyrightLines.prepend(tr("Licensed under the terms of the %1.").arg(wrapLicense(tr("GNU General Public License, version 3"))));
            break;
        case Gpl3OrLater:
            copyrightLines.prepend(tr("Licensed under the terms of the %1.").arg(wrapLicense(tr("GNU General Public License, version 3, or later"))));
            break;
        case Gpl2:
            copyrightLines.prepend(tr("Licensed under the terms of the %1.").arg(wrapLicense(tr("GNU General Public License, version 2"))));
            break;
        case Gpl2OrLater:
            copyrightLines.prepend(tr("Licensed under the terms of the %1.").arg(wrapLicense(tr("GNU General Public License, version 2, or later"))));
            break;
        case Lgpl3:
            copyrightLines.prepend(tr("Licensed under the terms of the %1.").arg(wrapLicense(tr("GNU Lesser General Public License, version 3"))));
            break;
        case Lgpl3OrLater:
            copyrightLines.prepend(tr("Licensed under the terms of the %1.").arg(wrapLicense(tr("GNU Lesser General Public License, version 3, or later"))));
            break;
        case Lgpl2_1:
            copyrightLines.prepend(tr("Licensed under the terms of the %1.").arg(wrapLicense(tr("GNU Lesser General Public License, version 2.1"))));
            break;
        case Lgpl2_1OrLater:
            copyrightLines.prepend(tr("Licensed under the terms of the %1.").arg(wrapLicense(tr("GNU Lesser General Public License, version 2.1, or later"))));
            break;
        case Other:;
    }

    if (!d->copyrightHolder.isEmpty()) {
        copyrightLines.prepend(tr("Copyright © %1.").arg(d->copyrightYear.isEmpty() ? d->copyrightHolder : d->copyrightHolder + " " + d->copyrightYear));
    }

    return copyrightLines;
}

tApplication::KnownLicenses tApplication::applicationLicense() {
    return d->license;
}

void tApplication::ensureSingleInstance(QJsonObject launchData) {
    QString sharedMemoryName = QStringList({"the-libs-single-instance", organizationName(), applicationName()}).join("_");

#if QT_CONFIG(sharedmemory)
    #ifdef Q_OS_UNIX
    // Mitigate crashes by discarding the memory if it is not being used
    d->singleInstanceMemory = new QSharedMemory(sharedMemoryName);
    d->singleInstanceMemory->attach();
    delete d->singleInstanceMemory;
    d->singleInstanceMemory = nullptr;
    #endif

    d->singleInstanceMemory = new QSharedMemory(sharedMemoryName);
    if (d->singleInstanceMemory->create(sharedMemoryName.length())) {
        QLocalServer::removeServer(sharedMemoryName);
        d->singleInstanceServer = new QLocalServer();
        connect(d->singleInstanceServer, &QLocalServer::newConnection, [=] {
            QLocalSocket* socket = d->singleInstanceServer->nextPendingConnection();
            connect(socket, &QLocalSocket::readyRead, [=] {
                QJsonObject obj = QJsonDocument::fromJson(socket->readAll()).object();
                emit static_cast<tApplication*>(tApplication::instance())->singleInstanceMessage(obj);
            });
            connect(socket, &QLocalSocket::disconnected, [=] {
                socket->deleteLater();
            });
        });
        d->singleInstanceServer->listen(sharedMemoryName);
    } else {
        if (!d->singleInstanceMemory->attach()) std::exit(0); // Can't attach to the memory

        QLocalSocket* socket = new QLocalSocket();
        socket->connectToServer(sharedMemoryName);
        socket->waitForConnected();
        socket->write(QJsonDocument(launchData).toJson());
        socket->waitForBytesWritten();
        socket->close();

        std::exit(0);
    }
#else
    // Can't ensure single instance on this platform
#endif
}

tApplication::Platform tApplication::currentPlatform() {
#if defined(Q_OS_WIN)
    if (!d->hasCheckedIsRunningAsUwp) {
        UINT32 length = PACKAGE_FAMILY_NAME_MAX_LENGTH + 1;
        wchar_t packageFamilyName[PACKAGE_FAMILY_NAME_MAX_LENGTH + 1];
        LONG result = GetPackageFamilyName(GetCurrentProcess(), &length, packageFamilyName);
        d->isRunningAsUwp = result == ERROR_SUCCESS;
        d->hasCheckedIsRunningAsUwp = true;
    }

    if (d->isRunningAsUwp) return WindowsAppPackage;
    return Windows;
#elif defined(Q_OS_MAC)
    return MacOS;
#else
    if (qEnvironmentVariableIsSet("FLATPAK_ID")) {
        return Flatpak;
    }

    QString desktop = qEnvironmentVariable("XDG_CURRENT_DESKTOP");
    if (desktop == "theDesk") {
        return TheDesk;
    } else {
        return OtherPlatform;
    }
#endif
}

bool tApplication::isInitialised() {
    return d->isInitialised;
}

QString tApplication::copyrightHolder() {
    return d->copyrightHolder;
}

QString tApplication::copyrightYear() {
    return d->copyrightYear;
}

bool tApplication::haveApplicationUrl(tApplication::UrlType type) {
    return d->applicationUrls.contains(type);
}

QUrl tApplication::applicationUrl(tApplication::UrlType type) {
    return d->applicationUrls.value(type);
}

void tApplication::restart() {
#ifdef Q_OS_MAC
    QProcess::startDetached("open", {macOSBundlePath(), "-n"});
#else
    QProcess::startDetached(tApplication::applicationFilePath(), QStringList());
#endif
    tApplication::quit();
}

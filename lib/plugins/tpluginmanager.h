#ifndef TPLUGINMANAGER_H
#define TPLUGINMANAGER_H

#include "../tlogger.h"
#include <QDir>
#include <QDirIterator>
#include <QPluginLoader>

template<typename T>
class tPluginManager {
    public:
        tPluginManager(QString libraryDirectory) {
            this->libraryDirectory = libraryDirectory;
        };

        void load() {
            // Load all available plugins
            QStringList searchPaths = {
                QDir::cleanPath(qApp->applicationDirPath() + "/../plugins")};

#ifdef T_OS_UNIX_NOT_MAC
            searchPaths.append(QString(SYSTEM_LIBRARY_DIRECTORY).append(QStringLiteral("/%1/plugins").arg(libraryDirectory)));
#elif defined(Q_OS_WIN)
            searchPaths.append(qApp->applicationDirPath() + "/../../plugins");
            searchPaths.append(qApp->applicationDirPath() + "/plugins");
            searchPaths.append(qApp->applicationDirPath());
#elif defined(Q_OS_MAC)
            searchPaths.append(tApplication::macOSBundlePath() + "/Contents/Resources/Plugins");
            searchPaths.append(tApplication::macOSBundlePath() + "/../../plugins");
#endif

            QStringList seenPlugins;

            for (QString searchPath : searchPaths) {
                QDirIterator iterator(searchPath, {"*.so", "*.dll", "*.dylib"}, QDir::NoFilter, QDirIterator::Subdirectories);
                while (iterator.hasNext()) {
                    iterator.next();
                    if (seenPlugins.contains(iterator.fileName())) continue;
                    QPluginLoader loader(iterator.filePath());
                    QObject* instance = loader.instance();
                    T* plugin = qobject_cast<T*>(instance);
                    if (plugin) {
                        plugin->activate();
                        seenPlugins.append(iterator.fileName());
                    } else {
                        tWarn("PluginManager") << "Could not load plugin " << iterator.filePath();
                        tWarn("PluginManager") << loader.errorString();
                    }
                }
            }
        }

    private:
        QString libraryDirectory;
};

#endif // TPLUGINMANAGER_H

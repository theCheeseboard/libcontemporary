#include "tbasedimanager.h"

#include <QStack>

struct tBaseDIManagerPrivate {
        static QStack<tBaseDIManager*> diManagers;
};

QStack<tBaseDIManager*> tBaseDIManagerPrivate::diManagers = {};

tBaseDIManager::tBaseDIManager(QObject* parent) {
    d = new tBaseDIManagerPrivate();
}

tBaseDIManager::~tBaseDIManager() {
    delete d;
}

tBaseDIManager* tBaseDIManager::currentDIManager() {
    return tBaseDIManagerPrivate::diManagers.top();
}

void tBaseDIManager::pushCurrentDIManager(tBaseDIManager* manager) {
    // HACK: multithreading could really screw this up
    tBaseDIManagerPrivate::diManagers.push(manager);
}

void tBaseDIManager::popCurrentDIManager() {
    tBaseDIManagerPrivate::diManagers.pop();
}

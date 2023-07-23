#ifndef TDIMANAGER_H
#define TDIMANAGER_H

#include "tbasedimanager.h"
#include <QObject>

struct tDIManagerPrivate;
class DIDependentObject;
class tDIManager : public tBaseDIManager {
        Q_OBJECT
    public:
        explicit tDIManager(QObject* parent = nullptr);
        ~tDIManager();

        void addSingleton(QMetaObject interface, QMetaObject implementation);
        template<Contemporary::Concepts::Mocd Interface, Contemporary::Concepts::IsValidImplementation<Interface> Implementation> void addSingleton() {
            this->addSingleton(Interface::staticMetaObject, Implementation::staticMetaObject);
        };
        template<Contemporary::Concepts::IsQObject Implementation> void addSingleton() {
            this->addSingleton<Implementation, Implementation>();
        };

        tInjectedPointer<QObject> requiredService(QMetaObject interface);

    signals:

    protected:
        friend DIDependentObject;
        QSharedPointer<QObject> internalRequiredService(QString interface);

    private:
        tDIManagerPrivate* d;
};

#define T_INJECTED(type, name) tInjectedPointer<type> name
#define T_INJECT(type, name) T_INJECTED(type, name) = {}

#endif // TDIMANAGER_H

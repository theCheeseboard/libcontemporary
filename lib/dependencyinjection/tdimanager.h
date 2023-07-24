#ifndef TDIMANAGER_H
#define TDIMANAGER_H

#include "diprovidedsingletonobject.h"
#include "tbasedimanager.h"
#include <QObject>

struct tDIManagerPrivate;
class DIDependentObject;
class LIBCONTEMPORARY_EXPORT tDIManager : public tBaseDIManager {
        Q_OBJECT
    public:
        explicit tDIManager(QObject* parent = nullptr);
        ~tDIManager();

        void addSingleton(QMetaObject interface, QMetaObject implementation);
        template<Contemporary::Concepts::IsValidInterface Interface, Contemporary::Concepts::IsValidImplementation<Interface> Implementation> void addSingleton() {
            this->addSingleton(Interface::staticMetaObject, Implementation::staticMetaObject);
        };
        template<Contemporary::Concepts::IsQObject Implementation> void addSingleton() {
            this->addSingleton<Implementation, Implementation>();
        };
        template<Contemporary::Concepts::IsValidInterface Interface, Contemporary::Concepts::Inherits<Interface> T> void addSingleton(T& implementation) {
            pushDependency(Interface::staticMetaObject, new DIProvidedSingletonObject(Interface::staticMetaObject, this, implementation));
        };

        tInjectedPointer<tDIBaseInterface> requiredService(QMetaObject interface);

    signals:

    protected:
        friend DIDependentObject;
        QSharedPointer<tDIBaseInterface> internalRequiredService(QString interface);

    private:
        tDIManagerPrivate* d;

        void pushDependency(QMetaObject interface, DIDependentObject* dependentObject);
};

#endif // TDIMANAGER_H

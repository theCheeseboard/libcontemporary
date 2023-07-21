#ifndef TDIMANAGER_H
#define TDIMANAGER_H

#include "tinjectedpointer.h"
#include <QObject>

struct tDIManagerPrivate;
class DIDependentObject;
class tDIManager : public QObject {
        Q_OBJECT
    public:
        explicit tDIManager(QObject* parent = nullptr);

        void addSingleton(QMetaObject interface, QMetaObject implementation);
        template<Contemporary::Concepts::Mocd Interface, Contemporary::Concepts::IsValidImplementation<Interface> Implementation> void addSingleton() {
            this->addSingleton(Interface::staticMetaObject, Implementation::staticMetaObject);
        };
        template<Contemporary::Concepts::IsQObject Implementation> void addSingleton() {
            this->addSingleton<Implementation, Implementation>();
        };

        tInjectedPointer<QObject> requiredService(QMetaObject interface);
        template<Contemporary::Concepts::Mocd Interface> tInjectedPointer<Interface> requiredService() {
            return this->requiredService(Interface::staticMetaObject).template reinterpretCast<Interface>();
        }

    signals:

    protected:
        friend DIDependentObject;
        QSharedPointer<QObject> internalRequiredService(QString interface);

    private:
        tDIManagerPrivate* d;
};

#endif // TDIMANAGER_H

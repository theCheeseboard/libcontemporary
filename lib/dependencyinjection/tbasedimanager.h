#ifndef TBASEDIMANAGER_H
#define TBASEDIMANAGER_H

#include "diconcepts.h"
#include "tdibaseinterface.h"
#include <QObject>

template<typename T> class tInjectedPointer;

class DIDependentObject;
struct tBaseDIManagerPrivate;
class tBaseDIManager : public QObject {
        Q_OBJECT
    public:
        explicit tBaseDIManager(QObject* parent = nullptr);
        ~tBaseDIManager();

        static tBaseDIManager* currentDIManager();

        virtual tInjectedPointer<tDIBaseInterface> requiredService(QMetaObject interface) = 0;
        template<Contemporary::Concepts::Mocd Interface> tInjectedPointer<Interface> requiredService() {
            return this->requiredService(Interface::staticMetaObject).template reinterpretCast<Interface>();
        }

    protected:
        friend DIDependentObject;
        static void pushCurrentDIManager(tBaseDIManager* manager);
        static void popCurrentDIManager();

    private:
        tBaseDIManagerPrivate* d;
};

#endif // TBASEDIMANAGER_H

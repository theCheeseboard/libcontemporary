#ifndef TBASEDIMANAGER_H
#define TBASEDIMANAGER_H

#include "../libcontemporary_global.h"
#include "diconcepts.h"
#include "tdibaseinterface.h"
#include <QObject>

template<typename T> class tInjectedPointer;

class DIDependentObject;
struct tBaseDIManagerPrivate;
class LIBCONTEMPORARY_EXPORT tBaseDIManager : public QObject {
        Q_OBJECT
    public:
        explicit tBaseDIManager(QObject* parent = nullptr);
        ~tBaseDIManager();

        static tBaseDIManager* currentDIManager();

        virtual tInjectedPointer<tDIBaseInterface> requiredService(QMetaObject interface) = 0;
        template<Contemporary::Concepts::Mocd Interface> tInjectedPointer<Interface> requiredService() {
            return this->requiredService(Interface::staticMetaObject).template reinterpretCast<Interface>();
        }

        template<Contemporary::Concepts::IsConstructible Constructible> tInjectedPointer<Constructible> construct() {
            tBaseDIManager::pushCurrentDIManager(this);
            auto returnValue = Constructible::staticMetaObject.newInstance();
            tBaseDIManager::popCurrentDIManager();
            return QSharedPointer<tDIBaseInterface>(dynamic_cast<tDIBaseInterface*>(returnValue));
        }

    protected:
        friend DIDependentObject;
        static void pushCurrentDIManager(tBaseDIManager* manager);
        static void popCurrentDIManager();

    private:
        tBaseDIManagerPrivate* d;
};

#endif // TBASEDIMANAGER_H

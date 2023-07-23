#ifndef TINJECTEDPOINTER_H
#define TINJECTEDPOINTER_H

#include "tbasedimanager.h"
#include <QSharedPointer>

struct tInjectedPointerPrivate {
        QSharedPointer<tDIBaseInterface> underlyingPointer;
};

class DIDependentObject;
template<typename T> class tInjectedPointer {
    public:
        tInjectedPointer() {
            d = new tInjectedPointerPrivate();
            d->underlyingPointer = tBaseDIManager::currentDIManager()->requiredService<T>().d->underlyingPointer;
        }

        tInjectedPointer(QSharedPointer<tDIBaseInterface> underlyingPointer) {
            d = new tInjectedPointerPrivate();
            d->underlyingPointer = underlyingPointer;
        }

        tInjectedPointer(const tInjectedPointer<T>& other) {
            d = new tInjectedPointerPrivate();
            d->underlyingPointer = other.d->underlyingPointer;
        }

        tInjectedPointer(tInjectedPointer<T>&& other) {
            this->d = other.d;
            other.d = nullptr;
        }

        ~tInjectedPointer() {
            delete d;
        }

        template<Contemporary::Concepts::Mocd CastTarget> tInjectedPointer<CastTarget> objectCast() {
            return tInjectedPointer<CastTarget>(d->underlyingPointer.template objectCast<CastTarget>());
        }

        template<Contemporary::Concepts::Mocd CastTarget> tInjectedPointer<CastTarget> staticCast() {
            return tInjectedPointer<CastTarget>(d->underlyingPointer.template staticCast<CastTarget>());
        }

        template<Contemporary::Concepts::Mocd CastTarget> tInjectedPointer<CastTarget> reinterpretCast() {
            return tInjectedPointer<CastTarget>(d->underlyingPointer);
        }

        T* operator->() {
            return static_cast<T*>(d->underlyingPointer.data());
        }

        tInjectedPointer<T> operator=(const tInjectedPointer<T>& other) {
            d->underlyingPointer = other.d->underlyingPointer;
            return *this;
        }

        bool operator==(tInjectedPointer<T> other) const {
            return d->underlyingPointer == other.d->underlyingPointer;
        }

    protected:
        friend DIDependentObject;
        void propagateTo(void* other) {
            reinterpret_cast<tInjectedPointer<QObject>*>(other)->d->underlyingPointer = d->underlyingPointer;
        }

    private:
        tInjectedPointerPrivate* d = nullptr;
};

Q_DECLARE_METATYPE(QList<tInjectedPointer<QObject>>)

#define T_INJECTED(type, name) tInjectedPointer<type> name
#define T_INJECT(type, name) T_INJECTED(type, name) = {}

#endif // TINJECTEDPOINTER_H

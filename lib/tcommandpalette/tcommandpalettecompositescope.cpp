#include "tcommandpalettecompositescope.h"

struct tCommandPaletteCompositeScopePrivate {
        QString displayName;
        QList<QPair<tCommandPaletteScope*, QModelIndex>> shown;
        QList<tCommandPaletteScope*> registeredScopes;

        QString currentFilter;

        int resetCount = 0;
};

tCommandPaletteCompositeScope::tCommandPaletteCompositeScope(QString displayName, QObject* parent) :
    tCommandPaletteScope{parent} {
    d = new tCommandPaletteCompositeScopePrivate();
    d->displayName = displayName;
}

tCommandPaletteCompositeScope::~tCommandPaletteCompositeScope() {
    delete d;
}

void tCommandPaletteCompositeScope::startReset() {
    if (d->resetCount == 0) {
        this->beginResetModel();
        d->shown.clear();
    }
    d->resetCount++;
}

void tCommandPaletteCompositeScope::endReset() {
    if (d->resetCount == 1) {
        // Increment and decrement the reset count in order to
        // avoid an infinite loop
        d->resetCount++;
        this->filter(d->currentFilter);
        d->resetCount--;
        this->endResetModel();
    }
    d->resetCount--;
}

void tCommandPaletteCompositeScope::registerScope(tCommandPaletteScope* scope) {
    connect(scope, &tCommandPaletteScope::modelAboutToBeReset, this, [this] {
        startReset();
    });
    connect(scope, &tCommandPaletteScope::modelReset, this, [this] {
        endReset();
    });
    d->registeredScopes.append(scope);
}

bool tCommandPaletteCompositeScope::scopeRegistered(tCommandPaletteScope* scope) {
    return d->registeredScopes.contains(scope);
}

int tCommandPaletteCompositeScope::rowCount(const QModelIndex& parent) const {
    return d->shown.length();
}

QVariant tCommandPaletteCompositeScope::data(const QModelIndex& index, int role) const {
    auto descriptor = d->shown.at(index.row());
    return descriptor.second.data(role);
}

void tCommandPaletteCompositeScope::filter(QString filter) {
    if (d->resetCount != 0) return;

    startReset();
    d->currentFilter = filter;
    if (filter.isEmpty()) {
        d->shown.clear();
        endReset();
        return;
    }

    for (auto* scope : d->registeredScopes) {
        if (scope == this) continue;

        scope->filter(filter);
        for (auto i = 0; i < scope->rowCount(); i++) {
            auto index = scope->index(i);
            d->shown.append({scope, index});
        }
    }
    endReset();
}

void tCommandPaletteCompositeScope::activate(QModelIndex index) {
    auto descriptor = d->shown.at(index.row());
    descriptor.first->activate(descriptor.second);
}

QString tCommandPaletteCompositeScope::displayName() {
    return d->displayName;
}

Qt::ItemFlags tCommandPaletteCompositeScope::flags(const QModelIndex& index) const {
    auto descriptor = d->shown.at(index.row());
    return descriptor.second.flags();
}

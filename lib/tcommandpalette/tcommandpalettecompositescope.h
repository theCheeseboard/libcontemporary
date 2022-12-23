#ifndef TCOMMANDPALETTECOMPOSITESCOPE_H
#define TCOMMANDPALETTECOMPOSITESCOPE_H

#include "libcontemporary_global.h"
#include "tcommandpalettescope.h"

class tCommandPaletteCompositeScopePrivate;
class LIBCONTEMPORARY_EXPORT tCommandPaletteCompositeScope : public tCommandPaletteScope {
        Q_OBJECT
    public:
        explicit tCommandPaletteCompositeScope(QString displayName, QObject* parent = nullptr);
        ~tCommandPaletteCompositeScope();

        void startReset();
        void endReset();

        void registerScope(tCommandPaletteScope* scope);
        bool scopeRegistered(tCommandPaletteScope* scope);

    private:
        tCommandPaletteCompositeScopePrivate* d;

        // QAbstractItemModel interface
    public:
        int rowCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;

        // tCommandPaletteScope interface
    public:
        QString displayName();
        void filter(QString filter);
        void activate(QModelIndex index);

        // QAbstractItemModel interface
    public:
        Qt::ItemFlags flags(const QModelIndex& index) const;
};

#endif // TCOMMANDPALETTECOMPOSITESCOPE_H

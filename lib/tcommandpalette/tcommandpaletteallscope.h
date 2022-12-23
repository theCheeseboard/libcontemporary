#ifndef TCOMMANDPALETTEALLSCOPE_H
#define TCOMMANDPALETTEALLSCOPE_H

#include "tcommandpalettecompositescope.h"

class tCommandPaletteController;
struct tCommandPaletteAllScopePrivate;
class tCommandPaletteAllScope : public tCommandPaletteCompositeScope {
        Q_OBJECT
    public:
        explicit tCommandPaletteAllScope(tCommandPaletteController* controller, QObject* parent = nullptr);
        ~tCommandPaletteAllScope();

    private:
        tCommandPaletteAllScopePrivate* d;

        void updateScopes();
};

#endif // TCOMMANDPALETTEALLSCOPE_H

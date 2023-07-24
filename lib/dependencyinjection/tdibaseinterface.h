#ifndef TDIBASEINTERFACE_H
#define TDIBASEINTERFACE_H

#include "../libcontemporary_global.h"

class LIBCONTEMPORARY_EXPORT tDIBaseInterface {
    public:
        virtual ~tDIBaseInterface() = default;
};

#define T_DI_CONSTRUCTOR                                     \
    static const bool t_injected_have_di_constructor = true; \
    Q_SCRIPTABLE

#endif // TDIBASEINTERFACE_H

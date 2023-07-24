#ifndef DICONCEPTS_H
#define DICONCEPTS_H

#include "tdibaseinterface.h"
#include <QObject>

namespace Contemporary::Concepts {
    template<typename T>
    concept Mocd = std::is_convertible_v<decltype(T::staticMetaObject), const QMetaObject>;

    template<typename Implementation, typename Interface>
    concept Inherits = std::is_base_of_v<Interface, Implementation> && Mocd<Implementation>;

    template<typename Implementation>
    concept IsQObject = Inherits<Implementation, QObject> && Mocd<Implementation>;

    template<typename Implementation, typename Interface>
    concept IsValidImplementation = Inherits<Implementation, Interface> && IsQObject<Implementation> && std::default_initializable<Implementation>;

    template<typename Interface>
    concept IsValidInterface = Mocd<Interface> && Inherits<Interface, tDIBaseInterface>;

    template<typename Constructible>
    concept IsConstructible = IsValidImplementation<Constructible, Constructible>;

    template<typename T, typename = int>
    concept HasTInjectedHaveDiConstructorAnnotation = requires(T t) {
        (t.t_injected_have_di_constructor);
    };

} // namespace Contemporary::Concepts

#endif // DICONCEPTS_H

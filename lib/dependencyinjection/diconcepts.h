#ifndef DICONCEPTS_H
#define DICONCEPTS_H

#include <QObject>

namespace Contemporary::Concepts {
    template<typename T>
    concept Mocd = std::is_convertible<typeof(T::staticMetaObject), const QMetaObject>::value;

    template<typename Implementation, typename Interface>
    concept Inherits = std::is_base_of<Interface, Implementation>::value && Mocd<Implementation>;

    template<typename Implementation>
    concept IsQObject = Inherits<Implementation, QObject> && Mocd<Implementation>;

    template<typename Implementation, typename Interface>
    concept IsValidImplementation = Inherits<Implementation, Interface> && IsQObject<Implementation>;
} // namespace Contemporary::Concepts

#endif // DICONCEPTS_H

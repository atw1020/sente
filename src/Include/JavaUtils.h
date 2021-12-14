//
// Created by arthur wesley on 12/13/21.
//

#ifndef SENTE_JAVAUTILS_H
#define SENTE_JAVAUTILS_H

#include <type_traits>

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

#endif //SENTE_JAVAUTILS_H

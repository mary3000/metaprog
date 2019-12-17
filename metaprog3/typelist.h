//
// Created by sham42 on 17.12.2019.
//

#ifndef METAPROG3_TYPELIST_H
#define METAPROG3_TYPELIST_H

#include <type_traits>

// From 2d homework

struct NullType {
};

template<typename T, typename... Others>
struct Typelist {
    using head = T;
    using tail = Typelist<Others...>;

    template<typename C>
    struct contains {
        static constexpr bool value = std::is_same<T, C>::value || Typelist<Others...>::template contains<C>::value;
    };

    template<bool is_subtype, typename U>
    struct GetSubtypeImpl {
    };

    template<typename U>
    struct GetSubtype {
        using value = typename GetSubtypeImpl<std::is_base_of<U, T>::value, U>::value;
    };

    template<typename U>
    struct GetSubtypeImpl<false, U> {
        using value = typename Typelist<Others...>::template GetSubtype<U>::value;
    };

    template<typename U>
    struct GetSubtypeImpl<true, U> {
        using value = T;
    };

    template<typename None, typename U>
    struct IsParentOf {
        static constexpr bool value =
                std::is_base_of<typename U::head, typename GetSubtype<typename U::head>::value>::value ||
                IsParentOf<None, typename U::tail>::value;
    };

    template<typename None>
    struct IsParentOf<None, NullType> {
        static constexpr bool value = false;
    };

    template<typename U>
    struct IsStrictParentOf {
        static constexpr bool value = !(std::is_same<Typelist<T, Others...>, U>::value) && IsParentOf<void, U>::value;
    };


};

template<typename T>
struct Typelist<T> {
    using head = T;
    using tail = NullType;

    template<typename C>
    struct contains {
        static constexpr bool value = std::is_same<T, C>::value;
    };

    template<typename U>
    struct GetSubtype {
        using value = T;
    };
};

#endif //METAPROG3_TYPELIST_H

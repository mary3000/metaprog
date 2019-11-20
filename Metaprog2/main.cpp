#include <iostream>
#include <any>
#include <functional>
#include <utility>

class Product {

};

class Chair : public Product {
public:
    Chair() {
        std::cout << "Chair" << std::endl;
    }
};

class Table : public Product {
public:
    Table() {
        std::cout << "Table" << std::endl;
    }
};

class Sofa : public Product {
public:
    Sofa() {
        std::cout << "Sofa" << std::endl;
    }
};

class WoodenChair : public Chair {
public:
    WoodenChair() {
        std::cout << "WoodenChair" << std::endl;
    }
};

class WoodenTable : public Table {
public:
    WoodenTable() {
        std::cout << "WoodenTable" << std::endl;
    }
};

class WoodenSofa : public Sofa {
public:
    WoodenSofa() {
        std::cout << "WoodenSofa" << std::endl;
    }
};

class MetalChair : public Chair {
public:
    MetalChair() {
        std::cout << "MetalChair" << std::endl;
    }
};

class MetalTable : public Table {
public:
    MetalTable() {
        std::cout << "MetalTable" << std::endl;
    }
};

class MetalSofa : public Sofa {
public:
    MetalSofa() {
        std::cout << "MetalSofa" << std::endl;
    }
};

class MetalRichChair : public MetalChair {
public:
    MetalRichChair() {
        std::cout << "MetalRichChair" << std::endl;
    }
};

class MetalRichTable : public MetalTable {
public:
    MetalRichTable() {
        std::cout << "MetalRichTable" << std::endl;
    }
};

class MetalRichSofa : public MetalSofa {
public:
    MetalRichSofa() {
        std::cout << "MetalRichSofa" << std::endl;
    }
};

class MetalPoorChair : public MetalChair {
public:
    MetalPoorChair() {
        std::cout << "MetalPoorChair" << std::endl;
    }
};

class MetalPoorTable : public MetalTable {
public:
    MetalPoorTable() {
        std::cout << "MetalPoorTable" << std::endl;
    }
};

class MetalPoorSofa : public MetalSofa {
public:
    MetalPoorSofa() {
        std::cout << "MetalPoorSofa" << std::endl;
    }
};

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

template<typename T, typename Head, typename... Tail>
struct ConcreteFactory {
    template<typename C>
    C *Get() {
        using CType = typename T::template GetSubtype<C>::value;
        if (std::is_base_of<C, CType>::value) {
            return reinterpret_cast<C*>(new CType());
        }
        return ConcreteFactory<closest_parent, Head, Tail...>().template Get<C>();
        //return new typename GetClosestValue<std::is_base_of<C, CType>::value, C, CType, closest_parent>::value();
    }

   /* template<bool is_base, typename C, typename CType, typename P>
    struct GetClosestValue;

    template<typename C, typename CType, typename P>
    struct GetClosestValue<true, C, CType, P> {
        using value = CType;
    };

    template<typename C, typename CType, typename P>
    struct GetClosestValue<false, C, CType, P> {
        static constexpr C value = ConcreteFactory<P, Head, Tail...>().template Get<C>;
    };*/

    template<bool parent, typename L, typename C, typename... C1>
    struct GetSomeParentImpl;

    template<typename L, typename C, typename... C1>
    struct GetSomeParent {
        using value = typename GetSomeParentImpl<L::template IsStrictParentOf<C>::value, L, C, C1...>::value;
    };

    template<typename L, typename C, typename... C1>
    struct GetSomeParentImpl<true, L, C, C1...> {
        using value = C;
    };

    template<typename L, typename C, typename... C1>
    struct GetSomeParentImpl<false, L, C, C1...> {
        using value = typename GetSomeParent<L, C1...>::value;
    };

    template<typename L, typename C>
    struct GetSomeParentImpl<false, L, C> {
        using value = C;
    };

    template<typename L, typename P, typename C, typename... C1>
    struct GetClosestParent;

    template<bool l, bool p, typename L, typename P, typename C, typename... C1>
    struct GetClosestParentImpl {
        using value = typename GetClosestParent<L, P, C1...>::value;
    };

    template<typename L, typename P, typename C, typename... C1>
    struct GetClosestParent {
        using value = typename GetClosestParentImpl<L::template IsStrictParentOf<C>::value,
                C::template IsStrictParentOf<P>::value, L, P, C, C1...>::value;
    };

    template<typename L, typename P, typename C, typename... C1>
    struct GetClosestParentImpl<true, true, L, P, C, C1...> {
        using value = typename GetClosestParent<L, C, C1...>::value;
    };

    template<bool l, bool p, typename L, typename P, typename C>
    struct GetClosestParentImpl<l, p, L, P, C> {
        using value = P;
    };

    template<typename L, typename P, typename C>
    struct GetClosestParentImpl<true, true, L, P, C> {
        using value = C;
    };

    using some_parent = typename GetSomeParent<T, Head, Tail...>::value;
    using closest_parent = typename GetClosestParent<T, some_parent, Head, Tail...>::value;
};

template<int N, int K, typename Head, typename... Tail>
struct GetAbstractFactory {

    using List = Typelist<Head, Tail...>;

    template<typename T, bool eq_type, typename C, typename... C1>
    struct GetListImpl;

    template<typename T, typename C, typename... C1>
    struct GetList {
        using value = typename GetListImpl<T, C::template contains<T>::value, C, C1...>::value;
    };

    template<typename T, typename C, typename... C1>
    struct GetListImpl<T, true, C, C1...> {
        using value = C;
    };

    template<typename T, typename C, typename... C1>
    struct GetListImpl<T, false, C, C1...> {
        using value = typename GetList<T, C1...>::value;
    };


    template<typename C>
    struct GetConcreteFactory {
        using outer_list = typename GetList<C, Head, Tail...>::value;
        using result = ConcreteFactory<outer_list, Head, Tail...>;
    };
};

using MyFactoryHierarchy = GetAbstractFactory<3, 5,
        Typelist<Chair, Table, Sofa>,
                Typelist<WoodenChair, WoodenTable, WoodenSofa>,
                        Typelist<MetalChair, MetalTable, MetalSofa>,
                                Typelist<MetalRichChair, MetalRichTable, MetalRichSofa>,
                                        Typelist<MetalPoorChair, MetalPoorTable, MetalPoorSofa> >;

using MyIncompleteHierarchy = GetAbstractFactory<3, 5,
        Typelist<Chair, Table, Sofa>,
                Typelist<WoodenChair, WoodenTable, WoodenSofa>,
                        Typelist<MetalChair, MetalSofa>,
                                Typelist<MetalRichChair, MetalRichTable, MetalRichSofa>,
                                        Typelist<MetalPoorChair, MetalPoorTable> >;

int main() {

    // Base task

    auto MyFactory = new MyFactoryHierarchy::GetConcreteFactory<MetalRichChair>::result;
    Sofa *a = MyFactory->Get<Sofa>();

    // Should print:
    // Sofa
    // MetalSofa
    // MetalRichSofa

    delete MyFactory;
    delete a;

    // Additional task

    auto MyIncompleteFactory = new MyIncompleteHierarchy::GetConcreteFactory<MetalPoorChair>::result;
    Sofa *b = MyIncompleteFactory->Get<Sofa>();

    // Should print:
    // Sofa
    // MetalSofa

    delete MyIncompleteFactory;
    delete b;

    return 0;
}
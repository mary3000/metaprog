#include <iostream>
#include <any>
#include <functional>
#include <utility>

class Product {

};

class Chair : public Product {
public:
    Chair() {
        std::cout << "Char" << std::endl;
    }
};

class Table : public Product {

};

class Sofa : public Product {
public:
    Sofa() {
        std::cout << "Sofa" << std::endl;
    }
};

class WoodenChair : public Chair {

};

class WoodenTable : public Table {

};

class WoodenSofa : public Sofa {

};

class MetalChair : public Chair {

};

class MetalTable : public Table {

};

class MetalSofa : public Sofa {
public:
    MetalSofa() {
        std::cout << "MetalSofa" << std::endl;
    }
};

class MetalRichChair : public MetalChair {

};

class MetalRichTable : public MetalTable {

};

class MetalRichSofa : public MetalSofa {
public:
    MetalRichSofa() {
        std::cout << "MetalRichSofa" << std::endl;
    }
};

class MetalPoorChair : public MetalChair {

};

class MetalPoorTable : public MetalTable {

};

class MetalPoorSofa : public MetalSofa {

};

class NullType{};

template <typename T, typename... Others>
struct Typelist {
    using head = T;
    using tail = Typelist<Others...>;

    template <typename C>
    struct contains {
        static constexpr bool value = std::is_same<T, C>::value || Typelist<Others...>::template contains<C>::value;
    };

    template <bool is_subtype, typename U>
    struct GetSubtypeImpl {
    };

    template <typename U>
    struct GetSubtype {
        using value = typename GetSubtypeImpl<std::is_base_of<U, T>::value, U>::value;
    };

    template <typename U>
    struct GetSubtypeImpl<false, U> {
        using value = typename Typelist<Others...>::template GetSubtype<U>::value;
    };

    template <typename U>
    struct GetSubtypeImpl<true, U> {
        using value = T;
    };
};

template <typename T>
struct Typelist<T> {
    using head = T;
    using tail = NullType;

    template <typename C>
    struct contains {
        static constexpr bool value = std::is_same<T, C>::value;
    };

    template <typename U>
    struct GetSubtype {
        using value = T;
    };
};

template <typename T>
struct ConcreteFactory {
    template <typename C>
    C* Get() {
        return new typename T::template GetSubtype<C>::value();
    }
};

template <int N, int K, typename Head, typename... Tail>
struct GetAbstractFactory {

    template <typename T, bool eq_type, typename C, typename... C1>
    struct GetListImpl;

    template <typename T, typename C, typename... C1>
    struct GetList {
        using value = typename GetListImpl<T, C::template contains<T>::value, C, C1...>::value;
    };

    template <typename T, typename C, typename... C1>
    struct GetListImpl<T, true, C, C1...> {
        using value = C;
    };

    template <typename T, typename C, typename... C1>
    struct GetListImpl<T, false, C, C1...> {
        using value = typename GetList<T, C1...>::value;
    };

    template <typename C>
    struct GetConcreteFactory {
        using outer_list = typename GetList<C, Head, Tail...>::value;
        using result = ConcreteFactory<outer_list>;
    };
};

using MyFactoryHierarchy = GetAbstractFactory<3,5, Typelist<Chair, Table, Sofa>, Typelist< WoodenChair, WoodenTable, WoodenSofa>, Typelist< MetalChair, MetalTable, MetalSofa>, Typelist< MetalRichChair, MetalRichTable, MetalRichSofa>, Typelist< MetalPoorChair, MetalPoorTable, MetalPoorSofa> >;

int main() {
    auto MyFactory = new MyFactoryHierarchy::GetConcreteFactory<MetalRichChair>::result;
    Sofa* a = MyFactory->Get<Sofa>();

    // Should print
    // Sofa
    // MetalSofa
    // MetalRichSofa

    delete MyFactory;
    delete a;

    return 0;
}
//
// Created by sham42 on 17.12.2019.
//

#ifndef METAPROG3_VISIT_FACTORY_H
#define METAPROG3_VISIT_FACTORY_H

#include "shared.h"
#include "functor.h"
#include "typelist.h"

template <class V, class AList, class R, class... P>
class AcceptFactory;

template <class VList, class AList, class R, class... P>
class VisitFactory {
public:
    static R Go(Visitor* v, Acceptor* a, P... params) {
        auto cur_visitor = dynamic_cast<typename VList::head*>(v);
        if (cur_visitor != nullptr) {
            return AcceptFactory<typename VList::head, AList, R, P...>::Go(cur_visitor, a, params...);
        }
        return VisitFactory<typename VList::tail, AList, R, P...>::Go(v, a, params...);
    }

private:
};

template <class AList, class R, class... P>
class VisitFactory<NullType, AList, R, P...>  {
public:
    static R Go(Visitor* v, Acceptor* a, P... params) {
        std::cout << "couldn't find Visitor" << std::endl;
        return R();
    }

private:
};

template <class V, class AList, class R, class... P>
class AcceptFactory {
public:
    static R Go(V* v, Acceptor* a, P... params) {
        auto cur_acceptor = dynamic_cast<typename AList::head*>(a);
        if (cur_acceptor != nullptr) {
            cur_acceptor->Accept(v);
            StandartFunctor<R, P...> functor;
            return functor(params...);
        }
        return AcceptFactory<typename AList::tail, AList, R, P...>::Go(v, a, params...);
    }

private:
};

template <class AList, class R, class... P>
class AcceptFactory<NullType, AList, R, P...>  {
public:
    static R Go(Visitor* v, Acceptor* a, P... params) {
        std::cout << "couldn't find Acceptor" << std::endl;
        return R();
    }

private:
};

#endif //METAPROG3_VISIT_FACTORY_H

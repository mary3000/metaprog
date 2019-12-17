//
// Created by sham42 on 17.12.2019.
//

#ifndef METAPROG3_FUNCTOR_H
#define METAPROG3_FUNCTOR_H

#include <memory>

template <typename R, typename... P>
class IFunctor {
public:
    virtual R operator()(P...) = 0;
};

template <typename F, typename R, typename... P>
class FunctorImpl: public IFunctor<R, P...> {
public:
    FunctorImpl(F& f) : f_(&f) {};

    R operator()(P... params) override {
        return (*f_)(params...);
    }

private:
    F* f_;
};

template <typename R, typename... P>
class StandartFunctor: public IFunctor<R, P...> {
public:
    R operator()(P... params) override {
        std::cout << "standart functor" << std::endl;
        return R();
    }
};

template <typename R, typename... P>
class Functor {
public:
    template <class F>
    Functor(F& f) :
            impl_(std::make_shared<FunctorImpl<F, R, P...>>(f))
    {}

    R operator()(P... params) {
        return impl_->operator()(params...);
    }
private:
    std::shared_ptr<IFunctor<R, P...>> impl_;
};

#endif //METAPROG3_FUNCTOR_H

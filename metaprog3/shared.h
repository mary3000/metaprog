//
// Created by sham42 on 17.12.2019.
//

#ifndef METAPROG3_SHARED_H
#define METAPROG3_SHARED_H

class Visitor;
class Acceptor;
class AcceptorA;
class AcceptorB;

class Acceptor {
public:
    virtual void Accept(Visitor* v) = 0;
};

class Visitor {
public:
    virtual void Visit(AcceptorA* a) = 0;
    virtual void Visit(AcceptorB* b) = 0;
};

class VisitorX : public Visitor {
public:
    void Visit(AcceptorA* a) override {
        std::cout << "VisitAX" << std::endl;
    }

    void Visit(AcceptorB* b) override {
        std::cout << "VisitBX" << std::endl;
    }
};

class VisitorY : public Visitor {
public:
    void Visit(AcceptorA* a) override {
        std::cout << "VisitAY" << std::endl;
    }

    void Visit(AcceptorB* b) override {
        std::cout << "VisitBY" << std::endl;
    }
};

class AcceptorA : public Acceptor {
public:
    void Accept(Visitor *v) override {
        v->Visit(this);
    }
};

class AcceptorB : public Acceptor {
public:
    void Accept(Visitor *v) override {
        v->Visit(this);
    }
};

#endif //METAPROG3_SHARED_H

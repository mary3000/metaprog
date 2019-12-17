#include <iostream>

#include "visit_factory.h"
#include "functor.h"
#include "typelist.h"

using namespace std;

int main() {

    AcceptorA a;
    VisitorY y;
    a.Accept(&y);

    VisitFactory<Typelist<VisitorY>, Typelist<AcceptorA>, int, int> factory;
    cout << factory.Go(&y, &a, 5) << endl;

    AcceptorB b;

    cout << factory.Go(&y, &b, 42) << endl;

    return 0;
}

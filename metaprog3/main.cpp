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
    factory.Go(&y, &a, 5);

    return 0;
}

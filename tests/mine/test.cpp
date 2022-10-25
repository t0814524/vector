// build:
//      in root:
//      g++ -o main ./tests/mine/test.cpp vector.cpp
// Info:
//      vsc testrunner ext is bs (stopped working unless u delete some temp files somewhere) and
//      gcc doesnt link automatically as u d hope (guess ppl use make or build tools).

#include "../../vector.h"
// #include "vector.h"
int main()
{
    Vector vec = Vector();
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(55);
    vec.push_back(2);
    // for (auto &elem : vec)
    // {
    //     cout << "for " << elem;
    // }

    cout << vec;
    return 0;
}
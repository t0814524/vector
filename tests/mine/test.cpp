#include "../../vector.h"
// #include "vector.h"
// #include "../../vector.cpp" // todo: idk y i need that here
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
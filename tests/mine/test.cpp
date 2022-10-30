// #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// #include "doctest.h"

// int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

// TEST_CASE("testing the factorial function")
// {
//     CHECK(factorial(1) == 1);
//     CHECK(factorial(2) == 2);
//     CHECK(factorial(3) == 6);
//     CHECK(factorial(10) == 3628800);
// }

// build:
//      in root:
//      g++ -o main ./tests/mine/test.cpp vector.cpp
// Info:
//      vsc testrunner ext is bs (stopped working unless u delete some temp files somewhere) and
//      gcc doesnt link automatically as u d hope (guess ppl use make or build tools).

#include "../../vector.h"
int main()
{
    // Vector vec{3, 5, 7, 33};

    const Vector a({0., 1, 2, 3, 4, 5, 6, 7, 8, 9});
    Vector b(0);
    b = a;
    // Just for memory stuff....
    for (double i{0}; i < 200; i += 1)
    {
        b.push_back(i);
    }

    for (size_t i = 0; i < b.size(); ++i)
        cout << (a[i] == b[i]);
    // vec.push_back(3);
    // vec.push_back(4);
    // vec.push_back(55);
    // vec.push_back(332);

    // // for (auto &elem : vec)
    // // {
    // //     cout << "for " << elem;
    // // }

    // cout << vec;

    return 0;
}
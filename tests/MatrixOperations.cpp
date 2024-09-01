#include <complex>
#include <iostream>
#include "../Matrix2x2.h"

int main(int argc, char* argv[]) {

    Matrix2x2 a = Matrix2x2(
        2, 0,
        1, 1
    );

    Matrix2x2 b = Matrix2x2(
        2, 1,
        -1, 3
    );

    a.preMultiply(b);
    std::cout << "a now is: [" << a.e11 << ", " << a.e12 << "; " << a.e21 << ", " << a.e22 << "]" << std::endl;

    a.postMultiply(b);
    std::cout << "a now is: [" << a.e11 << ", " << a.e12 << "; " << a.e21 << ", " << a.e22 << "]" << std::endl;

    return EXIT_SUCCESS;
}

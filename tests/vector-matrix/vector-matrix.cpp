#include <iostream>
#include "../../include/Matrix2x2.h"

int main(int argc, char* argv[]) {
    Matrix2x2 A(0, 1, -1, 0);
    Vector2 v(2, 3);

    Vector2 w = v.preMultiply(A);

    std::cout << "v = (" << w.e1 << ", " << w.e2 << ")" << std::endl; 

    return EXIT_SUCCESS;
}

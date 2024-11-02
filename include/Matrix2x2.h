#ifndef MATRIX_2X2_H
#define MATRIX_2X2_H

#include <complex>

/**
 *  A 2x2 matrix of std::complex<double> numbers.
 *  (lazy implementation: only required functions are defined)
 */
struct Matrix2x2 {
    std::complex<double> e11;
    std::complex<double> e12;
    std::complex<double> e21;
    std::complex<double> e22;

    Matrix2x2(std::complex<double> e11_, std::complex<double> e12_, std::complex<double> e21_, std::complex<double> e22_)
        : e11(e11_), e12(e12_), e21(e21_), e22(e22_) {}
    
    Matrix2x2(const Matrix2x2& other)
        : e11(other.e11), e12(other.e12), e21(other.e21), e22(other.e22) {}

    Matrix2x2& preMultiply(const Matrix2x2& other);

    Matrix2x2& postMultiply(const Matrix2x2& other);
};

struct Vector2 {
    std::complex<double> e1;
    std::complex<double> e2;

    Vector2(std::complex<double> e1_, std::complex<double> e2_)
        : e1(e1_), e2(e2_) {}
    
    Vector2(const Vector2& other)
        :e1(other.e1), e2(other.e2) {}
    
    Vector2& preMultiply(const Matrix2x2& other);
};

#endif /* MATRIX_2X2_H */

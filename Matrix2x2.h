#include <complex>

/**
 *  A 2x2 matrix of std::complex<float> numbers.
 *  (lazy implementation: only required functions are defined)
 */
struct Matrix2x2 {
    std::complex<float> e11;
    std::complex<float> e12;
    std::complex<float> e21;
    std::complex<float> e22;

    Matrix2x2(std::complex<float> e11_, std::complex<float> e12_, std::complex<float> e21_, std::complex<float> e22_)
        : e11(e11_), e12(e12_), e21(e21_), e22(e22_) {}

    Matrix2x2& preMultiply(const Matrix2x2& other);

    Matrix2x2& postMultiply(const Matrix2x2& other);
};

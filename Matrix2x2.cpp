#include "Matrix2x2.h"

Matrix2x2& Matrix2x2::preMultiply(const Matrix2x2& other) {
    Matrix2x2 old(this->e11, this->e12, this->e21, this->e22);
    this->e11 = other.e11 * old.e11 + other.e12 * old.e21;
    this->e12 = other.e11 * old.e12 + other.e12 * old.e22;
    this->e21 = other.e21 * old.e11 + other.e22 * old.e21;
    this->e22 = other.e21 * old.e12 + other.e22 * old.e22;
    return *this;
}

Matrix2x2& Matrix2x2::postMultiply(const Matrix2x2& other) {
    Matrix2x2 old(this->e11, this->e12, this->e21, this->e22);
    this->e11 = old.e11 * other.e11 + old.e12 * other.e21;
    this->e12 = old.e11 * other.e12 + old.e12 * other.e22;
    this->e21 = old.e21 * other.e11 + old.e22 * other.e21;
    this->e22 = old.e21 * other.e12 + old.e22 * other.e22;
    return *this;
}

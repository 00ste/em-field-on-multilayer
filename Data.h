#ifndef DATA_H
#define DATA_H

#include <vector>
#include <complex>

struct OutputDataLine {
    float xp;
    float zp;
    std::complex<float> Ex;
    std::complex<float> Ey;
    std::complex<float> Ez;
    std::complex<float> Hx;
    std::complex<float> Hy;
    std::complex<float> Hz;
};

struct InputData {
    size_t numberOfMedia;
    std::vector<float> mediaZInterfaces;
    std::vector<std::complex<float>> mediaEpsilon;
    std::vector<std::complex<float>> mediaMu;
    std::complex<float> Einc;
    float angleOfIncidence;
    float wavelength;
};

#endif /* DATA_H */

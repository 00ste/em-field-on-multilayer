#ifndef DATA_H
#define DATA_H

#include <vector>
#include <complex>

#include "Matrix2x2.h"

/**
 *  Contains all the data that should be printed to one line of the output file.
 *  (xp, zp) are the x, z coordinates of the point where the field was calculated.
 *  (Ex, Ey, Ez) are the x,y,z coordinates of the electric field at (xp, zp).
 *  (Hx, Hy, Hz) are the x,y,z coordinates of the magnetic field at (xp, zp).
 */
struct OutputDataLine {
    double xp;
    double zp;
    std::complex<double> Ex;
    std::complex<double> Ey;
    std::complex<double> Ez;
    std::complex<double> Hx;
    std::complex<double> Hy;
    std::complex<double> Hz;
};

/**
 *  The polarization of the wave.
 */
enum Polarization { TE, TM };

/**
 *  All the necessary parameters for the problem to be well-posed.
 */
struct InputData {
    InputData() = default;

    std::size_t numberOfMedia;
    std::vector<double> mediaZInterfaces;
    std::vector<std::complex<double>> mediaEpsilon;
    std::vector<std::complex<double>> mediaMu;
    std::complex<double> Einc;
    double angleOfIncidence;
    double wavelength;
    Polarization polarization;
};

/**
 *  All the useful parameters for one medium.
 */
struct MediumData {
  MediumData()
        : zBottom{ 0 }, refractionIndex{ 0 }, waveImpedance{ 0 },
        sinTheta{ 0 }, cosTheta{ 0 }, wavenumber{ 0 },
        transmissionMatrix{ Matrix2x2{ 0, 0, 0, 0 } } {}

    double zBottom;
    std::complex<double> refractionIndex;
    std::complex<double> waveImpedance;
    std::complex<double> sinTheta;
    std::complex<double> cosTheta;
    std::complex<double> wavenumber;
    Matrix2x2 transmissionMatrix;
};

/**
 *  The x, z coordinates of a point where the EM fields can be calculated.
 */
struct Point {
    double x;
    double z;
};

#endif /* DATA_H */

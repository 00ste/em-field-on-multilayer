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
    float xp;
    float zp;
    std::complex<float> Ex;
    std::complex<float> Ey;
    std::complex<float> Ez;
    std::complex<float> Hx;
    std::complex<float> Hy;
    std::complex<float> Hz;
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
    std::vector<float> mediaZInterfaces;
    std::vector<std::complex<float>> mediaEpsilon;
    std::vector<std::complex<float>> mediaMu;
    std::complex<float> Einc;
    float angleOfIncidence;
    float wavelength;
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

  float zBottom;
  std::complex<float> refractionIndex;
  std::complex<float> waveImpedance;
  std::complex<float> sinTheta;
  std::complex<float> cosTheta;
  std::complex<float> wavenumber;
  Matrix2x2 transmissionMatrix;
};

#endif /* DATA_H */

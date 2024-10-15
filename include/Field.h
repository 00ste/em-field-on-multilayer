#ifndef FIELDS_H
#define FIELDS_H

#include "Matrix2x2.h"
#include "Data.h"

#include <complex>
#include <csignal>
#include <cstddef>
#include <vector>
#include <cmath>

class Field {
public:
    // CONSTANTS
    const float C0 = 299792458.0;
    const float PI = M_PI;
    const float MI0 = 4*PI*0.0000001;
    const float ETA0 = C0 * MI0;
    const std::complex<float> I = std::complex(0.f, 1.f);

    // Output variables
    std::complex<float> E[3];
    std::complex<float> H[3];

    // Information regarding the media
    std::size_t numMedia;
    std::vector<MediumData> media;

    // Information regarding the incident field
    float wavelength;
    Polarization polarization;
    Matrix2x2 E_T0;

    Field(InputData inputData);

    /**
     *  Calculcates the electric and magnetic fields E and H at the
     *  given point (xp, 0, zp).
     *  @param xp The x coordinate of the point where the fields should be calculated.
     *  @param zp The z coordinate of the point where the fields should be calculated.
     */
    void calculateEHFields(
        float xp,
        float zp
    );

private:

    /**
     *  Finds the index of the medium which contains the given zp coordinate.
     *  @param zp The z coordinate of the point that should be contained in the medium.
     *  @return The index of the medium.
     */
    std::size_t findMedium(float zp);

    /**
     *  Calculates the propagation matrix inside a medium. The method does not check if zp is outside the medium.
     *  @param mediumIndex The index of the medium.
     *  @param zp The z coordinate relative to the medium's top.
     *  @return The propagation matrix
     */
    Matrix2x2 propagationMatrix(std::size_t mediumIndex, float zp);
};

#endif /* FIELDS_H */

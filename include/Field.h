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
    const double C0 = 299792458.0;
    const double PI = M_PI;
    const double MI0 = 4*PI*0.0000001;
    const double ETA0 = C0 * MI0;
    const std::complex<double> I = std::complex(0.f, 1.f);
    const std::complex<double> ONE = std::complex(1.f, 0.f);

    // Output variables
    std::complex<double> E[3];
    std::complex<double> H[3];

    // Information regarding the media
    std::size_t numMedia;
    std::vector<MediumData> media;

    // Information regarding the incident field
    double startZ;
    double wavelength;
    Polarization polarization;
    Vector2 E_T0;

    Field(InputData inputData);

    /**
     *  Calculcates the electric and magnetic fields E and H at the
     *  given point (xp, 0, zp).
     *  @param xp The x coordinate of the point where the fields should be calculated.
     *  @param zp The z coordinate of the point where the fields should be calculated.
     */
    void calculateEHFields(
        double xp,
        double zp
    );

private:

    /**
     *  Finds the index of the medium which contains the given zp coordinate.
     *  @param zp The z coordinate of the point that should be contained in the medium.
     *  @return The index of the medium.
     */
    std::size_t findMedium(double zp);

    /**
     *  Calculates the propagation matrix inside a medium. The method does not check if zp is outside the medium.
     *  @param mediumIndex The index of the medium.
     *  @param zp The z coordinate relative to the medium's top.
     *  @return The propagation matrix
     */
    Matrix2x2 propagationMatrix(std::size_t mediumIndex, double zp);
};

#endif /* FIELDS_H */

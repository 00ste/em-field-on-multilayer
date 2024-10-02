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
    // const std::complex<float> I = std::complex<float>(0, 1);

    // Output variables
    std::complex<float> E[3];
    std::complex<float> H[3];

    // CLASS ATTRIBUTES
    // Information regarding the media
    std::size_t numMedia;
    std::vector<MediumData> media;

    // Information regarding the incident field
    float wavelength;
    Polarization polarization;
    Matrix2x2 E_T0;

    // PUBLIC METHODS
    Field(InputData inputData);

    /**
     *  Calculcates the electric and magnetic fields E and H at the
     *  given point (xp, 0, zp).
     *  @param incidenceAngle The angle of incidence of the wave.
     *  @param polarization The polarization type of the wave.
     *  @param wavelength The wavelength of the incident wave.
     *  @param E1inc The amplitude of the incident electric field.
     *  @param xp The x coordinate of the point where the fields should be calculated.
     *  @param zp The z coordinate of the point where the fields should be calculated.
     */
    void calculateEHFields(
        float incidenceAngle,
        Polarization polarization,
        float wavelength,
        std::complex<float> E1inc,
        float xp,
        float zp
    );

private:

    // PRIVATE METHODS
    /**
     *  Finds the index of the medium which contains the given zp coordinate.
     *  @param zp The z coordinate of the point that should be contained in the medium.
     *  @return The index of the medium.
     */
    std::size_t findMedium(float zp);
};

#endif /* FIELDS_H */

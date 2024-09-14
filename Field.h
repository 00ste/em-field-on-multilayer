#ifndef FIELDS_H
#define FIELDS_H

#include <complex>
#include <csignal>
#include <cstddef>
#include <vector>
#include <cmath>

#include "Matrix2x2.h"

class Field {
public:
    // HELPER CLASSES
    /**
     *  The polarization of the wave.
     */
    enum Polarization { TE, TM };

    // CONSTANTS
    const float C0 = 299792458.0;
    const float PI = M_PI;
    const float MI0 = 4*PI*0.0000001;
    const float ETA0 = C0 * MI0;
    const std::complex<float> I = std::complex<float>(0, 1);

    // Output variables
    std::complex<float> E[3];
    std::complex<float> H[3];

    // PUBLIC METHODS
    Field(
        const std::vector<float>& interfaceZCoords_,
        const std::vector<float>& mediaRefractionIndexes_
    );

    /**
     *  Configures the incoming electric field and incidence angle.
     */
    void setIncidentField(float incidentEAmplitude, float incidenceAngle);

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
    // CLASS ATTRIBUTES
    // Information regarding the media
    std::vector<float> interfaceZCoords;
    std::vector<float> mediaRefractionIndexes;

    // Information regarding the incident field
    float wavelength;
    Polarization polarization;

    // Information regarding the angle of incidence (sin and cos stored as class attributes for efficiency)
    float theta0;
    std::vector<float> cosTheta;
    std::vector<float> sinTheta;

    // PRIVATE METHODS
    /**
     *  Finds the index of the medium which contains the given zp coordinate.
     *  @param zp The z coordinate of the point that should be contained in the medium.
     *  @return The index of the medium.
     */
    size_t findMedium(float zp);

    /**
     *  Calculates the sin of theta for every medium using Snell's law.
     *  @param theta0 The angle of incidence with the first medium.
     *  @param result A reference to the vector used as output.
     */
    void calculateSinCosTheta(double theta0);

    // TODO: Implement this
    /**
     *
     */
    void VAmplitudeET();

    /**
     *  Calculates the transverse wave number of a medium.
     *  @param cosTheta The cos of the angle of the field through the medium.
     *  @param pol The polarization of the medium.
     *  @return The wave number.
     */
    double transverseWaveNumber(double cosTheta, Polarization pol);

    /**
    *  Calculates the wave number of the wave in the given medium.
    *  @param mediumIndex The medium.
    *  @return The wave number.
    */
    double waveNumber(size_t mediumIndex);

    /**
     *  Calculates the phase thinkness of a oblique-incident field through a medium.
     *  @param refractionIndex The refraction index of the medium.
     *  @param wavelength The wavelength of the wave.
     *  @param cosTheta The cosine of the angle of incidence
     *  @param zDepth The depth from the top of the given medium.
     *  @return The phase thickness.
     */
    inline std::complex<float> phaseThickness(
        std::complex<float> refractionIndex,
        float wavelength,
        float cosTheta,
        float zDepth
    ) {
        return 2*PI * refractionIndex * zDepth * cosTheta / wavelength;
    }



    /**
    *  Calculates the matching matrix between one medium and the previous one.
    *  @param sinTheta The sines of the angles.
    *  @param pol The polarization of the wave.
    *  @param medium The index of .
    *  @return the transmission coefficient.
    */
    Matrix2x2 matchingMatrix(Polarization pol, size_t mediumIndex);

    /**
    *  Calculates the transmission coefficient between one medium and the previous one.
    *  @param pol The polarization of the wave.
    *  @param medium The index of the medium.
    *  @return the transmission coefficient.
    */
    std::complex<float> transmissionCoefficient(Polarization pol, size_t mediumIndex);

    /**
    *  Calculates the propagation matrix inside a medium at a given depth from the top.
    *  @param sinTheta The sines of the angles.
    *  @param pol The polarization of the wave.
    *  @param medium The index of the medium.
    *  @return the transmission coefficient.
    */
    Matrix2x2 propagationMatrix(int zDepth, size_t mediumIndex);
};

#endif /* FIELDS_H */

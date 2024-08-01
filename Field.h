#ifndef FIELDS_H
#define FIELDS_H

#include <complex>
#include <vector>

class Fields {
    enum Polarization { TE, TM };
    // TODO: Define this better
    const float C0 = 299792458.0;
    const float MI0 = (4*pi)*1.0E-7;
    const float ETA0 = C0 * MI0;

    public:
        Fields(
            const std::vector<float>& interfaceZCoords_,
            const std::vector<float>& mediaRefractionIndexes_    
        );

        /**
         *  Calculcates the electric and magnetic fields E and H at the
         *  given point (xp, 0, zp).
         *  @param theta1 The angle of incidence of the wave.
         *  @param pol The polarization type of the wave.
         *  @param lambda0 The wavelength ??
         *  @param E1inc The value of the incident electric field.
         *  @param xp The x coordinate of the point where the fields should be calculated.
         *  @param zp The z coordinate of the point where the fields should be calculated.
         */
        void calculateEHFields(
            std::complex<float> theta1,
            Polarization pol,
            std::complex<float> lambda0,
            std::complex E1inc[3],
            float xp,
            float zp
        );

    private:
        std::vector<float> interfaceZCoords;
        std::vector<std::complex<float>> mediaRefractionIndexes;

        std::complex<float> E[3];
        std::complex<float> H[3];

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
        void calculateSinTheta(double theta_0, const std::vector<float>& result);

        // TODO: Implement this
        /**
         *
         */
        void VAmplitudeET();

        /**
        *  Calculates the transverse wave number of a medium.
        *  @param n The refraction indexes of the media.
        *  @param pol The polarization of the medium.
        *  @return The wave number.
        */
        double transverseWaveNumber(std::vector<double> n, double sinTheta, Polarization pol);

        /**
        *  Calculates the sines of the angles of the progressive and regressive waves
        *  inside each medium using Snell's law.
        *  @param n The refraction indexes of the media.
        *  @param theta_0 The angle of incidence with the first medium.
        *  @return A vector containing the sines of the angles.
        */
        std::vector<double> sinTheta(std::vector<double> n, double theta_0);

        /**
        *  Calculates the wave number.
        *  @param lambda_0 The wavelength in a vacuum.
        *  @param n_i The refraction index.
        *  @return The wave number.
        */
        double waveNumber(double lambda_0, double n_i);

        /**
        *  Calculates the phase thickness.
        *  @param sinTheta_i The sine of the angle.
        *  @param z_p The difference between the coordinate where the field should be calculated and the lengths of all the other media before it.
        */
        complex phaseThickness(double n_i, double sinTheta_i, double lambda_0, double zp);

        /**
        *  Calculates the reflection coefficient between one medium and the previous one.
        *  @param sinTheta The sines of the angles.
        *  @param pol The polarization of the wave.
        *  @param medium The index of the medium.
        *  @return the reflection coefficient.
        */
        complex reflectionCoefficient(std::vector<double> n, std::vector<double> sinTheta, Polarization pol, size_t mediumIndex);

        /**
        *  Calculates the transmission coefficient between one medium and the previous one.
        *  @param sinTheta The sines of the angles.
        *  @param pol The polarization of the wave.
        *  @param medium The index of the medium.
        *  @return the transmission coefficient.
        */
        complex transmissionCoefficient(std::vector<double> n, std::vector<double> sinTheta, Polarization pol, size_t mediumIndex);

        /**
        *  Calculates the transmission matrix between one medium and the previous one.
        *  @param sinTheta The sines of the angles.
        *  @param pol The polarization of the wave.
        *  @param medium The index of the medium.
        *  @return the transmission coefficient.
        */
        matrix transmissionMatrix1(std::vector<double> n, std::vector<double> sinTheta, Polarization pol, size_t mediumIndex);

}

#endif /* FIELDS_H */
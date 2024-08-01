#include "Field.h"

Field::Field(
    const std::vector<float>& interfaceZCoords_,
    const std::vector<float>& mediaRefractionIndexes_    
) : interfaceZCoords(interfaceZCoords_), mediaRefractionIndexes(mediaRefractionIndexes_) {}

void Field::calculateEHFields(
    std::complex<float> theta1,
    Polarization pol,
    std::complex<float> lambda0,
    std::complex E1inc[3],
    float xp,
    float zp
) {
    // Find which medium contains the point where the fields should be calculated.
    size_t mediumIndex = findMedium(interfaceZcoords, zp);

    // Calculate sinTheta for all media.
    std::vector<float> sinTheta;
    calculateSinTheta(sinTheta);

    // Calculate temporary E and H vectors.
    std::complex<float> Etemp[3];
    std::complex<float> Htemp[3];
    VAmplitudeET(Etemp, Htemp, sinTheta, pol, mediumIndex, lambda0, E1inc, xp, zp);

    // Finally calculate E and H.
    float commonConst = mediaRefractionIndexes[mediumIndex] / ETA0;
    float commonTerm = (Etemp[1] - Etemp[0]) / sqrt(1 - pow(sinTheta[mediumIndex], 2));
    std::complex<float> sumEHTemp = 69; // TODO: This should be sum(MatrixEHProvv)

    if (pol == TM) {
        E[0] = sumEHTemp;
        E[1] = 0;
        E[2] = commonTerm * sinTheta[mediumIndex];

        H[0] = 0;
        H[1] = -commonConst * commonTerm;
        H[2] = 0;
    }
    else { // pol == EM
        E[0] = 0;
        E[1] = sumEHTemp;
        E[2] = 0;

        H[0] = commonConst * commonTemp;
        H[1] = 0;
        H[2] = commonConst * sumEHTemp * sinTheta[mediumIndex];
    }
}

size_t findMedium(float zp) {
    size_t n = interfaceZCoords.size();
    if (zp <= interfaceZCoords[0]) return 0;
    if (zp > interfaceZCoords[n-1]) return n

    for (size_t i = 0; i < n; i++) {
        if (zp > interfaceZCoords[i] && zp <= interfaceZCoords[i+1])
            return i+1;
    }
}

void calculateSinTheta(double theta_0, const std::vector<float>& result) {
    result.reserve(mediaRefractionIndexes.size());

    result[0] = sin(theta_0);
    for (int i = 1; i < mediaRefractionIndexes.size(); i++) {
        result[i] = mediaRefractionIndexes[0] * result[1] / mediaRefractionIndexes[i];
    }
}

double transverseWaveNumber(double n, double sinTheta, Polarization pol) {
    switch (pol) {
        case Polarization::TM:
            return n * sqrt(1 - pow(sinTheta, 2));

        default:
            return n / sqrt(1 - pow(sinTheta, 2));
    }
}

std::vector<double> sinTheta(std::vector<double> n, double theta_0) {
    std::vector<double> result = std::vector<double>();
    result.reserve(n.size());

    result[0] = sin(theta_0);
    for (int i = 1; i < n.size(); i++) {
        result[i] = n[0] * result[1] / n[i];
    }

    return result;
}

double waveNumber(double lambda_0, double n_i) {
    return 2 * pi * n_i / lambda_0;
}

complex phaseThickness(double n_i, double sinTheta_i, double lambda_0, double zp) {
    if (sinTheta_i > 1)
        return -i * waveNumber(lambda_0, n_i) * sqrt(pow(n_i * sinTheta_i, 2) - pow(n_i, 2));
    else
        return waveNumber(lambda_0, n_i) * zp * sqrt(1 - pow(sinTheta_i, 2));
}

complex reflectionCoefficient(std::vector<double> n, std::vector<double> sinTheta, Polarization pol, size_t mediumIndex) {
    // TODO: Implement this
    return 0;
}

complex transmissionCoefficient(std::vector<double> n, std::vector<double> sinTheta, Polarization pol, size_t mediumIndex) {
    return complex(1) + reflectionCoefficient(n, sinTheta, pol, mediumIndex);
}

matrix transmissionMatrix1(std::vector<double> n, std::vector<double> sinTheta, Polarization pol, size_t mediumIndex) {

}
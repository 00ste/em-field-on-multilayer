#include "Field.h"

Field::Field(
    const std::vector<float>& interfaceZCoords_,
    const std::vector<float>& mediaRefractionIndexes_
) : interfaceZCoords(interfaceZCoords_), mediaRefractionIndexes(mediaRefractionIndexes_) {}

void Field::calculateEHFields(
    Polarization pol,
    std::complex<float> lambda0,
    std::complex<float> E1inc[3],
    float xp,
    float zp
) {
    // Find which medium contains the point where the fields should be calculated.
    size_t mediumIndex = findMedium(zp);

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

        H[0] = commonConst * commonTerm;
        H[1] = 0;
        H[2] = commonConst * sumEHTemp * sinTheta[mediumIndex];
    }
}

size_t Field::findMedium(float zp) {
    size_t n = interfaceZCoords.size();
    if (zp <= interfaceZCoords[0]) return 0;
    if (zp > interfaceZCoords[n-1]) return n;

    for (size_t i = 0; i < n; i++) {
        if (zp > interfaceZCoords[i] && zp <= interfaceZCoords[i+1])
            return i+1;
    }
}

void Field::calculateSinCosTheta(double theta0) {
    sinTheta.reserve(mediaRefractionIndexes.size());
    cosTheta.reserve(mediaRefractionIndexes.size());

    sinTheta[0] = sin(theta0);
    cosTheta[0] = cos(theta0);

    for (int i = 1; i < mediaRefractionIndexes.size(); i++) {
        sinTheta[i] = mediaRefractionIndexes[0] * sinTheta[1] / mediaRefractionIndexes[i];
        cosTheta[i] = sqrt(1 - std::pow(sinTheta[i], 2));
    }
}

double Field::transverseWaveNumber(double n, double cosTheta, Polarization pol) {
    if (pol == Polarization::TM)
        return n * cosTheta;
    // pol == Polarization::TE
    return n / cosTheta;
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

double Field::waveNumber(size_t mediumIndex) {
    return 2 * PI * mediaRefractionIndexes[mediumIndex] / wavelength;
}

std::complex<float> Field::phaseThickness(size_t mediumIndex, double zDepth) {
    float n = mediaRefractionIndexes[mediumIndex];
    // TODO: Implement this (use cosTheta instead of std::sqrt(1 - std::pow(sinTheta, 2)))
    return 0;
}

std::complex<float> Field::transmissionCoefficient(Polarization pol, size_t mediumIndex) {
    // TODO: Implement this
    return 0;
}

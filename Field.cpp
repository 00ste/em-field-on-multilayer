#include "Field.h"
#include <cerrno>
#include <cmath>
#include <iostream>

Field::Field(
    const std::vector<float>& interfaceZCoords_,
    const std::vector<float>& mediaRefractionIndexes_
) : interfaceZCoords(interfaceZCoords_), mediaRefractionIndexes(mediaRefractionIndexes_) {}

void Field::calculateEHFields(
    float incidenceAngle,
    Polarization polarization,
    float wavelength,
    std::complex<float> E1inc,
    float xp,
    float zp
) {
    // Find which medium contains the point where the fields should be calculated.
    size_t mediumIndex = findMedium(zp);

    // Calculate the transmission matrix by combining all matrices from all media above the current one.
    Matrix2x2 transmissionMatrix = Matrix2x2(1, 0, 0, 1);
    for (size_t i = 0; i < mediumIndex; i++) {
        transmissionMatrix.preMultiply(propagationMatrix(interfaceZCoords[i], mediumIndex));
        transmissionMatrix.preMultiply(matchingMatrix(polarization, i));
    }

    // Combine the current transmission matrix with the propagation matrix in the current medium.
    transmissionMatrix.preMultiply(propagationMatrix(zp - interfaceZCoords[mediumIndex], mediumIndex));

    // TODO: Finish this
    Matrix2x2 progressiveRegressiveElectricField = Matrix2x2(E1inc, 0, 0, 0);
    progressiveRegressiveElectricField.preMultiply(transmissionMatrix);

    std::cout << "E+: " << progressiveRegressiveElectricField.e11 << std::endl;
    std::cout << "E-: " << progressiveRegressiveElectricField.e21 << std::endl;
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

std::complex<float> Field::transmissionCoefficient(Polarization pol, size_t mediumIndex) {
    // TODO: Implement this
    return 0;
}

Matrix2x2 Field::propagationMatrix(int zDepth, size_t mediumIndex) {
    std::complex<float> phaseThicknessValue = phaseThickness(
        mediaRefractionIndexes[mediumIndex],
        wavelength,
        cosTheta[mediumIndex],
        interfaceZCoords[mediumIndex]
    );
    return Matrix2x2(
        std::exp(-I * phaseThicknessValue), 0,
        0, std::exp(I * phaseThicknessValue)
    );
}

#include "../include/Field.h"
#include <iostream>
#include <cerrno>
#include <cmath>

Field::Field(InputData inputData)
    : polarization{inputData.polarization}, wavelength{ inputData.wavelength },
    E_T0{ 0, 0 }, startZ{ inputData.startZ } {
    
    numMedia = inputData.numberOfMedia;
    media.resize(numMedia);

    // Calculate medium data for the 0th medium
    MediumData* medium = &media[0];
    medium->zBottom = inputData.mediaZInterfaces[0];
    medium->refractionIndex =  std::sqrt(inputData.mediaMu[0] * inputData.mediaEpsilon[0]);
    medium->waveImpedance = ETA0 * sqrt(inputData.mediaMu[0] / inputData.mediaEpsilon[0]);
    medium->sinTheta = std::sin(inputData.angleOfIncidence);
    medium->cosTheta = std::cos(inputData.angleOfIncidence);
    medium->wavenumber = 2*PI * medium->refractionIndex / inputData.wavelength;

    // Calculate medium data for all other media
    for (std::size_t i = 1; i < numMedia; i++) {
        media[i].zBottom = inputData.mediaZInterfaces[i];
        media[i].refractionIndex =  std::sqrt(inputData.mediaMu[i] / inputData.mediaEpsilon[i]);
        media[i].waveImpedance = media[i].refractionIndex * ETA0;
        media[i].sinTheta = media[0].sinTheta * media[0].refractionIndex / media[i].refractionIndex;
        media[i].cosTheta = std::sqrt(std::complex<float>(1, 0) - std::pow(media[i].sinTheta, 2));
        media[i].wavenumber = 2*PI * media[i].refractionIndex / inputData.wavelength;
    }

    // Calculate transmission matrix for the 0th medium
    std::complex<float> refractionIndexCurr;
    std::complex<float> refractionIndexNext;

    // Calculate transverse refraction index of the current and next medium
    if (polarization == TM) {
        refractionIndexCurr = media[0].refractionIndex / media[0].cosTheta;
        refractionIndexNext = media[1].refractionIndex / media[1].cosTheta;
    }
    else {
        refractionIndexCurr = media[0].refractionIndex * media[0].cosTheta;
        refractionIndexNext = media[1].refractionIndex * media[1].cosTheta;
    }

    // Calculate reflection and transmission fresnel coefficients
    std::complex<float> reflectionCoefficient =
        (refractionIndexNext - refractionIndexCurr) /
        (refractionIndexNext + refractionIndexCurr);

    std::complex<float> transmissionCoefficient = std::complex(1.f, 0.f) - reflectionCoefficient;

    // Calculate matching and propagation matrices
    Matrix2x2 matchingMatrix = Matrix2x2(
        std::complex(1.f, 0.f) / transmissionCoefficient, reflectionCoefficient / transmissionCoefficient,
        reflectionCoefficient / transmissionCoefficient, std::complex(1.f, 0.f) / transmissionCoefficient
    );

    std::complex<float> phaseThickness = 2*PI * media[0].wavenumber * media[0].cosTheta * (media[0].zBottom - startZ);
    Matrix2x2 propagationMatrix = Matrix2x2(
        std::exp(-I * phaseThickness), 0,
        0, std::exp(I * phaseThickness)
    );

    media[0].transmissionMatrix = propagationMatrix.preMultiply(matchingMatrix);

    // Calculate the transmission matrices for all other media
    for (std::size_t i = 1; i < numMedia-1; i++) {
        // Calculate transverse refraction index of the current and next medium
        refractionIndexCurr = refractionIndexNext;
        if (polarization == TM) {
            refractionIndexNext = media[i].refractionIndex / media[i].cosTheta;
        }
        else {
            refractionIndexNext = media[i].refractionIndex * media[i].cosTheta;
        }

        // Calculate reflection and transmission fresnel coefficients
        reflectionCoefficient =
            (refractionIndexNext - refractionIndexCurr) /
            (refractionIndexNext + refractionIndexCurr);

        transmissionCoefficient = std::complex(1.f, 0.f) - reflectionCoefficient;

        // Calculate matching and propagation matrices
        matchingMatrix = Matrix2x2(
            std::complex(1.f, 0.f) / transmissionCoefficient, reflectionCoefficient / transmissionCoefficient,
            reflectionCoefficient / transmissionCoefficient, std::complex(1.f, 0.f) / transmissionCoefficient
        );

        std::complex phaseThickness = media[i].wavenumber * media[i].cosTheta * (media[i].zBottom - media[i-1].zBottom);
        propagationMatrix = Matrix2x2(
            std::exp(-I * phaseThickness), 0,
            0, std::exp(I * phaseThickness)
        );

        // Combine matching and propagation matrices to the transmission matrix of the previous medium
        media[i].transmissionMatrix = media[i-1].transmissionMatrix
            .preMultiply(propagationMatrix)
            .preMultiply(matchingMatrix);
    }

    // Calculate the forward and backward components of E at z=0
    // The forward component is given
    // The backward component is calculated from the boundary condition: E_-(z) = 0 when z > z_N
    if (polarization == TM) {
        E_T0.e1 = inputData.Einc * media[0].cosTheta;
    }
    else {
        E_T0.e1 = inputData.Einc;
    }
    std::complex<float> temp = - media[numMedia-2].transmissionMatrix.e21 /
        media[numMedia-2].transmissionMatrix.e22;
    E_T0.e2 = temp * E_T0.e1;
}

void Field::calculateEHFields(float xp, float zp) {
    // Find which medium contains the point where the fields should be calculated.
    std::size_t mediumIndex = findMedium(zp);

    // Calculate forward and backward transverse components of E at zp
    Vector2 E_Tzp = E_T0;
    if (mediumIndex > 0)
        E_Tzp = E_Tzp
            .preMultiply(media[mediumIndex-1].transmissionMatrix)
            .preMultiply(propagationMatrix(mediumIndex, zp));
    else
        E_Tzp = E_Tzp
            .preMultiply(propagationMatrix(mediumIndex, zp));
        
    // Calculate the full forward and backward components of E at zp
    MediumData* medium = &media[mediumIndex];
    std::complex<float> E_zp_forward[3];
    std::complex<float> E_zp_backward[3];
    if (polarization == TM) {
        E_zp_forward[0] = E_Tzp.e1;
        E_zp_forward[1] = 0;
        E_zp_forward[2] = -E_Tzp.e1 * (medium->sinTheta / medium->cosTheta);

        E_zp_backward[0] = E_Tzp.e2;
        E_zp_backward[1] = 0;
        E_zp_backward[2] = E_Tzp.e2 * (medium->sinTheta / medium->cosTheta);
    }
    else {
        E_zp_forward[0] = 0;
        E_zp_forward[1] = E_Tzp.e1;
        E_zp_forward[2] = 0;

        E_zp_backward[0] = 0;
        E_zp_backward[1] = E_Tzp.e2;
        E_zp_backward[2] = 0;
    }

    // Calculate total E and H fields at zp
    E[0] = E_zp_forward[0] + E_zp_backward[0];
    E[1] = E_zp_forward[1] + E_zp_backward[1];
    E[2] = E_zp_forward[2] + E_zp_backward[2];

    if (polarization == TM) {
        H[0] = 0;
        H[1] =
            ((E_zp_forward[0] * medium->cosTheta - E_zp_forward[2] * medium->sinTheta) -
            (E_zp_backward[0] * medium->cosTheta + E_zp_backward[2] * medium->sinTheta))
            / medium->waveImpedance;
        H[2] = 0;
    }
    else {
        H[0] = ((E_zp_backward[1] - E_zp_forward[1]) * medium->cosTheta) / medium->waveImpedance;
        H[1] = 0;
        H[2] = ((E_zp_backward[1] - E_zp_forward[1]) * medium->sinTheta) / medium->waveImpedance;
    }

    // Add horizontal phase to get the value at xp
    for (std::size_t i = 0; i < 3; i++) {
        E[i] *= exp(-I * medium->wavenumber * medium->sinTheta * xp);
        H[i] *= exp(-I * medium->wavenumber * medium->sinTheta * xp);
    }    
}

std::size_t Field::findMedium(float zp) {
    if (zp < media[0].zBottom) return 0;
    
    for (std::size_t i = 0; i < numMedia-1; i++) {
        if (zp >= media[i].zBottom) return i;
    }
    return numMedia-1;
}

Matrix2x2 Field::propagationMatrix(std::size_t mediumIndex, float zp) {
    std::complex<float> phaseThickness = media[mediumIndex].wavenumber * media[mediumIndex].cosTheta;
    if (mediumIndex == 0)
        phaseThickness *= zp - startZ;
    else
        phaseThickness *= zp - media[mediumIndex-1].zBottom;

    return Matrix2x2(
        std::exp(-I * phaseThickness), 0,
        0, std::exp(I * phaseThickness)
    );
}

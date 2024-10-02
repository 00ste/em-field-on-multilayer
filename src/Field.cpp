#include "../include/Field.h"
#include <iostream>
#include <cerrno>
#include <cmath>

Field::Field(InputData inputData)
    : polarization{inputData.polarization}, wavelength{inputData.wavelength}, E_T0{0, 0, 0, 0} {

    numMedia = inputData.numberOfMedia;
    media.resize(numMedia);

    // Calculate medium data for the 0th medium
    MediumData* medium = &media[0];
    medium->zBottom = inputData.mediaZInterfaces[0];
    medium->refractionIndex =  std::sqrt(inputData.mediaMu[0] / inputData.mediaEpsilon[0]);
    medium->waveImpedance = medium->refractionIndex * ETA0;
    medium->sinTheta = std::sin(inputData.angleOfIncidence);
    medium->cosTheta = std::cos(inputData.angleOfIncidence);
    medium->wavenumber = 2*PI * medium->refractionIndex / inputData.wavelength;

    // Calculate medium data for all other media
    for (std::size_t i = 1; i < numMedia; i++) {
        media[i].zBottom = inputData.mediaZInterfaces[i];
        media[i].refractionIndex =  std::sqrt(inputData.mediaMu[i] / inputData.mediaEpsilon[i]);
        media[i].waveImpedance = media[i].refractionIndex * ETA0;
        media[i].sinTheta = media[0].sinTheta * media[0].refractionIndex / media[i].refractionIndex;
        media[i].cosTheta = std::sqrt(std::complex<float>(1, 0) - std::pow(media[0].sinTheta, 2));
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

    std::complex<float> phaseThickness = 2*PI * media[0].wavenumber * media[0].cosTheta * media[0].zBottom;
    Matrix2x2 propagationMatrix = Matrix2x2(
        std::exp(-std::complex(0.f, 1.f) * phaseThickness), 0,
        0, std::exp(std::complex(0.f, 1.f) * phaseThickness)
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
            std::exp(-std::complex(0.f, 1.f) * phaseThickness), 0,
            0, std::exp(std::complex(0.f, 1.f) * phaseThickness)
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
        E_T0.e11 = inputData.Einc * media[0].cosTheta;
    }
    else {
        E_T0.e11 = inputData.Einc;
    }
    std::complex<float> temp = - media[numMedia-1].transmissionMatrix.e21 /
        media[numMedia-1].transmissionMatrix.e22;
    E_T0.e21 = temp * E_T0.e11;
}

void Field::calculateEHFields(
    float incidenceAngle,
    Polarization polarization,
    float wavelength,
    std::complex<float> E1inc,
    float xp,
    float zp
) {
    /*
    // Find which medium contains the point where the fields should be calculated.
    size_t mediumIndex = findMedium(zp);

    // Combine the current transmission matrix with the propagation matrix in the current medium.
    media[mediumIndex].transmissionMatrix.preMultiply(propagationMatrix(zp - media[mediumIndex].zBottom, mediumIndex));

    // TODO: Finish this
    Matrix2x2 progressiveRegressiveElectricField = Matrix2x2(E1inc, 0, 0, 0);
    progressiveRegressiveElectricField.preMultiply(transmissionMatrix);

    std::cout << "E+: " << progressiveRegressiveElectricField.e11 << std::endl;
    std::cout << "E-: " << progressiveRegressiveElectricField.e21 << std::endl;
    */
}

std::size_t Field::findMedium(float zp) {
    if (zp <= media[0].zBottom) return 0;
    if (zp > media[numMedia - 1].zBottom) return numMedia;

    for (std::size_t i = 0; i < numMedia; i++) {
        if (zp > media[i].zBottom && zp <= media[i+1].zBottom);
            return i+1;
    }

    // unreachable
    return numMedia;
}

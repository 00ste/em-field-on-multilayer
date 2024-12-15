#include "../../include/Multilayer.h"
#include "../../include/Data.h"
#include "../../include/FileInterface.h"

#include <iostream>

int main(int argc, char* argv[]) {
    InputData inputData;
    if (!FileInterface::readData(inputData, "data.txt")) {
        std::cerr << "Unable to read data." << std::endl;
        return EXIT_FAILURE;
    }

    Multilayer field = Multilayer(inputData);

    // Print calculated data
    std::cout << "- E_T0+: " << field.E_T0.e11 << std::endl;
    std::cout << "- E_T0-: " << field.E_T0.e21 << std::endl;

    for (std::size_t i = 0; i < field.numMedia; i++) {
        std::cout << "Data for media " << i << std::endl;
        std::cout << "- Refraction index:    " << field.media[i].refractionIndex << std::endl;
        std::cout << "- sin(theta):          " << field.media[i].sinTheta << std::endl;
        std::cout << "- cos(theta):          " << field.media[i].cosTheta << std::endl;
        std::cout << "- Wave impedance:      " << field.media[i].waveImpedance << std::endl;
        std::cout << "- Transmission matrix: ";
        std::cout << field.media[i].transmissionMatrix.e11 << ", ";
        std::cout << field.media[i].transmissionMatrix.e12 << "; ";
        std::cout << field.media[i].transmissionMatrix.e21 << ", ";
        std::cout << field.media[i].transmissionMatrix.e22 << std::endl;
    }

    return EXIT_SUCCESS;
}

#include <iostream>
#include "../../include/Data.h"
#include "../../include/FileInterface.h"

int main(int argc, char* argv[]) {
    InputData data;

    if (!FileInterface::readData(data, "data.txt")) {
        std::cerr << "An error occurred!" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Found " << data.numberOfMedia << " media with parameters:" << std::endl;
    for (std::size_t i = 0; i < data.numberOfMedia; i++) {
        std::cout << "z_" << i << " = " << data.mediaZInterfaces[i] << ", ";
        std::cout << "e_" << i << " = " << data.mediaEpsilon[i] << ", ";
        std::cout << "m_" << i << " = " << data.mediaMu[i] << ", ";
        std::cout << std::endl;
    }

    std::string polarizationString = data.polarization == TM ? "TM" : "TE";
    std::cout << "Incident field magnitude is " << data.Einc << std::endl;
    std::cout << "Angle of incidence is " << data.angleOfIncidence << std::endl;
    std::cout << "Incident field Wavelength is " << data.wavelength << std::endl;
    std::cout << "Polarization is " << polarizationString << std::endl;

    return EXIT_SUCCESS;
}

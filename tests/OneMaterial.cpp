#include <complex>
#include <iostream>
#include "../Multilayer.h"

int main(int argc, char* argv[]) {
    Multilayer field = Multilayer({ 10 }, { 1 });

    field.calculateEHFields(0, Multilayer::Polarization::TE, 800, 1, 0, 5);

    std::cout << "Electric field is: " << field.E[0] << ", " << field.E[1] << ", " << field.E[2] << std::endl;
    std::cout << "Magnetic field is: " << field.H[0] << ", " << field.H[1] << ", " << field.H[2] << std::endl;

    return EXIT_SUCCESS;
}

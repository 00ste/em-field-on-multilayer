#include <iostream>

#include "../Data.h"
#include "../FileInterface.h"

int main(int argc, char* argv[]) {
    std::vector<OutputDataLine> data = std::vector<OutputDataLine>();
    data.reserve(3);

    data.push_back({
        10.0, 1.0,
        std::complex<float>(1.1, 2.4), std::complex<float>(3.6, 4.8), std::complex<float>(5.10, 6.12),
        std::complex<float>(11.14, 12.16), std::complex<float>(13.18, 14.20), std::complex<float>(15.22, 16.24)
    });
    data.push_back({
        1.2, 2.0,
        std::complex<float>(0.32876, 2.576), std::complex<float>(0.3948, 4.462), std::complex<float>(0.363242, 6.45342),
        std::complex<float>(1.45322, 12.3523), std::complex<float>(1.14124, 14.5555), std::complex<float>(1.5636, 16.3534)
    });
    data.push_back({
        1.42929, 2.23142,
        std::complex<float>(0.5553421, 0.4242222), std::complex<float>(3.3534, 0.34344444241), std::complex<float>(5.6644443, 0.9004824999),
        std::complex<float>(0.3452233, 12.452525141), std::complex<float>(0.3413, 14.334242513), std::complex<float>(100000.123124, 16.45302849282)
    });

    if (!FileInterface::writeData(data, "FileOutput-1.txt")) {
        std::cerr << "An error occurred!" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

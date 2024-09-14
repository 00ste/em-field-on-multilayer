#ifndef FILE_INTERFACE_H
#define FILE_INTERFACE_H

#include <complex>
#include <iomanip>
#include <ios>
#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <utility>

#include "Data.h"

class FileInterface {
public:
    static bool writeData(const std::vector<OutputDataLine>& data, std::string outputFilePath) {
        std::ofstream outputFile;
        outputFile.open(outputFilePath);

        if (!outputFile.is_open()) {
            std::cerr << "Could not open the file" << std::endl;
            return false;
        }

        try {
            for (auto line : data) {
                outputFile << std::setprecision(5) << std::setw(8) << line.xp;
                outputFile << std::setprecision(5) << std::setw(8) << line.zp;
                outputFile << std::setprecision(5) << std::setw(8) << line.Ex.real();
                outputFile << std::setprecision(5) << std::setw(8) << line.Ex.imag();
                outputFile << std::setprecision(5) << std::setw(8) << line.Ey.real();
                outputFile << std::setprecision(5) << std::setw(8) << line.Ey.imag();
                outputFile << std::setprecision(5) << std::setw(8) << line.Ez.real();
                outputFile << std::setprecision(5) << std::setw(8) << line.Ez.imag();
                outputFile << std::endl;
            }

            outputFile.close();
            return true;
        }
        catch (std::exception e) {
            outputFile.close();
            std::cerr << "An error occurred while writing to output file" << std::endl;
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    static bool readData(InputData& inputData, std::string inputFilePath) {
        std::ifstream inputFile;
        inputFile.open(inputFilePath, std::ios::in);

        if (!inputFile.is_open()) {
            std::cerr << "Could not open the file" << std::endl;
            return false;
        }

        try {
            std::string line;

            // Read number of media
            std::getline(inputFile, line);
            std::cout << "Found line [" << line << "]" << std::endl;
            inputData.numberOfMedia = std::stof(line);
            inputData.mediaZInterfaces.reserve(inputData.numberOfMedia);
            inputData.mediaEpsilon.reserve(inputData.numberOfMedia);
            inputData.mediaMu.reserve(inputData.numberOfMedia);

            // Read interface z, epsilon and mu of each medium
            for (size_t i = 0; i < inputData.numberOfMedia; i++) {
                std::getline(inputFile, line);
                std::string token;
                std::vector<float> values = std::vector<float>();
                values.reserve(5);
                size_t pos = 0;

                while ((pos = line.find(",")) != std::string::npos) {
                    token = line.substr(0, pos);
                    values.push_back(std::stof(token));
                    line.erase(0, pos + 1);
                }
                values.push_back(std::stof(line));

                // Assign values to the respective fields
                inputData.mediaZInterfaces.push_back(values[0]);
                inputData.mediaEpsilon.push_back(std::complex<float>(values[1], values[2]));
                inputData.mediaMu.push_back(std::complex<float>(values[3], values[4]));
            }

            // Read incident field amplitude
            std::getline(inputFile, line);
            size_t pos = line.find(",");

            // if the amplitude is complex
            if (pos != std::string::npos)
                inputData.Einc = std::complex<float>(
                    std::stof(line.substr(0, pos)),
                    std::stof(line.substr(pos+1, std::string::npos))
                );
            // if the amplitude is real
            else
                inputData.Einc = std::stof(line);

            // Read angle of incidence
            std::getline(inputFile, line);
            inputData.angleOfIncidence = std::stof(line);

            // Read incident field wavelength
            std::getline(inputFile, line);
            inputData.wavelength = std::stof(line);

            return true;
        }
        catch (std::exception e) {
            inputFile.close();
            std::cerr << "An error occurred while parsing data file" << std::endl;
            std::cerr << e.what() << std::endl;
            return false;
        }
    }
};

#endif /* FILE_INTERFACE_H */

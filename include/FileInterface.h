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

        std::size_t precision = 5;
        std::size_t width = 12;

        try {
            for (auto line : data) {
                outputFile << std::setprecision(precision) << std::setw(width) << line.xp << ";";
                outputFile << std::setprecision(precision) << std::setw(width) << line.zp << ";";
                outputFile << std::setprecision(precision) << std::setw(width) << line.Ex.real() << ";";
                outputFile << std::setprecision(precision) << std::setw(width) << line.Ex.imag() << ";";
                outputFile << std::setprecision(precision) << std::setw(width) << line.Ey.real() << ";";
                outputFile << std::setprecision(precision) << std::setw(width) << line.Ey.imag() << ";";
                outputFile << std::setprecision(precision) << std::setw(width) << line.Ez.real() << ";";
                outputFile << std::setprecision(precision) << std::setw(width) << line.Ez.imag() << ";";
                outputFile << std::setprecision(precision) << std::setw(width) << line.Hx.real() << ";";
                outputFile << std::setprecision(precision) << std::setw(width) << line.Hx.imag() << ";";
                outputFile << std::setprecision(precision) << std::setw(width) << line.Hy.real() << ";";
                outputFile << std::setprecision(precision) << std::setw(width) << line.Hy.imag() << ";";
                outputFile << std::setprecision(precision) << std::setw(width) << line.Hz.real() << ";";
                outputFile << std::setprecision(precision) << std::setw(width) << line.Hz.imag() << ";";
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

            // Read z coordinate of each interface
            inputData.numberOfMedia = 0;
            std::getline(inputFile, line);
            std::string token;
            std::size_t pos = 0;
            while ((pos = line.find(",")) != std::string::npos) {
                token = line.substr(0, pos);
                inputData.mediaZInterfaces.push_back(std::stof(token));
                line.erase(0, pos + 1);
                inputData.numberOfMedia += 1;
            }
            inputData.mediaZInterfaces.push_back(std::stof(line));
            
            // If there are n interfaces, there are n+1 media
            inputData.numberOfMedia += 2;

            inputData.mediaEpsilon.reserve(inputData.numberOfMedia);
            inputData.mediaMu.reserve(inputData.numberOfMedia);

            // Read epsilon and mu of each medium
            for (std::size_t i = 0; i < inputData.numberOfMedia; i++) {
                std::getline(inputFile, line);
                std::vector<float> values;
                values.reserve(4);
                pos = 0;

                while ((pos = line.find(",")) != std::string::npos) {
                    token = line.substr(0, pos);
                    values.push_back(std::stof(token));
                    line.erase(0, pos + 1);
                }
                values.push_back(std::stof(line));

                // Assign values to the respective fields
                inputData.mediaEpsilon.push_back(std::complex<float>(values[0], values[1]));
                inputData.mediaMu.push_back(std::complex<float>(values[2], values[3]));
            }

            // Read incident field amplitude
            std::getline(inputFile, line);
            pos = line.find(",");

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

            // Read wave polarization
            std::getline(inputFile, line);
            inputData.polarization = line == "TM" ? Polarization::TM : Polarization::TE;
            if (line != "TM" && line != "TE")
                std::cout << "Warning! " << line << " is not a valid polarization, TE is selected by default.";

            return true;
        }
        catch (std::exception e) {
            inputFile.close();
            std::cerr << "An error occurred while parsing data file" << std::endl;
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    static bool readPoints(std::vector<Point>& data, std::string inputFilePath) {
        std::ifstream inputFile;
        inputFile.open(inputFilePath, std::ios::in);

        if (!inputFile.is_open()) {
            std::cerr << "Could not open the file" << std::endl;
            return false;
        }

        std::size_t lineNumber = 0;
        try {
            std::string line;
            std::size_t pos;

            while (std::getline(inputFile, line)) {
                lineNumber++;
                pos = line.find(",");

                if (pos == std::string::npos) {
                    std::cerr << "No comma found at line " << lineNumber << std::endl;
                    return false;
                }

                data.push_back(Point{
                    std::stof(line.substr(0, pos)),
                    std::stof(line.substr(pos+1, std::string::npos))
                });
            }
        }
        catch (std::exception e) {
            inputFile.close();
            std::cerr << "An error occurred while reading input points file at line " << lineNumber << std::endl;
            std::cerr << e.what() << std::endl;
            return false;
        }

        return true;
    }
};

#endif /* FILE_INTERFACE_H */

#include <iostream>
#include <string>
#include <cstring>

#include "../include/Data.h"
#include "../include/FileInterface.h"
#include "../include/Field.h"

void printHelp(char* exeName) {
    std::cout << "Usage: " << exeName << " input-data-file input-points-file output-file" << std::endl;
}

int main(int argc, char* argv[]) {
    InputData data;
    std::vector<Point> points;
    std::vector<OutputDataLine> output;

    if (argc < 4) {
        std::cout << "Not enough parameters provided!" << std::endl;
        printHelp(argv[0]);
        return EXIT_FAILURE;
    }

    // Get data from command line args
    FileInterface::readData(data, argv[1]);
    FileInterface::readPoints(points, argv[2]);
    bool polar = argc >= 5 && std::strcmp(argv[4], "-p") == 0;

    output.reserve(points.size());

    // Init Field class
    Field f = Field{ data };

    for (Point p : points) {
        // Calculate fields for all points
        f.calculateEHFields(p.x, p.z);

        // Save output
        OutputDataLine outputLine;
        outputLine.Ex = f.E[0];
        outputLine.Ey = f.E[1];
        outputLine.Ez = f.E[2];
        outputLine.Hx = f.H[0];
        outputLine.Hy = f.H[1];
        outputLine.Hz = f.H[2];
        outputLine.xp = p.x;
        outputLine.zp = p.z;

        output.push_back(outputLine);
    }

    // Write results into the output file
    FileInterface::writeData(output, argv[3], polar);

    return EXIT_SUCCESS;
}
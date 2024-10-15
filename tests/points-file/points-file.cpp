#include <iostream>
#include "../../include/Data.h"
#include "../../include/FileInterface.h"

int main(int argc, char* argv[]) {
    std::vector<Point> data;

    if (!FileInterface::readPoints(data, "points.txt")) {
        std::cerr << "An error occurred!" << std::endl;
        return EXIT_FAILURE;
    }

    for (Point point : data) {
        std::cout << "x=" << point.x << ", z=" << point.z << std::endl;
    }

    return EXIT_SUCCESS;
}

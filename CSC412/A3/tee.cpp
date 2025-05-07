#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[]) {
    std::vector<std::ofstream> files;

    for (int i = 1; i < argc; i++) {
        files.emplace_back(argv[i]);
        if (!files.back()) {
            std::cerr << "Error: Could not open file " << argv[i] << "." << std::endl;
            return 1;
        }
    }

    std::string line;
    while (std::getline(std::cin, line)) {
        std::cout << line << std::endl;
        for (std::ofstream &file : files) {
            file << line << std::endl;
        }
    }
    return 0;
}
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    std::ofstream file;
    if (argc == 2) {
        file.open(argv[1]);
    }
    std::string line;
    while (std::getline(std::cin, line)) {
        std::cout << line << std::endl;
        if (argc == 2) {
            file << line << std::endl;
        }
    }
    file.close();
    return 0;
}
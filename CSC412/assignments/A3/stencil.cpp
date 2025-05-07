#include <iostream>

int main(int argc, char* argv[]) {
    std::string line; // will this be enough memory to store data?
    while (std::getline(std::cin, line)) {
        std::cout << line << std::endl;
    }
    return 0;
}
#include <iostream>
#include <filesystem>

int main(int argc, char *argv[]) {
    try {
        std::cout << std::filesystem::current_path().string() << std::endl;
    } catch (const std::filesystem::filesystem_error&) {
        std::cerr <<  (argc > 0 ? argv[0] : "pwd") << ": error: No such file or directory" << std::endl;
        return 1;
    }
    
    return 0;
}
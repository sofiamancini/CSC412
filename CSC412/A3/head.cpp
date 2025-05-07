#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    std::vector<std::ifstream> files;
    std::vector<std::string> file_names; // Store file names

    for (int i = 1; i < argc; i++) {
        std::ifstream file(argv[i]);
        if (!file) {
            std::cerr << "head: cannot open " << "'" << argv[i] << "'" << " for reading: No such file or directory" << std::endl;
            continue;
        }
        files.push_back(std::move(file));
        file_names.push_back(argv[i]);
    }

    if (argc == 1) {
        std::string line;
        int line_count = 0;
        while (std::getline(std::cin, line) && line_count < 10) {
            std::cout << line << std::endl;
            line_count++;
        }
        return 0;
    }

    std::string line;
    for (size_t i = 0; i < files.size(); i++) {
        if (files.size() > 1) { // Only print filename if multiple files exist
            std::cout << "==> " << file_names[i] << " <==" << std::endl;
        }

        std::vector<std::string> first_10_lines;

        int line_count = 0;
        while (std::getline(files[i], line) && line_count < 10) {
            first_10_lines.push_back(line);
            line_count++;
        }
        
        if (first_10_lines.empty()) {
            std::cerr << "head: " << file_names[i] << " is empty or could not be read." << std::endl;
            continue;
        }
        for (const std::string &line : first_10_lines) {
            std::cout << line << std::endl;
        }

        if ( i < files.size() - 1) {
            std::cout << std::endl;
        }
    }
    return 0;
}
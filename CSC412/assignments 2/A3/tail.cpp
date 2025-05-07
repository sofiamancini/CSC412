#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>

void print_last_10_lines(std::istream &input, const std::string &filename, bool print_filename) {
    std::string line;
    std::deque<std::string> last_10_lines;

    while (std::getline(input, line)) {
        if (last_10_lines.size() == 10) {
            last_10_lines.pop_front();
        }
        last_10_lines.push_back(line);
    }

    if (print_filename) {
        std::cout << "==> " << filename << " <==" << std::endl;
    }

    for (const std::string &line : last_10_lines) {
        std::cout << line << std::endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        print_last_10_lines(std::cin, "", false);
        return 0;
    }

    bool multiple_files = (argc > 2);

    for (int i = 1; i < argc; i++) {
        std::ifstream file(argv[i]);
        if (!file) {
            std::cerr << "tail: cannot open " << "'" << argv[i] << "'" << " for reading: No such file or directory" << std::endl;
            continue;
        }
        print_last_10_lines(file, argv[i], multiple_files);
        if (i < argc - 1) {
            std::cout << std::endl;
        }
    }
    return 0;
}
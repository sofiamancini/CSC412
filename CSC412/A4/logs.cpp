
// read in all log files from logFolder, combine them into one file, and sort them by timestamp

#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>

namespace fs = std::filesystem;

// Combine all log files into one file
void combineFiles (std::vector<std::string> files) {
    fs::path path = fs::path("logFolder") / "actions.txt";
    std::ofstream outFile(path);
    if (!outFile) {
        std::cerr << "Error opening file: actions.txt" << std::endl;
        return;
    }

    for (const auto& file : files) {
        std::ifstream inFile(file);
        if (!inFile) {
            std::cerr << "Error opening file: " << file << std::endl;
            continue;
        }

        outFile << inFile.rdbuf();
        inFile.close();
    }

    outFile.flush();
    outFile.close();

    chmod(path.c_str(), 0755);
}

// Sort the combined file by timestamp
void sortFile() {
    system("sort -t, -k1 -o logFolder/actions.txt logFolder/actions.txt");
}

int main() {
    std::vector<std::string> files;
    
    for (const auto& entry : fs::directory_iterator("logFolder")) {
        if (entry.path().filename().string().find("inkling") != std::string::npos) {
            files.push_back(entry.path().string());
        }
    }
    if (files.empty()) {
        std::cerr << "No log files found in logFolder" << std::endl;
        return 1;
    }

    combineFiles(files);
    sortFile();

    return 0;
}
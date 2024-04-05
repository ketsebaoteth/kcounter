#include <iostream>
#include <stdio.h>
#include <fstream>
#include <filesystem>
#include <string>
#include <thread>
#include <algorithm>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>


std::vector<std::string> possible_code_extensions = {"cpp", "c", "py", "go", "rs", "asm", "js", "html", "css", "scss", "cs", "jsx"};
std::vector<std::string> blockedfolders = {
    "node_modules", // Node.js modules
    ".git", // Git source control
    "dist", // Common distribution folder for compiled files
    "build", // Common build folder
    "bin", // Binary files
    "obj", // Object files from build
    "vendor", // Dependencies in PHP projects
    ".vscode", // VS Code settings
    ".idea", // IntelliJ IDEA settings
    "__pycache__", // Python cache files
    ".pytest_cache", // Pytest cache files
    "venv", // Python virtual environment
    "packages", // Dart and Flutter packages
    "Pods", // CocoaPods in iOS development
    "Carthage", // Carthage in iOS development
    "cargo", // Rust package manager
    ".stack-work" // Haskell's stack work folder
};

bool validatepath(const std::string &filepath)
{
    std::filesystem::path path(filepath);
    std::string extension = path.extension().string();
    if (!extension.empty() && extension[0] == '.')
    {
        extension = extension.substr(1);
    }
    for (const std::string &ext : possible_code_extensions)
    {
        if (ext == extension)
        {
            return true;
        }
    }
    return false;
}

bool isFolderInBlocklist(const std::string &foldername, const std::vector<std::string> &blocklist){
    return std::find(blocklist.begin(), blocklist.end(), foldername) != blocklist.end();
}

int count_in_folder(const std::string& pathtostart, int& total_lines, int& filecount) {
    int linecount = 0;
    std::string dirpath = pathtostart;
    for (const auto& entry : std::filesystem::directory_iterator(dirpath)) {
        if (entry.is_regular_file()) {
            if (validatepath(entry.path().string())) {
                std::ifstream file(entry.path().string());
                std::string lines;
                filecount++;
                while (std::getline(file, lines)) {
                    linecount++;
                }
            } else {
                std::cout << "\033[1;33mskipping:\033[0m File " << entry.path().string() << " is not a valid code file!" << '\n';
            }
        } else if (entry.is_directory()) {
            std::filesystem::path folder(entry.path().string());
            if (isFolderInBlocklist(folder.filename().string(),blockedfolders)) {
                std::cout << "\033[1;33mSkipping node_modules folder:\033[0m " << entry.path().string() << '\n';
            } else {
                std::thread subfolder_thread(count_in_folder, entry.path().string(), std::ref(total_lines), std::ref(filecount));
                subfolder_thread.join(); // Join the thread
            }
        }
    }
    total_lines += linecount;
    return linecount;
}

int count_in_file(std::string pathtofile)
{
    if (validatepath(pathtofile))
    {
        int linecount;
        std::ifstream file(pathtofile);
        std::string lines;
        while (std::getline(file, lines))
        {
            linecount++;
        }
        return linecount;
    }
    else
    {
        std::cout << "file " << pathtofile << "        is not a valid code file!!" << '\n';
        return 0;
    }
};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "\033[1;31mError:\033[0m Path to folder not specified. Please specify a destination folder." << std::endl;
        return 1;
    }

    std::cout << "Starting code line count..." << std::endl;
    if (!std::filesystem::exists(argv[1]))
    {
        std::cerr << "\033[1;31mError:\033[0m The specified path " << argv[1] << " doesn't exist!" << std::endl;
        return 1;
    }

    int total_lines = 0;
    int totalfiles = 0;
    auto start_time = std::chrono::high_resolution_clock::now();

    count_in_folder(argv[1], total_lines, totalfiles);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << std::endl;
    std::cout << "\033[1;32mSuccess:\033[0m scanned " << totalfiles << " files total." << std::endl;
    std::cout << "\033[1;32mSuccess:\033[0m Read " << total_lines << " lines of code in the entire directory tree." << std::endl;
    std::cout << "\033[1;32mTime taken: \033[0m" << duration.count() << " ms" << std::endl;

    return 0;
}
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <cstdlib>


// Checks to see if a given IPv4 address is valid or not
bool isValidIPv4(const std::string& ip) {
    // regex object asks if there will be a octet that is (250-255, 200-249, 100-199, 0-99)
    std::regex ipv4Regex(R"(^(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9]?|0)\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9]?|0)\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9]?|0)\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9]?|0)$)");
    


    return std::regex_match(ip, ipv4Regex);
}



// Reads file and runs isValidIPv4 through each line
void processFile(const std::string& fileName){
    std::ifstream infile(fileName);
    std::string ip;

    if(!infile){
        std::cerr << "Error: no file found " << fileName << std::endl;
        return;
    }

    std::string line;
    while(std::getline(infile, line)){
          
        if(isValidIPv4(line)){
            std::cout << line << "+" << std::endl; 
        }
        else{
            std::cout << line <<"-" <<std::endl;
        }
    }

    infile.close();
    
}

// Processes the file in list mode
void processFileListMode(const std::string& fileName){
    std::ifstream infile(fileName);
    std::string line;

    if (!infile) {
        std::cerr << "Error: no file found " << fileName << std::endl;
        return;
    }

    while (std::getline(infile, line)) {
        if (isValidIPv4(line)) {
            std::cout << line << " +" << std::endl;
        } else {
            std::cout << line << " -" << std::endl;
        }
    }

    infile.close();
}

// Processes the file in summary mode
void processFileSummaryMode(const std::string& fileName) {
    std::ifstream infile(fileName);
    std::string line;
    int totalLines = 0, validCount = 0, invalidCount = 0;

    if (!infile) {
        std::cerr << "Error: no file found :" << fileName << std::endl;
        return;
    }

    while (std::getline(infile, line)) {
        ++totalLines;
        if (isValidIPv4(line)) {
            ++validCount;
        } else {
            ++invalidCount;
        }
    }

    infile.close();

    // Print summary
    std::cout << "LINES: " << totalLines << std::endl;
    std::cout << "VALID: " << validCount << std::endl;
    std::cout << "INVALID: " << invalidCount << std::endl;
}

// Main function to handle arguments 
int main(int argc, char* argv[]) {
    bool listMode = false;
    bool summaryMode = false;
    std::string fileName;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-l") {
            if (summaryMode) {
                std::cerr << "Error: You cannot use -l and -s together" << std::endl;
                return 1;
            }
            listMode = true;
        } else if (arg == "-s") {
            if (listMode) {
                std::cerr << "Error: You cannot use -l and -s together" << std::endl;
                return 1;
            }
            summaryMode = true;
        } else if (arg == "-f") {
            if (i + 1 < argc) {
                fileName = argv[++i];  // Get the next argument 
            } else {
                std::cerr << "Error: -f must be followed by a filename" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Error: unknown argument '" << arg << "'" << std::endl;
            return 1;
        }
    }

    // Check that the file is provided
    if (fileName.empty()) {
        std::cerr << "Error: missing filename. Use -f <filename>" << std::endl;
        return 1;
    }

    // Execute the right process mode
    if (listMode) {
        processFileListMode(fileName);
    } else if (summaryMode) {
        processFileSummaryMode(fileName);
    } else {
        std::cerr << "Error: Process mode not specified. Use either -l (list) or -s (summary)." << std::endl;
        return 1;
    }

    return 0;
}
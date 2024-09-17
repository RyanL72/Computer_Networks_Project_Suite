#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <cstdlib>


// Checks to see if a given IPv4 address is valid or not
bool isValidIPv4(const std::string& ip) {
    int num = 0; 
    int dotCount = 0; 
    int segmentLength = 0; 
    
    for (std::string::size_type i = 0; i < ip.size(); ++i) {
        char ch = ip[i];
        
        if (ch == '.') {
            // Invalid if no digits before dot or number is out of range
            if (segmentLength == 0 || num < 0 || num > 255) {
                return false;
            }
            
            // Check for leading zeroes 
            if (segmentLength > 1 && ip[i - segmentLength] == '0') {
                return false;
            }
            
            dotCount++; 
            num = 0; 
            segmentLength = 0; 
        }
        else if (isdigit(ch)) {
            num = num * 10 + (ch - '0');
            segmentLength++; 
        }
        else {
            return false;
        }
    }
    
    // Final segment validation 
    if (segmentLength == 0 || num < 0 || num > 255) {
        return false;
    }
    if (segmentLength > 1 && ip[ip.size() - segmentLength] == '0') {
        return false;
    }

    // Check that there are exactly 3 dots so that there are 4 segments
    return dotCount == 3;
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
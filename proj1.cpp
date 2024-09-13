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

// Main function to test the isValidIPv4 function
int main() {
    std::string filename = "sample-input.txt";
    processFile(filename);  // Call function to process file
    return 0;
}

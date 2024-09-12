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

// Main function to test the isValidIPv4 function
int main() {
    // Array of test IP addresses
    std::string testIPs[] = {
        "192.168.1.1",    // Valid
        "255.255.255.0",  // Valid
        "127.0.0.1",      // Valid
        "256.256.256.256",// Invalid (out of range)
        "192.168.1",      // Invalid (missing octet)
        "192.168.1.999",  // Invalid (octet out of range)
        "abc.def.ghi.jkl" // Invalid (not a number)
    };

    // Loop through the array and check each IP address
    for (const std::string& ip : testIPs) {
        if (isValidIPv4(ip)) {
            std::cout << ip << "+" << std::endl;
        } else {
            std::cout << ip << "-" << std::endl;
        }
    }

    return 0;
}
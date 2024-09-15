#include <iostream>
#include <fstream>
#include <string>

int linux_fstream()
{
    std::string filename = "output.txt";

    std::ofstream outFile(filename);

    if(!outFile)
    {
        std::cerr << "do not open file" << filename <<
        "writing" << std::endl;
    
    }

    outFile << "Hello, Linux file I/O." << std::endl;
    
    outFile.close();

    outFile << "write success\r\n";
    return 0;
}
#include "colors.h"
#include <fstream>
#include <iostream>
#include <iomanip>

void displayBlockOffset() {
    static int blockNumber{ 0 };
    std::cout.fill('0');

    std::cout << TXT_RED_FG;
    std::cout << std::setw(8) << std::hex <<
        blockNumber * 16 << "  ";
    std::cout << TXT_RESET;
    ++blockNumber;
}

void displayBlockData(const char* buffer, int size) {
    std::cout << TXT_CYAN_FG;
    for (int i{ 0 }; i < size; ++i) {
        unsigned int c = 0x000000ff & static_cast<unsigned int>(buffer[i]);
        std::cout << std::setw(2) << std::hex << c << ' ';
        if (i == 7)
            std::cout << "- ";
    }
    std::cout << TXT_RESET;

    // account for read size less than 16
    if (size < 16) {
        for(int i{ size }; i < 16; ++i)
            std::cout << "   ";
        if (size < 8)
            std::cout << "  ";
    }
}

void displayBlockChars(const char* buffer, int size) {
    std::cout << TXT_YELLOW_FG;
    std::cout << " |";
    for (int i{ 0 }; i < size; ++i) {
        unsigned char c = static_cast<unsigned char>(buffer[i]);
        if (c >= 0 && c <= 127) {
            if (iscntrl(buffer[i]))
                std::cout << '.';
            else
                std::cout << buffer[i];
        } else
            std::cout << '.';
    }
    std::cout << TXT_RESET;
    std::cout << std::endl;
}

void displayBlock(const char* buffer, int size) {
    displayBlockOffset();
    displayBlockData(buffer, size);
    displayBlockChars(buffer, size);
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
		if (argv[0])
			std::cout << "Usage: " << argv[0] << " <filename>" << '\n';
		else
			std::cout << "Usage: ./bytedump <filename>" << '\n';
		std::exit(1);
	}

    char* filename{ argv[1] };
    const int blocksize { 16 };
    std::ifstream inf{ filename, std::ios::binary };
    
    if (!inf) {
        // Print an error and exit
        std::cerr << filename << " could not be opened for reading" << std::endl;
        return 1;
    }

    std::cout << "Dump of: '" << filename << "'\n";

    // get # bytes in file
    size_t size { 0 };
    inf.seekg(0, std::ios::end);
    size = inf.tellg();

    inf.seekg(0, std::ios::beg);
    char* buffer = new char[blocksize];
    while(inf) {
        inf.read(buffer, blocksize);
        displayBlock(buffer, inf.gcount());
    }

    // display # bytes in file
    std::cout << std::dec << ' ' << size << " total bytes\n";
    
    delete [] buffer;
    inf.close();

    return 0;
}
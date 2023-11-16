#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>

using namespace std;
using namespace std::chrono;

struct ArithmeticEncoder {
    ofstream output;
    uint64_t rangeLow = 0;
    uint64_t rangeSize = 0xFFFFFFFF;
    uint64_t pendingBits = 0;
    int pendingBitsCount = 0;

    ArithmeticEncoder(const string& fileName) {
        output.open(fileName, ios::binary);
    }

    ~ArithmeticEncoder() {
        if (pendingBitsCount > 0) {
            output.put(pendingBits);
        }
        output.close();
    }

    void writeBit(bool bit) {
        rangeSize /= 2;
        if (bit) {
            rangeLow += rangeSize;
        }
        if (rangeSize < 0x01000000) {
            rangeSize <<= 8;
            pendingBitsCount += 8;
            pendingBits |= (rangeLow >> 24) & 0xFF;
            rangeLow <<= 8;
        }
        while (pendingBitsCount >= 8) {
            output.put(pendingBits);
            pendingBits >>= 8;
            pendingBitsCount -= 8;
        }
    }
};

void compressArithmeticCoding(const string& text, const string& outputFileName) {
    ArithmeticEncoder encoder(outputFileName);

    // Implement arithmetic coding for text compression
    // You need to define your probability model and encoding logic here.

    for (char c : text) {
        // Example: Encode each character using your arithmetic coding model
        bool bit = (c == '1');  // Replace with your encoding logic
        encoder.writeBit(bit);
    }
}

int main() {
    cout << "Welcome to the Text File Arithmetic Coding Compressor!" << endl;

    string inputFileName;
    cout << "\nEnter input file name: ";
    cin >> inputFileName;
    cout << endl;

    ifstream inputFile(inputFileName, ios::binary);
    if (!inputFile) {
        cout << "Error: Cannot open input file." << endl;
        return 1;
    }

    string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    // Measure the original file size
    size_t originalSize = text.size();

    // Define the output folder path and output file name without extension
    string outputFolderPath = "C:\\Users\\techn\\OneDrive\\Documents\\3rd sem projects\\DSA C++ Project\\MainProject\\CompressedFile\\";
    size_t lastDotPosition = inputFileName.find_last_of(".");
    string outputFileNameWithoutExtension = inputFileName.substr(0, lastDotPosition);
    string outputFileName = outputFolderPath + outputFileNameWithoutExtension + "Compressed.ac";  // Use a custom file extension

    // Compress the text using Arithmetic Coding and measure the time taken
    auto start = high_resolution_clock::now();
    compressArithmeticCoding(text, outputFileName);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); // Change the time unit to microseconds

    // Measure the compressed file size
    ifstream compressedFile(outputFileName, ios::binary);
    compressedFile.seekg(0, ios::end);
    size_t compressedSize = compressedFile.tellg();
    compressedFile.close();

    int microsec = duration.count();
    float millisec = microsec/1000;

    cout << "Original File Size: " << originalSize << " bytes" << endl;
    cout << "Compressed File Size: " << compressedSize << " bytes" << endl << endl;
    cout << "Time Taken To Compress: " << millisec << " milliseconds" << endl << endl; // Display time in milliseconds
    cout << "File compressed successfully!" << endl;

    return 0;
}

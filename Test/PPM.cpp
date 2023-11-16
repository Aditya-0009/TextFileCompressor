#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>  // For using system()

using namespace std;
using namespace std::chrono;

int main() {
    cout << "Welcome to the Text File PPM Compressor!" << endl;

    string inputFileName;
    cout << "Enter input file name: ";
    cin >> inputFileName;
    cout << endl;

    // Define the output folder path and output file name without extension
    string outputFolderPath = "C:\\Users\\techn\\OneDrive\\Documents\\3rd sem projects\\DSA C++ Project\\MainProject\\CompressedFile\\";
    size_t lastDotPosition = inputFileName.find_last_of(".");
    string outputFileNameWithoutExtension = inputFileName.substr(0, lastDotPosition);
    string outputFileName = outputFolderPath + outputFileNameWithoutExtension + "Compressed.ppm";

    // Compress the text using PPM and measure the time taken
    auto start = high_resolution_clock::now();

    // Use the ppmcomp executable to compress the text
    string ppmCommand = "ppmcomp " + inputFileName + " " + outputFileName;
    int ppmResult = system(ppmCommand.c_str());

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); // Change the time unit to microseconds

    // Measure the original file size
    ifstream inputFile(inputFileName, ios::binary);
    inputFile.seekg(0, ios::end);
    size_t originalSize = inputFile.tellg();
    inputFile.close();

    // Measure the compressed file size
    ifstream compressedFile(outputFileName, ios::binary);
    compressedFile.seekg(0, ios::end);
    size_t compressedSize = compressedFile.tellg();
    compressedFile.close();

    int microsec = duration.count();
    float millisec = microsec / 1000;

    cout << "Original File Size: " << originalSize << " bytes" << endl;
    cout << "Compressed File Size: " << compressedSize << " bytes" << endl << endl;
    cout << "Time Taken To Compress: " << millisec << " milliseconds" << endl << endl;
    
    if (ppmResult != 0) {
        cout << "Error: PPM compression failed." << endl;
    } else {
        cout << "File compressed successfully!" << endl;
    }

    return 0;
}

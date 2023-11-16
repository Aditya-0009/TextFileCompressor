#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Function to compress text using Run-Length Encoding (RLE)
string compressRLE(const string& text) {
    string compressedText = "";
    int n = text.length();

    for (int i = 0; i < n; i++) {
        char currentChar = text[i];
        int count = 1;

        while (i < n - 1 && text[i] == text[i + 1]) {
            count++;
            i++;
        }

        compressedText += currentChar;
        compressedText += to_string(count);
    }

    return compressedText;
}

// Function to write the compressed text to a file
void writeCompressedTextToFile(const string& compressedText, const string& outputFileName) {
    ofstream outputFile(outputFileName);
    outputFile << compressedText;
    outputFile.close();
}

int main() {
    cout << "\nWelcome to the Text File RLE Compressor!" << endl;

    string inputFileName;
    cout << "Enter input file name: ";
    cin >> inputFileName;
    cout << endl;

    ifstream inputFile(inputFileName);
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
    string outputFileName = outputFolderPath + outputFileNameWithoutExtension + "Compressed.rle";

    // Compress the text using RLE and measure the time taken
    auto start = high_resolution_clock::now();
    string compressedText = compressRLE(text);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); // Change the time unit to microseconds

    // Write compressed data to a file
    writeCompressedTextToFile(compressedText, outputFileName);

    // Measure the compressed file size
    ifstream compressedFile(outputFileName);
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

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Function to compress text using Delta encoding
string compressText(const string& text) {
    string compressedText;
    char prevChar = text[0];  // Initialize with the first character
    compressedText += prevChar;

    for (size_t i = 1; i < text.size(); i++) {
        char currentChar = text[i];
        int delta = currentChar - prevChar;
        compressedText += to_string(delta);  // Convert delta to a string
        prevChar = currentChar;
    }

    return compressedText;
}

// Function to decompress text using Delta encoding
string decompressText(const string& compressedText) {
    string decompressedText;
    char prevChar = compressedText[0];
    decompressedText += prevChar;

    for (size_t i = 1; i < compressedText.size(); i++) {
        int delta = stoi(compressedText.substr(i, 1));  // Extract and convert delta from string to int
        char currentChar = prevChar + delta;
        decompressedText += currentChar;
        prevChar = currentChar;
    }

    return decompressedText;
}

int main() {
    cout << "\nWelcome to the Text File Delta Encoder!" << endl;

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
    string outputFileName = outputFolderPath + outputFileNameWithoutExtension + "Compressed.delta";

    // Compress the text using Delta encoding and measure the time taken
    auto start = high_resolution_clock::now();
    string compressedText = compressText(text);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    // Write compressed data to a text file
    ofstream outputFile(outputFileName);
    outputFile << compressedText;
    outputFile.close();

    // Measure the compressed file size
    size_t compressedSize = compressedText.size();

    int microsec = duration.count();
    float millisec = microsec / 1000;

    cout << "Original File Size: " << originalSize << " bytes" << endl;
    cout << "Compressed File Size: " << compressedSize << " bytes" << endl << endl;
    cout << "Time Taken To Compress: " << millisec << " milliseconds" << endl << endl;
    cout << "File compressed successfully!" << endl;

    // Decompress and compare
    string decompressedText = decompressText(compressedText);
    if (decompressedText == text) {
        cout << "Decompressed data matches the original data." << endl;
    } else {
        cout << "Decompressed data does not match the original data." << endl;
    }

    return 0;
}

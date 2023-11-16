#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Define the maximum search buffer and lookahead buffer sizes
const int SEARCH_BUFFER_SIZE = 1024;
const int LOOKAHEAD_BUFFER_SIZE = 64;

// Function to compress text using LZ77
string compressLZ77(const string& text) {
    string compressedText = "";
    int textLength = text.length();
    int currentPos = 0;

    while (currentPos < textLength) {
        int maxMatchLength = 0;
        int maxMatchOffset = 0;

        // Search for the longest match in the search buffer
        for (int offset = 1; offset <= min(currentPos, SEARCH_BUFFER_SIZE); offset++) {
            int matchLength = 0;
            while (matchLength < LOOKAHEAD_BUFFER_SIZE &&
                   currentPos + matchLength < textLength &&
                   text[currentPos - offset + matchLength] == text[currentPos + matchLength]) {
                matchLength++;
            }
            if (matchLength > maxMatchLength) {
                maxMatchLength = matchLength;
                maxMatchOffset = offset;
            }
        }

        // Add the LZ77 token to the compressed text
        if (maxMatchLength > 0) {
            compressedText += '(' + to_string(maxMatchOffset) + ',' + to_string(maxMatchLength) + ',' + text[currentPos + maxMatchLength] + ')';
            currentPos += maxMatchLength + 1;
        } else {
            // No match found, output the current character as a literal
            compressedText += text[currentPos];
            currentPos++;
        }
    }

    return compressedText;
}

// Function to write the compressed text to a binary file
void writeCompressedTextToFile(const string& compressedText, const string& outputFileName) {
    ofstream outputFile(outputFileName, ios::binary);
    outputFile << compressedText;
    outputFile.close();
}

int main() {
    cout << "\nWelcome to the Text File LZ77 Compressor!" << endl;

    string inputFileName;
    cout << "Enter input file name: ";
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
    string outputFileName = outputFolderPath + outputFileNameWithoutExtension + "Compressed.lz77";

    // Compress the text using LZ77 and measure the time taken
    auto start = high_resolution_clock::now();
    string compressedText = compressLZ77(text);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); // Change the time unit to microseconds

    // Write compressed data to a binary file
    writeCompressedTextToFile(compressedText, outputFileName);

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

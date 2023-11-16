#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>
#include <bitset>

using namespace std;
using namespace std::chrono;

// Function to compress the input text using LZW
string compressLZW(const string& text) {
    unordered_map<string, int> dictionary;
    vector<int> compressedData;
    int dictSize = 256;  // Start with ASCII values

    string currentString;
    for (char c : text) {
        currentString += c;
        if (dictionary.find(currentString) == dictionary.end()) {
            dictionary[currentString] = dictSize++;
            currentString.pop_back();  // Remove the last character
            compressedData.push_back(dictionary[currentString]);
            currentString = c;
        }
    }

    if (!currentString.empty()) {
        compressedData.push_back(dictionary[currentString]);
    }

    // Convert compressed data to a binary string
    string compressedText = "";
    for (int code : compressedData) {
        bitset<16> bits(code); // Assuming a fixed bit length for each code
        compressedText += bits.to_string();
    }

    return compressedText;
}

// Function to decompress the LZW-compressed data
string decompressLZW(const string& compressedText) {
    unordered_map<int, string> dictionary;
    int dictSize = 256;  // Start with ASCII values

    for (int i = 0; i < 256; i++) {
        dictionary[i] = string(1, static_cast<char>(i));
    }

    string decompressedText = "";
    string currentString = "";
    int codeSize = 9; // Adjust the code size as needed

    for (char c : compressedText) {
        currentString += c;
        if (currentString.length() == codeSize) {
            int code = bitset<16>(currentString).to_ulong();
            if (dictionary.find(code) != dictionary.end()) {
                decompressedText += dictionary[code];
                if (dictSize < (1 << codeSize)) {
                    dictionary[dictSize++] = dictionary[(codeSize < 9) ? code : code & 255] + dictionary[code & 255][0];
                }
            }
            currentString = "";
        }
    }

    return decompressedText;
}

// Function to write the LZW-compressed data to a binary file
void writeCompressedLZWToFile(const string& compressedText, const string& outputFileName) {
    ofstream outputFile(outputFileName, ios::binary);
    int n = compressedText.length();
    for (int i = 0; i < n; i += 8) {
        bitset<8> bits(compressedText.substr(i, 8));
        char byte = static_cast<char>(bits.to_ulong());
        outputFile.write(&byte, sizeof(byte));
    }
    outputFile.close();
}

int main() {
    cout << "Welcome to the LZW Compressor!" << endl;

    string inputFileName;
    cout << "Enter input file name: ";
    cin >> inputFileName;

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
    string outputFileName = outputFolderPath + outputFileNameWithoutExtension + "Compressed.lzw"; // Specify the output file extension

    // Compress the text using LZW and measure the time taken
    auto start = high_resolution_clock::now();
    string compressedText = compressLZW(text);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); // Change the time unit to microseconds

    // Write compressed data to a binary file
    writeCompressedLZWToFile(compressedText, outputFileName);

    // Measure the compressed file size
    ifstream compressedFile(outputFileName, ios::binary);
    compressedFile.seekg(0, ios::end);
    size_t compressedSize = compressedFile.tellg();
    compressedFile.close();

    int microsec = duration.count();
    float millisec = microsec / 1000;

    cout << "Original File Size: " << originalSize << " bytes" << endl;
    cout << "Compressed File Size: " << compressedSize << " bytes" << endl << endl;
    cout << "Time Taken To Compress: " << millisec << " milliseconds" << endl << endl; // Display time in milliseconds
    cout << "File compressed successfully!" << endl;

    // Decompression (optional)
    string decompressedText = decompressLZW(compressedText);
    string decompressedFileName = outputFolderPath + outputFileNameWithoutExtension + "Decompressed.txt";
    ofstream outputFileDecompressed(decompressedFileName);
    outputFileDecompressed << decompressedText;
    outputFileDecompressed.close();

    return 0;
}

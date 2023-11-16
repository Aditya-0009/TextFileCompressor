#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

// Function to perform Burrows-Wheeler Transform (BWT)
string burrowsWheelerTransform(const string& text) {
    vector<string> rotations;
    int n = text.size();

    for (int i = 0; i < n; i++) {
        rotations.push_back(text.substr(i) + text.substr(0, i));
    }

    sort(rotations.begin(), rotations.end());

    string bwtResult;
    int originalIndex = -1;
    for (int i = 0; i < n; i++) {
        if (rotations[i] == text) {
            originalIndex = i;
            break;
        }
    }

    for (int i = 0; i < n; i++) {
        int idx = (originalIndex + i) % n;
        bwtResult += rotations[idx].back();
    }

    return bwtResult;
}

string moveToFrontEncode(const string& text) {
    string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string mtfResult;
    for (char c : text) {
        size_t position = alphabet.find(c);
        if (position != string::npos) {  // Check if character exists in the alphabet
            mtfResult.push_back(static_cast<char>('A' + position));
            alphabet.erase(position, 1);
            alphabet = c + alphabet;
        }
    }
    return mtfResult;
}


// Function to write the compressed text to a binary file
void writeCompressedTextToFile(const string& compressedText, const string& outputFileName) {
    ofstream outputFile(outputFileName, ios::binary);
    for (char c : compressedText) {
        outputFile.write(&c, sizeof(c));
    }
    outputFile.close();
}

int main() {
    cout << "\nWelcome to the Text File BWT Compressor!" << endl;

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
    string outputFileName = outputFolderPath + outputFileNameWithoutExtension + "Compressed.bwt";

    // Apply Burrows-Wheeler Transform and measure the time taken in microseconds
    auto start = chrono::high_resolution_clock::now();
    string bwtResult = burrowsWheelerTransform(text);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    // Apply Move-to-Front (MTF) encoding
    string mtfResult = moveToFrontEncode(bwtResult);

    // Write compressed data to a binary file
    writeCompressedTextToFile(mtfResult, outputFileName);

    // Measure the compressed file size
    ifstream compressedFile(outputFileName, ios::binary);
    compressedFile.seekg(0, ios::end);
    size_t compressedSize = compressedFile.tellg();
    compressedFile.close();

    int microsec = duration.count();
    float milli = microsec/1000;

    cout << "Original File Size: " << originalSize << " bytes" << endl;
    cout << "Compressed File Size: " << compressedSize << " bytes" << endl;
    cout << "Time Taken To Compress: " << milli << " milliseconds" << endl;
    cout << "File compressed successfully!" << endl;

    return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <bitset>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Define a Huffman tree node
struct HuffmanNode {
    char data;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}};
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;}};
unordered_map<char, int> calculateFrequencies(const string& text) {
    unordered_map<char, int> freqMap;
    for (char c : text) {
        freqMap[c]++;
    }return freqMap;}
HuffmanNode* buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;
    for (const auto& entry : freqMap) {
        pq.push(new HuffmanNode(entry.first, entry.second));
    }
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();

        HuffmanNode* newNode = new HuffmanNode('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }
    return pq.top();
}
void buildHuffmanCodes(HuffmanNode* root, const string& code, unordered_map<char, string>& huffmanCodes) {
    if (root->left == nullptr && root->right == nullptr) {
        huffmanCodes[root->data] = code;
        return; }
    buildHuffmanCodes(root->left, code + "0", huffmanCodes);
    buildHuffmanCodes(root->right, code + "1", huffmanCodes);
}
string compressText(const string& text) {
    unordered_map<char, int> freqMap = calculateFrequencies(text);
    HuffmanNode* root = buildHuffmanTree(freqMap);
    unordered_map<char, string> huffmanCodes;
    buildHuffmanCodes(root, "", huffmanCodes);
    string compressedText = "";
    for (char c : text) {
        compressedText += huffmanCodes.at(c);}
    return compressedText;
}

void writeCompressedTextToFile(const string& compressedText, const string& outputFileName) {
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
    cout << "\nWelcome to the Text File Huffman Compressor!" << endl;

    string inputFileName;
    cout << "Enter input file name: ";
    cin >> inputFileName;
    cout << endl;

    ifstream inputFile(inputFileName, ios::binary);
    if (!inputFile) {
        cout << "Error: Cannot open input file." << endl;
        return 1;}
    string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();
    size_t originalSize = text.size();
    string outputFolderPath = "C:\\Users\\techn\\OneDrive\\Documents\\3rd sem projects\\DSA C++ Project\\MainProject\\CompressedFile\\";
    size_t lastDotPosition = inputFileName.find_last_of(".");
    string outputFileNameWithoutExtension = inputFileName.substr(0, lastDotPosition);
    string outputFileName = outputFolderPath + outputFileNameWithoutExtension + "Compressed.huf";
    auto start = high_resolution_clock::now();
    string compressedText = compressText(text);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); // Change the time unit to microseconds
    writeCompressedTextToFile(compressedText, outputFileName);
    ifstream compressedFile(outputFileName, ios::binary);
    compressedFile.seekg(0, ios::end);
    size_t compressedSize = compressedFile.tellg();
    compressedFile.close();
    int microsec = duration.count();
    float millisec = microsec/1000;
    cout << "Original File Size: " << originalSize << " bytes" << endl;
    cout << "Compressed File Size: " << compressedSize << " bytes" << endl << endl;
    cout << "Time Taken To Compress: " << millisec << " milliseconds" << endl << endl; // Display time in microseconds
    cout << "File compressed successfully!" << endl;
    return 0;
}
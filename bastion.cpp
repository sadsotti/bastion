#define NOMINMAX 
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <bitset>
#include <windows.h>
#include <limits>

using namespace std;

class Bastion {
public:
    void loadingBar(string text) {
        cout << "  " << text << " [";
        for (int i = 0; i < 20; i++) {
            cout << "#";
            Sleep(50);
        }
        cout << "] Done" << endl;
        Sleep(300);
    }

    void showHeader() {
        system("cls");
        system("color 0A");

        cout << endl;
        cout << "  ____            _   _                _     _   " << endl;
        cout << " |  _ \\          | | (_)              | |   | |  " << endl;
        cout << " | |_) | __ _ ___| |_ _  ___  _ __  __| | __| |__ " << endl;
        cout << " |  _ < / _` / __| __| |/ _ \\| '_ \\|__   |__   __|" << endl;
        cout << " | |_) | (_| \\__ \\ |_| | (_) | | | |  | |   | |  " << endl;
        cout << " |____/ \\__,_|___/\\__|_|\\___/|_| |_|  |_|   |_|  " << endl;
        cout << endl;
        cout << "      >>> Secure Steganography System <<<    " << endl;
        cout << "      >>> Developed by Lorenzo Sottile <<<    " << endl;
        cout << "  ==================================================" << endl;
        cout << endl;
    }

    void showInstructions() {
        cout << "  [!] System instructions:" << endl;
        cout << "   1. Format:   Use .BMP images only (24-bit)." << endl;
        cout << "   2. Location: Files must be in the same folder." << endl;
        cout << "   3. Syntax:   NO SPACES in filenames (Use: my_file.bmp)." << endl;
        cout << "   4. Security: Remember your password. No recovery." << endl;
        cout << endl;
        cout << "  --------------------------------------------------" << endl;
    }

    void hide(const string& imgInputPath, const string& secretFilePath, const string& imgOutputPath, const string& password) {
        cout << endl;
        loadingBar("Reading Image Data  ");

        vector<char> imgBuffer = readBinaryFile(imgInputPath);
        if (imgBuffer.empty()) return;

        if (imgBuffer.size() < 54) {
            cerr << "\n  [X] Error: Image file is too small or not a valid BMP." << endl;
            return;
        }

        loadingBar("Reading Secret File ");
        string message = readTextFile(secretFilePath);
        if (message.empty()) {
            cerr << "\n  [X] Error: Secret file is empty or not found." << endl;
            return;
        }

        message += "|EOF|";

        loadingBar("Encrypting with Key ");
        string encryptedMessage = xorCipher(message, password);

        int dataOffset = *reinterpret_cast<int*>(&imgBuffer[10]);
        if (dataOffset < 0 || static_cast<size_t>(dataOffset) >= imgBuffer.size()) {
            cerr << "\n  [X] Error: BMP header corrupted." << endl;
            return;
        }

        size_t availableBytes = imgBuffer.size() - static_cast<size_t>(dataOffset);
        if (availableBytes < (encryptedMessage.size() * 8)) {
            cerr << "\n  [X] Error: Image too small! Need more pixels." << endl;
            return;
        }

        loadingBar("Injecting Bitstream ");

        size_t pixelIndex = static_cast<size_t>(dataOffset);

        for (char c : encryptedMessage) {
            bitset<8> bits(c);
            for (int i = 0; i < 8; i++) {
                if (pixelIndex >= imgBuffer.size()) {
                    cerr << "\n  [X] Error: Overflow prevented." << endl;
                    return;
                }
                int bitToHide = bits[7 - i];
                imgBuffer[pixelIndex] = (imgBuffer[pixelIndex] & 0xFE) | bitToHide;
                pixelIndex++;
            }
        }

        writeBinaryFile(imgOutputPath, imgBuffer);

        cout << endl;
        cout << "  ==================================================" << endl;
        cout << "  [V] Operation successful" << endl;
        cout << "  [>] Output saved as: " << imgOutputPath << endl;
        cout << "  ==================================================" << endl;
    }

    void extract(const string& imgInputPath, const string& fileOutputPath, const string& password) {
        cout << endl;
        loadingBar("Analyzing Bitmap    ");

        vector<char> imgBuffer = readBinaryFile(imgInputPath);
        if (imgBuffer.empty()) return;

        if (imgBuffer.size() < 54) {
            cerr << "\n  [X] Error: Invalid BMP." << endl;
            return;
        }

        int dataOffset = *reinterpret_cast<int*>(&imgBuffer[10]);
        if (dataOffset < 0 || static_cast<size_t>(dataOffset) >= imgBuffer.size()) {
            cerr << "\n  [X] Error: Corrupt BMP header." << endl;
            return;
        }

        string extractedData = "";

        loadingBar("Decrypting Stream   ");

        size_t pixelIndex = static_cast<size_t>(dataOffset);
        char c = 0;
        int bitCount = 0;
        int charIndex = 0;

        while (pixelIndex < imgBuffer.size()) {
            int extractedBit = imgBuffer[pixelIndex] & 1;
            c = (c << 1) | extractedBit;
            bitCount++;

            if (bitCount == 8) {
                char decryptedChar = c ^ password[charIndex % password.length()];
                extractedData += decryptedChar;
                charIndex++;

                if (extractedData.size() >= 5 &&
                    extractedData.substr(extractedData.size() - 5) == "|EOF|") {
                    extractedData = extractedData.substr(0, extractedData.size() - 5);
                    break;
                }

                if (extractedData.size() > 5000000) {
                    cout << "\n  [!] Warning: Decryption limit reached." << endl;
                    cout << "      (Did you use the correct password?)" << endl;
                    break;
                }

                c = 0;
                bitCount = 0;
            }
            pixelIndex++;
        }

        ofstream out(fileOutputPath);
        out << extractedData;
        out.close();

        cout << endl;
        cout << "  ==================================================" << endl;
        cout << "  [V] Extraction complete" << endl;
        cout << "  [>] Secret saved to: " << fileOutputPath << endl;
        cout << "  ==================================================" << endl;
    }

private:
    string xorCipher(string data, string key) {
        if (key.empty()) return data;
        string result = data;
        for (size_t i = 0; i < data.size(); i++) {
            result[i] = data[i] ^ key[i % key.size()];
        }
        return result;
    }

    vector<char> readBinaryFile(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "\n  [X] File error: Cannot find '" << filename << "'" << endl;
            cerr << "      Tip: Check spelling and avoid spaces." << endl;
            return {};
        }
        return vector<char>((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    }

    string readTextFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "\n  [X] File error: Cannot find '" << filename << "'" << endl;
            return "";
        }
        return string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    }

    void writeBinaryFile(const string& filename, const vector<char>& buffer) {
        ofstream file(filename, ios::binary);
        file.write(buffer.data(), buffer.size());
    }
};

int main() {
    Bastion app;
    int choice;
    string imgIn, textFile, imgOut, password;

    while (true) {
        app.showHeader();
        app.showInstructions();

        cout << "  Select operation:" << endl;
        cout << "  [1] Encrypt & Hide (Lock)" << endl;
        cout << "  [2] Decrypt & Open (Unlock)" << endl;
        cout << "  [0] Exit" << endl;
        cout << "\n  >> Input choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\n  [!] Invalid input. Please enter a number (0, 1, 2)." << endl;
            Sleep(1500);
            continue;
        }

        if (choice == 0) {
            cout << "\n  Exiting Bastion++..." << endl;
            break;
        }
        else if (choice == 1) {
            cout << "\n  [Input] Source Image (e.g. boat.bmp): ";
            cin >> imgIn;

            cout << "  [Input] Secret File (e.g. secret.txt): ";
            cin >> textFile;

            cout << "  [Output] New Filename (e.g. boat_sec.bmp): ";
            cin >> imgOut;

            cout << "  [Key] Set Password: ";
            cin >> password;

            app.hide(imgIn, textFile, imgOut, password);
            system("pause");
        }
        else if (choice == 2) {
            cout << "\n  [Input] Carrier Image (e.g. boat_sec.bmp): ";
            cin >> imgIn;

            cout << "  [Output] Save Secret As (e.g. revealed.txt): ";
            cin >> textFile;

            cout << "  [Key] Enter Password: ";
            cin >> password;

            app.extract(imgIn, textFile, password);
            system("pause");
        }
        else {
            cout << "\n  [!] Invalid selection." << endl;
            Sleep(1000);
        }
    }

    return 0;
}
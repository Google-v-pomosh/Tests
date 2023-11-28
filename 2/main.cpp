#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>

class Logger {
public:
    void writeToLog(const std::string& logMessage) {
        std::ofstream logFile("Log.txt", std::ios_base::app);
        if (!logFile.is_open()) {
            std::cerr << "Error creating or opening log file.\n";
            return;
        }

        std::time_t currentTime = std::time(nullptr);
        logFile << std::ctime(&currentTime) << ": " << logMessage << "\n";

        logFile.close();
    }
};

class List {
private:
    class Node {
    public:
        Node* next;
        std::string word;
        int count;
        Node(const std::string& w) : word(w), count(1), next(nullptr) {}
    };

    Node* head;
    Logger logger;

   void addWord(const std::string& newWord) {
        if (head == nullptr) {
            head = new Node(newWord);
            return;
        }

        Node* current = head;
        while (current->next != nullptr) {
            if (current->word == newWord) {
                current->count++;
                return;
            }
            current = current->next;
        }
        current->next = new Node(newWord);
    }

    void writeResultToFile(const std::string& outputFilename) {
        try {
            std::ofstream outputFile(outputFilename);
            if (!outputFile.is_open()) {
                logger.writeToLog("Error creating file to record results.");
                throw std::runtime_error("Error creating file to record results");
            }
            Node* current = head;
            while (current != nullptr) {
                outputFile << current->word << " - " << current->count << "\n";
                current = current->next;
            }
            outputFile.close();
        } catch (const std::exception& e) {
            logger.writeToLog(std::string("An error has occurred: ") + e.what());
            throw;
        }
    }

    void sortList() {
        if (head == nullptr || head->next == nullptr) return;

        Node *i, *j;
        std::string tempWord;
        int tempCount;

        for (i = head; i != nullptr; i = i->next) {
            for (j = i->next; j != nullptr; j = j->next) {
                if (j->count > i->count || (j->count == i->count && j->word < i->word)) {
                    tempWord = i->word;
                    tempCount = i->count;
                    i->word = j->word;
                    i->count = j->count;
                    j->word = tempWord;
                    j->count = tempCount;
                }
            }
        }
    }


public:
    List() : head(nullptr) {}

    ~List() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
    }

    void writeLog(const std::string &msg)
    {
        logger.writeToLog(msg);
    }

    void processFile(const std::string& filename) {
        try {
            logger.writeToLog("Started processing file: " + filename);

            std::ifstream file(filename);
            if (!file.is_open()) {
                logger.writeToLog("Error opening file: " + filename);
                throw std::runtime_error("Error opening file");
            }

            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string word;
                while (iss >> word) {
                    word.erase(std::remove_if(word.begin(), word.end(), [](char c) {
                        return !std::isalpha(c);
                    }), word.end());
                    if (!word.empty()) {
                        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                        addWord(word);
                    }
                }
            }

            file.close();
            sortList();
            writeResultToFile("ResultPairs.txt");

            logger.writeToLog("Finished processing file: " + filename + " Results written to ResultPairs.txt");
        } catch (const std::exception& e) {
            logger.writeToLog(std::string("An error has occurred: ") + e.what());
            throw;
        }
    }
    
};

int main() {
    List list;
    try {
        const std::string inputFilename = "WordsList.txt";
        list.processFile(inputFilename);
        std::cout << "Results successfully written to file ResultPairs.txt.\n";
        return 0;
    } catch (const std::exception& e) {
        list.writeLog(std::string("An error has occurred: ") + e.what());
        std::ofstream logFile("Log.txt", std::ios_base::app);
        if (logFile.is_open()) {
            std::time_t currentTime = std::time(nullptr);
            logFile << std::ctime(&currentTime) << ": An error has occurred: " << e.what() << "\n";
            logFile.close();
        } else {
            std::cerr << "Error creating or opening log file.\n";
        }
        std::cerr << "An error has occurred: " << e.what() << "\n";
        return 1;
    }
}

#include <fstream>
#include <string>
#include <iostream>

bool inList(std::string word, std::ifstream* list);
void cleanFile(std::string path);

int main() {

    std::ifstream newWords;
    newWords.open("../suggestedbadwords.txt");

    if (!newWords) {
        std::cerr << "no suggestions yet" << std::endl;
        return 1;
    }

    std::ofstream mainWords;
    mainWords.open("../badwords.txt", std::ofstream::app);
    std::ifstream mainWordsRead;
    mainWordsRead.open("../badwords.txt");

    std::string word;
    while (std::getline(newWords, word)) {
        if (!inList(word, &mainWordsRead)) {
            mainWords << "\n" << word;
            std::cout << "added: " << word << std::endl;
        }
    }
    newWords.close();
    mainWords.close();
    mainWordsRead.close();

    cleanFile("../suggestedbadwords.txt");

    std::cout << "updated!" << std::endl;
    return 0;

}

bool inList(std::string word, std::ifstream* list) {
    std::string current;
    while (std::getline(*list, current)) {
        std::cout << "currently checking if " << word << " is " << current << std::flush;
        if (word == current) {
            std::cout << " [cross]" << std::endl;
            return true;
        }
        std::cout << " [tick]" << std::endl;
    }
    return false;
}

void cleanFile(std::string path) {
    std::ofstream file;
    file.open(path);
    file << "";
    file.close();
}
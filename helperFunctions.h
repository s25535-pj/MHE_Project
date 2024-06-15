#ifndef PROJECT2_HELPERFUNCTIONS_H
#define PROJECT2_HELPERFUNCTIONS_H

#include <iostream>
#include <vector>
#include <algorithm>

std::random_device rd;
std::mt19937 randomGenerator(rd());

struct Nonogram {
public:
    int rating = 0;
    std::vector<std::vector<int>> board;

    void printRating() {
        std::cout << "Rating: " << rating << std::endl;
    }

    // Wydrukuj nonogram
    void printBoard() {
        for (const auto& n : board) {
            for (int value : n) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
    }

    std::string boardToString() {
        std::string string;
        for (const auto& n : board) {
            for (int value : n) {
                string.append(std::to_string(value));
                string.append(" ");
            }
            string.append("\n");
        }
//        std::cout << string;
        return string;
    }

//    bool operator==(const Nonogram& other) const {
//        return board == other.board;
//    }

};

struct Vector2d {
    std::vector<std::vector<int>> x;
    std::vector<std::vector<int>> y;

    // Zwróć sumę zamalowanych pól w wierszu o danym indeksie
    int getXRowSum(int index) const {
        int sum = 0;
        for (int j = 0; j < x[index].size(); j++) {
            sum += x[index][j];
        }
        return sum;
    }

    // Zwróć sumę zamalowanych pól w kolumnie o danym indeksie
    int getYRowSum(int index) const {
        int sum = 0;
        for (int j = 0; j < y[index].size(); j++) {
            sum += y[index][j];
        }
        return sum;
    }

    std::vector<int> checkIfIsIncluded(int index) {
        return x[index];
    }

    std::vector<int> getYRowNumbers(int index) {
        return y[index];
    }

};

// Wydrukuj zawartość vektora
void printVector(std::vector<std::vector<int>> vector) {
    for (const auto& v : vector) {
        for (int value : v) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

// Zaczytaj nonogram do zadania
Vector2d generate_hintsFromNonogram(Nonogram nonogram) {
    Vector2d hints;
    std::vector<int> tmp;
    int blockLength = 0;

    // X
    for (int i = 0; i < nonogram.board.size(); i++) {
        for (int j = 0; j < nonogram.board[i].size(); j++) {
            if (nonogram.board[i][j] == 1) {
                blockLength++;
            } else {
                if (blockLength > 0) {
                    tmp.push_back(blockLength);
                }
                blockLength = 0;
            }
        }
        // Dodawanie wartości na końcu iteracji jeśli nie jest zerem by nie ominąć żadnej liczby
        if (blockLength > 0) {
            tmp.push_back(blockLength);
        }
        // Wpychanie zaczytanego wiersza i zerowanie pośredników
        hints.x.push_back(tmp);
        blockLength = 0;
        tmp.clear();
    }

    // Y
    for (int i = 0; i < nonogram.board[0].size(); i++) { // Małe machlojki by pętla działała
        for (int j = 0; j < nonogram.board.size(); j++) {
            if (nonogram.board[j][i] == 1) {
                blockLength++;
            } else {
                if (blockLength > 0) {
                    tmp.push_back(blockLength);
                }
                blockLength = 0;
            }
        }
        // Dodawanie wartości na końcu iteracji jeśli nie jest zerem by nie ominąć żadnej liczby
        if (blockLength > 0) {
            tmp.push_back(blockLength);
        }
        // Wpychanie zaczytanej kolumny i zerowanie pośredników
        hints.y.push_back(tmp);
        blockLength = 0;
        tmp.clear();
    }
    return hints;
}

void compareNonograms(Nonogram pattern, Nonogram compared) {
    for (int i = 0; i < compared.board.size(); i++) {
        for (int j = 0; j < compared.board[i].size(); j++) {
            if (compared.board[i][j] != pattern.board[i][j]) {
                if (compared.board[i][j] != 0) {
                    std::cout << "\033[1;31m" << "1" << " ";
                } else {
                    std::cout << "\033[1;31m" << "0" << " ";
                }

            } else {
                if (compared.board[i][j] != 0) {
                    std::cout << "\033[0m" << "1" << " ";
                } else {
                    std::cout << "\033[0m" << "0" << " ";
                }
            }
        }
        std::cout << "\033[0m" << std::endl;
    }
}

bool numberIsInVector(int number, std::vector<int> vector) {
    if (std::find(vector.begin(), vector.end(), number) != vector.end()) {
        return true;
    }
    return false;
}

bool comparisonFunctionMore(Nonogram& a, Nonogram& b) {
    return a.rating > b.rating;
}

bool comparisonFunctionLess(Nonogram& a, Nonogram& b) {
    return a.rating < b.rating;
}

// Wygeneruj losową planszę gry o zadanym rozmiarze
Nonogram generate_randomNonogram(int sizeX, int sizeY) {
    Nonogram nonogram;
    // Jak nie określałem rozmiaru wektora to miałem SEG faulty
    std::vector<std::vector<int>> board(sizeX, std::vector<int>(sizeY));
    std::uniform_int_distribution<int> value(0,1);

    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            board[i][j] = value(randomGenerator);
        }
    }
    nonogram.board = board;
    return nonogram;
}

Nonogram generate_zeroNonogram(int sizeX, int sizeY) {
    Nonogram nonogram;
    // Jak nie określałem rozmiaru wektora to miałem SEG faulty
    std::vector<std::vector<int>> board(sizeX, std::vector<int>(sizeY));

    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            board[i][j] = 0;
        }
    }
    nonogram.board = board;
    return nonogram;
}

bool isInList(Nonogram nonogram, std::list<Nonogram> list) {
    for (auto e : list) {
//        printVector(nonogram.board);
//        std::cout << "==" << std::endl;
//        printVector(e.board);
//        std::cout << std::endl;
        if (nonogram.board == e.board) {
            return true;
        }
    }
    return false;
}

std::vector<std::vector<int>> readSection(std::ifstream &inputFile) {
    std::vector<std::vector<int>> vector;
    std::string line;

    while (std::getline(inputFile, line)) {
        if (line == "columns") {
            break;
        }
        std::istringstream lineStream(line);
        std::vector<int> row;
        int number;

        while (lineStream >> number) {
            row.push_back(number);
        }

        vector.push_back(row);
    }
    return vector;
}
Vector2d readFile(std::string filePath) {
    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cout << "Failed to open the input file" << std::endl;
        exit(1);
    }

    Vector2d hints;
    std::string line;

    while (std::getline(inputFile, line)) {
        hints.x = readSection(inputFile);
        hints.y = readSection(inputFile);
    }

    inputFile.close();
    return hints;
}

#endif //PROJECT2_HELPERFUNCTIONS_H

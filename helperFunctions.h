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

//    bool operator==(const Nonogram& other) const {
//        return board == other.board;
//    }

};

struct Vector2d {
    std::vector<std::vector<int>> x;
    std::vector<std::vector<int>> y;

    int sizeX() {
        return (int)x.size();
    }

    int sizeY() {
        return (int)y.size();
    }

    // Zwróć sumę zamalowanych pól w wierszu o danym indeksie
    int getXRowSum(int index) {
        int sum = 0;
        for (int j = 0; j < x[index].size(); j++) {
            sum += x[index][j];
        }
        return sum;
    }

    // Zwróć sumę zamalowanych pól w kolumnie o danym indeksie
    int getYRowSum(int index) {
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
Vector2d readHintsFromBoard(Nonogram nonogram) {
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
                    std::cout << "\033[1;31m" << "#" << " ";
                } else {
                    std::cout << "\033[1;31m" << "." << " ";
                }

            } else {
                if (compared.board[i][j] != 0) {
                    std::cout << "\033[0m" << "#" << " ";
                } else {
                    std::cout << "\033[0m" << "." << " ";
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

bool comparisonFunction(Nonogram a, Nonogram& b) {
    return a.rating < b.rating;
}

std::vector<Nonogram> generateAllNonograms(const Vector2d& hints) {
    std::vector<Nonogram> nonogramList;
    int x = (int)hints.x.size();
    int y = (int)hints.y.size();

    auto totalCombinations = (unsigned long long)pow(2,x*y);

    // nonogram 5x5 = 2^25 = 33,554,443 kombinacji.
    std::cout << "Generating..." << std::endl;
    for (unsigned long long combination = 0; combination < totalCombinations; combination++) {
        std::vector<std::vector<int>> currentBoard(x, std::vector<int>(y)); // rozmiar wektora pośredniego
        unsigned long long currentCombination = combination;

        // Zaczytywanie liczb do board
        for (int i = 0; i < x * y; i++) {
            int row = i / x; // int zaokrągla w dół
            int col = i % y;

            currentBoard[row][col] = (int)currentCombination & 1; // przepisz skrajnie prawy bit do komórki nonogramu.
            currentCombination >>= 1; // przesuń wszytkie bity w prawo.
        }
        Nonogram nonogram;
        nonogram.board = currentBoard;
        nonogramList.push_back(nonogram);
    }
    return nonogramList;
}

// Wygeneruj losową planszę gry o zadanym rozmiarze


Nonogram generate_randomNonogram(int sizeX, int sizeY) {
    Nonogram nonogram;
    // Jak nie określałem rozmiaru wektora to miałem SEG faulty
    std::vector<std::vector<int>> puzzle(sizeX, std::vector<int>(sizeY));
    std::uniform_int_distribution<int> value(0,1);

    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            puzzle[i][j] = value(randomGenerator);
        }
    }
    nonogram.board = puzzle;
    return nonogram;
}

Nonogram generate_zeroNonogram(int sizeX, int sizeY) {
    Nonogram nonogram;
    // Jak nie określałem rozmiaru wektora to miałem SEG faulty
    std::vector<std::vector<int>> puzzle(sizeX, std::vector<int>(sizeY));

    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            puzzle[i][j] = 0;
        }
    }
    nonogram.board = puzzle;
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

#endif //PROJECT2_HELPERFUNCTIONS_H

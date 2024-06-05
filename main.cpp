#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "helperFunctions.h"

// Wygeneruj planszę gry
Nonogram generate_puzzleManually() {
    Nonogram nonogram;
    std::vector<std::vector<int>> board = {
//            {1, 1, 0, 1, 1},
//            {1, 0, 1, 0, 0},
//            {1, 0, 0, 0, 1},
//            {0, 1, 1, 1, 0},
//            {0, 0, 0, 1, 0}
            {1,0,1},
            {0,1,0},
            {1,1,0},
    };
    nonogram.board = board;
    return nonogram;
}

// Pobierz zadanie, podpowiedzi
Vector2d getHints() {
    Vector2d hints;
    hints.x = {
//            {2,2},
//            {1,1},
//            {1,1},
//            {3},
//            {1}
            {1,1},
            {1},
            {2}
    };
    hints.y = {
//            {3},
//            {1,1},
//            {1,1},
//            {1,2},
//            {1,1}
            {1,1},
            {2},
            {1}
    };

    return hints;
}

int rateNonogramBasingOnHints(Nonogram nonogram, Vector2d hints) {
    int rating = 0;
    int painted = 0;
    int blockLength = 0;
    std::vector<int> tmp;
    auto board = nonogram.board;

    // X
    // Sprawdź wartości na zadanym indexie
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j <board[i].size(); j++) {
            if (board[i][j] == 1) {
                painted++;
                blockLength++;
            } else {
                // Nie ma sensu wstawiać 0
                if (blockLength > 0) tmp.push_back(blockLength);
                blockLength = 0;
            }
        }
        // Odejmij różnicę zamalowanych pól od wyniku
//        std::cout << "PaintedX: " << painted << std::endl;
//        std::cout << "rowSumX : " << hints.getXRowSum(i) << std::endl;
        if (blockLength > 0) tmp.push_back(blockLength);

        for (int j = 0; j < tmp.size(); j++) {
            if(j <= hints.x[i].size()) {
                if (tmp[j] != hints.x[i][j]) {
                    rating -= 1; // - Za każdą złą liczbę na indexie
                }
            }
        }
        rating -= std::abs((int)hints.x[i].size() - (int)tmp.size()) * 2; // - za każdy brak liczby
        rating -= std::abs((int)hints.getXRowSum(i) - painted); // - za każde źle zamalowane

        blockLength = 0;
        painted = 0;
        tmp.clear();
    }

    // Y
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j <board[i].size(); j++) {
            if (board[j][i] == 1) {
                painted++;
                blockLength++;
            } else {
                // Nie ma sensu wstawiać 0
                if (blockLength > 0) tmp.push_back(blockLength);
                blockLength = 0;
            }
        }
        // Odejmij różnicę zamalowanych pól od wyniku
//        std::cout << "PaintedY: " << painted << std::endl;
//        std::cout << "rowSumX : " << hints.getYRowSum(i) << std::endl;
        if (blockLength > 0) tmp.push_back(blockLength);

        for (int j = 0; j < tmp.size(); j++) {
            if(j <= hints.y[i].size()) {
                if (tmp[j] != hints.y[i][j]) {
                    rating -= 1; // - Za każdą złą liczbę na indexie
                }
            }
        }
        rating -= std::abs((int)hints.y[i].size() - (int)tmp.size()) * 2; // - za każdy brak liczby
        rating -= std::abs((int)hints.getYRowSum(i) - painted); // - za każde źle zamalowane

        blockLength = 0;
        painted = 0;
        tmp.clear();
    }
    return rating;
}

// Wygeneruj losową planszę gry o zadanym rozmiarze
std::random_device rd;
std::mt19937 randomGenerator(rd());

Nonogram generate_randomSolution(Vector2d hints) {
    Nonogram nonogram;
    // Jak nie określałem rozmiaru wektora to miałem SEG faulty
    std::vector<std::vector<int>> puzzle(hints.sizeX(), std::vector<int>(hints.sizeY()));
    std::uniform_int_distribution<int> value(0,1);

    for (int i = 0; i < hints.sizeX(); i++) {
        for (int j = 0; j < hints.sizeY(); j++) {
            puzzle[i][j] = value(randomGenerator);
        }
    }
    nonogram.board = puzzle;
    return nonogram;
}

// Utwórz nonogram sąsiadujący z dostarczonym
Nonogram generate_randomNeighbor(Nonogram nonogram) {
    std::uniform_int_distribution<int> value(0,(int)nonogram.board.size()-1);

    int x = value(randomGenerator);
    int y = value(randomGenerator);

    if (nonogram.board[x][y] == 0) {
        nonogram.board[x][y] = 1;
    } else {
        nonogram.board[x][y] = 0;
    }
    return nonogram;
}

std::vector<Nonogram> generate_allNeighbors(Nonogram nonogram) {
    std::vector<Nonogram> allNeighbors;

    for (int i = 0; i < nonogram.board.size(); i++) {
        for (int j = 0; j < nonogram.board[i].size(); j++) {
            Nonogram tmp = nonogram;
            if (nonogram.board[i][j] == 0) {
                tmp.board[i][j] = 1;
            } else {
                tmp.board[i][j] = 0;
            }
            allNeighbors.push_back(tmp);
        }
    }
    return allNeighbors;
}

// Algorytm pełnego przeglądu
Nonogram solve_fullReview(const Vector2d& hints) {
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

        if (rateNonogramBasingOnHints(nonogram, hints) == 0){
            return nonogram;
        }
    }
}

// Algorytm wspinaczkowy z losowym sąsiadem
Nonogram solve_hillClimbing_random(Vector2d hints, int stop) {
    Nonogram currentSolution = generate_randomSolution(hints);
    currentSolution.rating = rateNonogramBasingOnHints(currentSolution, hints);

    while(stop > 0) {
        Nonogram randomNeighbor = generate_randomNeighbor(currentSolution);
        randomNeighbor.rating = rateNonogramBasingOnHints(randomNeighbor, hints);
        if (currentSolution.rating < randomNeighbor.rating) {
//            std::cout << std::endl;
//            printVector(currentSolution.board);
            currentSolution = randomNeighbor;
//            std::cout << "Rating: " << randomNeighbor.rating << std::endl;
        }
        stop--;
    }
    return currentSolution;
}

// Algorytm wspinaczkowy deterministyczny, wybór najlepszego sąsiada
Nonogram solve_hillClimbing_deterministic(Vector2d hints, int stop) {
    Nonogram currentSolution = generate_randomSolution(hints);
    currentSolution.rating = rateNonogramBasingOnHints(currentSolution, hints);

    while(stop > 0 && currentSolution.rating < 0) {
        std::vector<Nonogram> allNeighbors = generate_allNeighbors(currentSolution);

        for (auto n : allNeighbors) {
            n.rating = rateNonogramBasingOnHints(n, hints);
            if (n.rating > currentSolution.rating) {
                currentSolution = n;
//                std::cout << std::endl;
//                printVector(currentSolution.board);
//                std::cout << "Rating: " << currentSolution.rating << std::endl;
            }
        }
        stop--;
    }
    return currentSolution;
}


void test1() {
    std::vector<Nonogram> nonogramList;
    Vector2d hints;
    Nonogram nonogram = generate_puzzleManually();

    hints = getHints();
    nonogramList = generateAllNonograms(hints);

    std::cout << "Rating..." << std::endl;
    for (auto &n : nonogramList) {
        n.rating = rateNonogramBasingOnHints(n, hints);
    }

    std::cout << "Sorting..." << std::endl;
    std::sort(nonogramList.begin(), nonogramList.end(), comparisonFunction);

    for (auto &n : nonogramList) {
//        printVector(n.board);
        compareNonograms(nonogram,n);
        std::cout << "Rating: " << n.rating << std::endl;
        std::cout << std::endl;
    }
}

void test2() {
////    Nonogram nonogram = generate_randomSolution(20, 20);
//    Nonogram neighbor = generate_randomNeighbor(nonogram);
//
//    printVector(nonogram.board);
//    std::cout << std::endl;
//    compareNonograms(nonogram, neighbor);
}

void test3() {
    Nonogram nonogram = solve_fullReview(getHints());
    printVector(nonogram.board);
}
void test_hillClimbRandom() {
    Vector2d hints = getHints();
    Nonogram solution = solve_hillClimbing_random(hints, 100);
    std::cout << std::endl << "Best found solution: " << solution.rating << std::endl;
    printVector(solution.board);
}

void test_hillClimbDeterministic1() {
    Vector2d hints = getHints();
    Nonogram nonogram = generate_randomSolution(hints);
    std::vector<Nonogram> nonograms = generate_allNeighbors(nonogram);

    std::cout << "Pattern" << std::endl;
    printVector(nonogram.board);
    for (auto n : nonograms) {
        compareNonograms(nonogram, n);
        std::cout << std::endl;
    }

//    Nonogram solution = solve_hillClimbing_deterministic(hints, 100);
}

void test_hillClimbDeterministic2() {
    Nonogram nonogram = solve_hillClimbing_deterministic(getHints(), 100);
    printVector(nonogram.board);
    std::cout << "Rating: " << nonogram.rating << std::endl;
}

int main() {
//    test1();
//    test2();
//    test3();
//    test_hillClimbRandom();
//    test_hillClimbDeterministic1();
    test_hillClimbDeterministic2();

    return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <list>
#include <set>
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
//            {1,0,1,1},
//            {0,1,0,0},
//            {1,1,0,1},
            {1,0,1},
            {0,1,0},
            {1,1,0},
            {1,1,1}
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

//            {1,2},
//            {1},
//            {2,1}

            {1,1},
            {1},
            {2},
            {3}
    };
    hints.y = {
//            {3},
//            {1,1},
//            {1,1},
//            {1,2},
//            {1,1}

//            {1,1},
//            {2},
//            {1},
//            {1,1}

            {1,2},
            {3},
            {1,1}
    };

    return hints;
}

int rateNonogramBasingOnHints(Nonogram nonogram, Vector2d hints) {
    int rating = 0;
    int painted = 0;
    int blockLength = 0;
    std::vector<int> tmp;

    // X
    // Sprawdź wartości na zadanym indexie
    for (int i = 0; i < nonogram.board.size(); i++) {
        for (int j = 0; j < nonogram.board[i].size(); j++) {
            if (nonogram.board[i][j] == 1) {
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

        int size = 0;
        // Porównaj hinty i zaczytane z nonogramu bloki
        for (int j = 0; j < tmp.size(); j++) {
            // Weź ten co jest mniejszy
            if(j < hints.x[i].size()) {
                size = j;
            } else {
                size = (int)hints.x[i].size();
            }
            for (int k = 0; k < size; k++) {
                if (tmp[k] != hints.x[i][k]) {
                    rating -= 1; // - Za każdą złą liczbę na indexie
                }
            }
        }
        rating -= std::abs((int)hints.x[i].size() - (int)tmp.size()) * 2; // - za każdy brak liczby
        rating -= std::abs((int)hints.getXRowSum(i) - painted); // - za każde źle zamalowane

        blockLength = 0;
        painted = 0;
        tmp.clear();
//        std::cout << "Rating X: " << rating << std::endl;
    }

    // Y
    for (int i = 0; i < nonogram.board[0].size(); i++) {
        for (int j = 0; j < nonogram.board.size(); j++) {
            if (nonogram.board[j][i] == 1) {
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
//        std::cout << "rowSumY : " << hints.getYRowSum(i) << std::endl;
        if (blockLength > 0) tmp.push_back(blockLength);

        int size = 0;
        // Porównaj hinty i zaczytane z nonogramu bloki
        for (int j = 0; j < tmp.size(); j++) {
            // Weź ten co jest mniejszy
            if(j < hints.y[i].size()) {
                size = j;
            } else {
                size = (int)hints.y[i].size();
            }

            for (int k = 0; k < size; k++) {
                if (tmp[k] != hints.y[i][k]) {
                    rating -= 1; // - Za każdą złą liczbę na indexie
                }
            }
        }

        rating -= std::abs((int)hints.y[i].size() - (int)tmp.size()) * 2; // - za każdy brak liczby
        rating -= std::abs((int)hints.getYRowSum(i) - painted); // - za każde źle zamalowane

        blockLength = 0;
        painted = 0;
        tmp.clear();
//        std::cout << "Rating Y: " << rating << std::endl;
    }
    return rating;
}

// Wygeneruj losową planszę gry na podstawie wskazówek
Nonogram generate_randomSolution_rateIt(Vector2d hints) {
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
    nonogram.rating = rateNonogramBasingOnHints(nonogram,hints);
    return nonogram;
}

// Utwórz nonogram sąsiadujący z dostarczonym
Nonogram generate_randomNeighbor_rateIt(Nonogram nonogram, Vector2d hints) {
    std::uniform_int_distribution<int> value(0,(int)nonogram.board.size()-1);

    int x = value(randomGenerator);
    int y = value(randomGenerator);

    if (nonogram.board[x][y] == 0) {
        nonogram.board[x][y] = 1;
    } else {
        nonogram.board[x][y] = 0;
    }
    nonogram.rating = rateNonogramBasingOnHints(nonogram, hints);
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
    Nonogram fake;
    int x = (int)hints.x.size();
    int y = (int)hints.y.size();

    auto totalCombinations = (unsigned long long)pow(2,x*y);

    // nonogram 5x5 = 2^25 = 33,554,443 kombinacji.
    std::cout << "Generating..." << std::endl;
    for (unsigned long long combination = 0; combination < totalCombinations; combination++) {
        std::vector<std::vector<int>> currentBoard(x, std::vector<int>(y)); // rozmiar wektora pośredniego
        unsigned long long currentCombination = combination;

        // Zaczytywanie liczb do board
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                currentBoard[i][j] = (int)currentCombination & 1; // przepisz skrajnie prawy bit do komórki nonogramu.
                currentCombination >>= 1; // przesuń wszytkie bity w prawo.
            }
        }

        Nonogram nonogram;
        nonogram.board = currentBoard;

        if (rateNonogramBasingOnHints(nonogram, hints) == 0){
            return nonogram;
        }
    }
    std::cout << "You should never see this!";
    return fake;
}

// Algorytm wspinaczkowy z losowym sąsiadem
Nonogram solve_hillClimbing_random(Vector2d hints, int stop) {
    Nonogram currentSolution = generate_randomSolution_rateIt(hints);
    int currentStop = stop;

    while(currentStop > 0 && currentSolution.rating < 0) {
        Nonogram randomNeighbor = generate_randomNeighbor_rateIt(currentSolution, hints);
        if (currentSolution.rating < randomNeighbor.rating) {
            currentSolution = randomNeighbor;
            currentStop = stop;
        }
        currentStop--;
    }
    return currentSolution;
}

// Algorytm wspinaczkowy deterministyczny, wybór najlepszego sąsiada
Nonogram solve_hillClimbing_deterministic(Vector2d hints, int stop) {
    Nonogram currentSolution = generate_randomSolution_rateIt(hints);

    int currentStop = stop;
    while(currentStop > 0 && currentSolution.rating < 0) {
        std::vector<Nonogram> allNeighbors = generate_allNeighbors(currentSolution);

        for (auto n : allNeighbors) {
            n.rating = rateNonogramBasingOnHints(n, hints);
            if (n.rating > currentSolution.rating) {
                currentSolution = n;
                currentStop = stop;
            }
        }
        currentStop--;
    }
    return currentSolution;
}

Nonogram solve_tabu(Vector2d hints, int stop, int tabuSize) {
    Nonogram currentSolution = generate_randomSolution_rateIt(hints);
    Nonogram bestSolution = currentSolution;
    std::cout << "Starting pont, Rating: " << bestSolution.rating << std::endl;
    printVector(bestSolution.board);

    std::list<Nonogram> tabuList;
    std::list<Nonogram> history;

    int currentStop = stop;
    while(currentStop > 0 && currentSolution.rating < 0) {
        std::vector<Nonogram> allNeighbors = generate_allNeighbors(currentSolution);
        Nonogram bestNeighbor = bestSolution;

        for (auto n : allNeighbors) {
            if (isInList(n, tabuList)) {
//                printVector(n.board);
//                std::cout << std::endl;
//                std::cout << "Continue" << std::endl;
                continue; // Nie chcemy rozwiązań, które są na liście tabu
            }
//            std::cout << "Not in tabu" << std::endl;
            n.rating = rateNonogramBasingOnHints(n, hints);
            if (n.rating > bestNeighbor.rating) {
//                std::cout << "Yes" << std::endl;
                bestNeighbor = n;
            }
        }

        if (bestNeighbor.rating > bestSolution.rating) {
            bestSolution = bestNeighbor;
//            std::cout << "Found better solution" << std::endl;
//            currentStop = stop;
        }

        // Cofanie
        // Jeżeli obecny najlepszy sąsiad jest gorszy od obecnego rozwiązania, cofnij
        if (bestNeighbor.rating <= currentSolution.rating && !history.empty()) {
            currentSolution = history.back();
            history.pop_back();
        } else {
            // Jeżeli lepszy to ustaw i dodaj do historii
            currentSolution = bestNeighbor;
            history.push_back(currentSolution);
        }
//    currentSolution = bestNeighbor;
    // Dodanie rozwiązanie do tabu
        if (!isInList(bestSolution, tabuList)) {
            tabuList.push_back(currentSolution);
//            std::cout << "Adding to taboo list\n";
        }
//        } else {
//            std::cout << "Not added to taboo list\n";
//        }
        if (tabuList.size() > tabuSize) {
            tabuList.pop_front(); // Usuwamy jak przekracza rozmiar listy
        }
        currentStop--;
    }
    return bestSolution;
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
    Nonogram solution = solve_hillClimbing_random(hints, 1000);
    printVector(solution.board);
    std::cout << std::endl << "Best found solution: " << solution.rating << std::endl;
}

void test_hillClimbDeterministic1() {
    Vector2d hints = getHints();
    Nonogram nonogram = generate_randomSolution_rateIt(hints);
    std::vector<Nonogram> nonograms = generate_allNeighbors(nonogram);

    std::cout << "Pattern" << std::endl;
    printVector(nonogram.board);
    for (auto n : nonograms) {
        compareNonograms(nonogram, n);
        std::cout << std::endl;
    }

//    Nonogram solution = solve_hillClimbing_deterministic(hints, 100);
}

Vector2d test_hillClimbDeterministic2() {
    Vector2d hints = getHints();
    Nonogram nonogram = solve_hillClimbing_deterministic(hints, 1000);
    printVector(nonogram.board);
    std::cout << "Rating: " << nonogram.rating << std::endl;

    return hints;
}

void test_biggerNonogram() {
   Nonogram nonogram = generate_randomNonogram(50,10);
    printVector(nonogram.board);
//   Nonogram zero = generate_zeroNonogram(12,10);

   Vector2d hints = generate_hintsFromNonogram(nonogram);
   std::cout << "X: " << hints.x.size() << std::endl;
   printVector(hints.x);
   std::cout << "Koniec" << std::endl;

   std::cout << "Y: " << hints.y.size() << std::endl;
   printVector(hints.y);
   std::cout << "Koniec" << std::endl;

//   zero.rating = rateNonogramBasingOnHints(zero, hints);

//   Nonogram nonogram1 = solve_hillClimbing_random(hints, 100);
//   Nonogram nonogram1 = solve_hillClimbing_deterministic(hints, 100);
//   printVector(nonogram1.board);
//   std::cout << "Rating: " << nonogram1.rating << std::endl;
//   std::cout << "Zero: " << zero.rating << std::endl;
}

void test5() {
    Nonogram nonogram = generate_randomNonogram(5,10);
    Nonogram nonogram1 = generate_randomNonogram(5,10);
    Vector2d hints = generate_hintsFromNonogram(nonogram);
//    Nonogram solution = solve_hillClimbing_random(hints, 10);
    printVector(nonogram.board);
    printVector(hints.x);
    std::cout << std::endl;
    printVector(hints.y);

    nonogram.rating = rateNonogramBasingOnHints(nonogram1, hints);
    std::cout << "Rating: " << nonogram.rating << std::endl;
}

void test6() {
    Nonogram answer = generate_puzzleManually();
    Nonogram solution = solve_fullReview(getHints());

//    printVector(answer.board);
//    compareNonograms(answer, solution);

    printVector(solution.board);
}

void test_tabu() {
    Nonogram nonogram = solve_tabu(getHints(), 500, 100000);
    printVector(nonogram.board);
    nonogram.printRating();
}

void test_tabu_big() {
    Nonogram input = generate_randomNonogram(10,5);
    Vector2d hints = generate_hintsFromNonogram(input);

    Nonogram nonogram = solve_tabu(hints, 500, 100);
    std::cout << "\n";

    std::cout << "Input\n";
    input.printBoard();
    input.printRating();
    std::cout << "Proposed Solution\n";
    nonogram.printBoard();
    nonogram.printRating();

    std::cout << "Worst rating: " << rateNonogramBasingOnHints(generate_zeroNonogram(10,5), hints) << std::endl;

}

void test_comparing() {
    Nonogram nonogram1 = generate_puzzleManually();
    Nonogram nonogram2 = nonogram1;
    std::list<Nonogram> list;
    list.push_back(nonogram2);
    if(nonogram1.board == nonogram2.board) {
        std::cout << "yes";
    }
    for (auto n : list) {
        if(isInList(n, list)) {
            std::cout << "x2yes";
        }
    }
}
int main() {
//    test1();
//    test2();
//    test3();
//    test_hillClimbRandom();
//    test_hillClimbDeterministic1();
//    test_hillClimbDeterministic2();
//    test_biggerNonogram();
//    test5();
//    test6();
//    test_tabu();
    test_tabu_big();
//test_comparing();



    return 0;

}
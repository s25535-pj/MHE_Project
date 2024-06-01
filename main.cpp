#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "scoreFunctions.cpp"
#include "Nonogram.cpp"

std::random_device rd;
std::mt19937 randomGenerator(rd());

// Szukaj numeru w wektorze
bool numberFoundInVector(std::vector<int> first, std::vector<int> second) {
    for (auto number : second) {
        if (std::find(first.begin(), first.end(), number) != first.end()) {
            return true;
        }
    }
    return false;
}

// Oceniaj poprzez sprawdzanie czy ilość cyfr w komórce problemu i proponowanego rozwiązania się zgadza
int rateBasingOnNumbersCount(std::vector<std::vector<int>> problem, std::vector<std::vector<int>> solution) {
    int rating = 0;
    int nonogramSize = (int)problem.size();

    for (int i = 0; i < nonogramSize; i++) {
        for (int j = 0; j < problem[i].size(); j++) {
            // Różnica ilości cyfr w vektorach
            switch (std::abs((int)(problem[i].size() - solution[i].size())) ) {
                case 0:
                    rating += 1;
                    break;
                case 1:
                    rating -= 1;
                    break;
                case 2:
                    rating -= 2;
                    break;
                case 3:
                    rating -= 3;
                    break;
                case 4:
                    rating -= 4;
                    break;
                default:
                    rating -= 10;
                    break;
            }
        }
    }
    return rating;
}

// Oceniaj poprzez Sumowanie cyfr problemu i proponowanego rozwiązania na dany wiersz
int rateBasingOnSumOfNumbersValues(std::vector<std::vector<int>> problem, std::vector<std::vector<int>> solution) {
    int rating = 0;
    int problemSum = 0;
    int solutionSum = 0;
    int nonogramSize = (int)problem.size();

    for (int i = 0; i < nonogramSize; i++) {
        for (int j = 0; j < problem[i].size(); j++) {
            problemSum += problem[i][j];
        }
        for (int j = 0; j < solution[i].size(); j++) {
            solutionSum += solution[i][j];
        }

        switch (std::abs(problemSum - solutionSum)) {
            case 0:
                rating += 1;
                break;
            case 1:
                rating -= 2;
                break;
            case 2:
                rating -= 4;
                break;
            case 3:
                rating -= 6;
                break;
            case 4:
                rating -= 8;
                break;
            default:
                rating -= 10;
                break;
        }
        solutionSum = 0;
        problemSum = 0;
    }
    return rating;
}

// Oceniaj poprzez sprawdzanie czy cyfry z proponowanego rozwiązania znajdują się w problemie
int rateBasingOnIfNumbersAreIncluded(std::vector<std::vector<int>> problem, std::vector<std::vector<int>> solution) {
    int rating = 0;
    int nonogramSize = (int)problem.size();

    for (int i = 0; i < nonogramSize; i++) {
        for (int j = 0; j < solution[i].size(); j++) {
            if (numberFoundInVector(problem[i], solution[i])) {
                problem[i].erase(problem[i].begin() + j);
                rating += 1;
            } else {
                rating -= 1;
            }
        }
    }
//    printVector(problem);
//    printVector(solution);
    return rating;
}

// Oceniaj poprzez sprawdzanie czy cyfra na zadanym indexie jest dobra w danej komórce
int rateBasingOnNumbersOrder(std::vector<std::vector<int>> problem, std::vector<std::vector<int>> solution) {
    int rating = 0;
    int nonogramSize = (int)problem.size();

    for (int i = 0; i < nonogramSize; i++) {
        size_t size;
        if (problem[i].size() >= solution[i].size()) {
            size = solution[i].size();
        } else {
            size = problem[i].size();
        }
        for (int j = 0; j < size; j++) {
            if (problem[i][j] == solution[i][j]) {
                rating += 3;
            } else {
                rating -= 3;
            }
        }
    }
    return rating;
}

// Oceń rozwiązanie wszytkimi meteodami
//int getTotalRatingOfProposedSolution(std::vector<std::vector<int>> problemX, std::vector<std::vector<int>> problemY,
//                                     std::vector<std::vector<int>> proposedSolutionX, std::vector<std::vector<int>> proposedSolutionY) {
//    int totalRating = 0;
//    totalRating += rateBasingOnNumbersCount(problemX, proposedSolutionX, nonogramSize);
//    totalRating += rateBasingOnSumOfNumbersValues(problemX, proposedSolutionX, nonogramSize);
//    totalRating += rateBasingOnIfNumbersAreIncluded(problemX, proposedSolutionX);
//    totalRating += rateBasingOnNumbersOrder(problemX, proposedSolutionX);
//
//    totalRating += rateBasingOnNumbersCount(problemY, proposedSolutionY);
//    totalRating += rateBasingOnSumOfNumbersValues(problemY, proposedSolutionY);
//    totalRating += rateBasingOnIfNumbersAreIncluded(problemY, proposedSolutionY);
//    totalRating += rateBasingOnNumbersOrder(problemY, proposedSolutionY);
//
//    return totalRating;
//}

//// Wygeneruj planszę gry
//std::vector<std::vector<int>> generatePuzzleManually() {
//    std::vector<std::vector<int>> puzzle = {
//            {1, 1, 0, 1, 1},
//            {1, 0, 1, 0, 0},
//            {1, 0, 0, 0, 1},
//            {0, 1, 1, 1, 0},
//            {0, 0, 0, 1, 0}
//    };
//
//    return puzzle;
//}
// Wygeneruj losową planszę gry
std::vector<std::vector<int>> generateRandomPuzzle(int nonogramSize) {
    std::vector<std::vector<int>> puzzle(nonogramSize, std::vector<int>(nonogramSize));
    std::uniform_int_distribution<int> value(0,1);

    for (int i = 0; i < nonogramSize; i++) {
        for (int j = 0; j < nonogramSize; j++) {
            puzzle[i][j] = value(randomGenerator);
        }
    }

    return puzzle;
}
// Wygeneruj zadanie do gry dla kolumn X lub Y
std::vector<std::vector<int>> getProblem(int isX = true) {
    std::vector<std::vector<int>> problem;
    if (isX) {
        problem = {
//                {2,2},
//                {1,1},
//                {1,1},
//                {3},
//                {1}
//                {1,1},
//                {2,1},
//                {2},
//                {1,1},

                {1,1},
                {2},
                {1}
        };
    } else {
        problem = {
//                {3},
//                {1,1},
//                {1,1},
//                {1,2},
//                {1,1}

//                {2,1},{2},{1},{2,1}

                {1,1},{1},{2}
        };
    }
    return problem;
}
// Wydrukuj zawartość vektora
void printVector(std::vector<std::vector<int>> vector) {

    std::cout << "===========" << std::endl;
    for (const auto& v : vector) {
        for (int value : v) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "===========" << std::endl;
}
void printAll(std::vector<std::vector<int>> puzzle, std::vector<std::vector<int>> problemX, std::vector<std::vector<int>> problemY) {
    for (int i = 0; i < problemY.size(); i++) {
        for (int j = 0; j < problemY[i].size(); j++) {
            std::cout << problemY[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "===========" << std::endl;
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle[i].size(); j++) {
            std::cout << puzzle[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "===========" << std::endl;

    for (int i = 0; i < problemX.size(); i++) {
        for (int j = 0; j < problemX[i].size(); j++) {
            std::cout << problemX[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// Przeczytaj wygenerowane rozwiązanie i zwróć vektor który można porównać z problemem
std::vector<std::vector<int>> readGeneratedPuzzle(std::vector<std::vector<int>> puzzle, int ifX = true) {
    int blockLength = 0;
    std::vector<std::vector<int>> readedProposedSolution;
    std::vector<int> readedValues;

    if (ifX) {
        for (int i = 0; i < puzzle.size(); i++) {
            for (int j = 0; j < puzzle[i].size(); j++) {
//                std::cout << vector[i][j];
                if (puzzle[i][j] == 1) {
                    blockLength++;
                } else {
                    if (blockLength > 0) {
                        readedValues.push_back(blockLength);
                    }
                    blockLength = 0;
                }
            }
            // Dodawanie wartości na końcu iteracji jeśli nie jest zerem
            if (blockLength > 0) {
                readedValues.push_back(blockLength);
            }

            blockLength = 0;
            readedProposedSolution.push_back(readedValues);
            readedValues.clear();
//            std::cout << std::endl;
        }
    } else {
        for (int i = 0; i < puzzle.size(); i++) {
            for (int j = 0; j < puzzle[i].size(); j++) {
//                std::cout << vector[i][j];
                if (puzzle[j][i] != 0) {
                    blockLength++;
                } else {
                    if (blockLength > 0) {
                        readedValues.push_back(blockLength);
                    }
                    blockLength = 0;
                }
            }
            // Dodawanie wartości na końcu iteracji jeśli nie jest zerem
            if (blockLength > 0) {
                readedValues.push_back(blockLength);
            }
            blockLength = 0;
            readedProposedSolution.push_back(readedValues);
            readedValues.clear();
//            std::cout << std::endl;
        }
    }
    return readedProposedSolution;
}



void solveFullReview(int nonogramSize, std::vector<Nonogram> *allNonogramsList) {

    // Info do kompilatora że to będzie Unsinged long o rozmiarze nonogramu
    unsigned long long totalCobinations = 1ULL << (nonogramSize * nonogramSize);

    // nonogram 5x5 = 2^25 = 33,554,443 kombinacji. Unsinged long long
    for (unsigned long long cobination = 0; cobination < totalCobinations; ++cobination) {
        std::vector<std::vector<int>> currentNonogram(nonogramSize, std::vector<int>(nonogramSize));

        unsigned long long currentCombination = cobination;
        for (int i = 0; i < nonogramSize * nonogramSize; i++) {
            int row = i / nonogramSize; // int zaokrągla w dół
            int col = i % nonogramSize;

            currentNonogram[row][col] = currentCombination & 1; // przepisz skrajnie prawy bit do komórki nonogramu.
            currentCombination >>= 1; // przesuń wszytkie bity w prawo.

            Nonogram nonogram(currentNonogram);
            allNonogramsList->push_back(nonogram);
        }
    }
}

void compareNonograms(std::vector<std::vector<int>> comparedNonogram, std::vector<std::vector<int>> patternNonogram) {
    std::cout << "===========" << std::endl;
    for (int i = 0; i < comparedNonogram.size(); i++) {
        for (int j = 0; j < comparedNonogram[i].size(); j++) {
            if (comparedNonogram[i][j] != patternNonogram[i][j]) {
                std::cout << "\033[1;31m" << comparedNonogram[i][j] << " ";
            } else {
                std::cout << "\033[0m" << comparedNonogram[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\033[0m" << "===========" << std::endl;
}

bool comparisonFunction(Nonogram a, Nonogram& b) {
    return a.ratingTotal() < b.ratingTotal();
}

int main() {

    int nonogramSize = 3;

    std::vector<std::vector<int>> puzzleAnswer = {
//            {1, 1, 0, 1, 1},
//            {1, 0, 1, 0, 0},
//            {1, 0, 0, 0, 1},
//            {0, 1, 1, 1, 0},
//            {0, 0, 0, 1, 0}

            {1,0,1},
            {0,1,1},
            {1,0,0}



    };

//    std::vector<std::vector<int>> puzzleSolution = {
//            {1, 1, 0, 1, 1},
//            {1, 0, 1, 0, 0},
//            {1, 0, 0, 0, 1},
//            {0, 1, 1, 1, 0},
//            {0, 0, 0, 1, 0}
//    };

    auto problemX = getProblem(true);
    auto problemY = getProblem(false);

    auto puzzle = generateRandomPuzzle(nonogramSize);

    auto soulutionX = readGeneratedPuzzle(puzzle, true);
    auto soulutionY = readGeneratedPuzzle(puzzle, false);

    Nonogram nonogram(puzzle);

    nonogram.ratingBasedOnNumbersCount += rateBasingOnNumbersCount(problemX, soulutionX);
    nonogram.ratingBasedOnNumbersCount += rateBasingOnNumbersCount(problemY, soulutionY);

    nonogram.ratingBasedOnSumOfNumbersValues += rateBasingOnSumOfNumbersValues(problemX, soulutionX);
    nonogram.ratingBasedOnSumOfNumbersValues += rateBasingOnSumOfNumbersValues(problemY, soulutionY);

    nonogram.ratingBasedOnIfNumbersAreIncluded += rateBasingOnIfNumbersAreIncluded(problemX, soulutionX);
    nonogram.ratingBasedOnIfNumbersAreIncluded += rateBasingOnIfNumbersAreIncluded(problemY, soulutionY);

    nonogram.ratingBasedOnNumbersOrder += rateBasingOnNumbersOrder(problemX, soulutionX);
    nonogram.ratingBasedOnNumbersOrder += rateBasingOnNumbersOrder(problemY, soulutionY);


//    printVector(puzzle);
//    printVector(puzzleSolution);
    compareNonograms(puzzle, puzzleAnswer);

//    std::cout << "Rating ratingBasedOnNumbersCount: " << nonogram.ratingBasedOnNumbersCount << std::endl;
//    std::cout << "Rating ratingBasedOnSumOfNumbersValues: " << nonogram.ratingBasedOnSumOfNumbersValues << std::endl;
//    std::cout << "Rating ratingBasedOnIfNumbersAreIncluded: " << nonogram.ratingBasedOnIfNumbersAreIncluded << std::endl;
//    std::cout << "Rating ratingBasedOnNumbersOrder: " << nonogram.ratingBasedOnNumbersOrder << std::endl;
//    std::cout << "Rating ratingTotal: " << nonogram.ratingTotal() << std::endl;

/*
    std::vector<Nonogram> allNonogramsList;

    solveFullReview(nonogramSize, &allNonogramsList);

    double rating = 0;

    for (auto& nonogram : allNonogramsList) {
        double currentRating = getTotalRatingOfProposedSolution(problemX, problemY, readGeneratedPuzzle(nonogram.proposedSolution, true),readGeneratedPuzzle(nonogram.proposedSolution, false));
        nonogram.ratingTotal = currentRating;
//        if (currentRating > rating) {
//            rating = currentRating;
//        }
    }

    std::sort(allNonogramsList.begin(), allNonogramsList.end(), comparisonFunction);

    for (auto& nonogram : allNonogramsList) {
        std::cout << nonogram.ratingTotal << std::endl;
    }

    std::cout << "Best Rating: "<< rating << std::endl;

    int size = allNonogramsList.size();
    for (int i = size-10; i < size; i++) {
        compareNonograms(allNonogramsList[i].proposedSolution, allNonogramsList[size-1].proposedSolution);
        std::cout << "Rating: " << allNonogramsList[i].ratingTotal << std::endl;
    }
*/
    return 0;
}

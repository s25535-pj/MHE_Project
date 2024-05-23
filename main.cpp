#include <iostream>
#include <vector>
#include <random>
#include <algorithm>


std::random_device rd;
std::mt19937 randomGenerator(rd());
// Wygeneruj planszę gry
std::vector<std::vector<int>> generateManualPuzzle() {
    std::vector<std::vector<int>> puzzle = {
            {1, 1, 0, 1, 1},
            {1, 0, 1, 0, 0},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1, 0},
            {0, 0, 0, 1, 0}
    };

    return puzzle;
}
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
                {1,1},
                {2,1},
                {2},
                {1,1},
        };
    } else {
        problem = {
                {2,1},{2},{1},{2,1}
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
// Sukaj numeru w wektorze
bool numberFoundInVector(std::vector<int> first, std::vector<int> second) {
    for (auto number : second) {
        if (std::find(first.begin(), first.end(), number) != first.end()) {
            return true;
        }
    }
    return false;
}
// Przeczytaj wygenerowane rozwiązanie i zwróć vektor który można porównać z problemem
std::vector<std::vector<int>> readGeneratedPuzzle(std::vector<std::vector<int>> vector, int ifX = true) {
    int blockLength = 0;
    std::vector<std::vector<int>> readedProposedSolution;
    std::vector<int> readedValues;

    if (ifX) {
        for (int i = 0; i < vector.size(); i++) {
            for (int j = 0; j < vector[i].size(); j++) {
//                std::cout << vector[i][j];
                if (vector[i][j] == 1) {
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
        for (int i = 0; i < vector.size(); i++) {
            for (int j = 0; j < vector[i].size(); j++) {
//                std::cout << vector[i][j];
                if (vector[j][i] != 0) {
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

// Oceniaj poprzez sprawdzanie czy ilość cyfr w komórce problemu i proponowanego rozwiązania się zgadza
double rateBasingOnNumbersCount(std::vector<std::vector<int>> problem, std::vector<std::vector<int>> solution) {
    double rating = 0;
    for (int i = 0; i < problem.size(); i++) {
        for (int j = 0; j < problem[i].size(); j++) {
            switch (std::abs((int)(problem[i].size() - solution[i].size())) ) {
                case 0:
                    rating += 5;
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
                    rating -= 5;
                    break;
            }
        }
    }
    return rating;
}

// Oceniaj poprzez Sumowanie cyfr problemu i proponowanego rozwiązania na dany wiersz
double rateBasingOnSumOfNumbersValues(std::vector<std::vector<int>> problem, std::vector<std::vector<int>> solution) {
    double rating = 0;
    int problemSum = 0;
    int solutionSum = 0;

    for (int i = 0; i < problem.size(); i++) {
        for (int j = 0; j < problem[i].size(); j++) {
            problemSum += problem[i][j];
        }
        for (int j = 0; j < solution[i].size(); j++) {
            solutionSum += solution[i][j];
        }

        switch (std::abs(problemSum - solutionSum)) {
            case 0:
                rating += 5;
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
                rating -= 5;
                break;
        }
        solutionSum = 0;
        problemSum = 0;
    }
    return rating;
}

// Oceniaj poprzez sprawdzanie czy cyfry z proponowanego rozwiązania znajdują się w problemie
double rateBasingOnIfNumbersAreIncluded(std::vector<std::vector<int>> problem, std::vector<std::vector<int>> solution) {
    double rating = 0;

    for (int i = 0; i < solution.size(); i++) {
        for (int j = 0; j < solution[i].size(); j++) {
            if (numberFoundInVector(problem[i], solution[i])) {
                problem[i].erase(problem[i].begin() + j);
                rating += 1;
            } else {
                rating -= 2;
            }
        }
    }
//    printVector(problem);
//    printVector(solution);
    return rating;
}

// Oceniaj poprzez sprawdzanie czy cyfra na zadanym indexie jest dobra w danej komórce
double rateBasingOnNumbersOrder(std::vector<std::vector<int>> problem, std::vector<std::vector<int>> solution) {
    double rating = 0;

    for (int i = 0; i < problem.size(); i++) {
        size_t size = 0;
        if (problem[i].size() >= solution[i].size()) {
            size = solution[i].size();
        } else {
            size = problem[i].size();
        }
        for (int j = 0; j < size; j++) {
            if (problem[i][j] == solution[i][j]) {
                rating += 1;
            } else {
                rating -= 1;
            }
        }
    }
    return rating;
}

// Oceń rozwiązanie wszytkimi meteodami
double getTotalRatingOfProposedSolution(std::vector<std::vector<int>> problemX, std::vector<std::vector<int>> problemY,
                                        std::vector<std::vector<int>> proposedSolutionX, std::vector<std::vector<int>> proposedSolutionY) {
    double totalRating = 0;
    totalRating += rateBasingOnNumbersCount(problemX, proposedSolutionX);
    totalRating += rateBasingOnSumOfNumbersValues(problemX, proposedSolutionX);
    totalRating += rateBasingOnIfNumbersAreIncluded(problemX, proposedSolutionX);
    totalRating += rateBasingOnNumbersOrder(problemX, proposedSolutionX);

    totalRating += rateBasingOnNumbersCount(problemY, proposedSolutionY);
    totalRating += rateBasingOnSumOfNumbersValues(problemY, proposedSolutionY);
    totalRating += rateBasingOnIfNumbersAreIncluded(problemY, proposedSolutionY);
    totalRating += rateBasingOnNumbersOrder(problemY, proposedSolutionY);

    return totalRating;
}


auto solveFullReview(int nonogramSize) {
    std::vector<std::vector<std::vector<int>>> nonograms;

    unsigned long long totalConfigurations = 1ULL << (nonogramSize * nonogramSize);

    for (unsigned long long config = 0; config < totalConfigurations; ++config) {
        std::vector<std::vector<int>> nonogram(nonogramSize, std::vector<int>(nonogramSize));

        unsigned long long currentConfig = config;
        for (int i = 0; i < nonogramSize * nonogramSize; i++) {
            int row = i / nonogramSize;
            int col = i % nonogramSize;

            nonogram[row][col] = currentConfig & 1;
            currentConfig >>= 1;
        }

        nonograms.push_back(nonogram);
    }
    return nonograms;
}
int main() {

    int nonogramSize = 4;

    auto problemX = getProblem(true);
    auto problemY = getProblem(false);
//    auto generatedPuzzle = generateManualPuzzle();
//    auto generatedPuzzle = generateRandomPuzzle(nonogramSize);

//    auto proposedSolutionX = readGeneratedPuzzle(generatedPuzzle, true);
//    auto proposedSolutionY = readGeneratedPuzzle(generatedPuzzle, false);
//
//    if (proposedSolutionX == problemX) {
//        std::cout << "PerfectX"<< std::endl;
//    }
//    if (proposedSolutionY == problemY) {
//        std::cout << "PerfectY"<< std::endl;
//    }

//    printVector(generatedPuzzle);
//    printAll(generatedPuzzle, proposedSolutionX, proposedSolutionY);

    auto nonograms = solveFullReview(nonogramSize);
    double rating = 0;
    std::vector<std::vector<int>> bestNonogram, bestNonogram2, bestNonogram3, bestNonogram4 ,bestNonogram5;
    std::vector<double> scoreBoard;
    for (auto n : nonograms) {
        double currentRating = getTotalRatingOfProposedSolution(problemX, problemY, readGeneratedPuzzle(n, true),readGeneratedPuzzle(n, false));
//        std::cout << currentRating << std::endl;
        scoreBoard.push_back(currentRating);
        if (currentRating > rating) {
            rating = currentRating;
            bestNonogram5 = bestNonogram4;
            bestNonogram4 = bestNonogram3;
            bestNonogram3 = bestNonogram2;
            bestNonogram2 = bestNonogram;
            bestNonogram = n;
        }
    }
    std::cout << "Sorting" << std::endl;
    std::sort(scoreBoard.begin(), scoreBoard.end());
    for (auto score : scoreBoard) {
        std::cout << score << std::endl;
    }

    std::cout << "Best Rating: "<< rating << std::endl;
    printVector(bestNonogram5);
    printVector(bestNonogram4);
    printVector(bestNonogram3);
    printVector(bestNonogram2);
    printVector(bestNonogram);

//    std::cout << "Total rating: " << getTotalRatingOfProposedSolution(problemX, problemY, proposedSolutionX, proposedSolutionY) << std::endl;

    return 0;
}

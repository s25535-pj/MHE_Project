#include <iostream>
#include <vector>

// Wygeneruj planszę gry
std::vector<std::vector<int>> generateManualSolution() {
    std::vector<std::vector<int>> generatedSolution = {
            {1, 1, 0, 1, 1},
            {1, 0, 1, 0, 0},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1, 0},
            {0, 0, 0, 1, 0}
    };

    return generatedSolution;
}

// Wygeneruj zadanie do gry dla kolumn X lub Y
std::vector<std::vector<int>> getProblem(int isX = true) {
    std::vector<std::vector<int>> problem;
    if (isX) {
        problem = {
                {2, 2},
                {1, 1},
                {1, 1},
                {3},
                {1},
        };
    } else {
        problem = {
                {3},{1,1},{1,1},{1,2},{1,1}
        };
    }
    return problem;
}

// Wydrukuj zawartość vektora
void printVector(std::vector<std::vector<int>> const &vector) {
    for (const auto& v : vector) {
        for (int value : v) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

// Przeczytaj wygenerowane rozwiązanie i zwróć vektor który można porównać z zadaniem
std::vector<std::vector<int>> readGeneratedSolution(std::vector<std::vector<int>> vector, int ifX = true) {
    int blockLength = 0;
    std::vector<std::vector<int>> readedProposedSolution;
    std::vector<int> readedValues;

    if (ifX) {
        for (int i = 0; i < vector.size(); ++i) {
            for (int j = 0; j < vector[i].size(); ++j) {
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
        for (int i = 0; i < vector.size(); ++i) {
            for (int j = 0; j < vector[i].size(); ++j) {
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

//void rateSolution(std::vector<std::vector<int>> problem, std::vector<std::vector<int>> readedSolution) {
//
//}

int main() {

    auto problemX = getProblem(true);
    auto problemY = getProblem(false);
    auto generatedSolution = generateManualSolution();

    auto readedGeneratedSolutionX = readGeneratedSolution(generatedSolution, true);
    auto readedGeneratedSolutionY = readGeneratedSolution(generatedSolution, false);

//    printVector(readedGeneratedSolutionX);
//    std::cout << std::endl;
//    printVector(problemX);
//

    if (readedGeneratedSolutionX == problemX) {
        std::cout << "goodX";
    }
    if (readedGeneratedSolutionY == problemY) {
        std::cout << "goodY";
    }


//    printVector(generatedSolution);
//    printVector(problemX);
//    printVector(problemY);


    return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <list>
#include <fstream>
#include <map>
#include <functional>
#include <chrono>
#include <iomanip>
#include "helperFunctions.h"

int rateNonogramBasingOnHints(const Nonogram& nonogram, const Vector2d& hints) {
    int rating = 0;
    int painted = 0;
    int blockLength = 0;
    std::vector<int> tmp;

    // X
    // Sprawdź wartości na zadanym indexie
    for (int i = 0; i < nonogram.board.size(); i++) {
        for (int j = 0; j < nonogram.board[i].size(); j++) {
            if (nonogram.board[i][j] == 1) {
                // zlicza długość klocków i ile jest pol zamalowanych
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
        rating -= std::abs((int)hints.getXRowSum(i) - painted) * 1; // - za każde źle zamalowane

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

// Wygeneruj losowy nonogram podstawie hintów
Nonogram generate_randomSolution_rateIt(const Vector2d& hints) {
    Nonogram nonogram;
    // Jak nie określałem rozmiaru wektora to miałem SEG faulty
    std::vector<std::vector<int>> board(hints.x.size(), std::vector<int>(hints.y.size()));
    std::uniform_int_distribution<int> value(0,1);

    for (int i = 0; i < hints.x.size(); i++) {
        for (int j = 0; j < hints.y.size(); j++) {
            board[i][j] = value(randomGenerator);
        }
    }
    nonogram.board = board;
    nonogram.rating = rateNonogramBasingOnHints(nonogram, hints);
    return nonogram;
}

// Utwórz nonogram sąsiadujący z dostarczonym przez odwrócenie jednego losowego pola
Nonogram generate_randomNeighbor_rateIt(Nonogram nonogram, const Vector2d& hints) {
    std::uniform_int_distribution<int> value(0,(int)nonogram.board.size()-1);

    int x = value(randomGenerator);
    int y = value(randomGenerator);

    // Jeżeli 0 usaw 1 i odwrotnie, działamy na kopii więc jest spoko
    nonogram.board[x][y] = nonogram.board[x][y] == 0 ? 1 : 0;
    nonogram.rating = rateNonogramBasingOnHints(nonogram, hints);
    return nonogram;
}

// Wygeneruj wszystkich sąsiadów
// Przeleci po dostarczonym nonogramie odwracając po jednym polu tworząc sąsiada i wrzuci na listę sąsiadów
std::vector<Nonogram> generate_allNeighbors(const Nonogram& nonogram) {
    std::vector<Nonogram> allNeighbors;

    for (int i = 0; i < nonogram.board.size(); i++) {
        for (int j = 0; j < nonogram.board[i].size(); j++) {
            Nonogram neighbor = nonogram;
            neighbor.board[i][j] = nonogram.board[i][j] == 0 ? 1 : 0;
            allNeighbors.push_back(neighbor);
        }
    }
    return allNeighbors;
}

// Algorytm pełnego przeglądu
Nonogram solve_fullReview(const Vector2d& hints) {
    Nonogram fake;
    int x = (int)hints.x.size();
    int y = (int)hints.y.size();

    // Wszytkie kombinacje, Dla 3x3 to 2^3*3 = 512, ale liczymy od zera to 511 = 1 1111 1111 binarnie
    auto totalCombinations = (unsigned long long)pow(2,x*y);

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

        // Zrób z tego struct i oceń (zapisz też ocenę) czy przypadkiem to już nie jest rozwiązanie.
        Nonogram nonogram;
        nonogram.board = currentBoard;

        if (rateNonogramBasingOnHints(nonogram, hints) == 0){
            return nonogram;
        }
    }

    std::cout << "You should never see this! It means Nonogram is unsolvable.";
    return fake;
}

// Algorytm wspinaczkowy z losowym sąsiadem
Nonogram solve_hillClimbing_random(const Vector2d& hints, int stop) {
    Nonogram currentSolution = generate_randomSolution_rateIt(hints);
    int currentStop = stop;

    // Jeżeli stop > 0 i nie masz jeszcze rozwiązania
    while(currentStop > 0 && currentSolution.rating < 0) {
        Nonogram randomNeighbor = generate_randomNeighbor_rateIt(currentSolution, hints);

        if (currentSolution.rating < randomNeighbor.rating) {
            currentSolution = randomNeighbor;
            // Jeżeli jest progres, zresetuj stopa
            currentStop = stop ;
        }
        currentStop--;
    }
    return currentSolution;
}

// Algorytm wspinaczkowy deterministyczny, wybór najlepszego sąsiada z wszystkich możliwych
Nonogram solve_hillClimbing_deterministic(const Vector2d& hints, int stop) {
    Nonogram currentSolution = generate_randomSolution_rateIt(hints);

    int currentStop = stop;
    while(currentStop > 0 && currentSolution.rating < 0) {
        // Wygeneruj nowych sąsiadów
        std::vector<Nonogram> allNeighbors = generate_allNeighbors(currentSolution);

        for (auto &neighbor : allNeighbors) {
            neighbor.rating = rateNonogramBasingOnHints(neighbor, hints);
            // Wybierz najlepszego
            if (neighbor.rating > currentSolution.rating) {
                currentSolution = neighbor;
                // Jeżeli jest progres, zresetuj stopa
                currentStop = stop;
            }
        }
        currentStop--;
    }
    return currentSolution;
}
// Algorytm tabu z cofaniem
Nonogram solve_tabu(const Vector2d& hints, int stop, int tabuSize) {
    Nonogram currentSolution = generate_randomSolution_rateIt(hints);
    Nonogram bestSolution = currentSolution;

    std::list<Nonogram> tabuList, history;
    tabuList.push_back(currentSolution);
    history.push_back(currentSolution);

    int currentStop = stop;
    history.push_back(currentSolution);

    // Jeżeli stop > 0 i nie masz jeszcze rozwiązania
    while(currentStop > 0 && currentSolution.rating < 0) {
        std::vector<Nonogram> allNeighbors = generate_allNeighbors(currentSolution);
        Nonogram bestNeighbor = bestSolution;

        // Wybierz najlepszego sąsiada z listy sąsiadów, który nie jest na liście tabu
        for (auto &neighbor : allNeighbors) {
            neighbor.rating = rateNonogramBasingOnHints(neighbor, hints);

            // Nie chcemy rozwiązań, które są na liście tabu, jeśli jest na liście olej iterację
            if (!isInList(neighbor, tabuList) && (neighbor.rating > bestNeighbor.rating)) {
                bestNeighbor = neighbor;
            }
        }

        // Podmień najlepszego sąsiada z najlepszym rozwiązaniem jakie do tej pory się trafiło jeśli jest lepszy
        if (bestNeighbor.rating > bestSolution.rating) {
            bestSolution = bestNeighbor;
            currentStop = stop;
        }

        // Cofanie
        // Jeżeli obecny najlepszy sąsiad jest gorszy od rozwiązania z poprzedniej iteracji, cofnij
        if (bestNeighbor.rating <= currentSolution.rating && !history.empty()) {
            currentSolution = history.back();
            history.pop_back();
        } else {
            // Jeżeli lepszy to ustaw i dodaj do historii
            currentSolution = bestNeighbor;
            history.push_back(currentSolution);
        }

        // Dodanie rozwiązanie do tabu jeśli do tej pory nie było
        if (!isInList(bestSolution, tabuList)) {
            tabuList.push_back(currentSolution);
        }

        // Usuwamy front jak tabu przekracza wyznaczony rozmiar
        if (tabuList.size() > tabuSize) {
            tabuList.pop_front();
        }
        currentStop--;
    }
    return bestSolution;
}

//###################################################################################################


// Oceń populację
void ratePopulation(std::vector<Nonogram>& population, const Vector2d& hints) {
    for (auto& nonogram : population) {
        nonogram.rating = rateNonogramBasingOnHints(nonogram, hints);
    }
}

// Wyznacz elitę, populacja musi być posortowana że najlepsze są na pierwszych indexach
std::vector<Nonogram> selection(std::vector<Nonogram>& population, int elite_size) {
    std::vector<Nonogram> elite(population.begin(), population.begin() + elite_size);
    return elite;
}

// Krzyżowanie (wklejanie części nonogramu od do końca) - metoda jednopunktowa
Nonogram crossover_singlePoint(Nonogram& parent1, Nonogram& parent2) {
    Nonogram child = parent1;
    std::uniform_int_distribution<int> dist(0, (int)parent1.board.size() - 1);
    int crossover_point = dist(randomGenerator);

    for (int i = crossover_point; i < parent1.board.size(); i++) {
        child.board[i] = parent2.board[i];
    }
    return child;
}

// Krzyżowanie (wklejanie części nonogramu od do) - metoda dwupunktowa
Nonogram crossover_twoPoint(Nonogram& parent1, Nonogram& parent2) {
    Nonogram child = parent1;
    std::uniform_int_distribution<int> dist(0, (int)parent1.board.size() - 1);
    int crossover_point1 = dist(randomGenerator);
    int crossover_point2 = dist(randomGenerator);

    if (crossover_point1 > crossover_point2) std::swap(crossover_point1, crossover_point2);

    for (int i = crossover_point1; i < crossover_point2; i++) {
        child.board[i] = parent2.board[i];
    }
    return child;
}



// Mutacja - odwróć wartość na jednym polu
void mutation_flip(Nonogram& nonogram) {
    std::uniform_int_distribution<int> dist(0, (int)nonogram.board.size() - 1);
    int x = dist(randomGenerator);
    int y = dist(randomGenerator);

    nonogram.board[x][y] = nonogram.board[x][y] == 0 ? 1 : 0;
}

// Mutacja - odwróć wartości na całym wierszu
void mutation_flipRow(Nonogram& nonogram) {
    std::uniform_int_distribution<int> dist(0, (int)nonogram.board.size() - 1);
    int x = dist(randomGenerator);

    for (int y = 0; y < nonogram.board[x].size(); y++) {
        nonogram.board[x][y] = nonogram.board[x][y] == 0 ? 1 : 0;
    }
}

// Mutacja - odwróć wartości na całej kolumnie
void mutation_flipColumn(Nonogram& nonogram) {
    std::uniform_int_distribution<int> dist(0, (int)nonogram.board[0].size() - 1);
    int y = dist(randomGenerator);

    for (int x = 0; x < nonogram.board[0].size(); x++) {
        nonogram.board[x][y] = nonogram.board[x][y] == 0 ? 1 : 0;
    }
}

// Algorytm genetyczny
Nonogram solve_geneticAlgorithm(Vector2d hints, int stop, int populationSize, int eliteSize, int mutationRate,
                           bool singlePointCrossover, bool useRowColMutation) {

    // Wygeneruj losowe nonogramy i dodaj do populacji
    std::vector<Nonogram> population;
    for (int i = 0; i < populationSize; i++) {
        population.push_back(generate_randomSolution_rateIt(hints));
    }

    int generation = 0;
    int bestRating = 1;
    // Szukaj dopóki nie znajdziesz lub przelecisz wyznaczoną liczbę pokoleń
    while(generation < stop || bestRating == 0) {

        // Posortuj po rating tak że najlepszy jest pierwszy
        std::sort(population.begin(), population.end(), comparisonFunctionMore);

        // Wybierz elitę, pierwsze ileś z najlepszym rating, dlatego sortujemy
        std::vector<Nonogram> elite = selection(population, eliteSize);

        // Elita się rozmnaża poprzez krzyżowanie, losowy wybór rodziców z elity, mają się rozmnożyć do rozmiaru populacji
        std::uniform_int_distribution<int> dist(0, eliteSize - 1);
        while (elite.size() < populationSize) {
            Nonogram parent1 = population[dist(randomGenerator)];
            Nonogram parent2 = population[dist(randomGenerator)];
            Nonogram child;

            if (singlePointCrossover) {
                 child = crossover_singlePoint(parent1, parent2);
            } else {
                 child = crossover_twoPoint(parent1, parent2);
            }

            elite.push_back(child);
        }

        // Losowa mutacja, która może nie musi zajść, zmiana jednego pola, lub zmiana wiersza/kolumny
        for (auto& nonogram : elite) {
            std::uniform_int_distribution<int> dist2(1, 100/mutationRate);
            if (dist2(randomGenerator) == 1) {
                if (useRowColMutation) {
                    mutation_flip(nonogram);
                } else {
                    // Szansa 50/50 że wiersz lub kolumna
                    std::uniform_int_distribution<int> dist3(0, 1);
                    if (dist3(randomGenerator) == 0)
                        mutation_flipRow(nonogram);
                    else {
                        mutation_flipColumn(nonogram);
                    }
                }
            }
        }

        // Rozmnożona elita to nowa populacja
        population = elite;

        // Oceń populację, populacja się zmienia więc trzeba ją oceniać po każdej iteracji
        ratePopulation(population, hints);
        bestRating = (*std::max_element(population.begin(), population.end(), comparisonFunctionLess)).rating;
        generation++;
    }

    // Zwróć najlepszy nonogram, gwiazda oznacza dereferencję ze wskaźnika na nonogram
    return *std::max_element(population.begin(), population.end(), comparisonFunctionLess);
}

//###################################################################################################


void runProgram(int argc, char **argv) {
    std::vector<std::string> args(argv, argv+argc);
    std::string selected_solver, inputFile;
    int stop, tabu_popultaion, eliteSize, mutationRate, singlePointCrossover, useRowColMutation;

    // Musi być odpowiednia ilość argumentów albo się wywali
    inputFile = args[1];
    selected_solver = args[2];
    stop = stoi(args[3]);
    tabu_popultaion = stoi(args[4]);
    eliteSize = stoi(args[5]);
    mutationRate = stoi(args[6]);
    singlePointCrossover = stoi(args[7]);
    useRowColMutation = stoi(args[8]);

    Vector2d hints = readFile(inputFile);

    // powiąż nazwę z funkcją (Zwraca Nonogram, Przyjmuje Vector 2d hints)
    std::map<std::string, std::function<Nonogram(Vector2d)>> solvers;

    // Przypisywanie funckji do kluczy w mapie. Funkcje lambda. [&] oznacza, że lambda przechwytuje wszystkie zmienne z zasięgu, w którym została zdefiniowana, przez referencję.
    solvers["solve_fullReview"] = [&](auto hints){return solve_fullReview(hints);};
    solvers["solve_hillClimbing_random"] = [&](auto hints){return solve_hillClimbing_random(hints, stop);};
    solvers["solve_hillClimbing_deterministic"] = [&](auto hints){return solve_hillClimbing_deterministic(hints, stop);};
    solvers["solve_tabu"] = [&](auto hints){return solve_tabu(hints, stop, tabu_popultaion);};
    solvers["solve_geneticAlgorithm"] = [&](auto hints){return solve_geneticAlgorithm(hints, stop, tabu_popultaion,eliteSize, mutationRate, singlePointCrossover, useRowColMutation);};

    // Liczenie czasu
    auto start_time = std::chrono::system_clock::now();
    auto result = solvers[selected_solver](hints);
    auto end_time = std::chrono::system_clock::now();
    auto computation_time = std::chrono::nanoseconds(end_time - start_time);

    // Pliki z outputem
    std::ofstream fileStream;
    std::string statisticsFilePath = "statistics.txt";
    std::string solutionFilePath = "solution.txt";

    // Pisanie do pliku w trybie append
    fileStream.open (statisticsFilePath, std::ios_base::app);

    // Wyrównaj do prawej, niech rating zajmuje 4 znaki miejsca
    fileStream << std::setw(35) << std::left  << selected_solver << " Rating: " << std::setw(4) << std::right << result.rating << " Time taken: " << computation_time.count() << std::endl;
    fileStream.close();

    // Pisanie do pliku w trybie append
    fileStream.open (solutionFilePath, std::ios_base::app);
    fileStream << std::setw(35) << std::left  << selected_solver << std::endl << "Rating: " << result.rating << std::endl << result.boardToString() << std::endl;
    fileStream.close();
}


void test() {
    Vector2d hints = readFile("C:/Users/oem/Desktop/Studia/MHE/MHE_Project/10x10.txt");
    Nonogram nonogram = solve_geneticAlgorithm(hints,1000,100,5,100,0,1);
    nonogram.printBoard();
    nonogram.printRating();
}

void test2() {
    Vector2d hints = readFile("C:/Users/oem/Desktop/Studia/MHE/MHE_Project/10x10.txt");
    Nonogram parent1 = generate_randomSolution_rateIt(hints);
    Nonogram parent2 = generate_randomSolution_rateIt(hints);

    Nonogram child = crossover_twoPoint(parent1, parent2);

    child.printBoard();
    std::cout << std::endl;
    compareNonograms(child, parent1);
    std::cout << std::endl;
    compareNonograms(child, parent2);
}

int main(int argc, char **argv) {

    test2();
//    runProgram(argc, argv);



    return 0;

}
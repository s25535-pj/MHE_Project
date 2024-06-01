#include <vector>

class Nonogram {
public:
    int ratingBasedOnNumbersCount = 0;
    int ratingBasedOnIfNumbersAreIncluded = 0;
    int ratingBasedOnSumOfNumbersValues = 0;
    int ratingBasedOnNumbersOrder = 0;

    std::vector<std::vector<int>> proposedSolution;

    Nonogram(std::vector<std::vector<int>> proposedSolution): proposedSolution(proposedSolution) {}
    int ratingTotal() {
        return ratingBasedOnNumbersCount + ratingBasedOnIfNumbersAreIncluded + ratingBasedOnSumOfNumbersValues + ratingBasedOnNumbersOrder;
    }
};
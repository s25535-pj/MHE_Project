
FILE="C:/Users/Art/Desktop/Studia/MHE/MHE_Project/3x3.txt"
STOP=1000
TABU_SIZE=1000

GENERATIONS=$STOP
POPULATION_SIZE=100
ELITE_SIZE=10
MUTATION_RATE=100
SINGLE_POINT_CROSSOVER=0
USE_ROW_COL_MUTATION=1

echo "solve_fullReview"
cmake-build-debug/MHE_Project.exe $FILE "solve_fullReview" 0 0 0 0 0 0
#echo "solve_hillClimbing_random"
#cmake-build-debug/MHE_Project.exe $FILE solve_hillClimbing_random $STOP 0 0 0 0 0
#echo "solve_hillClimbing_deterministic"
#cmake-build-debug/MHE_Project.exe $FILE solve_hillClimbing_deterministic $STOP 0 0 0 0 0
#echo "solve_tabu"
#cmake-build-debug/MHE_Project.exe $FILE solve_tabu $STOP $TABU_SIZE 0 0 0 0 0
#echo "solve_geneticAlgorithm"
#cmake-build-debug/MHE_Project.exe $FILE solve_geneticAlgorithm $GENERATIONS $POPULATION_SIZE $ELITE_SIZE $MUTATION_RATE $SINGLE_POINT_CROSSOVER $USE_ROW_COL_MUTATION
echo "====================================================================================" >> statistics.txt
echo "All done"

#for METHOD in solve_fullReview solve_hillClimbing_random solve_hillClimbing_deterministic; do
#  echo "Starting: ${METHOD}"
#  cmake-build-debug/MHE_Project.exe $FILE $METHOD
#  echo "Finished: ${METHOD}"
#done

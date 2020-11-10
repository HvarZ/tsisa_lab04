
#include "genetic_algorithm.hpp"




int main() {
    auto population = fill_population(0, 2, -2, 2);
    std::sort(population.begin(), population.end());
    print_results(population);
    return 0;
}

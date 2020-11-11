#ifndef TSISA_RK2_GENETIC_ALGORITHM_HPP
#define TSISA_RK2_GENETIC_ALGORITHM_HPP

#include <utility>
#include <stdexcept>
#include <random>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>


auto function(const double x, const double y) -> double {
    return pow(sin(x),2) / (1 + pow(x, 2) + pow(y, 2));
}

struct Point {
    double x;
    double y;
    double fit;

    Point() = default;

    Point(const double x1, const double y1) {
        x = x1;
        y = y1;
        fit = function(x, y);
    }

    auto operator<(const Point& point) const noexcept -> bool {
        return fit < point.fit;
    }
};

auto random(const double a, const double b) -> double {
    if (a >= b) throw std::logic_error("Invalid argument");
    std::random_device rd;
    std::mt19937_64 rng(rd());
    std::uniform_real_distribution<double> rand(a, b);
    return rand(rng);
}

auto random_point(const double x1, const double x2, const double y1, const double y2) -> Point {
    return Point(random(x1, x2), random(y1, y2));
}

auto fill_population(const double x1, const double x2, const double y1, const double y2) -> std::vector<Point> {
    const size_t number_of_points = 4;
    std::vector<Point> result(0);
    for (size_t i = 0; i < number_of_points; i++) {
        result.push_back(random_point(x1, x2, y1, y2));
    }
    return result;
}

void print_results(const std::vector<Point>& points) {
    for (const auto& i : points) {
        std::cout << "x:" << i.x << "\ty:" << i.y << "\tfit:" << i.fit << std::endl;
     }
}

void genetic_algorithm() {
    const double probability = 0.3;
    const size_t number_generation = 10;
    auto population = fill_population(0, 2, -2, 2);
    std::vector<Point> children(12);

    for (size_t i = 0; i < number_generation; i++) {
        size_t count = 0;
        for (size_t j = 0; j < 4; j++) { // crossover
            for (size_t k = 0; k < 4; k++) {
                if (j == k) continue;
                children[count] = Point(population[j].x, population[k].y);
                count++;
            }
        }
        for (size_t child = 0; child < 12; child++) {
            population.push_back(children[child]);
        }

        for (auto& individual : population) { // mutation
            auto buf_prob = random(0, 1);
            if (buf_prob < probability) {
                individual.x = fmod(individual.x * random(0, 2), 2);
                individual.y = fmod(individual.x * random(-2, 2), 2);
            }
        }
        std::sort(population.begin(), population.end()); // selection
        std::vector<Point> new_population(4);
        count = 15;
        for (auto& individual : new_population) {
            individual = population[count];
            count--;
        }
        std::cout << "Generation # " << i + 1 << std::endl;
        print_results(new_population);
        population = new_population;
    }
}

#endif //TSISA_RK2_GENETIC_ALGORITHM_HPP

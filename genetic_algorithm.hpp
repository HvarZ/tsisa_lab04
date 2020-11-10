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

    Point(const double x1, const double y1) {
        x = x1;
        y = y1;
        fit = function(x, y);
    }

    Point() {
        x = 0;
        y = 0;
        fit = function(x, y);
    }

};

auto SortFunc(const Point& point1, const Point& point2) {
    return point1.fit > point2.fit;
}


auto random_values(const double x1, const double x2, const double y1, const double y2) -> Point {
    if (x1 >= x2 || y1 >= y2) throw std::invalid_argument("Invalid segment");
    std::random_device rd;
    std::mt19937_64 rng(rd());
    std::uniform_real_distribution<double> rand_x(x1, x2);
    std::uniform_real_distribution<double> rand_y(y1, y2);
    return Point(rand_x(rng), rand_y(rng));
}

auto fill_population(const double x1, const double x2, const double y1, const double y2) -> std::vector<Point> {
    const size_t number_of_points = 4;
    std::vector<Point> result(0);
    for (size_t i = 0; i < number_of_points; i++) {
        result.push_back(random_values(x1, x2, y1, y2));
    }
    return result;
}



void print_results(const std::vector<Point>& points) {
    for (const auto& i : points) {
        std::cout << "x:" << i.x << "\ty:" << i.y << "\tfit:" << i.fit << std::endl;
     }
}

void Algorithm() {
    const double probability = 0.3;
    const size_t generations = 100;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> prob(0, 1);
    std::uniform_real_distribution<double> dis_mut(-5, 5);

    auto population = fill_population(0, 2, -2, 2);
    std::sort(population.begin(), population.end(), SortFunc);
    print_results(population);

    for (size_t i = 0; i < generations; i++) {
        for (auto &point : population) {
            auto p = prob(gen);
            if (p < probability) {
                point.x = fmod(point.x * dis_mut(gen), 1);
                point.y = fmod(point.y * dis_mut(gen), 1);
                point.fit = function(point.x, point.y);
            }
        }
        std::sort(population.begin(), population.end(), SortFunc);
        size_t buf = 0;
        std::vector<Point> new_population(4);
        if (population[0].fit != population[1].fit) {
            buf = 1;
        } else {
            buf = 2;
        }

        new_population[0].x = population[0].x;
        new_population[0].y = population[buf].y;
        new_population[0].fit = function(new_population[0].x, new_population[0].y);

        new_population[1].x = population[buf].x;
        new_population[1].y = population[0].y;
        new_population[1].fit = function(new_population[1].x, new_population[1].y);

        new_population[2].x = population[0].x;
        new_population[2].y = population[buf + 1].y;
        new_population[2].fit = function(new_population[2].x, new_population[2].y);

        new_population[3].x = population[buf + 1].x;
        new_population[3].y = population[0].y;
        new_population[3].fit = function(new_population[3].x, new_population[3].y);

        std::sort(new_population.begin(), new_population.end(), SortFunc);
        std::cout << "Iteration # " << i + 1 << std::endl;
        print_results(new_population);
    }
}




#endif //TSISA_RK2_GENETIC_ALGORITHM_HPP

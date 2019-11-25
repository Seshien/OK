#pragma once
#include "city.h"
#include "generator.h"
class Greedy
{
public:
	Greedy();
	~Greedy();
	double find_shortest_dist(vector<City>& c_not_visited, City start, int& pos);
	std::vector<int> TSP_greedy(vector<City> cities_copy, vector<City>& cities);
};


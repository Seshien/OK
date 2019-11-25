#pragma once
#include <vector>
#include <iostream>
#include "city.h"
#include <algorithm>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Gene
{
private:
	vector<int> road;
	double fitness;
	double distance;
	int rank;
public:
	Gene(vector<int> road, vector<City> &cities);
	void calculate_fitness();
	void set_road(vector<int> road);
	void set_rank(int rank);
	void calculate_distance(vector<City> &cities);
	double get_fitness();
	double get_distance();
	int get_rank();
	vector<int> get_road();
	void mutate(double mutate_chance, int amount_of_cities, vector<City> &cities);
	bool operator <(Gene& other);
};


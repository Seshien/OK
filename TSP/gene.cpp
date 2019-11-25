#include "gene.h"

Gene::Gene(vector<int> road, vector<City> &cities) {
	Gene::road = road;
	Gene::calculate_distance(cities);
	Gene::calculate_fitness();
}
void Gene::calculate_fitness() {
	Gene::fitness = 1/Gene::distance;
	//cout << "Fitness: " << Gene::fitness << endl;
}

void Gene::set_road(vector<int> road) {
	Gene::road = road;
}

void Gene::set_rank(int rank) {
	Gene::rank = rank;
}

void Gene::calculate_distance(vector <City> &cities) {
	double distance = 0;
	for (int i = 0; i < Gene::road.size() - 1; i++) {
		distance += cities[Gene::road[i]-1].distance_between(cities[Gene::road[i + 1]-1]);
	}
	distance += cities[Gene::road[Gene::road.size() - 1]-1].distance_between(cities[Gene::road[0]-1]);
	cout << "Dystans przed: " << Gene::distance << endl;
	Gene::distance = distance;
	cout << "Dysntans po: " << Gene::distance << endl;
}
double Gene::get_fitness() {
	return Gene::fitness;
}

double Gene::get_distance() {
	return Gene::distance;
}

int Gene::get_rank() {
	return Gene::rank;
}

vector<int> Gene::get_road() {
	return Gene::road;
}

void Gene::mutate(double mutate_chance, int amount_of_cities, vector<City> &cities) {
	int random_number;
	for (int i = 0; i < Gene::road.size(); i++)
	{
		random_number = rand() % 100;
		if (int(mutate_chance) > random_number)
		{
			random_number = rand() % amount_of_cities;
			swap(Gene::road[i], Gene::road[random_number]);
		}
	}
	//cout << Gene::get_distance() << endl;
	Gene::calculate_distance(cities);
	//cout << Gene::get_distance() << endl;
	Gene::calculate_fitness();
}

bool Gene::operator < (Gene& other)
{
	return (Gene::fitness < other.fitness);
}


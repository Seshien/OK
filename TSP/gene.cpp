#include "gene.h"

Gene::Gene(vector<int> road, vector<City> &cities) {
	this->road = road;
	this->calculate_distance(cities);
	this->calculate_fitness();
}
void Gene::calculate_fitness() {
	this->fitness = 1 / this->distance;
	//cout << "Fitness: " << Gene::fitness << endl;
}

void Gene::set_road(vector<int> road) {
	this->road = road;
}

void Gene::set_rank(int rank) {
	this->rank = rank;
}

void Gene::calculate_distance(vector <City> &cities) {
	double distance = 0;
	for (int i = 0; i < road.size() - 1; i++) {
		distance += cities[road[i]-1].distance_between(cities[road[i + 1]-1]);
	}
	distance += cities[road[road.size() - 1]-1].distance_between(cities[road[0]-1]);
	cout << "Dystans przed: " << this->distance << endl;
	this->distance = distance;
	cout << "Dysntans po: " << this->distance << endl;
}
double Gene::get_fitness() {
	return this->fitness;
}

double Gene::get_distance() {
	return this->distance;
}

int Gene::get_rank() {
	return this->rank;
}

vector<int> Gene::get_road() {
	return this->road;
}

void Gene::mutate(double mutate_chance, int amount_of_cities, vector<City> &cities) {
	int random_number;
	for (int i = 0; i < this->road.size(); i++)
	{
		random_number = rand() % 100;
		if (int(mutate_chance) > random_number)
		{
			random_number = rand() % amount_of_cities;
			swap(this->road[i], this->road[random_number]);
		}
	}
	//cout << Gene::get_distance() << endl;
	this->calculate_distance(cities);
	//cout << Gene::get_distance() << endl;
	this->calculate_fitness();
}

bool Gene::operator < (Gene& other)
{
	return (this->fitness < other.fitness);
}


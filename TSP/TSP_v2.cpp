#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "city.h"
#include "generator.h"

using namespace std;
// meta-heurystyki
// Instancje EUC_2D
// mmachowiak/instances
// mmachowiak/ok

int read_file(string file_name, vector<City>& cities, int beginning = 0)
{
	ifstream my_file;
	int amount;
	int* tmp = new int[3];
	int index = 0;
	my_file.open(file_name.c_str());
	if (my_file.is_open() == false) {
		cout << "Problem otwarcia pliku!" << endl;
	}
	my_file >> amount;
	while (!my_file.eof()) {
		tmp[0] = NULL;
		tmp[1] = NULL;
		tmp[2] = NULL;
		my_file >> tmp[0];
		my_file >> tmp[1];
		my_file >> tmp[2];
		if (tmp[0] != NULL && tmp[1] != NULL && tmp[2] != NULL) {
			cities.push_back(City(tmp[0], tmp[1], tmp[2]));
			index++;
		}
		else {
			cout << "nie dziala" << endl;
			break;
		}
	}
	delete[] tmp;
	tmp = NULL;
	my_file.close();
	return amount;
}

double find_shortest_dist(vector<City>& c_not_visited, City start, int& pos)
{
	double shortest_dist;
	bool first_time_set = false;
	int destination_pos = 0;
	double temp;
	for (int i = 0; i < c_not_visited.size(); i++) {
		if (first_time_set == false) {
			shortest_dist = start.distance_between(c_not_visited[i]);
			destination_pos = i;
			first_time_set = true;
		}
		else {
			if (shortest_dist > (temp = start.distance_between(c_not_visited[i]))) {
				shortest_dist = temp;
				destination_pos = i;
			}
		}
	}
	pos = destination_pos;
	return shortest_dist;
}

void TSP_greedy(vector<City> cities_copy, vector<City>& cities, int amount) {
	double road_length = 0;
	int next_pos;
	vector<int> road_by_id;
	road_by_id.push_back(cities[0].get_id());
	City from_city = cities_copy[0];
	cout << "Ilocs miast: " << cities_copy.size() << endl;
	cities_copy.erase(cities_copy.begin());
	
	while (cities_copy.size() > 0) {
		road_length += find_shortest_dist(cities_copy, from_city, next_pos);
		road_by_id.push_back(cities_copy[next_pos].get_id());
		if (cities_copy.size() > 1)
		{
			from_city = cities_copy[next_pos];
			cities_copy.erase(cities_copy.begin() + next_pos);
		}
		else
		{
			road_length += cities[0].distance_between(cities_copy[next_pos]);
			cities_copy.erase(cities_copy.begin() + next_pos);
		}
	}

	cout << "Dlugosc drogi: " << road_length << endl;
	cout << "Sciezka: ";
	for (int i = 0; i < road_by_id.size(); i++) {
		cout << road_by_id[i] << " ";
	}
	cout << endl;
}

int main()
{
	srand(time(0));
	string file_name;
	vector<City> cities;
	int amount;
	int choice;
	cout << "1) Z pliku, 2) Generuj: ";
	cin >> choice;
	if (choice == 1) {
		cout << "Podaj nazwe pliku: ";
		cin >> file_name;
		amount = read_file(file_name, cities);
	}
	else {
		cout << "Podaj ilosc wierzcholkow: ";
		cin >> amount;
		cities = generate_cities(amount);
		if (cities.size() == 0)
		{
			exit(0);
		}
	}
	cout << "Ilosc miast: " << amount << endl;
	for (int i = 0; i < amount; i++)
	{
		cities[i].print_city();
	}
	TSP_greedy(cities, cities, amount);
	cin.sync();
	cin.get();
	return 0;
}

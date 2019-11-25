#include "Greedy.h"



Greedy::Greedy()
{
}


Greedy::~Greedy()
{
}


double Greedy::find_shortest_dist(vector<City>& c_not_visited, City start, int& pos)
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

std::vector<int> Greedy::TSP_greedy(vector<City> cities_copy, vector<City>& cities) {
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
	return road_by_id;
}
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "city.h"
#include "generator.h"
#include "Tabo.h"
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
		cities.push_back(City(tmp[0], tmp[1], tmp[2]));
		index++;
	}
	delete[] tmp;
	tmp = NULL;
	my_file.close();
	return amount;
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
	//TSP_greedy(cities, cities, amount);
	Tabo tabo(cities, Tabo::Config());
	tabo.printMatrix();
	cin.sync();
	cin.get();
	return 0;
}

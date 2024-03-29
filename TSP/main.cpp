#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "city.h"
#include "generator.h"
#include "genetic_alg.h"
#include "Greedy.h"
#include "Tabo.h"
using namespace std;
// meta-heurystyki
// Instancje EUC_2D
// mmachowiak/instances
// mmachowiak/ok
void saveToFile(Tabo tabo, std::string file_name="default")
{
	if (file_name == "default")
	{
		std::cout << "Podaj nazwe pliku: ";
		std::cin >> file_name;
	}

	ofstream my_file;
	file_name = "wynik_" + file_name;
	my_file.open(file_name.c_str(), ios::app);
	pair<double, double> result = tabo.returnBest();
	vector<int> result_path = tabo.returnBestPath();
	Tabo::Config config = tabo.returnConfig();
	//my_file << config.Neigh_Size << ":" << config.Iteration_Amount << ":" << config.PENAL_LONG_TERM << ":" << config.Max_Taboo << ":" << config.TIME_TRY << std::endl;
	my_file << "L. iteracji | Czas Tabo | Czas dywersyfikacji | L. zmian w dywersyfikacji | Mno�nik kary Tabo | Precision | Shuffle try" << std::endl;
	my_file << config.Iteration_Amount << ":" << config.PENAL_LONG_TERM << ":" << config.DIVERSIFICATION << ":" << config.NUMBER_OF_CHANGES << ":" << config.TABO_VALUE_PENALTY<< ":" << config.Precision << ":" << config.Shuffle_Try << std::endl;
	my_file << result.first << ":" << result.second << std::endl;
	for (int i = 0; i < result_path.size(); i++)
	{
		my_file << result_path[i] + 1 << " ";
	}
	my_file << std::endl;
	my_file << std::endl;
	my_file << std::endl;
	my_file << std::endl;
	my_file.close();
}
int read_file( vector<City>& cities, std::string & file_name, int beginning = 0)
{
	std::cout << "Podaj nazwe pliku: ";
	std::cin >> file_name;
	ifstream my_file;
	int amount;
	int* tmp = new int[3];
	int index = 0;
	my_file.open(file_name.c_str());
	if (my_file.is_open() == false) 
	{
		std::cout << "Problem otwarcia pliku!" << endl;
		return -1;
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
	std::string file_name = "default";
	vector<City> cities;
	Greedy greedy;
	Tabo::Config config;
	int amount;
	//int choice;
	char choice='o';
	while (choice != 'e')
	{
		while (cities.empty())
		{
			std::cout << "Musisz utworzyc graf na ktorym chcesz pracowac. Wybierz sposob utworzenia:" << std::endl;
			std::cout << "Generator[g], plik[p], konsola[k]." << std::endl;
			std::cin >> choice;
			switch (choice)
			{
			case 'g':
				cout << "Podaj ilosc wierzcholkow: ";
				std::cin >> amount;
				cities = generate_cities(amount);
				break;
			case 'p':

				if ((amount = read_file(cities, file_name)) != -1)
					std::cout << "Wczytywanie udane." << std::endl;
				else
				{
					std::cout << "Wystapil blad, graf nie zostal wczytany." << std::endl;
					cities.clear();
					std::cin.clear();
				}
				break;
			case 'k':
				if (1 == 0)
					std::cout << "Wczytywanie udane." << std::endl;
				else
				{
					std::cout << "Wystapil blad, graf nie zostal wczytany." << std::endl;
					std::cin.clear();
				}
				break;
			default:
				std::cout << "Komenda nie rozpoznana." << std::endl;
				std::cin.clear();
				break;
			}
			std::cout << "<-------------------------------------------------------------->" << std::endl;
		}

		//TSP_genetic(cities, amount);
		//tabo.printMatrix();
		std::cin.sync();
		std::cin.get();
		Tabo tabo(cities, config);
		tabo.filename = "wynik_" + file_name;
		while (!cities.empty())
		{
			std::cout << "<-------------------------------------------------------------->" << std::endl;
			std::cout << "Graf istnieje. Wybierz czynnosc z listy." << std::endl;
			std::cout << "<-------------------------------------------------------------->" << std::endl;
			std::cout << "Pokaz liste miast[s]. Odpal algorytm[g]. Pokaz wynik[w]. " << std::endl;
			std::cout << "Zmien config[c]. Usun graf[u]. Wyjdz z programu[e]." << std::endl;
			std::cout << "<-------------------------------------------------------------->" << std::endl;
			std::cin >> choice;
			switch (choice)
			{
			case 's':
				cout << "Ilosc miast: " << amount << endl;
				for (int i = 0; i < amount; i++)
				{
					cities[i].print_city();
				}
				break;
			case 'f':
				saveToFile(tabo, file_name);
				break;
			case 'g':
				tabo.FullAlgorithm();
				saveToFile(tabo, file_name);
				break;
			case 'c':
				std::cout << "Work in Progress" << std::endl;
				break;
			case 'e':
				std::cout << "Wychodzenie z programu." << std::endl;
				cities.clear();
				break;
			case 'u':
				std::cout << "Usuwanie grafu." << std::endl;
				cities.clear();
				break;
			case 'w':
				tabo.showBest();
				break;
			default:
				std::cout << "Komenda nie rozpoznana." << std::endl;
			}
			std::cout << "<-------------------------------------------------------------->" << std::endl;
		}
	}

	return 0;
}

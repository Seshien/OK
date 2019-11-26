#include "genetic_alg.h"

Genetic_alg::Genetic_alg(vector<City> cities, int amount_of_c, double crossover, double mutate, int n_of_genes, int n_of_generations, int elite)
{
	this->cities = cities;
	this->amount_of_cities = amount_of_c;
	this->crossover_chance = crossover;
	this->mutate_chance = mutate;
	this->number_of_genes = n_of_genes;
	this->numer_of_generations = n_of_generations;
	this->number_of_elite_genes = elite;
	this->start_algorithm();
}

void Genetic_alg::start_algorithm() {
	cout << "ALGORYTM GENETYCZNY BEGIN" << endl;
	Genetic_alg::new_population();
	sort(Genetic_alg::genes.begin(), Genetic_alg::genes.end());
	cout << "Pokolenie poczatkowe: " << endl;
	cout << "D³ugoœæ drogi: " << Genetic_alg::genes[Genetic_alg::genes.size() - 1].get_distance() << endl;
	for (int i = 0; i < Genetic_alg::numer_of_generations; i++)
	{
		// Rank selection sam w sobie wywoluje crossovery pod koniec
		//Genetic_alg::show_genes();
		Genetic_alg::give_ranks();
		Genetic_alg::rank_selection();
		//Genetic_alg::show_next_genes();
		Genetic_alg::genes_mutation(cities);
		//Genetic_alg::show_next_genes();
		Genetic_alg::replace_old();
		sort(Genetic_alg::genes.begin(), Genetic_alg::genes.end());
		cout << "Pokolenie: " << i << endl;
		cout << "D³ugoœæ drogi: " << Genetic_alg::genes[Genetic_alg::genes.size() - 1].get_distance() << endl;
	}
	cout << "ALGORYTM GENETYCZNY END" << endl;
}

// WORKS
void Genetic_alg::new_population() {
	// Tworzê uporz¹dkowany wektor 1 - liczba_miast
	vector<int> temporary;
	for (int i = 1; i <= Genetic_alg::amount_of_cities; i++)
	{
		temporary.push_back(i);
	}
	//temporary = Greedy().TSP_greedy(cities, cities);
	for (int i = 0; i < Genetic_alg::number_of_genes; i++)
	{
		// Kopia uporz¹dkowanego wektora
		vector<int> copy_temporary = temporary;
		for (int j = 0; j < Genetic_alg::amount_of_cities; j++)
		{
			int random_number = rand() % amount_of_cities;
			swap(copy_temporary[j], copy_temporary[random_number]);
		}
		Genetic_alg::genes.push_back(Gene(copy_temporary, this->cities));
	}
}

void Genetic_alg::give_ranks() {
	// Sortuje geny rosn¹co wzglêdem fitness i przypisuje im rangi
	sort(Genetic_alg::genes.begin(), Genetic_alg::genes.end());
	int rank = 1;
	for (int i = 0; i < Genetic_alg::number_of_genes; i++)
	{
		Genetic_alg::genes[i].set_rank(rank);
		rank++;
	}
}

// WORKS
void Genetic_alg::rank_selection() {
	int sum_of_ranks = 0;
	for (int i = 0; i < Genetic_alg::number_of_genes; i++)
	{
		sum_of_ranks += Genetic_alg::genes[i].get_rank();
	}
	// Wybieram losowo z wagami dwóch rodziców genu i wywo³uje na nich breeding()
	// Zaklepuje miejsce w nastêpnej generacji na elitarne geny (znajduj¹ siê one na koñcu wektora genes, bo maj¹ najwiêszky fitness)
	for (int i = 0; i < Genetic_alg::number_of_genes - Genetic_alg::number_of_elite_genes; i++)
	{
		int parent_1_id;
		int parent_2_id;
		parent_1_id = Genetic_alg::select_parent_id(sum_of_ranks);
		parent_2_id = Genetic_alg::select_parent_id(sum_of_ranks);
		Genetic_alg::next_gen.push_back(Genetic_alg::breeding(Genetic_alg::genes[parent_1_id], Genetic_alg::genes[parent_2_id]));
	}
}

// I THINK IT WORKS
int Genetic_alg::select_parent_id(int sum_of_ranks)
{
	int random_rank = rand() % sum_of_ranks + 1;
	for (int i = 0; i < Genetic_alg::number_of_genes; i++)
	{
		random_rank -= Genetic_alg::genes[i].get_rank();
		if (random_rank <= 0)
		{
			return i;
		}
	}
}

// Single-point crossover
Gene Genetic_alg::breeding(Gene parent_1, Gene parent_2) {
	vector<int> road;
	int random_number = rand() % 100;
	//cout << endl;
	if (random_number > Genetic_alg::crossover_chance)
	{
		return rand() % 2 ? parent_1 : parent_2;
	}
	/*cout << "Parent 1: " << endl;
	for (int i = 0; i < parent_1.get_road().size(); i++)
	{
		cout << parent_1.get_road()[i] << " ";
	}
	cout << endl;
	cout << "Parent 2: " << endl;
	for (int i = 0; i < parent_2.get_road().size(); i++)
	{
		cout << parent_2.get_road()[i] << " ";
	}*/
	random_number = rand() % (Genetic_alg::amount_of_cities+1);
	for (int i = 0; i < random_number; i++)
	{
		road.push_back(parent_1.get_road()[i]);
	}
	for (int i = 0; i < Genetic_alg::amount_of_cities; i++)
	{
		if (find(road.begin(), road.end(), parent_2.get_road()[i]) == road.end())
		{
			road.push_back(parent_2.get_road()[i]);
		}
	}
	/*cout << endl;
	cout << "Childe: " << endl;
	for (int i = 0; i < parent_1.get_road().size(); i++)
	{
		cout << road[i] << " ";
	}*/
	return Gene(road, Genetic_alg::cities);
}

void Genetic_alg::genes_mutation(vector<City> &cities) {
	for (int i = 0; i < Genetic_alg::next_gen.size(); i++)
	{
		Genetic_alg::next_gen[i].mutate(Genetic_alg::mutate_chance, Genetic_alg::amount_of_cities, cities);
	}
}

void Genetic_alg::replace_old() {

	for (int i = 0; i < Genetic_alg::number_of_genes - Genetic_alg::number_of_elite_genes; i++)
	{
		Genetic_alg::genes[i] = Genetic_alg::next_gen[i];
	}
	Genetic_alg::next_gen.clear();
}

void Genetic_alg::show_genes()
{
	cout << "GENES: " << endl;
	for (int i = 0; i < Genetic_alg::genes.size(); i++)
	{
		for(int j = 0; j < Genetic_alg::genes[i].get_road().size(); j++)
			cout << Genetic_alg::genes[i].get_road()[j] << " ";
		cout << Genetic_alg::genes[i].get_distance() << endl;
	}
	cout << "=======" << endl;
}

void Genetic_alg::show_next_genes()
{
	cout << "NEXT GENES: " << endl;
	for (int i = 0; i < Genetic_alg::next_gen.size(); i++)
	{
		for (int j = 0; j < Genetic_alg::next_gen[i].get_road().size(); j++)
			cout << Genetic_alg::next_gen[i].get_road()[j] << " ";
		cout << Genetic_alg::next_gen[i].get_distance() << endl;
	}
	cout << "=======" << endl;
}
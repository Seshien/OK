#pragma once
#include <iostream>;
#include <vector>;
#include <algorithm>
#include "gene.h";
#include "city.h";

using namespace std;

class Genetic_alg
{
private:
	vector<City> cities;
	vector<Gene> genes;
	// selseced_genes i elite_genes chyba nie potrzebne XD
	vector<Gene> selected_genes;
	vector<Gene> elite_genes;
	vector<Gene> next_gen;
	int amount_of_cities;
	double crossover_chance;
	double mutate_chance;
	int number_of_elite_genes;
	int number_of_genes;
	int numer_of_generations;
public:
	Genetic_alg(vector <City> cities, int amount_of_c, double crossover, double mutate, int n_of_genes, int n_of_generations, int elite);
	void start_algorithm();
	void new_population();
	void give_ranks();
	void rank_selection();
	int select_parent_id(int sum_of_ranks);
	Gene breeding(Gene parent_1, Gene parent_2);
	void genes_mutation();
	void replace_old();
};


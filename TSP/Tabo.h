#pragma once
#include "city.h"
#include "generator.h"
#include "Greedy.h"
#include "tuple"
#include <tuple>
#include <algorithm>
#include <random>
#include <chrono>
class Tabo
{
public:
	struct Result
	{
		std::vector<int> path;
		bool isTabu = false;
		int time = -1;
		double value = 0;
		double dist = 0;
		std::pair<int, int> changed;
	};

	struct Tabo_info
	{
		int index1;
		int index2;
		int time;
	};

	struct Config
	{
		int ITERS_TO_SAVE[13] = { 5, 10, 20, 50, 100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000 };
		int Neigh_Size = 200;
		int Iteration_Amount = 7000;
		int PENAL_LONG_TERM = 180;
		int DIVERSIFICATION = 100;
		int NUMBER_OF_CHANGES = 10;
		float TABO_VALUE_PENALTY = 1.01f;
		bool SHUFFLE_CURRENT = true;
		
		int Precision = 10;//Iteration_Amount/10;
		int Shuffle_Try = 50;// Iteration_Amount / 10;
	};

	Tabo(std::vector<City> & cities, Config config);
	void init();
	~Tabo();

	void FullAlgorithm();
	bool Step();
	void Shuffle();
	void saveIter(int iter_num);
	void printMatrix();

	void showBest();
	pair<double, double> returnBest();
	vector<int> returnBestPath();
	Config returnConfig();

	std::string filename;
	
	bool checkPairs(pair<int, int> pair1, pair<int, int> pair2);
	bool checkTaboList(pair<int, int> position);

private:
	int iteration;
	int shuffleTimer;
	Config _config;
	Result _bestResult;
	Result currentSolution;
	std::vector<int> frequency_count;
	std::vector<int> frequency_zero;
	std::vector<Result> results;
	std::vector<Result> previous_results;
	//std::vector<Result> taboresults;
	std::vector<City> cities;
	std::vector<double> neighvalues;
	std::vector<Tabo_info> taboList;
	int iterImprovement;
	// Czas, ID1, ID2
	pair<int, int> tabu;
	Tabo::Result getFirstResult();
	Tabo::Result getResult(std::vector<int> & path, std::pair<int, int> changed = { -1,-1 });
	double getDistance(std::vector<int> path);

	pair<double, double> showResult(Result res);
	void showIteration(Result res);

	std::vector<Result> createNeighb(std::vector<int> & path);
	std::vector<Result> createNeighbPartial(std::vector<int> & path);
	std::vector<int> ShufflePath(std::vector<int> path, pair<int, int> & changed);
	std::vector<int> ChangeTwo(std::vector<int> &path, pair<int, int> positions);
	double getValue(Result & res);

	std::vector<std::vector<double>> distmatrix;
	void fillMatrix(std::vector<City> & cities);
	void fillMatrix();
};


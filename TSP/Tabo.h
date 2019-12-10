#pragma once
#include "city.h"
#include "generator.h"
#include "Greedy.h"
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

	struct Config
	{
		int Neigh_Size = 50;
		int Iteration_Amount = 10000;
		int PENAL_LONG_TERM = 10;

		
		int Max_Result = Iteration_Amount/10;
		int TIME_TRY = Iteration_Amount/10;
	};

	Tabo(std::vector<City> & cities, Config config);
	~Tabo();

	void FullAlgorithm();

	void printMatrix();

	void showBest();
	pair<double, double> returnBest();
	Config returnConfig();

private:

	int iteration;
	Config _config;
	Result _bestResult;
	std::vector<Result> results;
	//std::vector<Result> taboresults;
	std::vector<City> cities;
	std::vector<double> neighvalues;
	int iterImprovement;

	Tabo::Result getFirstResult();
	Tabo::Result getResult(std::vector<int> & path, std::pair<int, int> changed = { -1,-1 });
	double getDistance(std::vector<int> path);

	pair<double, double> showResult(Result res);
	void showIteration(Result res);

	std::vector<Result> createNeighb(std::vector<int> & path);
	std::vector<int> ShufflePath(std::vector<int> path, pair<int, int> & changed);
	double getValue(Result & res);

	std::vector<std::vector<double>> distmatrix;
	void fillMatrix(std::vector<City> & cities);
	void fillMatrix();
};


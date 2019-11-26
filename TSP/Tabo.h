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
	};

	struct Config
	{
		int Neigh_size = 30;
		int NUM_INTERATION = 1000;
		int PENAL_LONG_TERM = 10;


		int LONG_TERM_LENGTH = 100;
		int TIME_TRY = 500;
	};

	Tabo(std::vector<City> & cities, Config config);
	~Tabo();

	void FullAlgorithm();

	void printMatrix();

	void showBest();
private:

	int iteration;
	Config _config;
	Result _bestResult;
	std::vector<Result> results;
	//std::vector<Result> taboresults;
	std::vector<City> cities;



	Tabo::Result getFirstResult();
	Tabo::Result getResult(std::vector<int> & path);
	double getDistance(std::vector<int> path);

	void showResult(Result res);
	void showIteration(Result res);

	std::vector<vector<int>> createNeighb(std::vector<int> & path);
	std::vector<int> ShufflePath(std::vector<int> path);
	double getValue(Result & res);

	std::vector<std::vector<double>> distmatrix;
	void fillMatrix(std::vector<City> & cities);
	void fillMatrix();
};


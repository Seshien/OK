#pragma once
#include "city.h"
#include "generator.h"
class Tabo
{
public:
	struct Result
	{
		std::vector<int> path;
		bool isTabu;
		int time;
		int value;
	};

	struct Config
	{
		int TABU_LENGTH = 30;
		int NUM_INTERATION = 3000;
		int PENAL_LONG_TERM = 10;
		int LONG_TERM_LENGTH = 100;
		int TIME_TRY = 500;
	};

	Tabo(std::vector<City> & cities, Config config);
	~Tabo();

	void FullAlgorithm();

	void printMatrix();
private:
	Config _config;
	Result _bestResult;
	std::vector<Result> results;
	std::vector<City> cities;

	std::vector<std::vector<double>> distmatrix;
	void fillMatrix(std::vector<City> & cities);
	void fillMatrix();
};


#include "Tabo.h"


Tabo::Tabo(std::vector<City> & cities, Config config)
{
	this->results.clear();
	this->_config = config;
	this->cities = cities;
	this->fillMatrix(cities);
}
void Tabo::FullAlgorithm()
{
	_bestResult = getFirstResult();
	results.push_back(_bestResult);
	while (1)
	{
		auto pathlist = createNeighb(_bestResult.path);
		Result bestCandidate = getResult(pathlist[0]);
		for (int i = 1; i < pathlist.size(); i++)
		{
			Result candidate = getResult(pathlist[1]);
			if (candidate.value < bestCandidate.value)
				bestCandidate = candidate;
		}
		if (bestCandidate.value < _bestResult.value)
			_bestResult = bestCandidate;
		results.push_back(bestCandidate);
	}

	
}


Tabo::Result Tabo::getResult(std::vector<int> & path)
{
	Result res;
	res.path = path;
	res.isTabu = false;
	res.time = _config.LONG_TERM_LENGTH;
	res.value = getValue(res);
	return res;
}

double Tabo::getValue(Result & res)
{
	double value = 0;
	value += getDistance(res.path);
	value += res.time;
	for (auto test : this->results)
		if (test.path == res.path && test.isTabu)
		{

		}
	return value;
}

std::vector<vector<int>> Tabo::createNeighb(std::vector<int> & path)
{
	std::vector<vector<int>> neigh;
	for (int i = 0; i < this->_config.Neigh_size; i++)
	{
		neigh.push_back(ShufflePath(path));
	}
	return neigh;
}
std::vector<int> Tabo::ShufflePath(std::vector<int> path)
{
	int a=0, b=0;
	while (a == b)
	{
		a = rand() % path.size();
		b = rand() % path.size();
	}
	int temp = path[a];
	path[a] = path[b];
	path[b] = temp;
	return path;


}
void Tabo::fillMatrix(std::vector<City> & cities)
{
	this->distmatrix.clear();
	for (int i = 0; i < cities.size(); i++)
	{
		this->distmatrix.push_back(std::vector<double>());
		for (int j = i + 1; j < cities.size(); j++)
		{
			distmatrix[i].push_back(cities[i].distance_between(cities[j]));
		}
	}
}

Tabo::Result Tabo::getFirstResult()
{
	Result res;
	res.path = Greedy().TSP_greedy(cities, cities);
	res.value = getDistance(res.path);
	res.isTabu = true;
	res.time = _config.PENAL_LONG_TERM;
	return res;
}

double Tabo::getDistance(std::vector<int> path)
{
	double distance = 0;
	for (int id = 0; id < path.size() - 1; id++) 
	{
		distance+=this->distmatrix[path[id]][path[id + 1]];
	}
	distance += distmatrix[path[path.size() - 1]][path[0]];
	return distance;
}

void Tabo::fillMatrix()
{
	this->distmatrix.clear();
	for (int i = 0; i < cities.size(); i++)
	{
		this->distmatrix.push_back(std::vector<double>());
		for (int j = 0; j < cities.size(); j++)
		{
			distmatrix[i].push_back(cities[i].distance_between(cities[j]));
		}
	}
}
void Tabo::printMatrix()
{
	for (auto column : this->distmatrix)
	{
		for (auto row : column)
		{
			std::cout << row << " ";
		}
		std::cout << std::endl;
	}
}

Tabo::~Tabo()
{
}


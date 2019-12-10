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
	for (iteration = 0; iteration < _config.Iteration_Amount; iteration++)
	{
		auto pathlist = createNeighb(_bestResult.path); //tworzy neighbourhood
		Result bestCandidate = getResult(pathlist[0]); //tworzy result z pierwszego patha
		for (int i = 1; i < pathlist.size(); i++)
		{
			Result candidate = getResult(pathlist[i]); //tworzy result z kolejnego patha
			if (candidate.value < bestCandidate.value) //sprawdza czy wartosc resulta jest dotychczasowo najlepsza
				bestCandidate = candidate;
		}

		//showIteration(bestCandidate);
		if (bestCandidate.dist < _bestResult.dist) //sprawdza czy najlepszy wynik (value) z neigh, jest krotszy od tychczasowego najlepszego wyniku.
		{
			_bestResult = bestCandidate;
			std::cout << "Znaleziona lepsza wartosc" << std::endl;
		}


		if (!bestCandidate.isTabu) //Jezeli bestCandidate.isTabu, to znaczy ze result juz istnieje i go nie dodajemy.
		{
			bestCandidate.isTabu = false;
			results.push_back(bestCandidate);
		}

		if (results.size() > _config.Max_Result)
			results.erase(results.begin());

	}
	showBest();
	
}

void Tabo::showIteration(Result res)
{
	std::cout << "Iteracja: " << iteration << std::endl;
	showResult(res);
}

void Tabo::showBest()
{
	if (_bestResult.path.empty())
		return;
	showResult(_bestResult);
	for (auto city : _bestResult.path)
		std::cout << ++city << " ";
	std::cout << std::endl;
}

pair<double, double> Tabo::returnBest()
{
	return pair<double, double>(_bestResult.dist, _bestResult.value);
}

Tabo::Config Tabo::returnConfig()
{
	return this->_config;
}

pair<double, double> Tabo::showResult(Result res)
{
	std::cout << "Odleglosc: " << res.dist << " Wartosc: " << res.value << std::endl;
	return pair<double,double>(res.dist, res.value);

}


Tabo::Result Tabo::getResult(std::vector<int> & path)
{
	Result res;
	res.path = path;
	res.isTabu = false;
	res.time = 0;
	res.value = getValue(res);
	res.dist = getDistance(res.path);
	return res;
}

double Tabo::getValue(Result & res)
{
	double value = 0;
	value += getDistance(res.path);
	value += res.time;


	for (auto & test : this->results)
		if (test.path == res.path)
		{
			res.isTabu = true;
			test.time += _config.PENAL_LONG_TERM;
			if (test.isTabu)
			{
				test.isTabu = false;
				value = value + value / 10;
			}
			else
				test.isTabu = true;
			return value;
		}


	return value;
}

std::vector<vector<int>> Tabo::createNeighb(std::vector<int> & path)
{
	std::vector<vector<int>> neigh;
	for (int i = 0; i < this->_config.Neigh_Size; i++)
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
		for (int j = 0; j < cities.size(); j++)
		{
			distmatrix[i].push_back(cities[i].distance_between(cities[j]));
		}
	}
}

Tabo::Result Tabo::getFirstResult()
{
	Result res;
	res.path = Greedy().TSP_greedy(cities, cities);
	for (auto & p : res.path)
		--p;
	res.value = getDistance(res.path);
	res.dist = res.value;
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


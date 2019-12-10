#include "Tabo.h"


Tabo::Tabo(std::vector<City> & cities, Config config)
{
	this->results.clear();
	this->_config = config;
	this->cities = cities;
	if (_config.Neigh_Size< cities.size())
		this->_config.Neigh_Size = cities.size();
	this->fillMatrix(cities);
	this->iterImprovement = 0;
}
void Tabo::FullAlgorithm()
{
	this->iterImprovement = 0;
	this->results.clear();

	this->_bestResult = getFirstResult();
	this->results.push_back(_bestResult);

	for (iteration = 0; iteration < _config.Iteration_Amount; iteration++)
	{
		auto neigh = createNeighb(_bestResult.path); //tworzy neighbourhood
		Result bestCandidate = neigh[0]; //tworzy result z pierwszego patha
		double neighvalue = bestCandidate.value;
		for (int i = 1; i < neigh.size(); i++)
		{
			neighvalue += neigh[i].value;
			if (neigh[i].value < bestCandidate.value) //sprawdza czy wartosc resulta jest dotychczasowo najlepsza
				bestCandidate = neigh[i];
		}
		neighvalue /= neigh.size();
		this->neighvalues.push_back(neighvalue);
		//showIteration(bestCandidate);
		if (bestCandidate.dist < _bestResult.dist) //sprawdza czy najlepszy wynik (value) z neigh, jest krotszy od tychczasowego najlepszego wyniku.
		{
			iterImprovement = 0;
			_bestResult = bestCandidate;
			std::cout << "Znaleziona lepsza wartosc" << std::endl;
		}
		else
			iterImprovement++;


		if (!bestCandidate.isTabu) //Jezeli bestCandidate.isTabu, to znaczy ze result juz istnieje i go nie dodajemy.
		{
			bestCandidate.isTabu = false;
			results.push_back(bestCandidate);
		}

		if (results.size() > _config.Max_Taboo)
			results.erase(results.begin());
		if (iterImprovement > _config.TIME_TRY)
			break;
	}
	showBest();
	
}

void Tabo::showIteration(Result res)
{
	std::cout << "Iteracja: " << iteration <<" Srednia wartosc neighbourhood: "<< neighvalues.back() << std::endl;
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
	std::cout << "Odleglosc: " << res.dist << " Wartosc: " << res.value << " Zamienione: " << res.changed.first << ":" << res.changed.second << std::endl;
	return pair<double,double>(res.dist, res.value);

}


Tabo::Result Tabo::getResult(std::vector<int> & path, std::pair<int, int> changed)
{
	Result res;
	res.path = path;
	res.isTabu = false;
	res.time = 0;
	res.value = getValue(res);
	res.dist = getDistance(res.path);
	res.changed = changed;
	return res;
}

double Tabo::getValue(Result & res)
{
	double value = 0;
	value += getDistance(res.path);


	for (auto & test : this->results)
		if (test.path == res.path)
		{
			res.isTabu = true;
			test.time += _config.PENAL_LONG_TERM;
			value +=(value * test.time) / 100.0;
			if (test.isTabu)
			{
				value = value + value / 10;
			}
			else
				test.isTabu = true;
			return value;
		}


	return value;
}

std::vector<Tabo::Result> Tabo::createNeighb(std::vector<int> & path)
{
	std::vector<Tabo::Result> neigh;
	for (int i = 0; i < this->_config.Neigh_Size; i++)
	{
		std::pair<int, int> changed = { -1,-1 };
		auto shuffled = ShufflePath(path, changed);
		neigh.push_back(getResult(shuffled, changed));
	}
	return neigh;
}
std::vector<int> Tabo::ShufflePath(std::vector<int> path, pair<int,int> & changed)
{
	int a=0, b=0;
	while (a == b)
	{
		a = rand() % path.size();
		b = rand() % path.size();
	}
	changed.first = a;
	changed.second = b;
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


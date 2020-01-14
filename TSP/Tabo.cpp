#include "Tabo.h"


Tabo::Tabo(std::vector<City> & cities, Config config)
{
	this->results.clear();
	this->_config = config;
	this->cities = cities;
	if (_config.Neigh_Size< cities.size())
		this->_config.Neigh_Size = cities.size();
	this->fillMatrix(cities);
	//this->printMatrix();
	this->iterImprovement = 0;
}

void Tabo::saveIter(int iter_num)
{
	ofstream my_file;
	my_file.open(this->filename.c_str(), ios::app);
	my_file << "ZAPISANE ITERACJE: " << std::endl;
	my_file << "Iteracja[" << this->_config.ITERS_TO_SAVE[iter_num] << "]:" << std::endl;
	for (int j = 0; j < this->currentSolution.path.size(); j++)
	{
		my_file << this->currentSolution.path[j] + 1 << " ";
	}
	my_file << std::endl;
	my_file.close();
}

void Tabo::FullAlgorithm()
{
	for (int i = 0; i < cities.size(); i++)
	{
		frequency_count.push_back(0);
	}
	this->taboList.clear();
	this->iterImprovement = 0;
	this->results.clear();
	this->_bestResult = this->getFirstResult();
	this->currentSolution = this->_bestResult;
	this->results.push_back(_bestResult);

	for (iteration = 0; iteration < _config.Iteration_Amount; iteration++)
	{
		//std::cout << "Iteration";
		//auto neigh = createNeighb(_bestResult.path); //tworzy neighbourhood
		auto neigh = createNeighb2(this->currentSolution.path);
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
		currentSolution = bestCandidate;
		Tabo::Tabo_info tabo_info;
		tabo_info.index1 = currentSolution.changed.first;
		tabo_info.index2 = currentSolution.changed.second;
		tabo_info.time = this->_config.PENAL_LONG_TERM;
		this->taboList.push_back(tabo_info);
		frequency_count[bestCandidate.changed.first]++;
		frequency_count[bestCandidate.changed.second]++;
		for (int i = this->taboList.size() - 1; i >= 0; i--)
		{
			this->taboList[i].time--;
			if (this->taboList[i].time == 0)
			{
				this->taboList.erase(this->taboList.begin() + i);
			}
		}
		if (bestCandidate.dist < _bestResult.dist) //sprawdza czy najlepszy wynik (value) z neigh, jest krotszy od tychczasowego najlepszego wyniku.
		{
			iterImprovement = 0;
			_bestResult = bestCandidate;
			std::cout << "Znaleziona lepsza wartosc" << std::endl;
		}
		else
			iterImprovement++;
		if (iterImprovement >= this->_config.DIVERSIFICATION)
		{
			std::cout << "Shuffle!" << std::endl;
			long int average = 0;
			for (int i = 0; i < frequency_count.size(); i++)
			{
				average += frequency_count[i];
			}
			average /= frequency_count.size();
			for (int i = 0; i < frequency_count.size(); i++)
			{
				if (this->frequency_count[i]  < average)
				{
					frequency_zero.push_back(i);
				}
			}
			auto rng = std::default_random_engine{};
			std::shuffle(std::begin(frequency_zero), std::end(frequency_zero), rng);
			for (int i = 0; i < this->_config.NUMBER_OF_CHANGES; i++)
			{
				if (i+1 >= frequency_zero.size())
					break;
				ChangeTwo(currentSolution.path, std::make_pair(frequency_zero[i], frequency_zero[i+1]));
			}
			iterImprovement = 0;
		}
		if (!bestCandidate.isTabu) //Jezeli bestCandidate.isTabu, to znaczy ze result juz istnieje i go nie dodajemy.
		{
			bestCandidate.isTabu = false;
			results.push_back(bestCandidate);
		}

		if (results.size() > _config.Max_Taboo)
			results.erase(results.begin());
		//if (iterImprovement > _config.TIME_TRY)
			//break;
		for (int i = 0; i < sizeof(this->_config.ITERS_TO_SAVE) / sizeof(int); i++)
		{
			if (i == iteration)
			{
				saveIter(i);
			}
		}
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

vector<int> Tabo::returnBestPath()
{
	return _bestResult.path;
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

bool Tabo::checkPairs(pair<int, int> pair1, pair<int, int> pair2)
{
	if (pair1.first == pair2.first && pair1.second == pair2.second)
	{
		return true;
	}
	if (pair1.first == pair2.second && pair1.second == pair2.first)
	{
		return true;
	}
	return false;
}

double Tabo::getValue(Result & res)
{
	double value = 0;
	value += getDistance(res.path);

	
	/*for (auto& test : this->results)
	{
		if (checkPairs(test.changed, res.changed))
		{
			res.isTabu = true;
			test.time += _config.PENAL_LONG_TERM;
			value += (value * test.time) / 100.0;
			if (test.isTabu)
			{
				value = value + value / 10;
			}
			else
				test.isTabu = true;
			 value;
		}
	}*/
	


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

bool Tabo::checkTaboList(pair<int, int> position)
{
	for (int i = 0; i < this->taboList.size(); i++)
	{
		if (checkPairs(position, std::make_pair(this->taboList[i].index1, this->taboList[i].index2)))
			return true;
	}
	return false;
}

std::vector<Tabo::Result> Tabo::createNeighb2(std::vector<int>& path)
{
	std::vector<Tabo::Result> neigh;
	int i = 0;
	int j = 1;
	int start_point = 0;
	if (path.size() > 150)
	{
		start_point= rand() % (path.size() - 150);
		//std::cout << "Path_size: " << path.size() << std::endl;
		//std::cout << "Start point: " << start_point << std::endl;
		i = start_point;
		j = i + 1;
	}
	pair<int, int> positions;
	//int combinations = cities.size() * (cities.size() - 1);
	//std::cout << "Path_size: " << path.size() << std::endl;
	//std::cout << "Path at 50: " << path[50] << std::endl;
	//std::cout << "Path at 51: " << path[51] << std::endl;
	while (true)
	{
		positions.first = path[i];
		positions.second = path[j];
		
		std::vector<int> new_path = ChangeTwo(path, positions);
		if (checkTaboList(positions))
		{
			// Warunek aspiracji
			if (getDistance(new_path) * this->_config.TABO_VALUE_PENALTY < currentSolution.dist)
			{
				std::cout << "Warunek aspiracji spelniony!" << std::endl;
				neigh.push_back(getResult(new_path, positions));
			}
		}
		else
		{
			neigh.push_back(getResult(new_path, positions));
		}

		j++;
		if (j  == cities.size() || j  == start_point + 149)
		{
			i++;
			j = i + 1;
		}
		if (i + 1 == cities.size() || i + 1 == start_point + 149)
		{
			break;
		}
	}
	return neigh;
}

std::vector<int> Tabo::ChangeTwo(std::vector<int> &path, pair<int, int> positions)
{
	std::vector<int> new_path = std::vector<int>(path);
	int tmp = path[positions.first];
	//new_path = path;
	new_path[positions.first] = new_path[positions.second];
	new_path[positions.second] = tmp;
	return new_path;
}

std::vector<int> Tabo::ShufflePath(std::vector<int> path, pair<int,int> & changed)
{
	int a=0, b=0;
	while (a == b)
	{
		a = rand() % path.size();
		b = rand() % path.size();
	}
	changed.first = path[a];
	changed.second = path[b];
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


#include "Tabo.h"


Tabo::Tabo(std::vector<City> & cities, Config config)
{
	//this->results.clear();
	this->_config = config;
	this->cities = cities;
	this->fillMatrix(cities);
	this->init();
}
void Tabo::init()
{
	this->iterImprovement = 0;
	if (cities.size() > 4 * _config.Neigh_Size)
	{
		_config.Shuffle_Try /= 20;
		_config.Precision /= 10;
		_config.PENAL_LONG_TERM *= 8;
	}

	else if (cities.size() > 2 * _config.Neigh_Size)
	{
		_config.Shuffle_Try /= 10;
		_config.Precision /= 5;
		_config.PENAL_LONG_TERM *= 4;
	}

	else if (cities.size() > _config.Neigh_Size)
	{
		_config.Shuffle_Try /= 5;
		_config.Precision /= 3;
		_config.PENAL_LONG_TERM *= 2;
	}

	else if (cities.size() * 2 < _config.Neigh_Size)
	{
		_config.Iteration_Amount *= 2;
		_config.Shuffle_Try *= 5;
		_config.Precision *= 3;
		_config.PENAL_LONG_TERM /= 2;
	}
}

void Tabo::saveIter(int iter_num)
{
	ofstream my_file;
	my_file.open(this->filename.c_str(), ios::app);
	my_file << "ZAPISANE ITERACJE: " << std::endl;
	my_file << "Iteracja[" << this->_config.ITERS_TO_SAVE[iter_num] << "]:" << std::endl;
	my_file << "Obecna Iteracja " << std::endl;
	for (int j = 0; j < this->currentSolution.path.size(); j++)
	{
		my_file << this->currentSolution.path[j] + 1 << " ";
	}
	my_file << std::endl << "Odleglosc: " << currentSolution.dist << std::endl;
	my_file << "Najlepsza Iteracja " << std::endl;
	for (int j = 0; j < _bestResult.path.size(); j++)
	{
		my_file << this->_bestResult.path[j] + 1 << " ";
	}
	my_file << std::endl << "Odleglosc: " << _bestResult.dist << std::endl;
	my_file << std::endl;
	my_file.close();
	std::cout << "Iteracja[" << this->_config.ITERS_TO_SAVE[iter_num] << "]:" << std::endl;
	std::cout << "Obecna Iteracja " << std::endl;
	std::cout << "Odleglosc: " << currentSolution.dist << std::endl;
	std::cout << "Najlepsza Iteracja " << std::endl;
	std::cout << "Odleglosc: " << _bestResult.dist << std::endl;
}

void Tabo::FullAlgorithm()
{
	for (int i = 0; i < cities.size(); i++)
	{
		frequency_count.push_back(0);
	}
	this->taboList.clear();
	this->iterImprovement = 0;
	//this->results.clear();
	this->_bestResult = this->getFirstResult();
	this->currentSolution = this->_bestResult;
	//this->results.push_back(_bestResult);

	for (iteration = 0; iteration < _config.Iteration_Amount; iteration++)
		if (!this->Step())
			break;
	showBest();
	
}
bool Tabo::Step()
{
	std::vector<Tabo::Result> neigh;
	if (iterImprovement < this->_config.Precision)
		neigh = createNeighb(this->currentSolution.path);
	else
		neigh = createNeighbPartial(this->currentSolution.path);
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
		shuffleTimer=0;
		std::cout << "Znaleziona lepsza wartosc: " << bestCandidate.dist << std::endl;
		std::cout <<"Poprawa: " << _bestResult.dist - bestCandidate.dist << std::endl;
		_bestResult = bestCandidate;
	}
	else
		iterImprovement++;

	if (iterImprovement >= this->_config.DIVERSIFICATION)
	{
		Shuffle();
		shuffleTimer++;
	}
	for (int i = 0; i < sizeof(this->_config.ITERS_TO_SAVE) / sizeof(int); i++)
	{
		if (this->_config.ITERS_TO_SAVE[i] == iteration)
		{
			saveIter(i);
		}
	}
	if (shuffleTimer > this->_config.Shuffle_Try)
		return false;
	else
		return true;
}
void Tabo::Shuffle()
{
	std::cout << "Shuffle!" << std::endl;
	if (this->_config.SHUFFLE_CURRENT == true)
	{
		this->currentSolution = this->_bestResult;
	}
	
	long int average = 0;
	for (int i = 0; i < frequency_count.size(); i++)
	{
		average += frequency_count[i];
	}
	average /= frequency_count.size();
	for (int i = 0; i < frequency_count.size(); i++)
	{
		if (this->frequency_count[i] < average)
		{
			frequency_zero.push_back(i);
		}
	}
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	auto rng = std::default_random_engine(seed);
	//std::cout << "freq before: " << frequency_zero[0] << std::endl;
	std::random_shuffle(std::begin(frequency_zero), std::end(frequency_zero));
	//std::cout << "freq after: " << frequency_zero[0] << std::endl;
	//std::cout << "Sciezka przed: " << std::endl;
	//for (int i = 0; i < currentSolution.path.size(); i++)
	//{
	//	std::cout << currentSolution.path[i] << " ";
	//}
	//std::cout << std::endl;
	for (int i = 0; i < this->_config.NUMBER_OF_CHANGES; i++)
	{
		if (i + 1 >= frequency_zero.size())
			break;
		currentSolution.path = ChangeTwo(currentSolution.path, std::make_pair(frequency_zero[i], frequency_zero[i + 1]));
	}
	//std::cout << "Sciezka po: " << std::endl;
	//for (int i = 0; i < currentSolution.path.size(); i++)
	//{
	//	std::cout << currentSolution.path[i] << " ";
	//}
	//std::cout << std::endl;
	iterImprovement = 0;
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
	res.dist = getDistance(res.path);
	res.value = getValue(res);
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
	return res.dist;
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

std::vector<Tabo::Result> Tabo::createNeighb(std::vector<int> & path)
{
	std::vector<Tabo::Result> neigh;
	int i = 0;
	int j = 1;
	pair<int, int> positions;
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
				//std::cout << "Warunek aspiracji spelniony!" << std::endl;
				neigh.push_back(getResult(new_path, positions));
			}
		}
		else
		{
			neigh.push_back(getResult(new_path, positions));
		}

		j++;
		if (j == cities.size())
		{
			i++;
			j = i + 1;
		}
		if (i + 1 == cities.size())
		{
			break;
		}
	}
	return neigh;
}

std::vector<Tabo::Result> Tabo::createNeighbPartial(std::vector<int>& path)
{
	std::vector<Tabo::Result> neigh;
	int i = 0;
	int j = 1;
	int start_point = 0;
	int limit = this->_config.Neigh_Size;
	if (path.size() > limit)
	{
		start_point= rand() % (path.size() - limit);
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
				//std::cout << "Warunek aspiracji spelniony!" << std::endl;
				neigh.push_back(getResult(new_path, positions));
			}
		}
		else
		{
			neigh.push_back(getResult(new_path, positions));
		}

		j++;
		if (j  == cities.size() || j  == start_point + limit-1)
		{
			i++;
			j = i + 1;
		}
		if (i + 1 == cities.size() || i + 1 == start_point + limit-1)
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


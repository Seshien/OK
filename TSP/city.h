#pragma once
//#ifndef CITY.H
//#define CITY.H
//#endif // CITY
#include <iostream>
#include <math.h>
using namespace std;

class City {
private:
	int id;
	int coord_x;
	int coord_y;
	bool visited;
public:
	City();
	City(int new_id, int new_x, int new_y);
	void set_id(int new_id);
	void set_x(int new_x);
	void set_y(int new_y);
	void change_visited();
	int get_id();
	void print_city();
	double distance_between(City other);
};

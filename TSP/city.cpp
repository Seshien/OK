#include "city.h"


City::City() {
    id = -1;
    coord_x = 0;
    coord_y = 0;
    visited = false;
}
City::City(int new_id, int new_x, int new_y) {
    id = new_id;
    coord_x = new_x;
    coord_y = new_y;
    visited = false;
}
void City::set_id(int new_id) {
    id = new_id;
}
void City::set_x(int new_x) {
    coord_x = new_x;
}
void City::set_y(int new_y) {
    coord_y = new_y;
}
void City::change_visited() {
    visited = !visited;
}
int City::get_id() {
    return id;
}
void City::print_city() {
    cout << "Id: " << id << endl;
    cout << "X coord: " << coord_x << endl;
    cout << "Y ooord: " << coord_y << endl;
    cout << "Visited: " << visited << endl;
    cout << "###" << endl;
}
double City::distance_between(City other) {
    double dist = sqrt(pow((coord_x - other.coord_x), 2) + pow((coord_y - other.coord_y), 2));
    return dist;
}


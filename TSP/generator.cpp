#include "generator.h"


vector<City> generate_cities(int amount){
	vector<City> new_cities;
	ofstream save_file;
    string file_name;
    char choice;
    bool no_equal_cities = false;
    cout << "Czy chcesz wygenerowac plik? (Y|N): ";
    cin >> choice;
    if(choice == 'Y'){
            cout << "Podaj nazwe pliku do wygenerowania instancji: ";
        cin >> file_name;
        save_file.open(file_name.c_str());
        save_file << amount << endl;
    }
    for(int i = 0; i < amount; i++){
        int x_coord, y_coord;
		no_equal_cities = false;
        while(!no_equal_cities){
            no_equal_cities = true;
            x_coord = rand()%1000;
            y_coord = rand()%1000;
            for(int j = 0; j < i; j++){
                if(new_cities[j].distance_between(City(i+1, x_coord, y_coord)) == 0){
                    no_equal_cities = false;
                }
            }
            if(no_equal_cities == true){
				if (choice == 'Y')
					save_file << i + 1 << " " << x_coord << " " << y_coord << endl;
				new_cities.push_back(City(i + 1, x_coord, y_coord));
            }
        }

    }
    if(choice == 'Y')
        save_file.close();
	cout << new_cities.size() << endl;
    return new_cities;
}

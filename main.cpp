#include "includes.h"
#include "globals.h"

void makeInitialPopulation(void) { //Genera la poblacion inicial

};

int readConfigutarion(int argc, char **argv) { //Lee la configuracion entrega al ejecutar "make exe"
	instance_file = (char *)(argv[1]);
	results_file = (char *)(argv[2]);
	crossover_rate = atof(argv[3]);
	mutation_rate = atof(argv[4]);
	population_size = atof(argv[5]);
	max_iter = atoi(argv[6]);
	seed = atoi(argv[7]);
	debug = (bool)(argv[8]);
	if (debug) {
		cout << "Instancia: " << instance_file << endl;
		cout << "Resultados: " << results_file << endl;
		cout << "Crossover Rate: " << crossover_rate << endl;
		cout << "Mutation Rate: " << mutation_rate << endl;
		cout << "Population Size: " << population_size << endl;
		cout << "Max Iter: " << max_iter << endl;
		cout << "Seed: " << seed << endl;
 		cout << endl;
	}
	return 1;
}

int readInstance(void) { //Lee la informacion sobre la instancia del problema
	ifstream instance(instance_file);
	string line;
	istringstream stream;
	getline(instance, line); //Lee N H D
	stream.str(line);
	stream >> N >> H >> D;
	stream.clear();
	getline(instance, line); //Lee Tmax
	Tmax = stoi(line);
	getline(instance, line); //Lee la lista Td
	stream.str(line);
	int temp;
	while (stream >> temp) {
		Td.push_back(temp);
	};
	stream.clear();
	getline(instance, line); //Linea vacia
	while (getline(instance, line)) {
		float x;
		float y;
		int s;
		stream.str(line);
		stream >> x >> y >> s;
		if (s == 0) {
			Hotel newHotel(x, y);
			hotels.push_back(newHotel);
		} else {
			POI newPoi(s, x, y);
			pois.push_back(newPoi);
		};
		stream.clear();
	};
	if (debug) {
		cout << "N: " << N << endl;
		cout << "H: " << H << endl;
		cout << "D: " << D << endl;
		cout << "Tmax: " << Tmax << endl;
		cout << "Td:" << endl;
		for (int val : Td) {
			cout << "-" << val << endl;
		};
		cout << "Hoteles:" << endl;
		for (Hotel val : hotels) {
			cout << "-" << val << endl;
		};
		cout << "POIs:" << endl;
		for (POI val : pois) {
			cout << "-" << val << endl;
		};
		cout << endl;
	};
	return 1;
}

int main (int argc, char *argv[]){
	if(!readConfigutarion(argc, argv)){
		cout << "Error al leer la configuracion del problema." << endl;
		exit(1);
	}
	if(!readInstance()){
		cout << "Error al leer la instancia del problema." << endl;
		exit(1);
	}

}
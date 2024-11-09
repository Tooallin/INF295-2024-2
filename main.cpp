#include "includes.h"
#include "globals.h"

float calculateBudget(vector<int> chromosome) { //Calcula el presupuesto de un individuo
	float budget = 0;
	for (int i = 0; i < chromosome.size()-2; i++) {
		budget += distance_matrix[chromosome[i]][chromosome[i+1]];
	};
	return budget;
};

int calculateFitness(vector<int> chromosome) { //Calcula la aptitud de un individuo
	int fitness = 0;
	for (int i = 0; i < chromosome.size(); i++) {
		Vertex vertice = vertices[chromosome[i]];
		fitness += vertice.getS();
	};
	return fitness;
};

Individual makeRandomIndividual(void) { //Crea un individuo aleatorio
	uniform_int_distribution<> hotels_distrib(2, H+1);
	uniform_int_distribution<> pois_distrib(H+2, N-1);
	vector<int> chromosome;
	vector<int> hotels;
	vector<vector<int>> pois;
	hotels.push_back(0);
	for (int i = 0; i < D-1; i++) {
		hotels.push_back(hotels_distrib(generator));
	};
	hotels.push_back(1);
	vector<int> used;
	for (int i = 0; i < hotels.size()-1; i++) {
		int Ti = Td[i];
		int lastVertex = hotels[i];
		int nextVertex = hotels[i+1];
		vector<int> newPois;
		for (int j = 0; j < N; j++) {
			int newPoi = pois_distrib(generator);
			auto it = find(used.begin(), used.end(), newPoi);
			if (distance_matrix[lastVertex][newPoi] + distance_matrix[newPoi][nextVertex] <= Ti && it == used.end()) {
				used.push_back(newPoi);
				newPois.push_back(newPoi);
				Ti = Ti - distance_matrix[lastVertex][newPoi];
				lastVertex = newPoi;
			};
		};
		pois.push_back(newPois);
	};
	chromosome.push_back(0);
	for (int i = 0; i < hotels.size()-1; i++) {
		for (int j = 0; j < pois[i].size(); j++) {
			chromosome.push_back(pois[i][j]);
		};
		chromosome.push_back(hotels[i+1]);
	};
	int fitness = calculateFitness(chromosome);
	float budget = calculateBudget(chromosome);
	Individual newIndividual(chromosome, fitness, budget);
	return newIndividual;
};

int makeInitialPopulation(void) { //Genera la poblacion inicial
	for (int i = 0; i < population_size; i++) {
		Individual newIndividual = makeRandomIndividual();
		population.push_back(newIndividual);
	};
	if (debug) {
		for (Individual val : population) {
			cout << "Individuo: " << val << endl;
		};
	};
	return 1;
};

float calculateDistance(Vertex vertex1, Vertex vertex2) { //Calcula la distancia entres dos vertices
	vector<float> coords1 = vertex1.getCoords();
	vector<float> coords2 = vertex2.getCoords();
	float distance = sqrt(pow(coords1[0] - coords2[0], 2) + pow(coords1[1] - coords2[1], 2));
	return distance;
};

int calculateDistanceMatrix(void) { //Calcula la matriz de distancias
	for (int i = 0; i < vertices.size(); i++) {
		vector<float> newDistances;
		for (int j = 0; j < vertices.size(); j++) {
			newDistances.push_back(calculateDistance(vertices[i], vertices[j]));
		};
		distance_matrix.push_back(newDistances);
	};
	if (debug) {
		cout << "Matriz de Distancias:" << endl;
		for (int i = 0; i < vertices.size(); i++) {
			for (int j = 0; j < vertices.size(); j++) {
				cout << distance_matrix[i][j] << " - ";
			};
			cout << endl;
		};
		cout << endl;
	};
	return 1;
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
		cout << endl;
		cout << "Parametros de Ejecucion:" << endl;
		cout << "-Instancia: " << instance_file << endl;
		cout << "-Resultados: " << results_file << endl;
		cout << "-Crossover Rate: " << crossover_rate << endl;
		cout << "-Mutation Rate: " << mutation_rate << endl;
		cout << "-Population Size: " << population_size << endl;
		cout << "-Max Iter: " << max_iter << endl;
		cout << "-Seed: " << seed << endl;
 		cout << endl;
	}
	return 1;
};

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
	while (getline(instance, line)) { //Lee los Vertices
		float x;
		float y;
		int s;
		stream.str(line);
		stream >> x >> y >> s;
		if (s == 0) {
			Vertex newVertex(s, x, y, false);
			vertices.push_back(newVertex);
		} else {
			Vertex newVertex(s, x, y, true);
			vertices.push_back(newVertex);
		};
		stream.clear();
	};
	if (debug) {
		cout << "Parametros de la Instancia:" << endl;
		cout << "-N: " << N << endl;
		cout << "-H: " << H << endl;
		cout << "-D: " << D << endl;
		cout << "-Tmax: " << Tmax << endl;
		cout << "-Td: ";
		for (int val : Td) {
			cout << val << " ";
		};
		cout << endl;
		for (Vertex val : vertices) {
			if (!val.flag) {
				cout << "-Hotel: " << val << endl;
			} else {
				cout << "-POI: " << val << endl;
			};
		};
		cout << endl;
	};
	return 1;
};

int main (int argc, char *argv[]){
	if (!readConfigutarion(argc, argv)) {
		cout << "Error al leer la configuracion del problema." << endl;
		exit(1);
	};
	generator.seed(seed);
	if (!readInstance()) {
		cout << "Error al leer la instancia del problema." << endl;
		exit(1);
	};
	if (!calculateDistanceMatrix()) {
		cout << "Error al calcular la matriz de distancias." << endl;
		exit(1);
	};
	if (!makeInitialPopulation()) {
		cout << "Error al generar la poblacion inicial." << endl;
		exit(1);
	};
};
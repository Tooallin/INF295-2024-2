#include "includes.h"
#include "globals.h"

vector<Individual> crossoverPopulation() { //Realiza el cruzamiento entre individuos de la poblacion seleccionada
	vector<Individual> crossedPopulation;
	return crossedPopulation;
};

vector<Individual> mutatePopulation() { //Realiza mutaciones entre individuo de la poblacion cruzada
	vector<Individual> mutatedPopulation;
	return mutatedPopulation;
};

vector<Individual> newPopulation(vector<Individual> crossedPopulation, vector<Individual> mutatedPopulation) { //Elimina individuos hasta que el tamaño sea igual a population size
	vector<Individual> newPopulation;
	return newPopulation;
};

void updateBest() { //Actualiza el individuo con mayor aptitud de la poblacion
	return;
};

Individual rouletteWheel() { //Selecciona un individuo utilizando la metrica de ruleta
	float total_fitness = 0;
	vector<float> probabilities;
	vector<float> fitnesses;
	for (int i = 0; i < population_size; i++) {
		float temp_fitness = population[i].getFitness();
		total_fitness += temp_fitness;
		fitnesses.push_back(temp_fitness);
	};
	for (int i = 0; i < population_size; i++) {
		float temp_probability = fitnesses[i]/total_fitness;
		probabilities.push_back(temp_probability);
	};
	uniform_real_distribution<> prob_distrib(0.0, 1.0);
	float rand = prob_distrib(generator);
	for (int i = 0; i < population_size-1; i++) {
		if (probabilities[i] >= rand) {
			return population[i];
		};
	};
	return population[population_size-1];
};

vector<Individual> selectPopulation() { //Selecciona a una parte de la poblacion para cruzar/mutar
	vector<Individual> selectedPopulation;
	int i = 0;
	auto it = selectedPopulation.begin();
	while (i < population_size / 2) {
		Individual temp_selected = rouletteWheel();
		it = find(selectedPopulation.begin(), selectedPopulation.end(), temp_selected);
		if (it == selectedPopulation.end()) {
			selectedPopulation.push_back(temp_selected);
			i++;
			it = find(population.begin(), population.end(), temp_selected);
			population.erase(it);
		};
	};
	return selectedPopulation;
};

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

Individual makeRandomIndividual(uniform_int_distribution<> &hotels_distrib, uniform_int_distribution<> &pois_distrib) { //Crea un individuo aleatorio
	vector<int> chromosome;
	vector<int> hotels;
	vector<vector<int>> pois;
	vector<int> used;
	hotels.push_back(0);
	int idx = 1;
	while (idx < D) {
		int newHotel = hotels_distrib(generator);
		if (distance_matrix[hotels[idx-1]][newHotel] <= Td[idx-1]) {
			hotels.push_back(newHotel);
			idx++;
		};
	};
	hotels.push_back(1);
	for (int i = 0; i < hotels.size()-1; i++) {
		float Ti = Td[i];
		int lastVertex = hotels[i];
		int nextVertex = hotels[i+1];
		vector<int> newPois;
		for (int j = 0; j < N/D; j++) {
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

int makeInitialPopulation(uniform_int_distribution<> &hotels_distrib, uniform_int_distribution<> &pois_distrib) { //Genera la poblacion inicial
	for (int i = 0; i < population_size; i++) {
		Individual newIndividual = makeRandomIndividual(hotels_distrib, pois_distrib);
		population.push_back(newIndividual);
	};
	if (debug) {
		cout << "Poblacion Inicial:" << endl;
		for (Individual val : population) {
			cout << "-Individuo: " << val << endl;
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
		/*
		cout << "Matriz de Distancias:" << endl;
		for (int i = 0; i < vertices.size(); i++) {
			for (int j = 0; j < vertices.size(); j++) {
				cout << distance_matrix[i][j] << " - ";
			};
			cout << endl;
		};
		cout << endl;
		*/
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
	getline(instance, line);
	stream.str(line);
	stream >> N >> H >> D;
	stream.clear();
	getline(instance, line);
	Tmax = stoi(line);
	getline(instance, line);
	stream.str(line);
	float temp;
	while (stream >> temp) {
		Td.push_back(temp);
	}
	stream.clear();
	getline(instance, line);
	for (int i = 0; i < N+H; i++) {
		getline(instance, line);
		if (line.empty()) continue;
		float x, y;
		int s;
		stream.str(line);
		stream >> x >> y >> s;
		Vertex newVertex(s, x, y, s != 0);
		vertices.push_back(newVertex);
		stream.clear();
	};
	if (debug) {
		cout << "Parametros de la Instancia:" << endl;
		cout << "-N: " << N << endl;
		cout << "-H: " << H << endl;
		cout << "-D: " << D << endl;
		cout << "-Tmax: " << Tmax << endl;
		cout << "-Td: ";
		for (float val : Td) {
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
	uniform_int_distribution<> hotels_distrib(2, H+1);
	uniform_int_distribution<> pois_distrib(H+2, N-1);
	if (!calculateDistanceMatrix()) {
		cout << "Error al calcular la matriz de distancias." << endl;
		exit(1);
	};
	if (!makeInitialPopulation(hotels_distrib, pois_distrib)) {
		cout << "Error al generar la poblacion inicial." << endl;
		exit(1);
	};
	for (int iter = 0; iter < max_iter; iter++) {
		vector<Individual> selectedPopulation = selectPopulation();
		vector<Individual> crossedPopulation = crossoverPopulation();
		vector<Individual> mutatedPopulation = mutatePopulation();
		population = newPopulation(crossedPopulation, mutatedPopulation);
		updateBest();
	};
};
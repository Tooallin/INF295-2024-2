#include "includes.h"
#include "globals.h"

//Calcula el presupuesto de un individuo
float calculateBudget(vector<int> chromosome) {
	float budget = 0;
	for (int i = 0; i < chromosome.size()-1; i++) {
		budget += distance_matrix[chromosome[i]][chromosome[i+1]];
	};
	return budget;
};

//Calcula los presupuestos individuales de cada trip de un individuo
vector<float> calculateTripsBudgets(vector<int> chromosome) {
	vector<float> tripsBudgets;
	int idx = 0;
	for (int i = 0; i < D && idx < chromosome.size(); i++) {
		vector<int> trip;
		trip.push_back(chromosome[idx]);
		idx++;
		while (idx < chromosome.size() && chromosome[idx] > H + 1) {
			trip.push_back(chromosome[idx]);
			idx++;
		};
		float Ti = 0;
		for (int j = 0; j < trip.size() - 1; j++) {
			Ti += distance_matrix[trip[j]][trip[j+1]];
		};
		tripsBudgets.push_back(Ti);
	};
	return tripsBudgets;
};

//Calcula la aptitud de un individuo
int calculateFitness(vector<int> chromosome) {
	int fitness = 0;
	for (int i = 0; i < chromosome.size(); i++) {
		Node vertice = nodes[chromosome[i]];
		fitness += vertice.getS();
	};
	return fitness;
};

vector<Individual> onePointCrossover(Individual firstFather, Individual secondFather, uniform_int_distribution<> &hotels_distrib, mt19937 &generator) {
	vector<Individual> newChildrens;
	vector<int> firstFatherChromosome = firstFather.getChromosome();
	vector<int> secondFatherChromosome = secondFather.getChromosome();

	//Se encuentra un hotel en comun para utilizar como pivote
	bool stop = false;
	int pivot;
	auto it1 = firstFatherChromosome.end();
	auto it2 = secondFatherChromosome.end();
	while (!stop) {
		pivot = hotels_distrib(generator);
		it1 = find(firstFatherChromosome.begin(), firstFatherChromosome.end(), pivot);
		it2 = find(secondFatherChromosome.begin(), secondFatherChromosome.end(), pivot);
		if (it1 != firstFatherChromosome.end() && it2 != secondFatherChromosome.end() && pivot > 1) {
			stop = true;
		};
	};

	//Se crea el primer hijo
	vector<int> firstChildChromosome(secondFatherChromosome.begin(), next(it2));
	vector<int> firstSubvector(it1, firstFatherChromosome.end());
	firstChildChromosome.insert(firstChildChromosome.end(), firstSubvector.begin(), firstSubvector.end());
	int firstChildFitness = calculateFitness(firstChildChromosome);
	float firstChildBudget = calculateBudget(firstChildChromosome);
	vector<float> firstChildTripsBudgets = calculateTripsBudgets(firstChildChromosome);
	Individual firstChild(firstChildChromosome, firstChildFitness, firstChildBudget, firstChildTripsBudgets);
	newChildrens.push_back(firstChild);

	//Se crea el segundo hijo
	vector<int> secondChildChromosome(firstFatherChromosome.begin(), next(it1));
	vector<int> secondSubvector(it2, secondFatherChromosome.end());
	secondChildChromosome.insert(secondChildChromosome.end(), secondSubvector.begin(), secondSubvector.end());
	int secondChildFitness = calculateFitness(secondChildChromosome);
	float secondChildBudget = calculateBudget(secondChildChromosome);
	vector<float> secondChildTripsBudgets = calculateTripsBudgets(secondChildChromosome);
	Individual secondChild(secondChildChromosome, secondChildFitness, secondChildBudget, secondChildTripsBudgets);
	newChildrens.push_back(secondChild);

	return newChildrens;
};

vector<Individual> twoPointCrossover(Individual firstFather, Individual secondFather, uniform_int_distribution<> &hotels_distrib, mt19937 &generator) {
	vector<Individual> newChildrens;
	vector<int> firstFatherChromosome = firstFather.getChromosome();
	vector<int> secondFatherChromosome = secondFather.getChromosome();
	//Se encuentra el primer hotel en comun para utilizar como pivote
	bool stop = false;
	int pivot1;
	auto firstFatherit1 = firstFatherChromosome.end();
	auto secondFatherit1 = secondFatherChromosome.end();
	while (!stop) {
		pivot1 = hotels_distrib(generator);
		firstFatherit1 = find(firstFatherChromosome.begin(), firstFatherChromosome.end(), pivot1);
		secondFatherit1 = find(secondFatherChromosome.begin(), secondFatherChromosome.end(), pivot1);
		if (firstFatherit1 != firstFatherChromosome.end() && secondFatherit1 != secondFatherChromosome.end() && pivot1 > 1) {
			stop = true;
		};
	};
	//Se encuentra el segundo hotel en comun para utilizar como pivote
	stop = false;
	int pivot2;
	auto firstFatherit2 = firstFatherChromosome.end();
	auto secondFatherit2 = secondFatherChromosome.end();
	while (!stop) {
		pivot2 = hotels_distrib(generator);
		firstFatherit2 = find(firstFatherChromosome.begin(), firstFatherChromosome.end(), pivot2);
		secondFatherit2 = find(secondFatherChromosome.begin(), secondFatherChromosome.end(), pivot2);
		if (firstFatherit2 != firstFatherChromosome.end() && secondFatherit2 != secondFatherChromosome.end() && pivot2 > 1 && pivot2 != pivot1) {
			stop = true;
		};
	};
	if (firstFatherit1 > firstFatherit2 || secondFatherit1 > secondFatherit2) {
		swap(firstFatherit1, firstFatherit2);
		swap(secondFatherit1, secondFatherit2);
	}
	//Se crea el primer hijo
	vector<int> firstChildChromosome(secondFatherChromosome.begin(), secondFatherit1 + 1);
	vector<int> firstChildSubvector1(firstFatherit1, firstFatherit2 + 1);
	firstChildChromosome.insert(firstChildChromosome.end(), firstChildSubvector1.begin(), firstChildSubvector1.end());
	vector<int> firstChildSubvector2(secondFatherit2, secondFatherChromosome.end());
	firstChildChromosome.insert(firstChildChromosome.end(), firstChildSubvector2.begin(), firstChildSubvector2.end());
	int firstChildFitness = calculateFitness(firstChildChromosome);
	float firstChildBudget = calculateBudget(firstChildChromosome);
	vector<float> firstChildTripsBudgets = calculateTripsBudgets(firstChildChromosome);
	Individual firstChild(firstChildChromosome, firstChildFitness, firstChildBudget, firstChildTripsBudgets);
	newChildrens.push_back(firstChild);

	//Se crea el segundo hijo
	vector<int> secondChildChromosome(firstFatherChromosome.begin(), firstFatherit1 + 1);
	vector<int> secondChildSubvector1(secondFatherit1, secondFatherit2 + 1);
	secondChildChromosome.insert(secondChildChromosome.end(), secondChildSubvector1.begin(), secondChildSubvector1.end());
	vector<int> secondChildSubvector2(firstFatherit2, firstFatherChromosome.end());
	secondChildChromosome.insert(secondChildChromosome.end(), secondChildSubvector2.begin(), secondChildSubvector2.end());
	int secondChildFitness = calculateFitness(secondChildChromosome);
	float secondChildBudget = calculateBudget(secondChildChromosome);
	vector<float> secondChildTripsBudgets = calculateTripsBudgets(secondChildChromosome);
	Individual secondChild(secondChildChromosome, secondChildFitness, secondChildBudget, secondChildTripsBudgets);
	newChildrens.push_back(secondChild);

	return newChildrens;
};

//Realiza el cruzamiento entre individuos de la poblacion seleccionada
vector<Individual> crossoverIndividuals(Individual firstFather, Individual secondFather, uniform_int_distribution<> &hotels_distrib, uniform_int_distribution<> &crossover_distrib, mt19937 &generator) {
	vector<Individual> newChildrens;
	int rand = crossover_distrib(generator);
	vector<int> newChromosome;
	if (rand == 1) {
		vector<Individual> newChildrens = onePointCrossover(firstFather, secondFather, hotels_distrib, generator);
	} else if (rand == 2 && D > 2) {
		vector<Individual> newChildrens = twoPointCrossover(firstFather, secondFather, hotels_distrib, generator);
	} else {
	};
	return newChildrens;
};

//Realiza cruzamientos entre individuos de la poblacion seleccionada
vector<Individual> crossoverPopulation(vector<Individual> selectedPopulation, uniform_real_distribution<> &prob_distrib, uniform_int_distribution<> &hotels_distrib, uniform_int_distribution<> &crossover_distrib, mt19937 &generator) {
	vector<Individual> fathersPopulation = selectedPopulation;
	vector<Individual> childrensPopulation;
	for (int i = 0; i < fathersPopulation.size()-1; i++) {
		float rand = prob_distrib(generator);
		if (rand <= crossover_rate) {
			vector<Individual> newChildrens = crossoverIndividuals(fathersPopulation[i], fathersPopulation[i+1], hotels_distrib, crossover_distrib, generator);
			for (Individual children : newChildrens) {
				childrensPopulation.push_back(children);
			};
		};
	};

	vector<Individual> crossedPopulation = childrensPopulation;
	for (Individual father : fathersPopulation) {
		crossedPopulation.push_back(father);
	};

	//Debug
	if (debug) {
		cout << "Poblacion Cruzada:" << endl;
		for (Individual val : crossedPopulation) {
			cout << "-Individuo Cruzado: " << val << endl;
		};
		cout << endl;
	};
	return crossedPopulation;
};


//Mutacion que busca añadir un poi al tour
vector<int> addPoiMutation(Individual individual, uniform_int_distribution<> &pois_distrib, mt19937 &generator) {
	vector<int> newChromosome = individual.getChromosome();
	bool stop = false;
	int addedPoi;
	auto it = newChromosome.end();
	while (!stop) {
		addedPoi = pois_distrib(generator);
		it = find(newChromosome.begin(), newChromosome.end(), addedPoi);
		if (it == newChromosome.end()) {
			stop = true;
		};
	};
	int idx = 0;
	for (int i = 0; i < D; i++) {
		vector<int> trip;
		trip.push_back(newChromosome[idx]);
		idx++;
		while (newChromosome[idx] > H+1) {
			trip.push_back(newChromosome[idx]);
			idx++;
		};
		trip.push_back(newChromosome[idx]);
		float Ti = Td[i];
		for (int j = 0; j < trip.size()-1; j++) {
			Ti = Ti - distance_matrix[trip[j]][trip[j+1]];
		};
		for (int j = 0; j < trip.size(); j++) {
			if (distance_matrix[trip[j]][addedPoi] + distance_matrix[addedPoi][trip[j+1]] <= Ti) {
				it = find(newChromosome.begin(), newChromosome.end(), trip[j]);
				newChromosome.insert(it+1, addedPoi);
				return newChromosome;
			};
		};
	};
	return newChromosome;
};

//Mutacion que busca quitar un poi al tour
vector<int> removePoiMutation(Individual individual, uniform_int_distribution<> &pois_distrib, mt19937 &generator) {
	vector<int> newChromosome = individual.getChromosome();
	bool stop = false;
	int removedPoi;
	auto it = newChromosome.end();
	while (!stop) {
		removedPoi = pois_distrib(generator);
		it = find(newChromosome.begin(), newChromosome.end(), removedPoi);
		if (it != newChromosome.end()) {
			stop = true;
		};
	};
	if ((it != newChromosome.begin() && *(it-1) > H+1) || (it != newChromosome.end() && *(it+1) > H+1)) {
		newChromosome.erase(it);
	};
	return newChromosome;
};

//Mutacion que busca intercambiar un poi por uno no utilizado
vector<int> exchangePoiMutation(Individual individual, uniform_int_distribution<> &pois_distrib, mt19937 &generator) {
	vector<int> newChromosome = individual.getChromosome();
	bool stop = false;
	int exchangedPoi;
	auto it = newChromosome.end();
	while (!stop) {
		exchangedPoi = pois_distrib(generator);
		it = find(newChromosome.begin(), newChromosome.end(), exchangedPoi);
		if (it == newChromosome.end()) {
			stop = true;
		};
	};
	for (int i = 1; i < newChromosome.size()-1; i++) {
		if (newChromosome[i] > H+1) {
			if (distance_matrix[newChromosome[i-1]][exchangedPoi] + distance_matrix[exchangedPoi][newChromosome[i+1]] <= distance_matrix[newChromosome[i-1]][exchangedPoi] + distance_matrix[exchangedPoi][newChromosome[i+1]]) {
				newChromosome[i] = exchangedPoi;
				break;
			};
		};
	};
	return newChromosome;
};

//Mutacion que busca intercambiar un hotel por uno no utilizado
vector<int> exchangeHotelMutation(Individual individual, uniform_int_distribution<> &hotels_distrib, mt19937 &generator) {
	vector<int> newChromosome = individual.getChromosome();
	bool stop = false;
	int exchangedHotel;
	auto it = newChromosome.end();
	while (!stop) {
		exchangedHotel = hotels_distrib(generator);
		it = find(newChromosome.begin(), newChromosome.end(), exchangedHotel);
		if (it == newChromosome.end()) {
			stop = true;
		};
	};
	for (int i = 1; i < newChromosome.size()-1; i++) {
		if (newChromosome[i] <= H+1) {
			if (distance_matrix[newChromosome[i-1]][exchangedHotel] + distance_matrix[exchangedHotel][newChromosome[i+1]] <= distance_matrix[newChromosome[i-1]][exchangedHotel] + distance_matrix[exchangedHotel][newChromosome[i+1]]) {
				newChromosome[i] = exchangedHotel;
				break;
			};
		};
	};
	return newChromosome;
};

//Elige aleatoriamente un tipo de mutacion
void mutateIndividual(Individual &individual, uniform_int_distribution<> &hotels_distrib, uniform_int_distribution<> &pois_distrib, uniform_int_distribution<> &mutation_distrib, mt19937 &generator) {
	int rand = mutation_distrib(generator);
	vector<int> newChromosome;
	if (rand == 1) {
		newChromosome = addPoiMutation(individual, pois_distrib, generator);
	} else if (rand == 2) {
		newChromosome = removePoiMutation(individual, pois_distrib, generator);
	} else if (rand == 3) {
		newChromosome = exchangePoiMutation(individual, pois_distrib, generator);
	} else if (rand == 4 && H+2 > D+1) {
		newChromosome = exchangeHotelMutation(individual, hotels_distrib, generator);
	} else {
		newChromosome = individual.getChromosome();
	};
	int newFitness = calculateFitness(newChromosome);
	float newBudget = calculateBudget(newChromosome);
	vector<float> newTripsBudgets = calculateTripsBudgets(newChromosome);
	individual.updateIndividual(newChromosome, newFitness, newBudget, newTripsBudgets);
	return;
}

//Realiza mutaciones entre individuo de la poblacion cruzada
vector<Individual> mutatePopulation(vector<Individual> crossedPopulation, uniform_real_distribution<> &prob_distrib, uniform_int_distribution<> &pois_distrib, uniform_int_distribution<> &hotels_distrib, uniform_int_distribution<> &mutation_distrib, mt19937 &generator) {
	vector<Individual> mutatedPopulation = crossedPopulation;
	for (int i = 0; i < mutatedPopulation.size(); i++) {
		float rand = prob_distrib(generator);
		if (rand <= mutation_rate) {
			mutateIndividual(mutatedPopulation[i], hotels_distrib, pois_distrib, mutation_distrib, generator);
		};
	};

	//Debug
	if (debug) {
		cout << "Poblacion Mutada:" << endl;
		for (Individual val : mutatedPopulation) {
			cout << "-Individuo Mutado: " << val << endl;
		};
		cout << endl;
	};
	return mutatedPopulation;
};

//Elimina individuos hasta que el tamaño sea igual a population size
void updatePopulation(vector<Individual> &population, vector<Individual> mutatedPopulation) {
	for (int i = 0; i < mutatedPopulation.size(); i++) {
		population.push_back(mutatedPopulation[i]);
	};
	while (population.size() > population_size) {
		Individual worst = population[0];
		for (int i = 1; i < population.size(); i++) {
			if (population[i].getFitness() < worst.getFitness()) {
				worst = population[i];
			};
		};
		auto it = find(population.begin(), population.end(), worst);
		population.erase(it);
	};
	return;
};

//Actualiza el individuo con mayor aptitud de la poblacion
void updateBest(Individual &best, vector<Individual> population) {
	for (int i = 0; i < population.size(); i++) {
		if (best.getFitness() < population[i].getFitness()) {
			best = population[i];
		};
	};
	return;
};

//Selecciona un individuo utilizando la metrica de ruleta
Individual rouletteWheel(float rand) {
	float total_fitness = 0;
	vector<float> probabilities;
	vector<float> fitnesses;
	for (int i = 0; i < population_size; i++) {
		float temp_fitness = population[i].getFitness();
		total_fitness += temp_fitness;
		fitnesses.push_back(temp_fitness);
	};
	float sum = 0;
	for (int i = 0; i < population_size; i++) {
		float temp_probability = (fitnesses[i] + sum)/total_fitness;
		sum += fitnesses[i];
		probabilities.push_back(temp_probability);
	};
	for (int i = 0; i < population_size-1; i++) {
		if (probabilities[i] >= rand) {
			return population[i];
		};
	};
	return population[population_size-1];
};

//Selecciona a una parte de la poblacion para cruzar/mutar
vector<Individual> selectPopulation(vector<Individual> population, uniform_real_distribution<> &prob_distrib, mt19937 &generator) {
	//Selecciona un individuo aleatoriamente utilizando ruleta
	vector<Individual> selectedPopulation;
	int i = 0;
	auto it = selectedPopulation.begin();
	while (i < population_size / 2) {
		float rand = prob_distrib(generator);
		Individual temp_selected = rouletteWheel(rand);
		selectedPopulation.push_back(temp_selected);
		i++;
	};

	//Debug
	if (debug) {
		cout << "Poblacion Seleccionada:" << endl;
		for (Individual val : selectedPopulation) {
			cout << "-Individuo Seleccionado: " << val << endl;
		};
		cout << endl;
	};
	return selectedPopulation;
};

//Crea un individuo aleatorio
Individual makeRandomIndividual(uniform_int_distribution<> &hotels_distrib, uniform_int_distribution<> &pois_distrib, mt19937 &generator) {
	//Variables auxiliares para generar el individuo
	vector<int> chromosome;
	vector<int> hotels;
	vector<vector<int>> tripsPois;
	vector<int> usedHotels;
	vector<int> usedPois;

	//Genera una secuencia de hoteles factibles
	hotels.push_back(0);
	int idx = 1;
	while (idx < D) {
		int newHotel = hotels_distrib(generator);
		auto it = find(usedHotels.begin(), usedHotels.end(), newHotel);
		if (distance_matrix[hotels[idx-1]][newHotel] <= Td[idx-1] && it == usedHotels.end()) {
			hotels.push_back(newHotel);
			usedHotels.push_back(newHotel);
			idx++;
		};
	};
	hotels.push_back(1);

	//Rellena cada trip con pois hasta que no haya ningun trip vacio
	for (int i = 0; i < hotels.size()-1; i++) {
		float Ti = Td[i];
		int lastNode = hotels[i];
		int nextNode = hotels[i+1];
		vector<int> newPois;
		for (int j = 0; j < N; j++) {
			int newPoi = pois_distrib(generator);
			auto it = find(usedPois.begin(), usedPois.end(), newPoi);
			if (distance_matrix[lastNode][newPoi] + distance_matrix[newPoi][nextNode] <= Ti && it == usedPois.end()) {
				newPois.push_back(newPoi);
				Ti = Ti - distance_matrix[lastNode][newPoi];
				usedPois.push_back(newPoi);
				lastNode = newPoi;
			};
		};
		tripsPois.push_back(newPois);
	};

	//Asegura que no haya ningun trip vacio
	for (int i = 0; i < hotels.size()-1; i++) {

		//Trata de ingresar un poi que no haya sido utilizado
		if (tripsPois[i].size() == 0) {
			float Ti = Td[i];
			for (int newPoi = H+2; newPoi <= N-1; newPoi++) {
				auto it = find(usedPois.begin(), usedPois.end(), newPoi);
				if (distance_matrix[hotels[i]][newPoi] + distance_matrix[newPoi][hotels[i+1]] <= Ti && it == usedPois.end()) {
					tripsPois[i].push_back(newPoi);
					usedPois.push_back(newPoi);
				};
			};
		};

		//Trata de cambiar un poi que haya sido utilizado en otro trip
		if (tripsPois[i].size() == 0) {
			float Ti = Td[i];
			for (auto& usedPoi : usedPois) {
				if (distance_matrix[hotels[i]][usedPoi] + distance_matrix[usedPoi][hotels[i+1]] <= Ti) {
					for (auto& subpois : tripsPois) {
						subpois.erase(std::remove(subpois.begin(), subpois.end(), usedPoi), subpois.end());
					};
					tripsPois[i].push_back(usedPoi);
					break;
				};
			};
			
		};
	};

	//Traduce el tour generado al cromosoma del individuo
	chromosome.push_back(0);
	for (int i = 0; i < hotels.size()-1; i++) {
		for (int j = 0; j < tripsPois[i].size(); j++) {
			chromosome.push_back(tripsPois[i][j]);
		};
		chromosome.push_back(hotels[i+1]);
	};

	//Calcula la aptitud y el presupuesto utilizado
	int fitness = calculateFitness(chromosome);
	float budget = calculateBudget(chromosome);
	vector<float> tripsBudgets = calculateTripsBudgets(chromosome);

	//Crea al individuo
	Individual newIndividual(chromosome, fitness, budget, tripsBudgets);
	return newIndividual;
};

//Genera la poblacion inicial
int makeInitialPopulation(uniform_int_distribution<> &hotels_distrib, uniform_int_distribution<> &pois_distrib, mt19937 &generator) {
	//Crea individuos aleatorios hasta tener la poblacion completa
	for (int i = 0; i < population_size; i++) {
		Individual newIndividual = makeRandomIndividual(hotels_distrib, pois_distrib, generator);
		population.push_back(newIndividual);
	};

	//Debug
	if (debug) {
		cout << "Poblacion Inicial:" << endl;
		for (Individual val : population) {
			cout << "-Individuo: " << val << endl;
		};
		cout << endl;
	};
	return 1;
};

//Calcula la distancia entres dos nodos
float calculateDistance(Node node1, Node node2) {
	vector<float> coords1 = node1.getCoords();
	vector<float> coords2 = node2.getCoords();
	float distance = sqrt(pow(coords1[0] - coords2[0], 2) + pow(coords1[1] - coords2[1], 2));
	return distance;
};

//Calcula la matriz de distancias
int calculateDistanceMatrix(void) {
	//Calcula la distancia para cada nodo i hasta cada nodo j
	for (int i = 0; i < nodes.size(); i++) {
		vector<float> newDistances;
		for (int j = 0; j < nodes.size(); j++) {
			newDistances.push_back(calculateDistance(nodes[i], nodes[j]));
		};
		distance_matrix.push_back(newDistances);
	};

	//Debug
	if (debug) {
		cout << "Matriz de Distancias:" << endl;
		for (int i = 0; i < nodes.size(); i++) {
			for (int j = 0; j < nodes.size(); j++) {
				cout << distance_matrix[i][j] << " - ";
			};
			cout << endl;
		};
		cout << endl;
	};
	return 1;
};

//Lee la configuracion entrega al ejecutar "make exe"
int readConfigutarion(int argc, char **argv) {
	//Lee el nombre del archivo de la instancia
	instance_file = (char *)(argv[1]);

	//Lee la tasa de cruzamiento
	crossover_rate = atof(argv[2]);

	//Lee la tasa de mutacion
	mutation_rate = atof(argv[3]);

	//Lee el tamaño de la poblacion
	population_size = atof(argv[4]);

	//Lee el numero maximo de iteraciones
	max_iter = atoi(argv[5]);

	//Lee la semilla
	seed = atoi(argv[6]);

	//Lee la opcion debug
	debug = bool(argv[7]);

	//Debug
	if (debug) {
		cout << endl;
		cout << "Parametros de Ejecucion:" << endl;
		cout << "-Instancia: " << instance_file << endl;
		cout << "-Crossover Rate: " << crossover_rate << endl;
		cout << "-Mutation Rate: " << mutation_rate << endl;
		cout << "-Population Size: " << population_size << endl;
		cout << "-Max Iter: " << max_iter << endl;
		cout << "-Seed: " << seed << endl;
		cout << endl;
	}
	return 1;
};

//Lee la informacion sobre la instancia del problema
int readInstance(void) {
	//Abre el archivo correspondiente a la instancia
	ifstream instance(instance_file);
	string line;
	istringstream stream;

	//Lee N, H y D
	getline(instance, line);
	stream.str(line);
	stream >> N >> H >> D;
	stream.clear();

	//Lee Tmax
	getline(instance, line);
	Tmax = stoi(line);

	//Lee Td
	getline(instance, line);
	stream.str(line);
	float temp;
	while (stream >> temp) {
		Td.push_back(temp);
	}
	stream.clear();

	//Lee los nodos
	getline(instance, line);
	for (int i = 0; i < N+H; i++) {
		getline(instance, line);
		if (line.empty()) continue;
		float x, y;
		int s;
		stream.str(line);
		stream >> x >> y >> s;
		Node newNode(s, x, y, s != 0);
		nodes.push_back(newNode);
		stream.clear();
	};

	//Debug
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
		for (Node val : nodes) {
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
	//Lee la configuracion para la ejecucion
	if (!readConfigutarion(argc, argv)) {
		cout << "Error al leer la configuracion del problema." << endl;
		exit(1);
	};

	//Define la semilla para la ejecucion
	generator.seed(seed);

	//Lee los parametros de la instancia
	if (!readInstance()) {
		cout << "Error al leer la instancia del problema." << endl;
		exit(1);
	};

	//Crea distribuciones para elegir aleatoriamente hoteles, pois, probabilidades, mutacion
	uniform_int_distribution<> hotels_distrib(2, H+1);
	uniform_int_distribution<> pois_distrib(H+2, N-1);
	uniform_real_distribution<> prob_distrib(0.0, 1.0);
	uniform_int_distribution<> mutation_distrib(1, 4);
	uniform_int_distribution<> crossover_distrib(1, 2);

	//Calcula la matriz de distancias
	if (!calculateDistanceMatrix()) {
		cout << "Error al calcular la matriz de distancias." << endl;
		exit(1);
	};

	//Genera la poblacion inicial
	if (!makeInitialPopulation(hotels_distrib, pois_distrib, generator)) {
		cout << "Error al generar la poblacion inicial." << endl;
		exit(1);
	};

	//Encuentra el mejor inidividuo de la poblacion inicial
	Individual best;
	updateBest(best, population);

	//Empieza la ejecucion del algoritmo
	for (int iter = 0; iter < max_iter; iter++) {
		//Se cae al seleccionar la poblacion
		vector<Individual> selectedPopulation = selectPopulation(population, prob_distrib, generator);
		//Se cae al cruzar
		vector<Individual> crossedPopulation = crossoverPopulation(population, prob_distrib, hotels_distrib, crossover_distrib, generator);
		//Se cae al mutar
		vector<Individual> mutatedPopulation = mutatePopulation(selectedPopulation, prob_distrib, pois_distrib, hotels_distrib, mutation_distrib, generator);
		//Se cae al unir la poblacion
		updatePopulation(population, mutatedPopulation);
		//Se cae al actualizar el mejor
		updateBest(best, population);
	};
	cout << best << endl;
};
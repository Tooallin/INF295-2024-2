#ifndef extern

char *instance_file; //Nombre de la instancia
char *results_file; //Nombre del archivo de resultados
float crossover_rate; //Probabilidad de cruzamiento
float mutation_rate; //Probabilidad de mutacion
int population_size; //Tamaño de la poblacion
int max_iter; //Numero maximo de iteraciones
int seed; //Semilla
bool debug; //Debug

int N; //Numero de POIs + Hoteles
int H; //Numero de Hoteles
int D; //Numero de Trips
int Tmax; //Presupuesto del Tour
vector<int> Td; //Presupuestos de los Trips

vector<vector<float>> distance_matrix; //Matriz de distancias
mt19937 generator; //Generador

#endif

//Vertice
class Vertex {
friend ostream &operator<<(ostream &, const Vertex &);

public:
	bool flag; //Flag que indica si es un Hotel (False) o POI (True)
	int s;
	vector<float> coords;

	Vertex(int s, float x, float y, bool flag);
	bool getFlag() const;
	int getS() const;
	vector<float> getCoords() const;
};
//Constructor
Vertex::Vertex(int s, float x, float y, bool flag) {
	this->flag = flag;
	this->s = s;
	this->coords = {x,y};
};
//Obtener la Flag del Vertice
bool Vertex::getFlag() const {
	return this->flag;
};
//Obtener S del Vertice
int Vertex::getS() const {
	return this->s;
};
//Obtener las coordenadas del Vertice
vector<float> Vertex::getCoords() const {
	return this->coords;
};
//Operador para imprimir por pantalla el Vertice
ostream &operator<<(ostream &output, const Vertex &vertex) {
	//output << "[" << &vertex << "]";
	output << "[" << vertex.s << "] ";
	for (int i = 0; i < 2; i++) {
		output << vertex.coords[i] << " ";
	};
	return output;
};

#ifndef extern

vector<Vertex> vertices; //Vertices de la instancia

#endif

//Individuo
class Individual {
friend ostream &operator<<(ostream &, const Individual &);

public:
	vector<int> chromosome;
	int fitness;
	float budget;

	Individual(vector<int> chromosome, int fitness, float budget);
	int getFitness() const;
	float getBudget() const;
	void updateFitnessAndBudget();
	void mutateIndividual();
	void removePoiMutation();
	void changePoiMutation();
	void changeHotelMutation();
};
//Constructor
Individual::Individual(vector<int> chromosome, int fitness, float budget) {
	this->chromosome = chromosome;
	this->fitness = fitness;
	this->budget = budget;
};
//Obtener la aptitud del Individuo
int Individual::getFitness() const {
	return this->fitness;
};
//Obtener el presupuesto del Individuo
float Individual::getBudget() const {
	return this->budget;
};
//Actualiza la aptitud y el presupuesto del Individuo
void Individual::updateFitnessAndBudget() {
	float newBudget = 0;
	for (int i = 0; i < chromosome.size()-2; i++) {
		newBudget += distance_matrix[chromosome[i]][chromosome[i+1]];
	};
	int newFitness = 0;
	for (int i = 0; i < chromosome.size(); i++) {
		Vertex vertice = vertices[chromosome[i]];
		newFitness += vertice.getS();
	};
	this->budget = newBudget;
	this->fitness = newFitness;
	return;
};
//Elige la mutacion por parte de un Individuo
void Individual::mutateIndividual() {
	uniform_int_distribution<> mutation_distrib(1, 3);
	int rand = mutation_distrib(generator);
	if (rand == 1) {
		removePoiMutation();
	} else if (rand == 2) {
		changePoiMutation();
	} else {
		changeHotelMutation();
	};
	return;
};
//Remueve un POI del cromosoma
void Individual::removePoiMutation() {
	uniform_int_distribution<> vertex_distrib(0, chromosome.size()-1);
	bool valid = false;
	int poi = 0;
	auto it = chromosome.begin();
	while (!valid) {
		poi = chromosome[vertex_distrib(generator)];
		it = find(chromosome.begin(), chromosome.end(), poi);
		if (poi >= H+2) {
			valid = true;
		};
	};
	chromosome.erase(it);
	return;
};
//Intercambia un POI dentro del tour por uno no incluido que disminuya el presupuesto
void Individual::changePoiMutation() {
	uniform_int_distribution<> pois_distrib(H+2, N-1);
	bool valid = false;
	int newPoi = 0;
	auto it = chromosome.begin();
	while (!valid) {
		newPoi = pois_distrib(generator);
		it = find(chromosome.begin(), chromosome.end(), newPoi);
		if (it == chromosome.end()) {
			valid = true;
		};
	};
	for (int i = 1; i < chromosome.size()-1; i++) {
		int vertex = chromosome[i];
		if (vertex >= H+2) {
			if (distance_matrix[i-1][newPoi] + distance_matrix[newPoi][i+1] < distance_matrix[i-1][vertex] + distance_matrix[vertex][i+1]) {
				chromosome[i] = newPoi;
				updateFitnessAndBudget();
				break;
			};
		};
	};
	return;
};
//Intercambia un Hotel dentro del tour por uno no incluido que disminuya el presupuesto
void Individual::changeHotelMutation() {
	uniform_int_distribution<> hotel_distrib(2, H+1);
	bool valid = false;
	int newHotel = 0;
	auto it = chromosome.begin();
	while (!valid) {
		newHotel = hotel_distrib(generator);
		it = find(chromosome.begin(), chromosome.end(), newHotel);
		if (it == chromosome.end()) {
			valid = true;
		};
	};
	for (int i = 1; i < chromosome.size()-1; i++) {
		int vertex = chromosome[i];
		if (vertex >= 2 && vertex <= H+1) {
			if (distance_matrix[i-1][newHotel] + distance_matrix[newHotel][i+1] < distance_matrix[i-1][vertex] + distance_matrix[vertex][i+1]) {
				chromosome[i] = newHotel;
				updateFitnessAndBudget();
				break;
			};
		};
	};
	return;
};
//Operador para imprimir por pantalla el Individuo
ostream &operator<<(ostream &output, const Individual &individual) {
	//output << "[" << &individual << "]";
	output << "[" << individual.fitness << "] ";
	output << "[" << individual.budget << "] ";
	for (int i = 0; i < individual.chromosome.size(); i++) {
		output << individual.chromosome[i] << " ";
	};
	return output;
};

#ifndef extern

vector<Individual> population; //Poblacion

#endif

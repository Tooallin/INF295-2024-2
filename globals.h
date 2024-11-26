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
vector<float> Td; //Presupuestos de los Trips

vector<vector<float>> distance_matrix; //Matriz de distancias
mt19937 generator; //Generador

#endif

//Vertice
class Node {
friend ostream &operator<<(ostream &, const Node &);

public:
	bool flag; //Flag que indica si es un Hotel (False) o POI (True)
	int s;
	vector<float> coords;

	Node(int s, float x, float y, bool flag);
	bool getFlag() const;
	int getS() const;
	vector<float> getCoords() const;
};

//Constructor
Node::Node(int s, float x, float y, bool flag) {
	this->flag = flag;
	this->s = s;
	this->coords = {x,y};
};

//Obtener la Flag del Vertice
bool Node::getFlag() const {
	return this->flag;
};

//Obtener S del Vertice
int Node::getS() const {
	return this->s;
};

//Obtener las coordenadas del Vertice
vector<float> Node::getCoords() const {
	return this->coords;
};

//Operador para imprimir por pantalla el Vertice
ostream &operator<<(ostream &output, const Node &node) {
	output << "[" << node.s << "] ";
	for (int i = 0; i < 2; i++) {
		output << node.coords[i] << " ";
	};
	return output;
};

#ifndef extern

vector<Node> nodes; //Vertices de la instancia

#endif

//Individuo
class Individual {
friend ostream &operator<<(ostream &, const Individual &);

public:
	vector<int> chromosome;
	int fitness;
	float budget;
	vector<float> tripsBudgets;

	Individual();
	Individual(vector<int> chromosome, int fitness, float budget, vector<float> tripsBudgets);
	vector<int> getChromosome() const;
	int getFitness() const;
	float getBudget() const;
	vector<float> getTripsBudgets() const;
	void updateIndividual(vector<int> newChromosome, int newFitness, float newBudget, vector<float> tripsBudgets);
	bool operator==(const Individual &rhs) const;
};

//Constructor vacio
Individual::Individual() {
	vector<int> chromosome;
	vector<float> tripsBudgets;
	this->chromosome = chromosome;
	this->fitness = 0;
	this->budget = 0;
	this->tripsBudgets;
};

//Constructor
Individual::Individual(vector<int> chromosome, int fitness, float budget, vector<float> tripsBudgets) {
	this->chromosome = chromosome;
	this->fitness = fitness;
	this->budget = budget;
	this->tripsBudgets = tripsBudgets;
};

//Obtener el cromosoma del Individuo
vector<int> Individual::getChromosome() const {
	return this->chromosome;
};

//Obtener la aptitud del Individuo
int Individual::getFitness() const {
	return this->fitness;
};

//Obtener el presupuesto del Individuo
float Individual::getBudget() const {
	return this->budget;
};

//Obtiene los presupuestos utilizados para cada Trip
vector<float> Individual::getTripsBudgets() const {
	return this->tripsBudgets;
};

//Actualiza la aptitud y el presupuesto del Individuo
void Individual::updateIndividual(vector<int> newChromosome, int newFitness, float newBudget, vector<float> newTripsBudgets) {
	this->chromosome = newChromosome;
	this->fitness = newFitness;
	this->budget = newBudget;
	this->tripsBudgets = newTripsBudgets;
	return;
};

//Compara si es que dos individuos son iguales
bool Individual::operator==(const Individual &rhs) const {
	if (this->chromosome == rhs.chromosome) {
		return true;
	};
	return false;
};

//Operador para imprimir por pantalla el Individuo
ostream &operator<<(ostream &output, const Individual &individual) {
	for (int i = 0; i < individual.chromosome.size()-2; i++) {
		if (individual.chromosome[i] <= H+1) {
			output << "H" << individual.chromosome[i] << " ⮕ ";
		} else {
			int poi = individual.chromosome[i]-H+1;
			output << poi << " ⮕ ";
		};
	};
	output << "H" << individual.chromosome[individual.chromosome.size()-1] << endl;
	output << individual.fitness << endl;
	vector<float> tripsBudgets = individual.getTripsBudgets();
	int idx = 0;
	for (int i = 0; i < tripsBudgets.size()-1; i++) {
		idx = i+1;
		output << "Trip " << idx << ": " << tripsBudgets[i] << " - ";
	};
	idx = tripsBudgets.size();
	output << "Trip " << idx << ": " << tripsBudgets[tripsBudgets.size()-1] << endl;
	return output;
};

#ifndef extern

vector<Individual> population; //Poblacion

#endif

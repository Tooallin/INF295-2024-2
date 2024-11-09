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

vector<vector<float>> distance_matrix;

#endif

//Individuo
class Individual {
friend ostream &operator<<(ostream &, const Individual &);

public:
	vector<int> chromosome;
	int fitness;

	Individual(vector<int> chromosome, int fitness);
	int getFitness();
};
//Constructor
Individual::Individual(vector<int> chromosome, int fitness) {
	this->chromosome = chromosome;
	this->fitness = fitness;
};
//Operador para imprimir por pantalla el Individuo
ostream &operator<<(ostream &output, const Individual &individual) {
	//output << "[" << &individual << "]";
	output << "[" << individual.fitness << "]";
	for (int i = 0; i < individual.chromosome.size(); i++) {
		output << individual.chromosome[i] << " ";
	};
	return output;
};

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

vector<Individual> population; //Poblacion
vector<Vertex> vertices; //Vertices de la instancia

#endif
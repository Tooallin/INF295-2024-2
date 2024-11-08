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

vector<vector<int>> distance_matrix;

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

//POI
class POI {
friend ostream &operator<<(ostream &, const POI &);

public:
	int s;
	vector<float> coords;

	POI(int s, float x, float y);
	int getS() const;
	vector<float> getCoords() const;
};
//Constructor
POI::POI(int s, float x, float y) {
	this->s = s;
	this->coords = {x,y};
};
//Obtener S del POI
int POI::getS() const {
	return this->s;
};
//Obtener las coordenadas del POI
vector<float> POI::getCoords() const {
	return this->coords;
};
//Operador para imprimir por pantalla el POI
ostream &operator<<(ostream &output, const POI &poi) {
	//output << "[" << &poi << "]";
	output << "[" << poi.s << "] ";
	for (int i = 0; i < 2; i++) {
		output << poi.coords[i] << " ";
	};
	return output;
};

//Hotel
class Hotel {
friend ostream &operator<<(ostream &, const Hotel &);

public:
	vector<float> coords;

	Hotel(float x, float y);
	vector<float> getCoords() const;
};
//Constructor
Hotel::Hotel(float x, float y) {
	this->coords = {x,y};
};
//Obtener las coordenadas del Hotel
vector<float> Hotel::getCoords() const {
	return this->coords;
};
//Operador para imprimir por pantalla el Hotel
ostream &operator<<(ostream &output, const Hotel &hotel) {
	//output << "[" << &hotel << "]";
	for (int i = 0; i < 2; i++) {
		output << hotel.coords[i] << " ";
	};
	return output;
};

#ifndef extern

vector<Individual> population; //Poblacion
vector<POI> pois; //POIs de la instancia
vector<Hotel> hotels; //Hoteles de la instancia

#endif
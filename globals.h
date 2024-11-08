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

#endif

class POI {
	int s;
	list<float> coords;

public:

	POI(int s, float x, float y) {
		this->s = s;
		this->coords = {x,y};
	};

	int getS() const {
		return this->s;
	};

	list<float> getCoords() const {
		return this->coords;
	};
	
};

class Hotel {
	list<float> coords;

public:

	Hotel(float x, float y) {
		this->coords = {x,y};
	};

	list<float> getCoords() const {
		return this->coords;
	};

};

#ifndef extern

vector<POI> pois;
vector<Hotel> hotels;

#endif
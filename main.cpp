#include "includes.h"
#include "globals.h"

int readConfigutarion(int argc, char **argv) {
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
		cout << "------------------------------------------------" << endl;
 		cout << endl;
	}
	return 1;
}

int main (int argc, char *argv[]){
	if(!readConfigutarion(argc, argv)){
		cout << "Error al leer la configuracion del problema." << endl;
		exit(1);
	}

}
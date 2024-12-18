flags = -Wno-deprecated -O2
comp = g++
prog  = OPHS

#Parametros default
instancia = Instancias/10-24-1-2.ophs
cr = 0.3
mr = 0.1
ps = 50
max_iter = 50
seed = 123
debug = 0

comp:$(prog)

exe:$(prog)
	#valgrind --leak-check=full --show-reachable=yes 
	./$(prog) $(instancia) $(cr) $(mr) $(ps) $(max_iter) $(seed) $(debug)

$(prog): main.o
	$(comp) main.o -o $(prog) $(flags)

main.o: main.cpp includes.h
	$(comp) -c main.cpp $(flags)

clean:
	rm -f *~
	rm -f *.o
	rm -f *.txt
	rm -f *.ps
	rm -f $(prog)
	rm -f core*
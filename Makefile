flags = -Wno-deprecated -O2
comp = g++
prog  = OPHS

#Parametros default
instance = Instancias/10-24-1-2.ophs
results = results.txt
cr = 0.3
mr = 0.01
ps = 10
max_iter = 5
seed = 2
debug = true

comp:$(prog)

exe:$(prog)
	#valgrind --leak-check=full --show-reachable=yes 
	./$(prog) $(instance) $(results) $(cr) $(mr) $(ps) $(max_iter) $(seed) $(debug)

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
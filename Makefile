flags = -Wno-deprecated -O2
comp = g++
prog  = OPHS

#Parametros default
instance = instance.txt
results = results.txt
cr = 0.3
mr = 0.01
ps = 5
max_iter = 1e+4
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
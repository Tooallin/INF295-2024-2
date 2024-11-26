flags = -Wno-deprecated -O2
comp = g++
prog  = OPHS

#Parametros default
instance = Instancias/10-24-1-2.ophs
cr = 0.7
mr = 0.3
ps = 20
max_iter = 10
seed = 123
debug = true

comp:$(prog)

exe:$(prog)
	#valgrind --leak-check=full --show-reachable=yes 
	./$(prog) $(instance) $(cr) $(mr) $(ps) $(max_iter) $(seed) $(debug)

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
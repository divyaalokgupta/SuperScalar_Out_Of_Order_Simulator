all:
	g++ SuperScalar_OOO_sim.c -g -Wall -o sim

clean:
	rm -f sim *.o


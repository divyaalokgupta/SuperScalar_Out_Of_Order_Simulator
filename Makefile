all:
	g++ SuperScalar_OOO_sim.c -g -Wall -o sim

debug:
	gdb --args ./sim 64 32 4 ./trace/val_trace_gcc1

clean:
	rm -f sim *.o


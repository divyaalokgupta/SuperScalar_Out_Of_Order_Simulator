all:
	g++ SuperScalar_OOO_sim.c -g -Wall -o sim

debug:
	gdb --args ./sim 128 16 5 ./trace/val_trace_perl1

test:
	./sim 16 8 1 ./trace/val_trace_gcc1 > 5 && gvimdiff 5 ValidationRuns/val1.txt
	./sim 16 8 2 ./trace/val_trace_gcc1 > 6 && gvimdiff 6 ValidationRuns/val2.txt
	./sim 60 15 3 ./trace/val_trace_gcc1 > 7 && gvimdiff 7 ValidationRuns/val3.txt
	./sim 64 16 8 ./trace/val_trace_gcc1 > 8 && gvimdiff 8 ValidationRuns/val4.txt
	./sim 64 16 4 ./trace/val_trace_perl1 > 9 && gvimdiff 9 ValidationRuns/val5.txt
	./sim 128 16 5 ./trace/val_trace_perl1 > 10 && gvimdiff 10 ValidationRuns/val6.txt
	./sim 256 64 5 ./trace/val_trace_perl1 > 11 && gvimdiff 11 ValidationRuns/val7.txt
	./sim 512 64 7 ./trace/val_trace_perl1 > 12 && gvimdiff 12 ValidationRuns/val8.txt
	./sim 64 16 4 ./trace/val_trace_gcc1 > 1 && gvimdiff 1 SampleRuns/sample-64-16-4-gcc.txt.1
	./sim 128 32 4 ./trace/val_trace_gcc1 > 2 && gvimdiff 2 SampleRuns/sample-128-32-4-gcc.txt.1
	./sim 128 16 4 ./trace/val_trace_gcc1 > 3 && gvimdiff 3 SampleRuns/sample-128-16-4-gcc.txt.1
	./sim 64 32 4 ./trace/val_trace_gcc1 > 4 && gvimdiff 4 SampleRuns/sample-64-32-4-gcc.txt.1

clean:
	rm -f sim *.o 1 2 3 4

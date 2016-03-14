CC=icpc
exe=splitPhase

bin: $(exe)

$(exe): main.cc
	$(CC) -o $(exe) main.cc

run: bin
	./$(exe)

clean:
	rm *.o $(exe) -f

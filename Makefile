a.out: main.cc map.h Combat.h
	g++ -g -Og main.cc -lcurl -lncurses

clean:
	rm -rf a.out core *.o

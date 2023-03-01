a.out: main.cc map.h Combat.h Actors.h
	g++ -g -Og main.cc -lcurl -lncurses

clean:
	rm -rf a.out core *.o

main: main.o map.o agent.o
	gcc -o tp main.o map.o  agent.o -g
	rm *.o
	./tp 

map.o: src/map.c 
	gcc -c src/map.c

agent.o: src/agent.c
	gcc -c src/agent.c
	
main.o: src/main.c
	gcc -c src/main.c

debug:
	valgrind --leak-check=full ./tp

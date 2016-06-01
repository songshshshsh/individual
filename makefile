main:main.o random.o graph.o SAT.o
	g++ main.o random.o graph.o SAT.o -o main -lz3
main.o:main.cpp point.h graph.h random.h
	g++ -c main.cpp -o main.o
random.o:random.cpp random.h graph.h point.h
	g++ -c random.cpp -o random.o
graph.o:graph.cpp graph.h point.h
	g++ -c graph.cpp -o graph.o
SAT.o: SAT.h SAT.cpp graph.h point.h
	g++ -c SAT.cpp -o SAT.o -lz3
clean:
	rm -f *.o main
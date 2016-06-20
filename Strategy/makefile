main:main.o random.o graph.o SAT1.o test.o SAT2.o SAT3.o Strategy.o
	g++ -O4 main.o random.o graph.o SAT1.o test.o SAT2.o SAT3.o Strategy.o -o main -lz3
main.o:main.cpp point.h graph.h random.h way.h SAT1.h SAT2.h SAT3.h Strategy.h
	g++ -O4 -c main.cpp -o main.o
random.o:random.cpp random.h graph.h point.h
	g++ -O4 -c random.cpp -o random.o
graph.o:graph.cpp graph.h point.h
	g++ -O4 -c graph.cpp -o graph.o
SAT1.o: SAT1.h SAT1.cpp graph.h point.h
	g++ -O4 -c SAT1.cpp -o SAT1.o -lz3
SAT2.o: SAT2.h SAT2.cpp graph.h point.h
	g++ -O4 -c SAT2.cpp -o SAT2.o -lz3
SAT3.o: SAT3.h SAT3.cpp graph.h point.h
	g++ -O4 -c SAT3.cpp -o SAT3.o -lz3
Strategy.o: Strategy.h 
	g++ -O4 -c Strategy.cpp -o Strategy.o -lz3
Test.o: test.h graph.h
	g++ -O4 -c test.cpp -o test
clean:
	rm -f *.o main
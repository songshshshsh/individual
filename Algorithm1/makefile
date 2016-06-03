main:main.o random.o graph.o SAT.o test.o
	g++ -O4 main.o random.o graph.o SAT.o test.o -o main -lz3
main.o:main.cpp point.h graph.h random.h
	g++ -O4 -c main.cpp -o main.o
random.o:random.cpp random.h graph.h point.h
	g++ -O4 -c random.cpp -o random.o
graph.o:graph.cpp graph.h point.h
	g++ -O4 -c graph.cpp -o graph.o
SAT.o: SAT.h SAT.cpp graph.h point.h
	g++ -O4 -c SAT.cpp -o SAT.o -lz3
Test.o: test.h graph.h
	g++ -O4 -c test.cpp -o test
clean:
	rm -f *.o main
FLAG=-std=c++11

install: solparser clean

tree.o:
	g++ -c -o ./tree.o ./tree.cpp $(FLAG)

tokenize.o:
	g++ -c -o ./tokenize.o ./tokenize.cpp $(FLAG)

main.o:
	g++ -c -o ./main.o ./main.cpp $(FLAG)

solparser: main.o tokenize.o tree.o
	g++ -o solparser ./main.o ./tokenize.o ./tree.o $(FLAG)

clean:
	rm -f *.o

allclear:
	rm -f *.o
	rm -f solparser


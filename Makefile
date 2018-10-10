FLAG=-std=c++11

all: solparser clean

tokenize.o:
	g++ -c -o ./tokenize.o ./tokenize.cpp $(FLAG)

main.o:
	g++ -c -o ./main.o ./main.cpp $(FLAG)

solparser: main.o tokenize.o
	g++ -o solparser ./main.o ./tokenize.o $(FLAG)

clean:
	rm -f *.o

allclear:
	rm -f *.o
	rm -f solparser


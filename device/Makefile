# main:main.o
# 	g++ main.o -o main
# main.o:
# 	g++ -c main.cpp -o main.o

objects = main.cpp
exe : $(objects)
	g++ -o exe $(objects) -std=c++11

clean:
	rm -rf *.o exe

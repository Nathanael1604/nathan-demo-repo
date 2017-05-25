main: main.o PlateDetector.o PlateReader.o
	g++ main.o PlateDetector.o PlateReader.o -o main `pkg-config opencv --cflags --libs`

main.o: main.cpp
	g++ main.cpp -c -o main.o

PlateDetector.o: PlateDetector.cpp
	g++ PlateDetector.cpp -c -o PlateDetector.o

PlateReader.o: PlateReader.cpp
	g++ PlateReader.cpp -c -o PlateReader.o


clean:
	rm *.o main

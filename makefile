prog1: quash.o
	g++ quash.o -o prog1
quash.o: quash.cpp
	g++ quash.cpp -c
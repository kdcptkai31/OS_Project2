all: main

main: main.o
	g++ main.o -o main

main.o: main.cpp
	g++ -c main.cpp

tar:
	tar -cvf assignment2_Duty-Thompson-Webber.tar *.docx *.cpp *.h *.md *.png Makefile

clean: 
	rm *.o main assignment2_Duty-Thompson-Webber.tar

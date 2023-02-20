all:
	g++ -pthread Socket.cpp Roshambo.cpp main.cpp -o ./test

clean:
	rm *.out
	
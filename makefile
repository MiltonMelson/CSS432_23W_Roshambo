all:
	g++ -pthread Socket.cpp Server.cpp Client.cpp Player.cpp main.cpp -o ./test

clean:
	rm *.out
	
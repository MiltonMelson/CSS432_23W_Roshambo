all:
	g++ -pthread Client.cpp Data.cpp Player.cpp Server.cpp Socket.cpp main.cpp -o ./client.out
	g++ -pthread Client.cpp Data.cpp Player.cpp Server.cpp Socket.cpp main.cpp -o ./server.out

clean:
	rm *.out
	
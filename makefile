all:
	g++ -pthread Server.cpp -o ./server.out
	g++ Client.cpp -o ./client.out

clean:
	rm *.out
	
all:
	g++ server.cpp -std=c++11 -o chatsrv
	g++ client.cpp -std=c++11 -o chatcl

clean:
	rm chatsrv
	rm chctcl
all: clean client server

server:
	g++ -I"./soft" -o lib/sampleserver.x soft/sampleserver.cpp soft/udpapi.cpp soft/message.cpp

client:
	g++ -I"./soft" -o lib/sampleclient.x soft/sampleclient.cpp soft/udpapi.cpp soft/message.cpp

clean:
	rm ./lib/* || exit 0

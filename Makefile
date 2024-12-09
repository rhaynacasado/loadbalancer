compile:
	g++ -Wall -Wextra -g3 ./src/load_balancer.cpp -o ./bin/load_balancer.exe -lws2_32
	g++ -Wall -Wextra -g3 ./src/server.cpp -o ./bin/server.exe -lws2_32
	g++ -Wall -Wextra -g3 ./src/client.cpp -o ./bin/client.exe -lws2_32

server1:
	./bin/server.exe 8081

server2:
	./bin/server.exe 8082

server3:
	./bin/server.exe 8083

loadbalancer:
	./bin/load_balancer.exe

client:
	./bin/client.exe 127.0.0.1 8080
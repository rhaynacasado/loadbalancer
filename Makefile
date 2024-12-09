compile:
	gcc -o cliente cliente.c -lws2_32
	gcc -o balanceador balanceador.c -lws2_32
	gcc -o servidor servidor.c -lws2_32

balanceador:
	./balanceador

servidor1:
	./servidor 9090

servidor2:
	./servidor 9091

cliente:
	./cliente	

c:
	g++ -Wall -Wextra -g3 load_balancer.cpp -o load_balancer.exe -lws2_32
	g++ -Wall -Wextra -g3 server.cpp -o server.exe -lws2_32
	g++ -Wall -Wextra -g3 client.cpp -o client.exe -lws2_32

r1:
	server.exe 8081

r2:
	server.exe 8082

r3:
	server.exe 8083

r4:
	load_balancer.exe

r5:
	client.exe 127.0.0.1 8080

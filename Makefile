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
	g++ -Wall -Wextra -g3 c:\Users\Rhayna\OneDrive\Rhayna\USP\SistDist\load_balancer.cpp -o c:\Users\Rhayna\OneDrive\Rhayna\USP\SistDist\output\load_balancer.exe -lws2_32

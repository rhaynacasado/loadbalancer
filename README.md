<h1 align="center">
    <p> Distributed Computing Systems - Final Project  </p>
    <pp> Load Balancing Algorithms  </pp>
</h1>

## About

Final Project for the Course SSC0904 - Distributed Computing Systems. This project aims to apply knowledge of load balancing in distributed systems.  

To this end, a system with three servers was developed to handle client requests through a load balancer for resource distribution. The load-balancing algorithm used was Round-Robin, and TCP communication with sockets was implemented for server-client communication over the network.

## How to run

```bash
    # Run server 1
    $ make server1

    # Run server 2
    $ make server2

    # Run server 3
    $ make server3

    # Run load balancer
    $ make loadbalancer

    # Run client
    $ make client
```

## Authors

- Agnes Bressan

- Carolina Elias

- Caroline Clapis

- Rauany Secci

- Rhayna Casado

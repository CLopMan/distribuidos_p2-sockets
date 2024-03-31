#!/bin/bash

# Load the docker image with code and gcc 
docker load -i socketimg

# Create the network 
docker network create sc_testing

# creating client and server container
docker run -d -i --net sc_testing --name cliente socketp2:distribuidos
docker run -d -i --net sc_testing --name servidor socketp2:distribuidos

# compile code
docker exec -i cliente make 
docker exec -i servidor make

# exec everything
# docker exec -i servidor ./servidor 4500
docker exec cliente env IP_TUPLAS=servidor.sc_testing PORT_TUPLAS=4500 ./cliente 

# Stop everything 
docker stop cliente servidor 

# Clean up everything
docker network rm sc_testing
docker rm cliente servidor

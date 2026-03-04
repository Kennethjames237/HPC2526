#!/bin/sh

# https://www.open-mpi.org/doc/v4.1/

HOSTNAME=$(hostname)

mpicc mpi_bandwidth.c -o mpi_bandwidth
mpicc mpi_latency.c  -o mpi_latency

# it means i have 1 node(wn1 with 4 cores) and i can put max 2 process on that node 
mpirun -npernode 2 -np 2 -host wn1:4       mpi_bandwidth 2> band1-$HOSTNAME.csv

#it means i have 2 nodes(wn1 and wn2 with 4 cores each) and 2 processes tu run and can run max 1 process per node    
mpirun -npernode 1 -np 2 -host wn1:4,wn2:4 mpi_bandwidth 2> band2-$HOSTNAME.csv

#it means i have 1 node (wn1 with 4 cores avalable) and 2 mpi-process and max 2 process can run on this node
mpirun -npernode 2 -np 2 -host wn1:4       mpi_latency   2> lat1-$HOSTNAME.csv

#i have 2 nodes (wn1,wn2 with total 8 cores) and can run max 1 proces per node and i have 2 mpi-process to run
mpirun -npernode 1 -np 2 -host wn1:4,wn2:4 mpi_latency   2> lat2-$HOSTNAME.csv


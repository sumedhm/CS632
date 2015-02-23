#!/bin/bash

mkdir logs
mkdir ftp/logs
mkdir ftp/synclogs
mkdir logs/servers
gcc -o coda_server -pthread server.c
./coda_server


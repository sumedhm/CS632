#!/bin/bash
 parentDir="/home/optimus/Acads/ds/sock_project/Client/example/"
 gcc -Wall myfs.c `pkg-config fuse --cflags --libs` -o myfs
 gcc client.c -o client -pthread
 ./myfs $parentDir -o nonempty

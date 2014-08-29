#!bin/bash

reset
javac RoomBookingInterface.java RoomBooking.java User.java Users.java Room.java
javac RoomBookingServer.java RoomBookingClient.java
rmic RoomBooking
#rmiregistry &
java -Djava.security.policy=server.policy RoomBookingServer &
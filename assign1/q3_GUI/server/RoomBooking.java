package server;
import java.rmi.*;
import java.rmi.server.*;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

public class RoomBooking implements RoomBookingInterface{

	public RoomBooking(String msg) throws RemoteException{
		System.out.println("Server Up and Running..\n");
	}

	public String sayHello(String msg){
		return msg;
	}
}

package client;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import server.*;

public class RoomBookingClient {
    public static void main(String args[]) {
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }
        try {
            String name = "RoomBookingInterface";
            Registry registry = LocateRegistry.getRegistry(args[0]);
            RoomBookingInterface comp = (RoomBookingInterface) registry.lookup(name);
	    String pi = comp.sayHello("Hey");
            System.out.println(pi);
        } catch (Exception e) {
            System.err.println("ComputePi exception:");
            e.printStackTrace();
        }
    }    
}

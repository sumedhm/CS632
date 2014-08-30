import java.rmi.*;
import java.rmi.server.*;

public class RoomBookingServer{
	public static void main(String[] args){
		if (System.getSecurityManager() == null) {
            		System.setSecurityManager(new RMISecurityManager());
		}
        	try {
        			Naming.rebind("RoomBooking",new RoomBooking("Heloo"));
            		System.out.println("RoomBookingInterfaceEngine bound");
        	} catch (Exception e) {
            		System.err.println("RoomBookingInterface exception:");
            		e.printStackTrace();
        	}
	}
}

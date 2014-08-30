import java.rmi.*;
import java.rmi.server.*;

public class RoomBookingServer{
	
    public static void main(String[] args){
	
    	if (System.getSecurityManager() == null){
            		System.setSecurityManager(new RMISecurityManager());
		}
        	try {
                    RoomBooking prog = new RoomBooking("Hello");
        			Naming.rebind("RoomBooking", prog);
                    prog.initializeRooms();
            		System.out.println("Server started");
        	} catch (Exception e) {
            		System.err.println("RoomBookingInterface exception:");
            		e.printStackTrace();
        	}
	
    }

}

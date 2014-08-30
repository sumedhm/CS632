import java.rmi.*;

public interface RoomBookingInterface extends Remote{

	public String sayHello(String msg) throws RemoteException;
	public void initializeRooms() throws RemoteException;
	public void updateDate() throws RemoteException;
	public String getAllRooms() throws RemoteException;
	public String bookRoom(int id, int slot, int day, User user, String event, String desc) throws RemoteException;
	public String cancelBooking(int id, int slot, int day, User user) throws RemoteException;
	public String searchRooms(int min, int max, String loc, int day, int slot) throws RemoteException;
	public String getAllBookings(int day1, int day2) throws RemoteException;
	public User logIn(String username, String pass, User user) throws RemoteException;
	public void addNewUser(String username, String name, String password) throws RemoteException;
	public int adminLogIn(String username, String password) throws RemoteException;
	public void addNewAdmin(String username, String password) throws RemoteException;
	public void addNewRoom(String name, String location, int capacity) throws RemoteException;
	
}

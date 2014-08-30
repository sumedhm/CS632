import java.rmi.*;
import java.rmi.server.*;
import java.rmi.server.UnicastRemoteObject;
import java.util.*;
import java.text.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.Timer;
import java.lang.Thread;

public class RoomBooking extends UnicastRemoteObject implements RoomBookingInterface{

	public RoomBooking(String msg) throws RemoteException{
		System.out.println("\nServer Up and Running..");
	}
 	

 	//Rooms list
	List<Room> allRooms = new ArrayList<Room>(); 

	Users allUsers = new Users();

	Timer timer;

	/***
	Test method
	***/
	public String sayHello(String msg){
		return msg;
	}

	/***
	All Rooms declared and added to Data structure here.
	***/
	public void initializeRooms(){

		Room KD101 = new Room(100,"KD101","CSE");
		Room KD102 = new Room(50,"KD102","CSE");
		Room KD103 = new Room(50,"KD103","CSE");
		Room KD104 = new Room(50,"KD104","CSE");
		Room L1 = new Room(200,"L1","LHC");
		Room L2 = new Room(200,"L2","LHC");
		Room L3 = new Room(80,"L3","LHC");
		Room L4 = new Room(80,"L4","LHC");
		Room L5 = new Room(80,"L5","LHC");
		Room L6 = new Room(80,"L6","LHC");
		Room L7 = new Room(700,"L7","LHC");
		Room L8 = new Room(80,"L8","LHC");
		Room L9 = new Room(80,"L9","LHC");
		Room L10 = new Room(80,"L10","LHC");
		Room L11 = new Room(80,"L11","LHC");
		Room L12 = new Room(80,"L12","LHC");
		Room L13 = new Room(80,"L13","LHC");
		Room L14 = new Room(80,"L14","LHC");
		Room L15 = new Room(80,"L15","LHC");
		Room L16 = new Room(300,"L16","LHC");
		Room L17 = new Room(300,"L17","LHC");
		
		/***
		Add rooms declared. Now add them to DS.
		***/

		allRooms.add(KD101);
		allRooms.add(KD102);
		allRooms.add(KD103);
		allRooms.add(KD104);
		allRooms.add(L1);
		allRooms.add(L2);
		allRooms.add(L3);
		allRooms.add(L4);
		allRooms.add(L5);
		allRooms.add(L6);
		allRooms.add(L7);
		allRooms.add(L8);
		allRooms.add(L9);
		allRooms.add(L10);
		allRooms.add(L11);
		allRooms.add(L12);
		allRooms.add(L13);
		allRooms.add(L14);
		allRooms.add(L15);
		allRooms.add(L16);
		allRooms.add(L17);

		System.out.println("Initialized all rooms..");

		for (Room room : allRooms){
        	System.out.println(room.getName());
    	}
    	
    	Date now = new Date();
    	DateFormat dateFormat = new SimpleDateFormat("dd/MM/yyyy");
    	Date tomorrow = new Date(now.getTime() + (1000 * 60 * 60 * 24));
    	String dt = dateFormat.format(tomorrow);
    	try{
	    	tomorrow = dateFormat.parse(dt);
	    } catch(Exception e){}
    	int time = (int)(tomorrow.getTime() - now.getTime());

		timer = new Timer(time, new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				Thread t = new Thread(new Runnable() {
		         public void run()
			         {
						updateDate();
			         }
				});
			}
		});
		timer.setRepeats(false); // Only execute once
		timer.start(); // Go go go!
		System.out.println("Thread set to update data");
	
		return;
	}

	public void updateDate(){
		for(Room room : allRooms){
			room.updateDate();
		}
		int time = 24*60*60*1000;
		timer = new Timer(time, new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				Thread t = new Thread(new Runnable() {
		         public void run()
			         {
						updateDate();
			         }
				});
			}
		});
		timer.setRepeats(false); // Only execute once
		timer.start(); // Go go go!
		return;
	}


	/***
	This method shows all rooms.
	***/
	public String[] getAllRooms(){
		int i=0;
		String[] response = new String[allRooms.size()];
		for (Room room : allRooms){
			response[i] = room.getName();
			i++;
    	}
    	return response;
	}

	/***
	This method does room booking for users.
	***/
	public String bookRoom(int id, int slot, int day, User user, String event, String desc){
		Room room = allRooms.get(id);
		day += 91;
		int booking = room.bookRoom(slot,day,user,event,desc);
		if(booking==1) return "0";
		return new String("Room already booked.\n" + room.getBooking(day,slot));
	}

	/***
	The method searchRooms searches according to capacity or/and location.
	***/
	public String searchRooms(int min, int max, String loc, int day, int slot){
		String response = new String("");
		int room_added = 0;
		for(Room room : allRooms){
			room_added = 0;
			if(loc!=null){
				if(loc.equals(room.getLocation())){
					if(day!=-1){
						if(slot!=-1){
							if(room.isEmpty(day,slot)){
								if(min<=room.getCapacity()&&max>=room.getCapacity()){response += room.getDetails();continue;}
							}
						} else {
							for(int i=0;i<48;i++){
								if(room.isEmpty(day,i)){
									if(min<=room.getCapacity()&&max>=room.getCapacity()){response += room.getDetails();room_added = 1;break;}
								}
							}
							if(room_added==1) continue;
						}
					} else {
						for(int i=0;i<100;i++){
							if(slot!=-1){
								if(room.isEmpty(i,slot)){
									if(min<=room.getCapacity()&&max>=room.getCapacity()){response += room.getDetails();room_added = 1;break;}
								}
							} else {
								for(int j=0;j<48;j++){
									if(room.isEmpty(i,j)){
										if(min<=room.getCapacity()&&max>=room.getCapacity()){response += room.getDetails();room_added=1;break;}
									}
								}
								if(room_added==1) break;
							}
						}
						if(room_added==1) continue;
					}
				} else {
					//Do nothing
				}
			} else {
				if(day!=-1){
					if(slot!=-1){
						if(room.isEmpty(day,slot)){
							if(min<=room.getCapacity()&&max>=room.getCapacity()){response += room.getDetails();continue;}
						}
					} else {
						for(int i=0;i<48;i++){
							if(room.isEmpty(day,i)){
								if(min<=room.getCapacity()&&max>=room.getCapacity()){response += room.getDetails();room_added = 1;break;}
							}
						}
						if(room_added==1) continue;
					}
				} else {
					for(int i=0;i<100;i++){
						if(slot!=-1){
							if(room.isEmpty(i,slot)){
								if(min<=room.getCapacity()&&max>=room.getCapacity()){response += room.getDetails();room_added = 1;break;}
							}
						} else {
							for(int j=0;j<48;j++){
								if(room.isEmpty(i,j)){
									if(min<=room.getCapacity()&&max>=room.getCapacity()){response += room.getDetails();room_added = 1;break;}
								}
							}
							if(room_added==1) break;
						}
					}
					if(room_added==1) continue;
				}
			}
		}
		return response;

	}

	public User logIn(String username, String pass, User user){
		for(String[] users : allUsers.storage){
			if(users[1].equals(username)&&users[2].equals(pass)){
				user.username = username;
				user.id = Integer.parseInt(users[0]);
				user.name = users[3];
			}
		}
		return user;
	}

	/***ADMIN Methods***/

	public void addNewUser(String username, String name, String password){
		String[] user = new String[]{String.valueOf(allUsers.getSize()+1), username, password, name};
		allUsers.storage.add(user);
		return;
	}

	public int adminLogIn(String username, String password){
		for(String[] users : allUsers.adminStorage){
			if(users[0].equals(username)&&users[1].equals(password)){
				return 1;
			}
		}
		return 0;
	}

	public void addNewAdmin(String username, String password){
		String[] user = new String[]{username, password};
		allUsers.adminStorage.add(user);
		return;
	}

	public void addNewRoom(String name, String location, int capacity){
		Room newRoom = new Room(capacity,name,location);
		allRooms.add(newRoom);
		return;
	}


	/***
	Following method cancels room booking.
	***/
	public String cancelBooking(int id, int slot, int day, User user){
		Room room = allRooms.get(id);
		day += 91;
		int cancel = room.cancelBooking(slot,day,user);
		if(cancel==1) return "0";
		else if(cancel==-1) return new String("Room empty for the given slot. Why would you want to free an empty slot?\n");
		return new String("Room booked.\n" + room.getBooking(day,slot) + "\nYou don't have permission to cancel this booking.");
	}

	/***
	Following method returns all events between two days..
	***/
	public List<String> getAllBookings(int day1, int day2){
		List<String> response = new ArrayList<String>();
		for(int i=day1;i<=day2;i++){
			for(Room room : allRooms){
				for(int j=0;j<48;j++){
					if(!room.isEmpty(i,j)){
						response.add(room.getBooking(i,j));
					}
				}
			}
		}
		return response;
	}
	
}

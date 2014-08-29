/*Room.java*/
import java.util.*;
import java.text.*;

public class Room{

	private int capacity;
	private String name;
	private String location;
	private Booking[][] booked = new Booking[100][48];

	class Booking{

		String event;
		String description;
		User user;

		public Booking(){
			this.user = null;
			this.description = "";
			this.event = "";
		}

		public Booking(User user, String eve){
			this.user = user;
			this.event = eve;
			this.description = "";
		}

		public Booking(User user, String eve, String desc){
			this.user = user;
			this.event = eve;
			this.description = desc;
		}

	}

	public Room(){
		this.capacity = 50;
		for(int i=0;i<100;i++)
			for(int j=0;j<48;j++)
				booked[i][j] = null;
	}

	public Room(int cap, String name, String loc){
		this.capacity = cap;
		this.name = name;
		this.location = loc;
		for(int i=0;i<100;i++)
			for(int j=0;j<48;j++)
				booked[i][j] = null;
	}

	public int getCapacity(){
		return this.capacity;
	}

	public String getName(){
		return this.name;
	}

	public String getLocation(){
		return this.location;
	}

	public void changeCapacity(int cap){
		this.capacity = cap;
		return;
	}

	public void changeName(String name){
		this.name = name;
		return;
	}

	public int bookRoom(int slot, int day, User user, String event, String desc){
		if(booked[day][slot]==null) booked[day][slot] = new Booking(user, event, desc);
		else return 0;
		return 1;
	}

	public int cancelBooking(int slot, int day, User user){
		if(booked[day][slot]==null) return -1;
		if(booked[day][slot].user==user) booked[day][slot] = null;
		else return 0;
		return 1;
	}

	public boolean isEmpty(int day, int slot){
		return (booked[day][slot]==null);
	}

	public String getTime(int slot){
		int t = (slot-1)/2;
		String time = new String("");
		if(t<10) time = "0";
		else time = "";
		time = String.valueOf(t);
		if(slot%2==1){
			time += ":30 hrs";
		} else {
			time += ":00 hrs";
		}
		return time;
	}

	public String getDate(int day){
		DateFormat dateFormat = new SimpleDateFormat("dd/MM/yyyy");
		Date today = new Date();
		try{
			String s = dateFormat.format(today);
			today = dateFormat.parse(s);
		} catch(Exception e){
			System.out.println("Server error in Room.java, getDate -99");
		}
		Date d = new Date(today.getTime() + (day-91)*24*3600*1000);
		return (dateFormat.format(d));
	}


	public String getBooking(int day, int slot){
		return new String("\nDate - " + getDate(day) + ", Time - " + getTime(slot) + "\nRoom - " + this.name + ", " + this.location +
						 	"\nBy - " + booked[day][slot].user.name + "\nEvent: " + booked[day][slot].event +
							"\nEvent Description: " + booked[day][slot].description + "\n");
	}

	public String getDetails(){
		return new String("\nRoom - " + this.name + ", at " + this.location + ", Capacity - " + this.capacity + ".\n");
	}

}
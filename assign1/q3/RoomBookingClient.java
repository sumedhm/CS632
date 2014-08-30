import java.rmi.*;
import java.io.*;
import java.util.*;
import java.text.*;

public class RoomBookingClient {

	public static int daysBetween(Date dt1, Date dt2){
		
		long d1=dt1.getTime();
		long d2=dt2.getTime();
		int days = (int) ((d1-d2)/(1000*60*60*24));
		return days;

	}

    public static void main(String args[]){
		
		try {

			User user;
			RoomBookingInterface comp = (RoomBookingInterface) Naming.lookup("//localhost/RoomBooking");
			
			System.out.println("Connected to server..Logging you in..");
			BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
			DateFormat dateFormat = new SimpleDateFormat("dd/MM/yyyy");
			Date dt = new Date();
			String dt1 = dateFormat.format(dt);
			Date today = dateFormat.parse(dt1);
			
			while(true){

				System.out.print("Please enter your username- ");
				String username = null, pass = null;
				try{
					username = br.readLine();
				} catch(IOException ioe){
					System.out.println("IOException occured..");
					ioe.printStackTrace();
				}
				System.out.print("Please enter your password- ");
				try{
					pass = br.readLine();
				} catch(IOException ioe){
					System.out.println("IOException occured..");
		                        ioe.printStackTrace();
				}
				System.out.println("Logging you in..");
				
				user = new User();
				user = comp.logIn(username,pass,user);
				if(user.id==0){
					System.out.println("Wrong credentials..Please try again..\n");
				} else {
					System.out.println("You have been logged in, " + user.name + ".");
					break;
				}

			}

			while(true){
				int option=0;

				System.out.println("What would you like to do?");
				System.out.println("1 - Book a room.");
				System.out.println("2 - Search for a room.");
				System.out.println("3 - View room bookings.");
				System.out.println("4 - Cancel a room booking.");
				System.out.println("5 - Admin Privileges.");
				System.out.println("6 - Exit.");

				try{
					System.out.print("Please enter your response - ");
					option = Integer.parseInt(br.readLine());
				} catch(IOException ioe){
					System.out.println("IOException occured..");
					ioe.printStackTrace();
				}

				if(option==1){
					
					System.out.println("Please select a room no.- ");
					System.out.println(comp.getAllRooms());
					int sel = Integer.parseInt(br.readLine());
					
					System.out.println("Please enter the event name for which you want to book the room - ");
					String eve = br.readLine();
					System.out.println("Please enter the event description for which you want to book the room - ");
					String desc = br.readLine();
					System.out.print("Please enter the date(Today's - " + dateFormat.format(today) + ") - ");
					dt1 = br.readLine();
					Date date;
					
					try{
						date = dateFormat.parse(dt1);
					} catch(Exception e){
						System.out.print("Sorry! Invalid Date. Try again..");
						break;
					}
					while(date.compareTo(today)<0){
						System.out.print("Sorry! Cannot book a room in the past. Enter again - ");
						dt1 = br.readLine();
						date = dateFormat.parse(dt1);
					}
					if(daysBetween(date,today)>8){
						System.out.println("Cannot book for given date. Room booking is allowed only 8 days earlier.");
						break;
					}
					System.out.println("Enter slot number - ");
					int slot = Integer.parseInt(br.readLine());
					
					String response = comp.bookRoom(sel,slot,daysBetween(date,today),user,eve,desc);
					if(response.equals("0")){
						System.out.println("Successfully booked!\n\n");
					} else {
						System.out.println(response);
					}
				} else if(option==2){

					System.out.println("Searching room..Enter filters..");
					System.out.println("Enter min capacity");
					int min = 0;
					try{
						min = Integer.parseInt(br.readLine());
					} catch(Exception e){
						min = 0;
					}
					System.out.println("Enter max capacity");
					int max = 0;
					try{
						max = Integer.parseInt(br.readLine());
					} catch(Exception e){
						max = 1000;
					}
					System.out.println("Enter location");
					String loc = new String("");
					loc = br.readLine();
					if(loc.trim().isEmpty()) loc = null;
					System.out.println("Date");
					Date date;
					int day;
					try{
						date = dateFormat.parse(br.readLine());
						day = 91 + daysBetween(date,today);
					} catch(Exception e){
						day = -1;
					}
					System.out.println("Slot");
					int slot = 0;
					try{
						slot = Integer.parseInt(br.readLine());
					} catch(Exception e){
						slot = -1;
					}
					System.out.println(comp.searchRooms(min,max,loc,day,slot));

				} else if(option==3){

					System.out.println("Printing room bookings..");
					Date d1 = new Date(), d2 = new Date();
					String s1, s2;
					System.out.print("Date1 - ");
					s1 = br.readLine();
					try{
						d1 = dateFormat.parse(s1);
					} catch(Exception e){
						System.out.print("Sorry! Invalid Date1. Try again..");
						break;
					}
					System.out.print("Date2 - ");
					s2 = br.readLine();
					try{
						d2 = dateFormat.parse(s2);
					} catch(Exception e){
						System.out.print("Sorry! Invalid Date2. Try again..");
						break;
					}

					int day1 = daysBetween(d1,today);
					int day2 = daysBetween(d2,today);
					if(day1>8 || day2>8){
						System.out.println("Dates can be only 8 days from now. Try again.");
						break;
					} else if(day1<-91 || day2<-91){
						System.out.println("Date entered is way too much in the past. We keep only 90 days data from past.");
						break;
					}
					day1 += 91;
					day2 += 91;

					String response = comp.getAllBookings(day1, day2);
					System.out.println(response);

				} else if(option==4){

					System.out.println("Please select a room no.- ");
					System.out.println(comp.getAllRooms());
					int sel = Integer.parseInt(br.readLine());

					System.out.print("Please enter the date(Today's - " + dateFormat.format(today) + ") - ");
					dt1 = br.readLine();
					Date date;
					
					try{
						date = dateFormat.parse(dt1);
					} catch(Exception e){
						System.out.print("Sorry! Invalid Date. Try again..");
						break;
					}
					while(date.compareTo(today)<0){
						System.out.print("Sorry! Cannot cancel a room booking in the past. Enter again - ");
						dt1 = br.readLine();
						date = dateFormat.parse(dt1);
					}

					System.out.println("Enter slot number - ");
					int slot = Integer.parseInt(br.readLine());
					String response = comp.cancelBooking(sel,slot,daysBetween(date,today),user);
					if(response.equals("0")){
						System.out.println("Successfully cancelled event!\n\n");
					} else {
						System.out.println(response);
					}

				} else if(option==5){

					System.out.print("Enter admin username:");
					String username = br.readLine();
					System.out.print("Enter admin password:");
					String password = br.readLine();
					int res = comp.adminLogIn(username, password);
					if(res==0){
						System.out.println("Wrong credentials..Please try again..\n");break;
					} else {
						System.out.println("You have been logged in as admin.");
						while(true){
							System.out.println("What would you like to do?");
							System.out.println("1. Add a new user.");
							System.out.println("2. Add a new admin.");
							System.out.println("3. Add a new room.");
							System.out.println("4. Logout.");
							int opt = Integer.parseInt(br.readLine());
							if(opt==1){
								int cancel = 0;
								System.out.println("Adding a new user");
								System.out.println("Enter name of user(-1 to cancel adding user)");
								String name = br.readLine();
								try{
									cancel = Integer.parseInt(name);
									if(cancel==-1){
										System.out.println("Cancelled!");
										continue;
									}
								} catch(Exception e){

								}
								System.out.println("Enter username(-1 to cancel adding user)");
								username = br.readLine();
								try{
									cancel = Integer.parseInt(username);
									if(cancel==-1){
										System.out.println("Cancelled!");
										continue;
									}
								} catch(Exception e){

								}
								System.out.println("Enter password for user(-1 to cancel adding admin)");
								password = br.readLine();
								try{
									cancel = Integer.parseInt(password);
									if(cancel==-1){
										System.out.println("Cancelled!");
										continue;
									}
								} catch(Exception e){

								}

								comp.addNewUser(username,name,password);

							} else if(opt==2){
								int cancel = 0;
								System.out.println("Adding a new admin");
								System.out.println("Enter username of admin(-1 to cancel adding admin)");
								String name = br.readLine();
								try{
									cancel = Integer.parseInt(name);
									if(cancel==-1){
										System.out.println("Cancelled!");
										continue;
									}
								} catch(Exception e){

								}
								System.out.println("Enter password(-1 to cancel adding admin)");
								password = br.readLine();
								try{
									cancel = Integer.parseInt(password);
									if(cancel==-1){
										System.out.println("Cancelled!");
										continue;
									}
								} catch(Exception e){

								}

								comp.addNewAdmin(name,password);

								System.out.println("New admin added successfully with username-"+name+", password-"+password);

							} else if(opt==3){
								
								int cancel = 0;
								System.out.println("Adding a new room");
								System.out.println("Enter name of room(-1 to cancel adding room)");
								String name = br.readLine();
								try{
									cancel = Integer.parseInt(name);
									if(cancel==-1){
										System.out.println("Cancelled!");
										continue;
									}
								} catch(Exception e){

								}
								System.out.println("Enter location(-1 to cancel adding room)");
								String location = br.readLine();
								try{
									cancel = Integer.parseInt(location);
									if(cancel==-1){
										System.out.println("Cancelled!");
										continue;
									}
								} catch(Exception e){

								}

								System.out.println("Enter capacity of room(-1 to cancel adding room)");
								int capacity = Integer.parseInt(br.readLine());
								try{
									cancel = capacity;
									if(cancel==-1){
										System.out.println("Cancelled!");
										continue;
									}
								} catch(Exception e){

								}

								comp.addNewRoom(name,location,capacity);

								System.out.println("New room added successfully");

							} else if(opt==4){
								System.out.println("Logging you out..");
								break;
							}
						}
					}

				} else if(option==6){
					System.out.println("Exiting..Bye!!");
					break;
				} else {
					System.out.println("Invalid response.. Please try again..\n\n");
				}
			}

		} catch (Exception e){
			System.err.println("Client side exception:");
			e.printStackTrace();
		}
	    
    }    
}

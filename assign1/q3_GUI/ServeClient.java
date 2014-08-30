import java.awt.*;
import javax.swing.*;
import java.rmi.*;
import java.io.*;
import java.util.*;
import java.util.List;
import java.text.*;
import javax.swing.border.EmptyBorder;
import javax.swing.text.html.parser.ParserDelegator;
//import javax.swing.border.EtchedBorder;
import java.awt.event.*;

public class ServeClient extends JFrame implements ActionListener{

   	public JFrame frame = new JFrame("Book Room Online - IIT Kanpur");
   	public JPanel panel = new JPanel();
   	public JButton button = new JButton();

	public JTextField username1 = new JTextField(15);
	public JPasswordField password1 = new JPasswordField(15);

	public User user;
	public RoomBookingInterface comp;
	
	public JLabel head = new JLabel("<html><br>Please log in to continue..<br><br></html>", JLabel.CENTER);
	public JLabel l1 = new JLabel("Username", JLabel.CENTER);
	public JLabel l2 = new JLabel("Password", JLabel.CENTER);

	JComboBox roomList = new JComboBox();
	JTextField t1 = new JTextField(15);
	JTextArea t2 = new JTextArea(4, 15);
	JComboBox dateList = new JComboBox();
	JComboBox timeList = new JComboBox();
			
	BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
	DateFormat dateFormat = new SimpleDateFormat("dd/MM/yyyy");
	Date dt = new Date();
	String dt1 = dateFormat.format(dt);
	Date today;

	public String[] getDates(){
		String[] response = new String[9];
		for(int i=0;i<9;i++) response[i] = dateFormat.format(new Date(dt.getTime() + (i* 1000 * 60 * 60 * 24)));
		return response;
	}	

	public String[] getTimes(){
		String response[] = new String[48];
		int j=0;
		for(int i=0;i<24;i++){
			String s1 = new String();
			String s2 = new String();
			if(i<9){
				s1 = "0"+String.valueOf(i)+":00-0"+String.valueOf(i)+":30";
				s2 = "0" + String.valueOf(i) + ":30-0"+ String.valueOf(i+1) + ":00";
			} else if(i==9){
				s1 = "0"+String.valueOf(i)+":00-0"+String.valueOf(i)+":30";
				s2 = "0" + String.valueOf(i) + ":30-"+ String.valueOf(i+1) + ":00";
			} else {
				s1 = String.valueOf(i)+":00-"+String.valueOf(i)+":30";
				s2 = String.valueOf(i) + ":30-"+ String.valueOf(i+1) + ":00";
			}
			response[j++] = s1;
			response[j++] = s2;
		}
		return response;
	}

	public JScrollPane getTimePanel(){
		GridLayout l = new GridLayout(0,1);
		JPanel p = new JPanel(l);
		JCheckBox b = new JCheckBox();
		for(int i=0;i<24;i++){
			String s1 = new String();
			String s2 = new String();
			if(i<9){
				s1 = "0"+String.valueOf(i)+":00-0"+String.valueOf(i)+":30";
				s2 = "0" + String.valueOf(i) + ":30-0"+ String.valueOf(i+1) + ":00";
			} else if(i==9){
				s1 = "0"+String.valueOf(i)+":00-0"+String.valueOf(i)+":30";
				s2 = "0" + String.valueOf(i) + ":30-"+ String.valueOf(i+1) + ":00";
			} else {
				s1 = String.valueOf(i)+":00-"+String.valueOf(i)+":30";
				s2 = String.valueOf(i) + ":30-"+ String.valueOf(i+1) + ":00";
			}
			b = new JCheckBox(s1);
			p.add(b);
			b = new JCheckBox(s2);
			p.add(b);			
		}
		JScrollPane scrollPane = new JScrollPane(p);
		return scrollPane; 
	}

	public int daysBetween(Date dt1, Date dt2){
		
		long d1=dt1.getTime();
		long d2=dt2.getTime();
		int days = (int) ((d1-d2)/(1000*60*60*24));
		return days;

	}

	public void getUserHomeWindow(){
		frame.dispose();				
		frame = new JFrame("Book Room Online - IIT Kanpur");
		GridLayout layout = new GridLayout(0,2);
		panel = new JPanel(layout);
		JButton button1 = new JButton("Book a room");
		JButton button2 = new JButton("Search for a room");
		JButton button3 = new JButton("View room bookings");
		JButton button4 = new JButton("Cancel room bookings");
		JButton button5 = new JButton("Admin Privileges");
		JButton button6 = new JButton("Logout and Exit");
		button1.setActionCommand("1");
		button2.setActionCommand("2");
		button3.setActionCommand("3");
		button4.setActionCommand("4");
		button5.setActionCommand("5");
		button6.setActionCommand("6");
		button1.addActionListener(this);
		button2.addActionListener(this);
		button3.addActionListener(this);
		button4.addActionListener(this);
		button5.addActionListener(this);
		button6.addActionListener(this);
		panel.add(button1);
		panel.add(button2);
		panel.add(button3);
		panel.add(button4);
		panel.add(button5);
		panel.add(button6);
		frame.setLayout(new BorderLayout());
		frame.add(panel, BorderLayout.CENTER);
		frame.setDefaultLookAndFeelDecorated(false);
		frame.setLocation(50,50);
		frame.setSize(400,200);
		frame.setIconImage(new ImageIcon("./icon.png").getImage());
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	public void showBookingWindow(){
		String[] response = new String[1];
		try{
			response = comp.getAllRooms();
		} catch(Exception ex){
			JOptionPane.showMessageDialog(frame, "Could not connect to server..Please try again..");
			return;
		}
		roomList = new JComboBox(response);
		t1 = new JTextField(15);
		t2 = new JTextArea(4, 15);
		response = getDates();
		dateList = new JComboBox(response);
		response = getTimes();
		timeList = new JComboBox(response);
		
			try{
				button = new JButton("Back");
				button.setActionCommand("back");
				button.addActionListener(this);
				frame.dispose();
				frame = new JFrame("Book Room Online - IIT Kanpur");
				frame.setSize(500,300);
				GridLayout layout = new GridLayout(0,2);
				panel = new JPanel(layout);
				JLabel l1 = new JLabel("Room");
				roomList.setSelectedIndex(0);			
				JLabel eve = new JLabel("Event Name");
				JLabel desc = new JLabel("Event Description");
				JLabel l2 = new JLabel("Date");
				dateList.setSelectedIndex(0);
				JLabel l3 = new JLabel("Time");
				/*JScrollPane timePanel = getTimePanel();
				timePanel.setPreferredSize(new Dimension(60, 60));*/
				JButton button1 = new JButton("Book Room");
				JButton button2 = new JButton("Go Back");
				button1.setActionCommand("book");
				button2.setActionCommand("back");
				button1.addActionListener(this);
				button2.addActionListener(this);
				dateList.addActionListener(this);
				timeList.addActionListener(this);
				panel.add(l1);
				panel.add(roomList);
				panel.add(eve);
				panel.add(t1);
				panel.add(desc);
				panel.add(t2);
				panel.add(l2);
				panel.add(dateList);
				panel.add(l3);
				panel.add(timeList);
				panel.add(button2);
				panel.add(button1);
				frame.setLayout(new BorderLayout());
				frame.add(panel, BorderLayout.CENTER);
				frame.setDefaultLookAndFeelDecorated(false);
				frame.setLocation(50,50);
				//frame.setSize(400,200);
				frame.setIconImage(new ImageIcon("./icon.png").getImage());
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				frame.setVisible(true);
			} catch(Exception ex){
				JOptionPane.showMessageDialog(frame, "Could not connect to server..Please try again..");
				ex.printStackTrace();
				return;
			}
	}

	public void cancelBookingWindow(){
		String[] response = new String[1];
		try{
			response = comp.getAllRooms();
		} catch(Exception ex){
			JOptionPane.showMessageDialog(frame, "Could not connect to server..Please try again..");
			return;
		}
		roomList = new JComboBox(response);
		response = getDates();
		dateList = new JComboBox(response);
		response = getTimes();
		timeList = new JComboBox(response);
		
			try{
				button = new JButton("Back");
				button.setActionCommand("back");
				button.addActionListener(this);
				frame.dispose();
				frame = new JFrame("Cancel Room Booking Online - IIT Kanpur");
				frame.setSize(500,200);
				GridLayout layout = new GridLayout(0,2);
				panel = new JPanel(layout);
				JLabel l1 = new JLabel("Room");
				roomList.setSelectedIndex(0);			
				JLabel l2 = new JLabel("Date");
				dateList.setSelectedIndex(0);
				JLabel l3 = new JLabel("Time");
				/*JScrollPane timePanel = getTimePanel();
				timePanel.setPreferredSize(new Dimension(60, 60));*/
				JButton button1 = new JButton("Cancel Room Booking");
				JButton button2 = new JButton("Go Back");
				button1.setActionCommand("cancel");
				button2.setActionCommand("back");
				button1.addActionListener(this);
				button2.addActionListener(this);
				dateList.addActionListener(this);
				timeList.addActionListener(this);
				panel.add(l1);
				panel.add(roomList);
				panel.add(l2);
				panel.add(dateList);
				panel.add(l3);
				panel.add(timeList);
				panel.add(button2);
				panel.add(button1);
				frame.setLayout(new BorderLayout());
				frame.add(panel, BorderLayout.CENTER);
				frame.setDefaultLookAndFeelDecorated(false);
				frame.setLocation(50,50);
				//frame.setSize(400,200);
				frame.setIconImage(new ImageIcon("./icon.png").getImage());
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				frame.setVisible(true);
			} catch(Exception ex){
				JOptionPane.showMessageDialog(frame, "Could not connect to server..Please try again..");
				ex.printStackTrace();
				return;
			}
	}


	public void viewBookingWindow(){
		
			try{
				button = new JButton("Back");
				button.setActionCommand("back");
				button.addActionListener(this);
				frame.dispose();
				frame = new JFrame("View Room Bookings - Book Room Online - IIT Kanpur");
				frame.setSize(400,200);
				GridLayout layout = new GridLayout(0,2);
				panel = new JPanel(layout);
				JLabel l1 = new JLabel("Date 1");		
				JLabel l2 = new JLabel("Date 2");
				username1 = new JTextField(15);
				username1.setText(dateFormat.format(today));
				t1 = new JTextField(15);
				t1.setText(dateFormat.format(today));
				/*JScrollPane timePanel = getTimePanel();
				timePanel.setPreferredSize(new Dimension(60, 60));*/
				JButton button1 = new JButton("View Room Bookings");
				JButton button2 = new JButton("Go Back");
				button1.setActionCommand("view");
				button2.setActionCommand("back");
				button1.addActionListener(this);
				button2.addActionListener(this);
				dateList.addActionListener(this);
				timeList.addActionListener(this);
				panel.add(l1);
				panel.add(username1);
				panel.add(l2);
				panel.add(t1);
				panel.add(button2);
				panel.add(button1);
				frame.setLayout(new BorderLayout());
				frame.add(panel, BorderLayout.CENTER);
				frame.setDefaultLookAndFeelDecorated(false);
				frame.setLocation(50,50);
				//frame.setSize(400,200);
				frame.setIconImage(new ImageIcon("./icon.png").getImage());
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				frame.setVisible(true);
			} catch(Exception ex){
				JOptionPane.showMessageDialog(frame, "Could not connect to server..Please try again..");
				ex.printStackTrace();
				return;
			}
	}

	public void actionPerformed(ActionEvent e){
		
		if ("login".equals(e.getActionCommand())) {
			String username = username1.getText();
			String password = String.valueOf(password1.getPassword());
			user = new User();
			try{
				user = comp.logIn(username,password,user);
			} catch(Exception ex){
				JOptionPane.showMessageDialog(frame, "Could not connect to server..Please try again..");
				return;
			}
			if(user.id==0){
				JOptionPane.showMessageDialog(frame, "Wrong credentials..Please try again..");
				password1.setText("");
			} else {
				JOptionPane.showMessageDialog(frame, "You have been logged in, " + user.name + ".");
				System.out.println("You have been logged in, " + user.name + ".\n");
				getUserHomeWindow();
			}
		} else if("back".equals(e.getActionCommand())){
			getUserHomeWindow();
		} else if("1".equals(e.getActionCommand())){
			showBookingWindow();
		} else if("2".equals(e.getActionCommand())){
			
		} else if("3".equals(e.getActionCommand())){
			viewBookingWindow();
		} else if("4".equals(e.getActionCommand())){
			cancelBookingWindow();
		} else if("5".equals(e.getActionCommand())){
			
		} else if("6".equals(e.getActionCommand())){
			
			System.exit(0);

		}  else if("book".equals(e.getActionCommand())){
			int sel = roomList.getSelectedIndex();
			int slot = timeList.getSelectedIndex();
			Date date = new Date();
			try{
	   			date = dateFormat.parse((String) dateList.getSelectedItem());
	   		} catch(Exception ex){
	   			JOptionPane.showMessageDialog(frame, "Invalid date..Please try again..");
	   		}
   			String eve = t1.getText();
   			String desc = t1.getText();
   			String res = new String();
   			System.out.println("sel-"+String.valueOf(sel)+",user-"+user.name+",slot-"+String.valueOf(slot)+",eve -"+eve+",desc-"+desc+",days-"+String.valueOf(daysBetween(date,today)));
   			try{
				res = comp.bookRoom(sel,slot,daysBetween(date,today),user,eve,desc);
			} catch(Exception ex){
				JOptionPane.showMessageDialog(frame, "Could not connect to server..Please try again..");
				return;
			}
   			if(res.equals("0")){
				JOptionPane.showMessageDialog(frame, "Room booked successfully.");
				System.out.println("Successfully booked!\n\n");
				getUserHomeWindow();
			} else {
				JOptionPane.showMessageDialog(frame, res);
				System.out.println(res);
			}   			
		} else if("cancel".equals(e.getActionCommand())){

			int sel = roomList.getSelectedIndex();
			int slot = timeList.getSelectedIndex();
			Date date = new Date();
			try{
	   			date = dateFormat.parse((String) dateList.getSelectedItem());
	   		} catch(Exception ex){
	   			JOptionPane.showMessageDialog(frame, "Invalid date..Please try again..");
	   		}
			System.out.print("sel - "+String.valueOf(sel)+",user-"+user.name+",slot-"+String.valueOf(slot)+",days-"+String.valueOf(daysBetween(date,today)));
			if(date.compareTo(today)<0){
				JOptionPane.showMessageDialog(frame, "Sorry! Cannot cancel a room booking in the past. Enter again.");
				return;
			}
			String response = new String();
			try{
				response = comp.cancelBooking(sel,slot,daysBetween(date,today),user);
			} catch(Exception ex){
				JOptionPane.showMessageDialog(frame, "Could not connect to server..Please try again..");
				return;
			}
			
			if(response.equals("0")){
				JOptionPane.showMessageDialog(frame, "Successfully cancelled event!.");
				System.out.println("Successfully cancelled event!\n\n");
				getUserHomeWindow();						
			} else {
				JOptionPane.showMessageDialog(frame, response);
				System.out.println(response);
			}

		} else if("view".equals(e.getActionCommand())){

					System.out.println("Printing room bookings..");
					Date d1 = new Date(), d2 = new Date();
					String s1, s2;
					s1 = username1.getText();
					s2 = t1.getText();
					try{
						d1 = dateFormat.parse(s1);
					} catch(Exception ex){
						JOptionPane.showMessageDialog(frame, "Sorry! Invalid Date1. Try again..");
						return;
					}					
					try{
						d2 = dateFormat.parse(s2);
					} catch(Exception ex){
						JOptionPane.showMessageDialog(frame, "Sorry! Invalid Date2. Try again..");
						return;
					}

					int day1 = daysBetween(d1,today);
					int day2 = daysBetween(d2,today);
					if(day1>8 || day2>8){
						JOptionPane.showMessageDialog(frame, "Dates can be only 8 days from now. Try again.");
						return;
					} else if(day1<-91 || day2<-91){
						JOptionPane.showMessageDialog(frame, "Date entered is way too much in the past. We keep only 90 days data from past.");
						return;
					}
					day1 += 91;
					day2 += 91;
					List<String> response = new ArrayList<String>();
					try{
						response = comp.getAllBookings(day1, day2);
					} catch(Exception ex){
						JOptionPane.showMessageDialog(frame, "Could not connect to server..Please try again..");
						return;
					}
					JFrame frame1 = new JFrame("Showing room bookings - " + s1 + " to " + s2 + " - IIT Kanpur");
					JPanel panel1 = new JPanel();
					JLabel t = new JLabel("Here are the room bookings between dates - " + s1 + " to " + s2 + "\n", JLabel.CENTER);
					panel1.setLayout(new GridLayout(0,1));
					JLabel tr = new JLabel("\n" + response.size() + " events found.\n\n");
					panel1.add(tr);
					for(String x:response){
						tr = new JLabel(x);
						panel1.add(tr);
					}
					JScrollPane pane = new JScrollPane(panel1);
					pane.setBorder(new EmptyBorder(10, 10, 10, 10));
					frame1.setLayout(new BorderLayout());
					frame1.add(t, BorderLayout.NORTH);
					frame1.add(pane, BorderLayout.CENTER);
					frame1.setDefaultLookAndFeelDecorated(false);
					frame1.setLocation(150,150);
					frame1.setSize(500,300);
					frame1.setIconImage(new ImageIcon("./icon.png").getImage());
					frame1.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
					frame1.setVisible(true);
					System.out.println(response);

		}
	}

    public ServeClient(){
		
		try {
			comp = (RoomBookingInterface) Naming.lookup("//localhost/RoomBooking");
			today = dateFormat.parse(dt1);
			button = new JButton("Log In");
			panel = new JPanel(new GridBagLayout());
			GridBagConstraints c = new GridBagConstraints();
			c.fill = GridBagConstraints.HORIZONTAL;
			c.gridx = 0;
			c.gridy = 0;
			panel.add(l1, c);
			c.fill = GridBagConstraints.HORIZONTAL;
			c.gridx = 1;
			c.gridy = 0;
			panel.add(username1, c);
			c.fill = GridBagConstraints.HORIZONTAL;
			c.gridx = 0;
			c.gridy = 1;
			panel.add(l2, c);
			c.fill = GridBagConstraints.HORIZONTAL;
			c.gridx = 1;
			c.gridy = 1;
			panel.add(password1, c);
			c.fill = GridBagConstraints.HORIZONTAL;
			c.ipady = 10;
			c.weightx = 0.5;
			c.gridx = 1;
			c.gridy = 2;
			button.setActionCommand("login");
			button.addActionListener(this);
			panel.add(button, c);

			frame.setLayout(new BorderLayout());
			frame.setDefaultLookAndFeelDecorated(false);
			frame.add(head, BorderLayout.NORTH);
			frame.add(panel, BorderLayout.CENTER);
			frame.setLocation(50,50);
			frame.setSize(400,200);
			frame.setIconImage(new ImageIcon("./icon.png").getImage());
			frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
			frame.setVisible(true);

			
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

					//String response = comp.getAllBookings(day1, day2);
					//System.out.println(response);

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

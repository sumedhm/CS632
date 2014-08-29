/*
Users.java
*/

import java.io.Serializable;

public class User implements Serializable{

	public int id;
	public String username;
	public String name;
	
	public User(){
		this.name = null;
		this.username = null;
		this.id = 0;
	}

	public User(String username, String pass){
		this.name = null;
		this.username = null;
		this.id = 0;
		logIn(username, pass);
	}

	public void logIn(String username, String pass){
		String[][] storage = new String[][]{
			{"1","user1","pass1","User 1"},
			{"2","user2","pass2","User 2"}
		};
		for(int i=0;i<storage.length;i++){
			if(storage[i][1].equals(username)&&storage[i][2].equals(pass)){
				this.username = storage[i][1];
				this.name = storage[i][3];
				this.id = Integer.parseInt(storage[i][0]);
				break;
			}
		}
		return;
	}
}
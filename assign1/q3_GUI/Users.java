/***Users.java***/

import java.util.*;
public class Users{

	public List<String[]> storage = new ArrayList<String[]>();

	public List<String[]> adminStorage = new ArrayList<String[]>();

//	String[] myStringArray = new String[]{"a","b","c"};
	public Users(){
		storage.add(new String[]{"1","user1","pass1","User 1"});
		storage.add(new String[]{"2","user2","pass2","User 2"});
		adminStorage.add(new String[]{"admin","imdboss"});
	}

	public int getSize(){
		return storage.size();
	}

}
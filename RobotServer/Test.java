package server;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class Test {

	public static void main(String[] args) {
		//Client.sendData("hello","192.168.137.68",9000);
		//Client.receiveData(8000);
		Server server = new Server();
		server.listen();

		
	}

}

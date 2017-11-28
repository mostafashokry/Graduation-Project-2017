package com.multiagent.server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;


public class ClientSocket {
	private Socket socket = null;
	private DataOutputStream out = null;
	private DataInputStream in  = null;
	// create socket and output stream 
	public ClientSocket(String ip, int portnum) {

		try {
			this.socket = new Socket(ip, portnum);
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("can not connect");
			}
		
		try {
			out = new DataOutputStream(socket.getOutputStream());
			in = new DataInputStream(socket.getInputStream());
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("out problem");
		}

	}
	
	
	// get socket
	
	public Socket get() {
		return this.socket;
	}
	
	public String getString() {
		String data = null;
		try {
			data = in.readUTF();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return data;
	}
	
	public int getint() {
		int data = 0;
		try {
			data = in.readInt();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return data;
	}
	
	public DataInputStream getInputStream() {
		return in;
	}
	
	public DataOutputStream getOutputStream() {
		return out;
	}
	// send data 
	public void sendString(String data) {
		
		try {
			out.writeUTF(data);
			out.flush();

		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void sendInt(int data){
		try {
			out.writeInt(data);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	// recive data
	
	
	// terminate socket and output stream 
	public void end(){
		try {
			socket.close();
			out.close();
			in.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	


}


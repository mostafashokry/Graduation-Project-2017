package com.multiagent.taskallocation;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.sql.Connection;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;

import com.multiagent.pathplanning.PathPlanning;
import com.multiagent.robot.RobotUtils;
import com.multiagent.server.ClientSocket;
import com.mysql.fabric.xmlrpc.base.Array;

import c.beans.robot;
import db.connection.mySQLConnUtils;

public class AssignTask implements Runnable {
	
	String writeFile, exePath, readFile;
	int noOfAllRobot,  X_Start,  Y_Start,  X_End,  Y_End,  noOfAgent;
	
	public  AssignTask(String writeFile, String exePath, String readFile,
			int noOfAllRobot, int X_Start, int Y_Start, int X_End, int Y_End, int noOfAgent) {
		this.writeFile = writeFile;
		this.exePath = exePath;
		this.readFile = readFile;
		this.noOfAllRobot = noOfAllRobot;
		this.X_Start = X_Start;
		this.Y_Start = Y_Start;
		this.X_End = X_End;
		this.Y_End = Y_End;
		this.noOfAgent = noOfAgent;
		
	}
	

		@Override
		public void run() {

			
		    //static int state =0;
				
		    	Connection conn = null;
				try {
					conn = mySQLConnUtils.getMySQLConnection();
				} catch (ClassNotFoundException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (SQLException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
		    	List<String> ipRobot =new ArrayList<>();
		    	String dataToWrite = null;
		    	String xString =null;
		    	String yString = null;
		    	ClientSocket clientSocket = null;
		    	ClientSocket clientSocket1 = null;
		    	ClientSocket clientSocket2 = null;
		    	
		        //call task allocation method to get the best robot ID
		    	try {
					ipRobot = TaskAllocation.taskAllocationFn(writeFile, exePath, readFile, noOfAllRobot,
							X_Start, Y_Start, X_End, Y_End, noOfAgent);
				} catch (ClassNotFoundException | SQLException | IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} 
		    	
		    	System.out.println("IProbot list at assign task: "+ipRobot);
		    	//--------------------------------------------MUST CHANGE-----------------------------------------------
		    	for(int i=0 ; i<ipRobot.size();i++)
		    	{
		    		if(ipRobot.get(i)== "-1")
		    		{
				    	//--------------------------------------------MUST CHANGE-----------------------------------------------
						/*if taskAllocationFn returned a list of -1 so there is [no enough available robots] 
						 * 				*--  show message in the WEBSITE  --*
						 * 
						 *                      **********return***********
						 * 
						 */
				    	//--------------------------------------------MUST CHANGE-----------------------------------------------
		    		}
		    	}
		    	//--------------------------------------------MUST CHANGE-----------------------------------------------
		    	
		    	List<Integer> x = new ArrayList<>();
		    	List<Integer> y = new ArrayList<>();
		        List<String> dataToRead = new ArrayList<>();
		       
		        //get the current position of the robot
		        //kida momken n7tag 2n el task allocation tb3tlna mkan el robot el 7aly 3lshan lw mknsh fi el park     
		        // get location from db
	        	//int state = RobotUtils.robortState(conn, ipRobot);
	        	try {
	        		for(int i=0;i<ipRobot.size();i++)
	        		{
					x.add(RobotUtils.robortLocationX(conn, ipRobot.get(i)));
					y.add(RobotUtils.robortLocationY(conn, ipRobot.get(i)));
	        		}
				} catch (SQLException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
	        	//System.out.println("State: "+ state);
	        	//System.out.println("X: "+ x);
	        	//.out.println("Y: "+ y);
		        /*else if(state == 3){
	            // get feedback from raspberry
	        	}*/
	       
	        	 	
		        //single agent
		        //---------------------------------------------------------------------------------------------------------
		        if(noOfAgent == 1){
		        /*---------------------------------------------------------------------------------------------------------
		         *                      moving the robot from its position to the start point
		         */
		       
		        System.out.println("moving the robot from its position to the start point Assign Task Single Agent");
		        //call path planing with (robotpositon , start point)
		        dataToWrite = TaskAllocationUtils.dataWrite(x.get(0), y.get(0), X_Start, Y_Start);
				try {
					TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				//get the position matrix from the output file of the path planning
				try {
					dataToRead = PathPlanning.readFromFile(readFile);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

				xString = dataToRead.get(0);
				yString = dataToRead.get(1);

				System.out.println("Data Send To Robot " + ipRobot.get(0) + "  is     xString= " + xString + "     yString= " + yString);
		        //send the path to robot 
				clientSocket = new ClientSocket(ipRobot.get(0), 6001);
				clientSocket.sendInt(noOfAgent);
				clientSocket.sendString(xString);
				clientSocket.sendString(yString);
				
		        //wait till the robot reach to the start point feedback needed
				while (clientSocket.getint() != 1) {
					continue;
				}
				
				//change robot state to 2 
				try {
					RobotUtils.updateRobotStateyByIp(ipRobot.get(0),2);
				} catch (ClassNotFoundException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} catch (SQLException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
		        //---------------------------------------------------------------------------------------------------------
		       
		        /*---------------------------------------------------------------------------------------------------------
		         *                      moving the robot from its Start point to the End point
		         */
		       
		            System.out.println("moving the robot from its Start point to the End point");
		            dataToWrite = TaskAllocationUtils.dataWrite(X_Start, Y_Start, X_End, Y_End);
		    		try {
						TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
		    		//get the position matrix from the output file of the path planning
		    		try {
						dataToRead = PathPlanning.readFromFile(readFile);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}

		    		xString = dataToRead.get(0);
		    		yString = dataToRead.get(1);

		    		//send the path to robot 
		    		clientSocket.sendInt(noOfAgent);
		    		clientSocket.sendString(xString);
		    		clientSocket.sendString(yString);
		            //wait till the robot reach to the start point feedback needed
		    		while (clientSocket.getint() != 2) {
						continue;
					}
		    		
		    		//change robot state to 3
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(0),3);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
		       
		        //---------------------------------------------------------------------------------------------------------

		        /*---------------------------------------------------------------------------------------------------------
		         *                      moving the robot from its End point to the parking
		         */
		       
		            System.out.println("moving the robot from its End point to the parking");
		            dataToWrite = TaskAllocationUtils.dataWrite(X_End, Y_End, x.get(0), y.get(0));
		    		try {
						TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
		    		//get the position matrix from the output file of the path planning
		    		try {
						dataToRead = PathPlanning.readFromFile(readFile);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}

		    		xString = dataToRead.get(0);
		    		yString = dataToRead.get(1);

		    		//send the path to robot 
		    		clientSocket.sendString(xString);
		    		clientSocket.sendString(yString);
		            //wait till the robot reach to the start point feedback needed
		    		while (clientSocket.getint() != 3) {
						continue;
					}
		    		clientSocket.sendString("done");
		    		
		    		//change robot state to 0 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(0),0);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					//change Availability to 1
					try {
						RobotUtils.updateRobotAvailabilityByIp(ipRobot.get(0),1);
					} catch (ClassNotFoundException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (SQLException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}	
		    	}
		        //---------------------------------------------------------------------------------------------------------
		    	//single agent Finished
		        //---------------------------------------------------------------------------------------------------------
		        	        
		        
		        //--------------------------------------------------------------------------------------------------------- 
		        //Two agent
		        //---------------------------------------------------------------------------------------------------------
		        if(noOfAgent==2)
		        {
		        	/*---------------------------------------------------------------------------------------------------------
			         *                      moving the Two robot from its position to the start point
			         */
			       
			        System.out.println("moving the Two robot from its position to the start point");
			        //call path planing with (robotpositon , start point)

			        //---------------------------------------Robot1 "Leader"------------------------------------------------------------------
			        dataToWrite = TaskAllocationUtils.dataWrite(x.get(0), y.get(0), X_Start+1, Y_Start);
					try {
						TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					//get the position matrix from the output file of the path planning
					try {
						dataToRead = PathPlanning.readFromFile(readFile);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					//xString=TaskAllocationUtils.shiftPath(dataToRead.get(0), 1);
					xString = dataToRead.get(0);
					yString = dataToRead.get(1);

			        //send the path to robot 
					clientSocket = new ClientSocket(ipRobot.get(0), 6001);
					clientSocket.sendInt(1);
					clientSocket.sendString(xString);
					clientSocket.sendString(yString);
					//---------------------------------------Robot1 "Leader"------------------------------------------------------------------
					
					//--------------------------------------Robot2 "follower"------------------------------------------------------------------
					dataToWrite = TaskAllocationUtils.dataWrite(x.get(1), y.get(1), X_Start-1, Y_Start);
					try {
						TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					//get the position matrix from the output file of the path planning
					try {
						dataToRead = PathPlanning.readFromFile(readFile);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					//xString=TaskAllocationUtils.shiftPath(dataToRead.get(1), -1);
					xString = dataToRead.get(0);
					yString = dataToRead.get(1);

			        //send the path to robot 
					clientSocket1 = new ClientSocket(ipRobot.get(1), 6001);
					clientSocket1.sendInt(1);
					clientSocket1.sendString(xString);
					clientSocket1.sendString(yString);
					//--------------------------------------Robot2 "follower"------------------------------------------------------------------

			        //wait till the Tow robots reach to the start point feedback needed
					while (true) 
					{	
						if(clientSocket.getint() == 1 && clientSocket1.getint() == 1)
						{
							break;
						}
						continue;
					}
					//-------------------------------Two agents Phase 1 Finished----------------------------------------
					
					//change robot leader state to 2 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(0),2);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					//change robot follower state to 2 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(1),2);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					
					/*---------------------------------------------------------------------------------------------------------
			         *                 connect to the (leader , the follower) and send the path to the leader
			         */
					System.out.println("moving the Two robot from the start point to end point");
					
					//-------------------------call the Robot2 "follower first"------------------------------------
					clientSocket.sendInt(2);
					
					clientSocket1.sendInt(2);
					
					clientSocket1.sendString("follower");
					// hn7tag delay hena wala l2 ?
					try {
						TimeUnit.MILLISECONDS.sleep(500);
					} catch (InterruptedException e2) {
						// TODO Auto-generated catch block
						e2.printStackTrace();
					}
					clientSocket.sendString("leader");
					
					//call path planning
					dataToWrite = TaskAllocationUtils.dataWrite(X_Start, Y_Start, X_End, Y_End);
		    		try {
						TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
		    		//get the position matrix from the output file of the path planning
		    		try {
						dataToRead = PathPlanning.readFromFile(readFile);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}

		    		xString = dataToRead.get(0);
		    		yString = dataToRead.get(1);

		    		//send the path to the leader
		    		clientSocket.sendString(xString);
		    		clientSocket.sendString(yString);
		    		
		    		clientSocket.sendString(ipRobot.get(1));
		    		
		    		while (clientSocket.getint() != 2) 
		    		{
						continue;
					}
    
					//-------------------------------Two agents Phase 2 Finished----------------------------------------
		    		
		    		//change robot leader state to 3 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(0),3);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					//change robot follower state to 3 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(1),3);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}	
		    		
		    		
		    		
					//-------------------------------Two agents Phase 3----------------------------------------
			        //---------------------------------------Robot1 "Leader"------------------------------------------------------------------
			        dataToWrite = TaskAllocationUtils.dataWrite(X_End + 1, Y_End, x.get(0), y.get(0));
					try {
						TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					//get the position matrix from the output file of the path planning
					try {
						dataToRead = PathPlanning.readFromFile(readFile);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					xString= dataToRead.get(0);
					yString= dataToRead.get(1);
	
			        //send the path to robot 
					clientSocket.sendString(xString);
					clientSocket.sendString(yString);
					//---------------------------------------Robot1 "Leader"------------------------------------------------------------------
					
					//--------------------------------------Robot2 "follower"------------------------------------------------------------------
					dataToWrite = TaskAllocationUtils.dataWrite(X_End - 1, Y_End, x.get(1), y.get(1));
					try {
						TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					//get the position matrix from the output file of the path planning
					try {
						dataToRead = PathPlanning.readFromFile(readFile);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					xString= dataToRead.get(0);
					yString= dataToRead.get(1);
	
			        //send the path to robot 
					clientSocket1.sendString(xString);
					clientSocket1.sendString(yString);
					//--------------------------------------Robot2 "follower"------------------------------------------------------------------		        		        
					while (true) 
					{	
						if(clientSocket.getint() == 3 && clientSocket1.getint() == 3)
						{
							break;
						}
						continue;
					}
		        
					clientSocket.sendString("done");
			        clientSocket1.sendString("done");
			        
			      //change robot leader state to 0 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(0),0);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					//change robot follower state to 0 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(1),0);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					//change Availability to 1 for robot1
					try {
						RobotUtils.updateRobotAvailabilityByIp(ipRobot.get(0),1);
					} catch (ClassNotFoundException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (SQLException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					//change Availability to 1 for robot2
					try {
						RobotUtils.updateRobotAvailabilityByIp(ipRobot.get(1),1);
					} catch (ClassNotFoundException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (SQLException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
		        }
		        
		        //--------------------------------------------------------------------------------------------------------- 
		        //Two agent finished
		        //---------------------------------------------------------------------------------------------------------
		        
		        
	        
		        
		        //--------------------------------------------------------------------------------------------------------- 
		        //Three agent
		        //---------------------------------------------------------------------------------------------------------
		        if(noOfAgent==3)
		        {
		        	/*---------------------------------------------------------------------------------------------------------
			         *                      moving the Three robot from its position to the start point
			         */
			       
			        System.out.println("moving the Two robot from its position to the start point");
			        //call path planing with (robotpositon , start point)

			      //---------------------------------------Robot1 "Leader"------------------------------------------------------------------
			        dataToWrite = TaskAllocationUtils.dataWrite(x.get(0), y.get(0), X_Start, Y_Start+1);
					try {
						TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					//get the position matrix from the output file of the path planning
					try {
						dataToRead = PathPlanning.readFromFile(readFile);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					xString = dataToRead.get(0);
					//yString=TaskAllocationUtils.shiftPath(dataToRead.get(1), 1);
					yString = dataToRead.get(1);

			        //send the path to robot 
					clientSocket = new ClientSocket(ipRobot.get(0), 6001);
					clientSocket.sendInt(1);
					clientSocket.sendString(xString);
					clientSocket.sendString(yString);
					//---------------------------------------Robot1 "Leader"------------------------------------------------------------------
				
			        
			        //--------------------------------------Robot2 "follower2"------------------------------------------------------------------
			        dataToWrite = TaskAllocationUtils.dataWrite(x.get(1), y.get(1), X_Start+1, Y_Start);
					try {
						TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					//get the position matrix from the output file of the path planning
					try {
						dataToRead = PathPlanning.readFromFile(readFile);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					xString = dataToRead.get(0);
					//xString=TaskAllocationUtils.shiftPath(dataToRead.get(0), 1);
					yString = dataToRead.get(1);

			        //send the path to robot 
					clientSocket1 = new ClientSocket(ipRobot.get(1), 6001);
					clientSocket1.sendInt(1);
					clientSocket1.sendString(xString);
					clientSocket1.sendString(yString);
					//--------------------------------------Robot2 "follower2"------------------------------------------------------------------
					
					//--------------------------------------Robot3 "follower3"------------------------------------------------------------------
					dataToWrite = TaskAllocationUtils.dataWrite(x.get(2), y.get(2), X_Start-1, Y_Start);
					try {
						TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					//get the position matrix from the output file of the path planning
					try {
						dataToRead = PathPlanning.readFromFile(readFile);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					xString = dataToRead.get(0);
					//xString=TaskAllocationUtils.shiftPath(dataToRead.get(1), -1);
					yString = dataToRead.get(1);

			        //send the path to robot 
					clientSocket2 = new ClientSocket(ipRobot.get(2), 6001);
					clientSocket2.sendInt(1);
					clientSocket2.sendString(xString);
					clientSocket2.sendString(yString);
					//--------------------------------------Robot2 "follower"------------------------------------------------------------------

			        //wait till the Tow robots reach to the start point feedback needed
					while (true) 
					{	
						if(clientSocket.getint() == 1 && clientSocket1.getint() == 1 && clientSocket2.getint() == 1)
						{
							break;
						}
						continue;
					}
					//-------------------------------Three agents Phase 1 Finished----------------------------------------
					//change robot 1 leader state to 2 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(0),2);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					//change robot 2 follower state to 0 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(1),2);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					//change robot 3 follower state to 0 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(2),2);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					
					/*---------------------------------------------------------------------------------------------------------
			         *                 connect to the (leader , the follower) and send the path to the leader
			         */
					System.out.println("moving the Two robot from its position to the start point");
					
					//-------------------------call the Robot2 "follower first"------------------------------------
					clientSocket.sendInt(3);
					
					clientSocket1.sendInt(3);
					
					clientSocket2.sendInt(3);
					
					clientSocket1.sendString("follower");
					clientSocket2.sendString("follower");
					try {
						TimeUnit.MILLISECONDS.sleep(500);
					} catch (InterruptedException e2) {
						// TODO Auto-generated catch block
						e2.printStackTrace();
					}
					clientSocket.sendString("leader");
					
					//call path planning
					dataToWrite = TaskAllocationUtils.dataWrite(X_Start, Y_Start+1, X_End, Y_End+1);
		    		try {
						TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
		    		//get the position matrix from the output file of the path planning
		    		try {
						dataToRead = PathPlanning.readFromFile(readFile);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}

		    		xString = dataToRead.get(0);
		    		yString = dataToRead.get(1);

		    		//send the path to the leader
		    		clientSocket.sendString(xString);
		    		clientSocket.sendString(yString);
		    		
					clientSocket.sendString(ipRobot.get(1));
					clientSocket.sendString(ipRobot.get(2));
		    		
		    		while (clientSocket.getint() != 2) 
		    		{
						continue;
					}
		        
					//-------------------------------Three agents Phase 2 Finished----------------------------------------
			        	
		    		//change robot 1 leader state to 3 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(0),3);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					//change robot 2 follower state to 3 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(1),3);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					//change robot 3 follower state to 3 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(2),3);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
		    		
					//-------------------------------Three agents Phase 3----------------------------------------
			        //---------------------------------------Robot1 "Leader"------------------------------------------------------------------
			        dataToWrite = TaskAllocationUtils.dataWrite(X_End , Y_End + 1, x.get(0), y.get(0));
					try {
						TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					//get the position matrix from the output file of the path planning
					try {
						dataToRead = PathPlanning.readFromFile(readFile);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					xString= dataToRead.get(0);
					yString= dataToRead.get(1);
	
			        //send the path to robot 
					clientSocket.sendString(xString);
					clientSocket.sendString(yString);
					//---------------------------------------Robot1 "Leader"------------------------------------------------------------------
					
					//--------------------------------------Robot2 "follower2"------------------------------------------------------------------
					dataToWrite = TaskAllocationUtils.dataWrite(X_End + 1, Y_End, x.get(1), y.get(1));
					try {
						TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					//get the position matrix from the output file of the path planning
					try {
						dataToRead = PathPlanning.readFromFile(readFile);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					xString= dataToRead.get(0);
					yString= dataToRead.get(1);
	
			        //send the path to robot 
					clientSocket1.sendString(xString);
					clientSocket1.sendString(yString);
					//--------------------------------------Robot2 "follower2"------------------------------------------------------------------		        		        
			    
					//--------------------------------------Robot3 "follower3"------------------------------------------------------------------
					dataToWrite = TaskAllocationUtils.dataWrite(X_End - 1, Y_End, x.get(2), y.get(2));
					try {
						TaskAllocationUtils.writeAndPath(writeFile, dataToWrite, exePath, 1);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					//get the position matrix from the output file of the path planning
					try {
						dataToRead = PathPlanning.readFromFile(readFile);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					xString= dataToRead.get(0);
					yString= dataToRead.get(1);
	
			        //send the path to robot 
					clientSocket2.sendString(xString);
					clientSocket2.sendString(yString);
					
					while (true) 
					{	
						if(clientSocket.getint() == 3 && clientSocket1.getint() == 3 && clientSocket2.getint() == 3)
						{
							break;
						}
						continue;
					}
					clientSocket.sendString("done");
					clientSocket1.sendString("done");
					clientSocket2.sendString("done");
					//--------------------------------------Robot3 "follower3"------------------------------------------------------------------		        		        
					//-------------------------------Three agents Phase 3 Finished----------------------------------------
		        
					//change robot 1 leader state to 0 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(0),0);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					//change robot 2 follower state to 0 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(1),0);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					//change robot 3 follower state to 0 
					try {
						RobotUtils.updateRobotStateyByIp(ipRobot.get(2),0);
					} catch (ClassNotFoundException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					} catch (SQLException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					//change Availability to 1 for robot1
					try {
						RobotUtils.updateRobotAvailabilityByIp(ipRobot.get(0),1);
					} catch (ClassNotFoundException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (SQLException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					//change Availability to 1 for robot2
					try {
						RobotUtils.updateRobotAvailabilityByIp(ipRobot.get(1),1);
					} catch (ClassNotFoundException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (SQLException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					//change Availability to 1 for robot3
					try {
						RobotUtils.updateRobotAvailabilityByIp(ipRobot.get(2),1);
					} catch (ClassNotFoundException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (SQLException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
		        }
		        
		}
			
		}
       


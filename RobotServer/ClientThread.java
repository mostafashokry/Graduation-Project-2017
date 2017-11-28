package server;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.omg.CORBA.INTERNAL;


public class ClientThread implements Runnable {
	
	private Socket socket;
	private DataInputStream in = null;
    private static DataOutputStream out = null;
	private static ScheduledExecutorService executorService;
	private boolean running = true;
	private static ClientSocket clientSocket1;
	private static ClientSocket clientSocket2;
	private static int xfeedback, yfeedback;
	
	public ClientThread(){
		}
	
	public ClientThread(Socket socket) {
		
		this.socket = socket;
	
		try {
			out  = new DataOutputStream(socket.getOutputStream());
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	

	@Override
	public void run() {   
		String stopFlag="";
		int[] goalPoint = new int[2];
		 try {
			  in = new DataInputStream(socket.getInputStream());
			  out = new DataOutputStream(socket.getOutputStream());
		} catch (IOException e) {
			e.printStackTrace();
			 System.out.println("in or out failed");
		}
		 
		      try{
		    	  while (running ) {
		    		  
		    		
		    		  
		     /*-------------------------------------------------phase 1--------------------------------------------------------
		      *                             moving the robot from start point to object
		      */
		    		  int numOFAgents = in.readInt();
		    		  goalPoint =sendPath(numOFAgents, 0);
		    		  System.out.println("goalPoint "+ goalPoint[0]);
		    		  System.out.println("goalPoint "+ goalPoint[1]);
		    		   xfeedback = 0;
		    		   yfeedback = 0;
		    		  while ((xfeedback !=goalPoint[0]) || (yfeedback != goalPoint[1])) {
						  		try {
						TimeUnit.SECONDS.sleep(1);
						} catch (InterruptedException e1) {
							// TODO Auto-generated catch block
						e1.printStackTrace();
						}
						int[] feedback = feedback();
		    			  xfeedback = feedback[0];
		    			  yfeedback = feedback[1];
		    			  System.out.println("xfeedback "+ xfeedback);
		    			  System.out.println("yfeedback "+ yfeedback);
						
					}
					  System.out.println("xreached "+ xfeedback);
					  System.out.println("yreached "+ yfeedback);
		    		   
						
		    		  out.writeInt(1);
		    		  System.out.println("Phase1 Done");
		 	  /*------------------------------------phase 2 single agent--------------------------------------------------------
		 	   *                             moving the robot from object to goal
		 	   */		 
		    		  		    		  
		    		  
		    		  numOFAgents = in.readInt();
		    		  
		    		  if(numOFAgents == 1){
		    			  
		    			  goalPoint =sendPath(numOFAgents, 0);
		    			  			    		  System.out.println("goalPoint PHASE2 "+ goalPoint[0]);
		    		      System.out.println("goalPoint PHASE2 "+ goalPoint[1]);

			    		  
			    		   xfeedback = 0;
			    		   yfeedback = 0;
			    		  while ((xfeedback !=goalPoint[0]) || (yfeedback != goalPoint[1])) {
							  
							    		try {
						TimeUnit.SECONDS.sleep(1);
						} catch (InterruptedException e1) {
							// TODO Auto-generated catch block
						e1.printStackTrace();
						}
							int[] feedback = feedback();
							xfeedback = feedback[0];
			    			yfeedback = feedback[1];
			    			 System.out.println("xfeedback phase 2 "+ xfeedback);
		    			  System.out.println("yfeedback phase 2"+ yfeedback);
			    			
						}
			    		  System.out.println("xreached "+ xfeedback);
					  System.out.println("yreached "+ yfeedback);
		    		  
			    		
			    		  out.writeInt(2);
			 /*-------------------------------------------------phase 3 single agent--------------------------------------------------------
			  *                             moving the robot from goal to start point
			  */    		  
			    		 
			    		 
			    		 
			    		  goalPoint =sendPath(numOFAgents, 0);
			    		  System.out.println("goalPoint "+ goalPoint[0]);
		    		      System.out.println("goalPoint "+ goalPoint[1]);
			    		  
			    		   xfeedback = 0;
			    		   yfeedback = 0;
			    		   
			    		  while ((xfeedback !=goalPoint[0]) || (yfeedback != goalPoint[1])) {
							 try {
						TimeUnit.SECONDS.sleep(1);
						} catch (InterruptedException e1) {
							// TODO Auto-generated catch block
						e1.printStackTrace();
						}
								int[] feedback = feedback();
								xfeedback = feedback[0];
				    			yfeedback = feedback[1];
				    			 System.out.println("xfeedback phase 3 "+ xfeedback);
		    			  System.out.println("yfeedback phase 3"+ yfeedback);
						}
			    		  
			    		 
			    		  out.writeInt(3);

		    			  
		    		  }
		    		  
		 	   /*----------------------------------------------phase 2 multiagent--------------------------------------------------------
		 		*                                   moving the robot from goal to start point
		 		*/ 
		    		  else if (numOFAgents > 1) {
						
		    			  if(in.readUTF().equals("leader")){
		    				  
		    				  String x = in.readUTF();
				    		  String y = in.readUTF();
 
				    		  int[] xArray = convertToArray(x);
				    		  int[] yArray = convertToArray(y);
				    		  
				    		  
				    		  Runnable sendPositionTimer = null;
				    		  int xBack=0 , yBack =0 ;
							  
				    		  if (numOFAgents == 2) {
								String robot1Ip = in.readUTF();
								clientSocket1 = new ClientSocket(robot1Ip, 8001);
								
								 sendPositionTimer = new Runnable() {
						   		       public void run() {				   		 
											int[] feedback = feedback();
											String feed = convertToString(feedback);
						   				if( xBack ==xArray[xArray.length-1] && yBack == yArray[yArray.length-1])
						   			     {
						   				      clientSocket1.sendString("finish");
						   			           executorService.shutdown();
						   			     }
						   			     
						   				clientSocket1.sendString(feed);
						   			   
						   		       }
						    		  };
								
							}
				    		  
				    		  else if (numOFAgents == 3) {
				    			  String robot1Ip = in.readUTF();
				    			  String robot2Ip = in.readUTF();
									clientSocket1 = new ClientSocket(robot1Ip, 8001);
									clientSocket2 = new ClientSocket(robot2Ip, 8001);
									
									 sendPositionTimer = new Runnable() {
							   		       public void run() {				   		 
												int[] feedback = feedback();
												String feed = convertToString(feedback);

							   				if( xBack ==xArray[xArray.length-1] && yBack == yArray[yArray.length-1])
							   			     {
							   					clientSocket1.sendString("finish");
							   					clientSocket2.sendString("finish");
							   			        executorService.shutdown();
							   			     }
							   			    
							   				
												clientSocket1.sendString(feed);
												clientSocket2.sendString(feed);
											
							   					

							   		       }
							    		  };
								
							}
				    		  
				    			executorService = Executors.newSingleThreadScheduledExecutor();	
				    		      executorService.scheduleWithFixedDelay(sendPositionTimer, 0, 200, TimeUnit.MILLISECONDS);
				    		 
				    		   // call send path(xArray,yArray,no of agent,leader=1) 
				    		      sendPath1(xArray, yArray, numOFAgents, 1);
	
						    	xfeedback = 0;
					    		yfeedback = 0;
						    	while (xfeedback != goalPoint[0] || yfeedback != goalPoint[1]) {
									int[] feedback = feedback();
									xfeedback = feedback[0];
					    			yfeedback = feedback[1];
								}
				    		    
				    		  out.writeInt(2);
				    		  
				    	  
		    			}
		    			  else if (in.readUTF().equals("follower")) {
		    				ServerSocket  serverSocket = new ServerSocket(8001);
		    				Socket socket = serverSocket.accept();
		    				DataInputStream in = null;
		    				String feedback = "";
		    				in = new DataInputStream(socket.getInputStream());
		    				
		    				sendVelocity( 2,  0);
		    				
		    				
		    				while (!feedback.equals("finish")) {
								
		    					 feedback = in.readUTF();
		    					 // control function 
		    					 int[] LeaderFeedback = convertToArrayFeedback(feedback);
		    					 int[] FollowerFeedback=feedback();
		    					 
		    					 int xl =(LeaderFeedback[0]*25)+LeaderFeedback[3];
		    					 int yl =(LeaderFeedback[1]*25)+LeaderFeedback[4];
		    					 int xf =(FollowerFeedback[0]*25)+LeaderFeedback[3];
		    					 int yf =(FollowerFeedback[1]*25)+LeaderFeedback[4];
		    					 int Threshold = 30;
		    					 double Kp=0; 
		    					 
		    					 
		    					int X2=(int) Math.pow((xl-xf), 2); 	
		    					int Y2=(int) Math.pow((yl-yf), 2); 	
		    					int Vx=(int)Math.round( (LeaderFeedback[5]+Kp*(35.3553-(Math.sqrt(X2+Y2)))));
		    					int Vy=(int)Math.round( (LeaderFeedback[6]+Kp*(35.3553-(Math.sqrt(X2+Y2)))));
		    					
		    					if (LeaderFeedback[5]==0 &&LeaderFeedback[6]==0 )
		    					{
		    						continue;
		    					}
		    					
		    					if (Math.abs(LeaderFeedback[0]-FollowerFeedback[0])>1)
		    					{
		    						if (LeaderFeedback[0]>FollowerFeedback[0])
		    						{
		    							Vx=Threshold;
		    						}
		    						else if (FollowerFeedback[0]>LeaderFeedback[0])
		    						{
		    							Vx= Math.round(Vx/2);
		    						}
		    					}
		    					
		    					else if (Math.abs(LeaderFeedback[0]-FollowerFeedback[0])<1)
		    					{
		    						if (LeaderFeedback[0]>FollowerFeedback[0])
		    						{
		    							Vx=Math.round(Vx/2);
		    						}
		    						else if (FollowerFeedback[0]>LeaderFeedback[0])
		    						{
		    							Vx= Threshold;
		    						}
		    					}
		    					

		    					if (Math.abs(LeaderFeedback[1]-FollowerFeedback[1])>2)
		    					{
		    						Vy=Threshold;
		    					}
		    					else if (Math.abs(LeaderFeedback[1]-FollowerFeedback[1])<2)
		    					{
		    						Vy= Math.round(Vy/2);
		    					}	
		    					
		    					System.out.println(Vx+"    "+Vy);
		    					sendVelocity( Vx,  Vy);
		    					
							}
		    				//,vyvx=0
		    				sendVelocity(0, 0);
		    				serverSocket.close();
		    				socket.close();
		    				in.close();
		    				
						}
		    			  
		    	  /*----------------------------------------------phase 3 multiagent--------------------------------------------------------
		  		   *                                   moving the robot from object to start point
		  		   */ 
		    			  goalPoint =sendPath(numOFAgents, 0);
		    				
					    	xfeedback = 0;
				    		yfeedback = 0;
					    	while (xfeedback != goalPoint[0] && yfeedback != goalPoint[1]) {
								int[] feedback = feedback();
								xfeedback = feedback[0];
				    			yfeedback = feedback[1];
							}
			    		  
			    		  out.writeInt(3);
				    	
					}

		    		  
		    		stopFlag = in.readUTF();  
		    		  
		    		  if (stopFlag == null) {
							continue;
						}
				      
				      System.out.println("'"+stopFlag +"'"+ " from " + socket.getRemoteSocketAddress()+ " on port " + socket.getLocalPort());				      				    												
				      
				     if (stopFlag.equalsIgnoreCase("done")) {
							//executorService.shutdown();

						running = false;
						Server.clientNum--;
						System.out.println("*** Stopping client thread for client : " + socket.getRemoteSocketAddress()+" ***");
						System.out.println("*** number of clients "+Server.clientNum + "***");
					}
		    	  }
		      }catch (IOException e) {
		    	   e.printStackTrace();
		        System.out.println("Read failed");
		        
		       }
		      finally {
				try {
					socket.close();
					in.close();
					out.close();
					//out.close();
					System.out.println("...Stopped"); 
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		   	 
	}

	
	static{
		System.loadLibrary("SendPath");
		}
	private native  void sendPath1(int[] pathX, int[]  pathY, int numOfAgent, int leader);
	
	static {
        System.loadLibrary("Feedback");
    }
    
    private  native  int[] feedbackPosition();
    
    static{
		System.loadLibrary("SendVelocity");
		}
	private native   void sendVelocity(int vx, int vy);
	  
	private static int[] feedback() {
		// TODO Auto-generated method stub
		ClientThread f = new ClientThread();
		int[] feedback = f.feedbackPosition() ;
		return feedback;
	}



	public int[] sendPath(int numOfAgents , int leaderFlag) {
		ClientThread s = new ClientThread();
		String x = null;
		String y = null;
		int[] goalPoint = new int[2];
		try {
			 x= in.readUTF();
			 y= in.readUTF();
			 System.out.println("path x: "+x);
			 System.out.println("path y: "+y);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		 int[] xArray = convertToArray(x);
		 int[] yArray = convertToArray(y);
		 goalPoint[0] = xArray[xArray.length -2];
		 goalPoint[1] = yArray[yArray.length -2];

		// call send path(xArray,yArray,no of agent,leader=0)
		 s.sendPath1(xArray, yArray,  numOfAgents, leaderFlag);
		 
		 return goalPoint;
	}
	
	public static int[] convertToArray(String input) {
		input += ",255";
		String[] stringArray= input.split(",");
		int[] intArray = new int[stringArray.length];
		for (int i = 0; i < stringArray.length; i++) {
			intArray[i]=Integer.parseInt(stringArray[i]);
		}
		return intArray;
	}
	
	public static int[] convertToArrayFeedback(String input) {
		String[] stringArray= input.split(",");
		int[] intArray = new int[stringArray.length];
		for (int i = 0; i < stringArray.length; i++) {
			intArray[i]=Integer.parseInt(stringArray[i]);
		}
		return intArray;
	}
	
	public static String convertToString(int[] input){
		String feedback =Integer.toString(input[0]);
		for (int i = 1; i < input.length; i++) {
			feedback += ","+input[i]; 
		}
		return feedback;
	}
	
	
	public static void sendPosition() {
		
		
	}
	 
}

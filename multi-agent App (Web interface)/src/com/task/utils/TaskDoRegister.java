package com.task.utils;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.multiagent.pathplanning.PathPlanning;
import com.multiagent.robot.RobotUtils;
import com.multiagent.server.ClientSocket;
import com.multiagent.taskallocation.AssignTask;
import com.multiagent.taskallocation.TaskAllocation;
import com.multiagent.taskallocation.TaskAllocationUtils;
import com.mysql.jdbc.PreparedStatement;

import c.beans.robot;
import db.connection.mySQLConnUtils;

/**
 * Servlet implementation class TaskDoRegister
 */
@WebServlet("/TaskDoRegister")
public class TaskDoRegister extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public TaskDoRegister() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
//		response.getWriter().append("Served at: ").append(request.getContextPath());
		
		String writeFile = "C:\\Users\\DEll-Pc\\Desktop\\V11_NewMap\\V11\\bin\\Debug\\Input_Start_and_End_Points.txt" ;
		String exePath = "C:\\Users\\DEll-Pc\\Desktop\\V11_NewMap\\V11\\bin\\Debug\\PathPlanning.exe";
		String readFile = "C:\\Users\\DEll-Pc\\Desktop\\V11_NewMap\\V11\\bin\\Debug\\Output_Position_matrix.txt";
		
		String ip =null;
		Connection conn =null;
		try {
			conn =mySQLConnUtils.getMySQLConnection();
		} catch (ClassNotFoundException | SQLException e3) {
			// TODO Auto-generated catch block
			e3.printStackTrace();
		}
		Task task = null;
		int i=0;
		String noRobotS = null;
		int noOfAgent = 0;
		String taskName = request.getParameter("taskName");
		String taskLoadS = request.getParameter("taskLoad");
		String startX = request.getParameter("startLocationX");
		String startY = request.getParameter("startLocationY");
		String endX = request.getParameter("endLocationX");
		String endY = request.getParameter("endLocationY");
		float taskLoadF = Float.parseFloat(taskLoadS);
		int startXI = Integer.parseInt(startX);
		int startYI = Integer.parseInt(startY);
		int endXI = Integer.parseInt(endX);
		int endYI = Integer.parseInt(endY);
		List<Integer> data = new ArrayList<>();
		data.add(startXI);
		data.add(startYI);
		data.add(endXI);
		data.add(endYI);
//		String s = String.
		try {
			noOfAgent = RobotUtils.calculateNoOfRobotsForTask(conn,taskLoadF);
		} catch (ClassNotFoundException | SQLException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		}
//		String dataS = data.toString();
//		noRobotS = Integer.toString(noRobot);
//		PathPlanning.writeInFile(writeFile,noRobotS, dataS);
//		System.out.println("before run exe");
//		PathPlanning.pathForRobot(exePath);
//		System.out.println("after run exe");
//		
		try {
			TimeUnit.SECONDS.sleep(1);
		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		FileReader reader = new FileReader(readFile);
		BufferedReader bReader = new BufferedReader(reader);
		String xString = bReader.readLine();
		String yString = bReader.readLine();
		bReader.close();
		int noOfAllRobot = 0;
		try {
			noOfAllRobot = RobotUtils.robotCount(conn);
		} catch (SQLException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
//		TaskAllocationUtils.setFlage();
//		TaskAllocation.setTaskAlloBusy();
			
			AssignTask assignTask = new AssignTask(writeFile, exePath, readFile, noOfAllRobot,
					startXI, startYI, endXI, endYI, noOfAgent);
			Thread assignTaskThread = new Thread(assignTask);
			assignTaskThread.start();

//		try {
//			 ip = TaskAllocation.taskAllocationFn(writeFile, exePath, readFile, noOfAllRobot, startXI, startYI, endXI, endYI, noOfAgent);
//		} catch (ClassNotFoundException e1) {
//			// TODO Auto-generated catch block
//			e1.printStackTrace();
//		} catch (SQLException e1) {
//			// TODO Auto-generated catch block
//			e1.printStackTrace();
//		}
//		ClientSocket clientSocket = new ClientSocket(ip, 6001);
//		clientSocket.send(xString);
//		clientSocket.send(yString);
//		clientSocket.send("done");
//		clientSocket.end();
		System.out.println("end");
		
		// read file extract x and y
//		try {
//			BufferedReader bReader = new BufferedReader(new FileReader("F:\\ACMTraining2017\\PathPlanningC\\Output_Position_matrix.txt"));

		System.out.println("inside do task");
		try {
//			conn =mySQLConnUtils.getMySQLConnection();
			i = TaskUtils.taskRegister(conn, taskLoadF, startXI, startYI, endXI, endYI, taskName);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if(i>0){
			 RequestDispatcher dispatcher
           = this.getServletContext().getRequestDispatcher("/WEB-INF/views/homePage.jsp");

           dispatcher.forward(request, response);		}
             
//			else                
//				out.println("Insert Unsuccessful"); 
//		
		System.out.println("do task");
		
		
		
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		
		doGet(request, response);
	}

}

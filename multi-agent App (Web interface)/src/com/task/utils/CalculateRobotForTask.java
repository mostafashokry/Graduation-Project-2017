package com.task.utils;

import java.sql.Connection;
import java.sql.SQLException;
import java.util.List;

import com.multiagent.robot.RobotUtils;

import c.beans.robot;
import db.connection.mySQLConnUtils;

public class CalculateRobotForTask {
	
//	public static List<robotAccount> calculateRobotTask(Connection conn, List<robotAccount> robotAvailability) throws SQLException, ClassNotFoundException{
//		
////		Connection conn = mySQLConnUtils.getMySQLConnection();
//		List<robotAccount> availablreRobot = RobotUtils.assignRobotTask(conn);
//		
//		return availablreRobot;
//	}
	
	
	public static void main() throws ClassNotFoundException, SQLException{
		Connection conn = mySQLConnUtils.getMySQLConnection();
//		List <robotAccount> cn = CalculateRobotForTask.calculateRobotTask(conn);
//		System.out.println(cn.get(0));
	}

}

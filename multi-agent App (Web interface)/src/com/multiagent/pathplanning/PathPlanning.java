package com.multiagent.pathplanning;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

import com.multiagent.taskallocation.TaskAllocation;

public class PathPlanning {
	
	public static void writeInFile(String filePath, String noRobot, String data){
		FileWriter fop = null;
		File file;
		try {
			String h = noRobot + " ";
			file = new File(filePath);
			fop = new FileWriter(file);
			// if file doesnt exists, then create it
			if (!file.exists()) {
				file.createNewFile();
			}
//			String s = data.toString();
			// get the content in bytes
//			byte[] contentInBytes = sb.getBytes();
			
			fop.write(h);
			//fop.newLine();
			fop.write(data);
			fop.flush();
			fop.close();

			System.out.println("Done");

		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				if (fop != null) {
					fop.close();
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
		
	}
	
//	public static void writeInFile(String filePath,  String data){
//		FileWriter fop = null;
//		File file;
//		try {
//			//String h = noRobot + " ";
//			file = new File(filePath);
//			fop = new FileWriter(file);
//			// if file doesnt exists, then create it
//			if (!file.exists()) {
//				file.createNewFile();
//			}
////			String s = data.toString();
//			// get the content in bytes
////			byte[] contentInBytes = sb.getBytes();
//			
//		//fop.write(h);
//			//fop.newLine();
//			fop.write(data);
//			fop.flush();
//			fop.close();
//
//			System.out.println("Done");
//
//		} catch (IOException e) {
//			e.printStackTrace();
//		} finally {
//			try {
//				if (fop != null) {
//					fop.close();
//				}
//			} catch (IOException e) {
//				e.printStackTrace();
//			}
//		}
//		
//		
//	}

	
	public static List<String> readFromFile(String filePath) throws IOException{
		
		List<String> path = new ArrayList<>();
		BufferedReader br = null;
		FileReader fr = null;
		int lines = 0;
		
		try {

			fr = new FileReader(filePath);
			br = new BufferedReader(fr);

			String sCurrentLine;

			br = new BufferedReader(new FileReader(filePath));
//			while (br.readLine() != null) lines++;
//			for(int i=0; i<lines ; i++){
				while ((sCurrentLine = br.readLine()) != null) {
					path.add(sCurrentLine);
					System.out.println(sCurrentLine);
				}
//			}


		} catch (IOException e) {

			e.printStackTrace();

		} finally {

			try {

				if (br != null)
					br.close();

				if (fr != null)
					fr.close();

			} catch (IOException ex) {

				ex.printStackTrace();

			}

		}
		return path;

	}
		
	
	
	public static void pathForRobot(String pathOfExE) throws IOException{
		StringBuilder sb = new StringBuilder();
		Process process = Runtime.getRuntime().exec(pathOfExE);
	}
	
	public static void main(String[] args) throws IOException {
//		// TODO Auto-generated method stub		
//		System.out.println("Running");
//		String pathOfExE  = "F:\\ACMTraining2017\\PathPlanning\\Debug\\PathPlanning.exe";
//		String filePath = "F:\\Gradproject\\practice\\RunEXEFile\\file1.txt";
//		pathForRobot(pathOfExE);
//		System.out.println("Done");
//		List<String> g = readFromFile("E:\\@GP\\GradPro-master\\V7\\V7\\Input Map.txt");
//		for(int i=0; i<2; i++){
//			System.out.println("list"+g.get(i));
//		}
////		StringBuilder sb = new StringBuilder(g.get(1));
////		sb.deleteCharAt(0);
//		Vector<Integer> v = TaskAllocation.stringPathInt(g.get(1));
//		System.out.println(v);
//		System.out.println("length"+g.get(1).length());
	}
}

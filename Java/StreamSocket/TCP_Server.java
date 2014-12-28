package com.andreapivetta.server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

class Service extends Thread {
	private Socket clientSocket;
	
	public Service(Socket clientSocket) {
		this.clientSocket = clientSocket;
	}
	
	public void run() {
		System.out.println("Service " + getName() + " is running");
		try {
			DataInputStream in = new DataInputStream(clientSocket.getInputStream());
			DataOutputStream out = new DataOutputStream(clientSocket.getOutputStream());				
			out.writeUTF(in.readUTF());
			clientSocket.close();
			in.close();
			out.close();
		} catch (IOException e) {
			e.printStackTrace();
		};
	}
}

public class Server {

	private static final int ARGC = 1;
	private static final String USAGE = "java MyName port";
	private static final String PORT_MESSAGE = "Port must be in [1024, 65535";

	public static void main(String[] args) {
		if (args.length != ARGC) {
			System.out.println(USAGE);
			System.exit(1);
		}
		
		int port = 0;
		try {
			port = Integer.parseInt(args[0]);
		} catch (NumberFormatException e) {
			System.out.println(USAGE);
			System.exit(1);
		}
		
		if (port < 1024 || port > 65535) {
			System.out.println(PORT_MESSAGE);
			System.exit(1);
		}
				
		try {
			ServerSocket serverSocket = new ServerSocket(port);
			serverSocket.setReuseAddress(true);
			
			System.out.println("Server is running");
			while(true) {
				(new Service(serverSocket.accept())).start();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}

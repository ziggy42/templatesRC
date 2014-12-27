package com.andreapivetta.client;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.net.UnknownHostException;

public class Client {

	private static final int ARGC = 2;
	private static final String USAGE = "java MyName ip port";
	private static final String IN_MESSAGE = "Insert query: ";
	private static final String PORT_MESSAGE = "Port must be in [1024, 65535";

	public static void main(String[] args) {
		if (args.length != ARGC) {
			System.out.println(USAGE);
			System.exit(1);
		}

		int port = 0;
		try {
			port = Integer.parseInt(args[1]);
		} catch (NumberFormatException e) {
			System.out.println(USAGE);
			System.exit(1);
		}

		if (port < 1024 || port > 65535) {
			System.out.println(PORT_MESSAGE);
			System.exit(1);
		}

		String query;
		DataOutputStream out;
		DataInputStream in;
		
		try {
			Socket socket = new Socket(args[0], port);
			BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
			System.out.println(IN_MESSAGE);
			while ((query = stdIn.readLine()) != null) {
				socket = new Socket(args[0], port);
				out = new DataOutputStream(socket.getOutputStream());
				in = new DataInputStream(socket.getInputStream());
				
				out.writeUTF(query);
				System.out.println("ANSWER: " + in.readUTF() + "\n"
						+ IN_MESSAGE);
			}
			socket.close();
		} catch (UnknownHostException e) {
			System.out.println("Server not found");
			System.exit(1);
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("Ops: IOException");
			System.exit(1);
		}

	}

}

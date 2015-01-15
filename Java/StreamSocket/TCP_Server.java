package server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;

class Service extends Thread {

	private static final boolean DEBUG = true;
	private Socket clientSocket;

	public Service(Socket clientSocket) {
		this.clientSocket = clientSocket;
	}

	public void run() {
		System.out.println("\n" + getName() + " is running.");

		DataInputStream in = null;
		DataOutputStream out = null;

		try {
			in = new DataInputStream(clientSocket.getInputStream());
			out = new DataOutputStream(clientSocket.getOutputStream());
		} catch (IOException e) {
			if (DEBUG)
				e.printStackTrace();

			System.out.println("Error creating data stream");
			System.exit(1);
		}

		String query;
		try {
			while ((query = in.readUTF()) != null) {
				System.out.println(getName() + ": request: " + query);

				try {
					out.writeUTF(query);
				} catch (IOException e) {
					if (DEBUG)
						e.printStackTrace();

					System.out.println("Error writing response.");
					continue;
				}
			}
		} catch (EOFException e) {
			System.out.println("Connection closed.");
		} catch (IOException e) {
			if (DEBUG)
				e.printStackTrace();
			System.out.println("Ops: IOException");
		}

		try {
			in.close();
			out.close();
			clientSocket.close();
		} catch (IOException e) {
			if (DEBUG)
				e.printStackTrace();
			System.out.println("Error closing comunication");
		}
	}
}

public class TCP_Server {
	private static final boolean DEBUG = true;
	private static final int ARGC = 1;
	private static final String USAGE = "USAGE: java MyName port";
	private static final String PORT_MESSAGE = "Port must be in [1024, 65535]";

	@SuppressWarnings("resource")
	public static void main(String args[]) {
		if (args.length != ARGC) {
			System.out.println(USAGE);
			System.exit(1);
		}

		int port = 0;
		try {
			port = Integer.parseInt(args[0]);
		} catch (NumberFormatException e) {
			System.out.println(USAGE);
			System.out.println("Port must be integer.");
			System.exit(1);
		}

		if (port < 1024 || port > 65535) {
			System.out.println(PORT_MESSAGE);
			System.exit(1);
		}

		ServerSocket serverSocket = null;
		try {
			serverSocket = new ServerSocket(port);
			serverSocket.setReuseAddress(true);
		} catch (UnknownHostException e) {
			if (DEBUG)
				e.printStackTrace();

			System.out.println("Error creating socket: unknown host.");
			System.exit(1);
		} catch (IOException e) {
			if (DEBUG)
				e.printStackTrace();

			System.out.println("Ops: IOException.");
		}

		System.out.println("Server is running");
		while (true) {
			try {
				(new Service(serverSocket.accept())).start();
			} catch (IOException e) {
				if (DEBUG)
					e.printStackTrace();

				System.out.println("Error accepting connection.");
			}
		}
	}
}

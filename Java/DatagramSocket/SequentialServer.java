package com.andreapivetta.server;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

public class ServerMain {

	private static final int ARGC = 1;
	private static final String USAGE = "java MyName port";
	private static final String PORT_MESSAGE = "Port must be in [1024, 65535";
	private static final String QUERY_RECEIVED = "Query received, processing...";
	
	public static void main(String[] args) {
		if(args.length != ARGC) {
			System.out.println(USAGE);
			System.exit(1);
		}
				
		int port;
		byte[] buff = new byte[256];
		byte[] data;
		String query;
		
		ByteArrayInputStream biStream;
		ByteArrayOutputStream boStream;
		DataInputStream diStream;
		DataOutputStream doStream;
		
		port = Integer.parseInt(args[0]);
		if (port < 1024 || port > 65535) {
			System.out.println(PORT_MESSAGE);
			System.exit(1);
		}
		
		try {
			DatagramSocket socket = new DatagramSocket(port);
			DatagramPacket packet = new DatagramPacket(buff, buff.length);
			
			System.out.println("Server is waiting");
			
			while(true) {
				packet.setData(buff);
				socket.receive(packet);
				
				System.out.println(QUERY_RECEIVED);
				
				biStream = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
				diStream = new DataInputStream(biStream);
				query = diStream.readUTF();
								
				boStream = new ByteArrayOutputStream();
				doStream = new DataOutputStream(boStream);
				doStream.writeUTF(query);
				data = boStream.toByteArray();
				packet.setData(data);
				socket.send(packet);
				
				System.out.println("DONE!\n\nServer is waiting");
			}			
		} catch (SocketException e) {
			e.printStackTrace();
		} catch (IOException e2) {
			e2.printStackTrace();
		}
		
	}

}


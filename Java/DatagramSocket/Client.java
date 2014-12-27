package com.andreapivetta.client;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class ClientMain {

	private static final int ARGC = 2;
	private static final String USAGE = "java MyName ip port";
	private static final String IN_MESSAGE = "Insert phrase: ";
	
	public static void main(String[] args) {
		if(ARGC != args.length) {
			System.out.println(USAGE);
			System.exit(1);
		}
		
		int port = Integer.parseInt(args[1]);
		byte[] buff = new byte[256];
		byte[] data;
		String query, answer;
		
		ByteArrayOutputStream boStream;
		ByteArrayInputStream biStream;
		DataOutputStream doStream;
		DataInputStream diStream;
				
		try {
			InetAddress addr = InetAddress.getByName(args[0]);
			DatagramSocket socket = new DatagramSocket();
			socket.setSoTimeout(3000);
			DatagramPacket packet = new DatagramPacket(buff, buff.length, addr, port);
			
			BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
			System.out.println(IN_MESSAGE);
			try {
				while((query = stdIn.readLine()) != null) {					
					boStream = new ByteArrayOutputStream();
					doStream = new DataOutputStream(boStream);
					doStream.writeUTF(query);
					data = boStream.toByteArray();
					packet.setData(data);
					socket.send(packet);
					
					packet.setData(buff);
					socket.receive(packet);
					biStream = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
					diStream = new DataInputStream(biStream);
					answer = diStream.readUTF();
					
					System.out.println(answer);
					System.out.println(IN_MESSAGE);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		} catch (SocketException e) {
			e.printStackTrace();
		} catch (UnknownHostException eh) {
			eh.printStackTrace();
		}
	}
}


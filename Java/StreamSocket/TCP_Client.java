import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.net.UnknownHostException;

public class TCP_Client {

	private static final boolean DEBUG = true;
	private static final int ARGC = 2;
	private static final String USAGE = "USAGE: java Client ip port";
	private static final String IN_MESSAGE = "Insert query: ";
	private static final String PORT_MESSAGE = "Port must be in [1024, 65535]";

	public static void main(String args[]) {
		if (args.length != ARGC) {
			System.out.println(USAGE);
			System.exit(1);
		}

		int port = 0;
		try {
			port = Integer.parseInt(args[1]);
		} catch (NumberFormatException e) {
			System.out.println(USAGE);
			System.out.println("Port must be integer");
			System.exit(1);
		}

		if (port < 1024 || port > 65535) {
			System.out.println(PORT_MESSAGE);
			System.exit(1);
		}

		Socket socket = null;
		try {
			socket = new Socket(args[0], port);
		} catch (UnknownHostException e) {
			if (DEBUG)
				e.printStackTrace();

			System.out.println("Error creating socket: unknown host");
			System.exit(1);
		} catch (IOException e) {
			if (DEBUG)
				e.printStackTrace();

			System.out.println("Error creating socket");
			System.exit(1);
		}

		String query;
		DataOutputStream out = null;
		DataInputStream in = null;
		try {
			in = new DataInputStream(socket.getInputStream());
			out = new DataOutputStream(socket.getOutputStream());
		} catch (IOException e) {
			if (DEBUG)
				e.printStackTrace();

			System.out.println("Error creating data stream");
			System.exit(1);
		}

		try {
			BufferedReader stdIn = new BufferedReader(new InputStreamReader(
					System.in));
			System.out.print(IN_MESSAGE);
			while ((query = stdIn.readLine()) != null) {
				out.writeUTF(query);
				System.out.println("ANSWER: " + in.readUTF());
				System.out.print("\n" + IN_MESSAGE);
			}
			socket.close();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("Ops: IOException");
		}

	}

}

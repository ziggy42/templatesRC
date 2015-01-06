package server;

import interf.RemoteInterface;

import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class RemoteServer extends UnicastRemoteObject implements
		RemoteInterface {

	private static final long serialVersionUID = 1L;

	protected RemoteServer() throws RemoteException {
		super();
	}

	@Override
	public String rmiMethod(String input) throws RemoteException {
		return input;
	}

	public static void main(String[] args) {

		final int REGISTRYPORT = 1099;
		String registryHost = "localhost", serviceName = "ServiceName";

		try {
			RemoteServer rmServer = new RemoteServer();
			Naming.rebind("//" + registryHost + ":" + REGISTRYPORT + "/"
					+ serviceName, rmServer);
		} catch (RemoteException | MalformedURLException e) {
			e.printStackTrace();
		}

		System.out.println("Started");
	}

}

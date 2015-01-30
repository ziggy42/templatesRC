import interf.RMI_interfaceFile;

import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class RMI_Server extends UnicastRemoteObject implements
		RMI_interfaceFile {

	private static final long serialVersionUID = 1L;

	protected RMI_Server() throws RemoteException {
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
			RMI_Server rmServer = new RMI_Server();
			Naming.rebind("//" + registryHost + ":" + REGISTRYPORT + "/"
					+ serviceName, rmServer);
		} catch (RemoteException | MalformedURLException e) {
			e.printStackTrace();
		}

		System.out.println("Started");
	}

}

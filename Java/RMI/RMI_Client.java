package client;

import interf.RMI_interfaceFile;

import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;

public class RMI_Client {

	public static void main(String[] args) {

		final int REGISTRYPORT = 1099;
		String registryHost, serviceName = "ServiceName";

		if (args.length != 1) {
			System.out.println("Wrong syntax");
			System.exit(1);
		}
		registryHost = args[0];

		try {
			RMI_interfaceFile rmObj = (RMI_interfaceFile) Naming.lookup("rmi://"
					+ registryHost + ":" + REGISTRYPORT + "/" + serviceName);
			System.out.println("Result: " + rmObj.rmiMethod("Some input"));
		} catch (MalformedURLException | RemoteException | NotBoundException e) {
			e.printStackTrace();
		}
	}

}

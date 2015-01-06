package interf;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RemoteInterface extends Remote {
	
	public String rmiMethod(String input) throws RemoteException;
}

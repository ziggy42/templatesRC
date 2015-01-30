import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RMI_interfaceFile extends Remote {
	
	public String rmiMethod(String input) throws RemoteException;
}

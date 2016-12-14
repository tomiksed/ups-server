package cz.sedivy.ups.test.abstractServer;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.List;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public abstract class AbstractServer implements Runnable {
	private Logger log =  LogManager.getLogger(this.getClass());
	private int DEFAULT_PORT = 1234;
	private int port;
	private List<ClientThread> threads = new ArrayList<>();
	private ServerSocket serverSocket;
	
	public void init(int port) {
		this.port = port;

		Thread t = new Thread(this);
		t.setName("Testovaci server");
		t.start();

	}
	
	
	public void stop() {
		for (ClientThread clientThread : threads) {
			clientThread.close();
		}
		
		try {
			serverSocket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	

	public void init() {
		init(DEFAULT_PORT);
	}

	@Override
	public void run() {

		try {
			serverSocket = new ServerSocket(port);
			while (true) {
				Socket clientSocket = serverSocket.accept();
				ClientThread ct = new ClientThread(this, clientSocket);
				Thread t = new Thread(ct);
				t.setName("ClientSocktet");
				t.start();
				log.debug("Klient pripojen");
				clientConnected(ct);
				threads.add(ct);
			}
		} catch (SocketException e) {
			log.debug("Uzaviram master socket");
		}catch (IOException e) {
			e.printStackTrace();
		}

	}

	public abstract void processRawMessage(ClientThread clientThread, byte[] buff, int len);
	public abstract void clientConnected(ClientThread clientThread);
	public abstract void clientDisconnected(ClientThread clientThread);


	

}

package cz.sedivy.ups.test.abstractServer;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.SocketException;
import java.util.Arrays;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class ClientThread implements Runnable {
	private static final int BUFF_SIZE = 1000;
	private Logger log =  LogManager.getLogger(this.getClass());
	private AbstractServer callback;
	private Socket clientSocket;
	private OutputStream out;

	public ClientThread(AbstractServer abstractServer, Socket clientSocket) {
		this.callback = abstractServer;
		this.clientSocket = clientSocket;
	}

	@Override
	public void run() {
		try {
			out = clientSocket.getOutputStream();
			InputStream in = clientSocket.getInputStream();
			
			byte[] buff = new byte[BUFF_SIZE];
			while (true) {
				int len = in.read(buff);
				
				if(len == -1) {
					log.debug("Uzaviram socket");
					break;
				}
				
				
				
				log.debug("Prisla zprava:  " + Arrays.toString(buff));
				callback.processRawMessage(this, buff,len);
			}
			log.debug("Klient odpojen");
			callback.clientDisconnected(this);
		} catch (SocketException e) {
			log.debug("Uzaviram socket");
		}catch (IOException e) {
			e.printStackTrace();
		} 
	}
	
	public void sendMessage(byte[] message) {
		
		try {
			log.debug("Odelilam zpravu:  " + Arrays.toString(message));
			out.write(message);
			out.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}

	public void close() {
		try {
			clientSocket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}

}

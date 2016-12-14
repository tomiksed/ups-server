package cz.sedivy.ups.networking;

import java.io.IOException;
import java.io.InputStream;	
import java.io.OutputStream;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Arrays;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class TCPClient implements Runnable {
	private static final int BUFF_SIZE = 1000;

	private Logger log = LogManager.getLogger(this.getClass());

	private String host;
	private int port;
	private OutputStream out;
	private InputStream in;
	private Socket socket;
	private NetworkManager callback;

	public TCPClient(String host, int port, NetworkManager callback) {
		log.debug("Constuctor {}:{}", host, port);
		this.host = host;
		this.port = port;
		this.callback = callback;
	}

	public void connect() {
		log.info("Pripojuji se k {}:{}", host, port);
		try {
			socket = new Socket(host, port);
			out = socket.getOutputStream();
			in = socket.getInputStream();
			Thread t = new Thread(this);
			t.setName("TCPClient - naslouchaci vlakno");
			t.start();
			log.debug("-- pripojeno");

		} catch (UnknownHostException e) {
			log.error(e);
		} catch (IOException e) {
			log.error(e);
		}
	}

	@Override
	public void run() {
		log.debug("Spoustim naslouchaci vlakno");
		try {
			
			byte[] buff = new byte[BUFF_SIZE];
			while (true) {
				int len = in.read(buff);
				
				if(len == -1) {
					log.debug("Uzaviram socket");
					break;
				}
				
				log.debug("Prisla zprava:  " + Arrays.toString(buff));
				try {
					callback.processRawMessage(copyAndCut(buff,len));
				} catch (Exception e) {
					callback.rcvBadMsg(e);
				}
			}
			log.debug("Klient odpojen");
			callback.clientOff();
			
		} catch (SocketException e) {
			log.info("Uzaviram spojeni");
		} catch (IOException e) {
			log.warn("Spojeni selhalo");
			callback.clientOff();
		}

	}

	private byte[] copyAndCut(byte[] buff, int len) {
		byte[] bytes = new byte[len];
		for (int i = 0; i < bytes.length; i++) {
			bytes[i] = buff[i];
		}
		return buff;
	}

	public void sendMessage(byte[] bytes) {
		try {
			log.debug("Odesilam zpravu '{}'", Arrays.toString(bytes));
			out.write(bytes);
			out.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void stop() {
		log.info("Zastavuji naslouchani");
		try {
			socket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}

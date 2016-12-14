package cz.sedivy.ups.test.servers;

import java.util.Arrays;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import cz.sedivy.ups.test.abstractServer.AbstractServer;
import cz.sedivy.ups.test.abstractServer.ClientThread;

public class EchoServer extends AbstractServer {
	private Logger log =  LogManager.getLogger(this.getClass());
	
	

	@Override
	public void clientConnected(ClientThread clientThread) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void clientDisconnected(ClientThread clientThread) {
		// TODO Auto-generated method stub
		
	}

	

	@Override
	public void processRawMessage(ClientThread clientThread, byte[] buff,
			int len) {
		log.debug("Otacim zpravu {}", Arrays.toString(buff));
		
		byte[] message = new byte[len];
		for (int i = 0; i < message.length; i++) {
			message[i] = buff[i];
		}
		log.debug("Otacim zpravu {}", Arrays.toString(message));
		clientThread.sendMessage(message);
		
	}



}

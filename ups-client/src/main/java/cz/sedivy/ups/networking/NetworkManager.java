package cz.sedivy.ups.networking;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;



public class NetworkManager {
	private Logger log =  LogManager.getLogger(this.getClass());
	 
	private NetworkManagerCallback callback;
	private TCPClient client;
	private MessageSerializer serializer;
	
	private boolean serialize;  
	private String host;
	private int port;
	
	
	public void init(NetworkManagerCallback callback) {
		log.debug("Inicializuji NM");
		serializer = new MessageSerializer();
		this.callback = callback;
		serialize = true;
	}
	
	public boolean isConnected() {
		if(client == null) return false;
		return true;
	}
	
	public void connect() {
		log.debug("Pripojuji se");
		client = new TCPClient(host, port, this);
		client.connect();
	}
	
	public void disconnect() {
		log.debug("Odpojuji se");
		client.stop();
		client = null;
	}

	public void config(String HOST, int PORT) {
		log.debug("Konfiguruji '{}:{}'", HOST, PORT);
		this.host = HOST;
		this.port = PORT;
	}

	public void clientOff() {
		log.debug("Odpojeno ze strany serveru");
		client = null;
		
	}

	public void sendRawMessage(byte[] bytes) {
		
		if(!isConnected()) {
			log.warn("Neexistuje pripojeni");
			return;
		}
		client.sendMessage(bytes);
		log.debug("-- Odeslano");
	}
	
	
	public void sendMessage(Message message) {
		if(!isConnected()) {
			log.warn("Neexistuje pripojeni");
			return;
		}
		client.sendMessage(serializer.serializeMessage(message));
		log.debug("-- Odeslano");
		
	}

	public void processRawMessage(byte[] bytes) {	
		callback.rcvRawMessage(bytes);
		if(serialize) {
			serializer.checkMessage(bytes);
			Message message = serializer.initMessage(bytes);
			serializer.deserializeMessage(message);
			callback.rcvMessage(message);
		}
		
	}

	public void setSerializing(boolean serialize) {
		this.serialize = serialize;
	}

	public void rcvBadMsg(Exception e) {
		callback.rcvBadMsg(e);	
	}
	
	
}	

package cz.sedivy.ups.networking;

public interface NetworkManagerCallback {

	void rcvRawMessage(byte[] bytes);
	void rcvMessage(Message message);
	void rcvBadMsg(Exception e);

}

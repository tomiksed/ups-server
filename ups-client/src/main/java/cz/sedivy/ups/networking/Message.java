package cz.sedivy.ups.networking;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Message {
	private MessageType type;
	private int rawType;
	private int length;
	private List<Object> payload;
	private byte checksum;
	private byte[] rawPayloads;
	
	public Message() {
		super();
		payload = new ArrayList<Object>();
	}
	public MessageType getType() {
		return type;
	}
	public void setType(MessageType type) {
		this.type = type;
	}
	public int getRawType() {
		return rawType;
	}
	public void setRawType(int rawType) {
		this.rawType = rawType;
	}
	public int getLength() {
		return length;
	}
	public void setLength(int length) {
		this.length = length;
	}
	public List<Object> getPayload() {
		return payload;
	}
	public void setPayload(List<Object> payload) {
		this.payload = payload;
	}
	public byte getChecksum() {
		return checksum;
	}
	public void setChecksum(byte checksum) {
		this.checksum = checksum;
	}
	public byte[] getRawPayloads() {
		return rawPayloads;
	}
	public void setRawPayloads(byte[] rawPayloads) {
		this.rawPayloads = rawPayloads;
	}
	@Override
	public String toString() {
		return "Message [type=" + type + ", rawType=" + rawType + ", length="
				+ length + ", payload=" + payload + ", checksum=" + checksum
				+ ", rawPayloads=" + Arrays.toString(rawPayloads) + "]";
	}
	
	
	
	
	
	
	
}

package cz.sedivy.ups.networking;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import cz.sedivy.ups.networking.errors.BadCheckSumException;
import cz.sedivy.ups.networking.errors.BadHeaderException;
import cz.sedivy.ups.networking.errors.BadLengthException;
import cz.sedivy.ups.networking.errors.TooShortException;

public class MessageSerializer {

	private static final int INTEGER_SIZE = 4;
	private static final int SUM_SIZE = 1;
	private static final int HEADER_POSITION = 0;
	private static final int MSG_LEN_POSITION = 4;
	private static final int PAYLOAD_POSITION = 8;

	private Map<Integer, MessageType> types;

	private DataSerializer dataSerializer;

	public MessageSerializer() {
		types = new HashMap<Integer, MessageType>();
		dataSerializer = new DataSerializer();
		for (MessageType messageType : MessageType.values()) {
			types.put(messageType.headerId, messageType);
		}
	}

	public void checkMessage(byte[] bytes) throws TooShortException,
			BadLengthException, BadCheckSumException {
		checkMinimalSize(bytes);
		checkTotalSize(bytes);
		checkChecksum(bytes);
	}

	private void checkChecksum(byte[] bytes) throws BadCheckSumException {
		int sum = 0;
		for (int i = 0; i < bytes.length; i++) {
			sum = (sum + bytes[i]) % 128;
		}

		if (sum != 0) {
			System.out.println(sum);
			throw new BadCheckSumException();
		}

	}

	private void checkTotalSize(byte[] bytes) throws BadLengthException {
		byte[] lenBytes = Arrays.copyOfRange(bytes, MSG_LEN_POSITION,
				MSG_LEN_POSITION + INTEGER_SIZE);
		int realSize = bytes.length;
		int declaredSize = dataSerializer.bytesToInt(lenBytes);
		if (realSize != declaredSize)
			throw new BadLengthException();
	}

	private void checkMinimalSize(byte[] bytes) throws TooShortException {
		if (bytes.length < (2 * INTEGER_SIZE + SUM_SIZE))
			throw new TooShortException();
	}

	public Message initMessage(byte[] bytes) {
		int headerId = dataSerializer.bytesToInt(Arrays.copyOfRange(bytes,
				HEADER_POSITION, HEADER_POSITION + INTEGER_SIZE));

		Message message = new Message();
		message.setRawType(headerId);
		message.setChecksum(bytes[bytes.length - SUM_SIZE]);
		message.setLength(bytes.length);
		message.setRawPayloads(Arrays.copyOfRange(bytes, PAYLOAD_POSITION,
				bytes.length - SUM_SIZE));

		return message;
	}

	public void deserializeMessage(Message message) throws BadHeaderException {
		if (!types.containsKey(message.getRawType()))
			throw new BadHeaderException();

		message.setType(types.get(message.getRawType()));
		if (message.getType().payloadTypes != null
				&& message.getType().payloadTypes.length > 0) {
			message.setPayload(dataSerializer.bytesToPayload(
					message.getType().payloadTypes, message.getRawPayloads()));
		}

	}

	public byte[] serializeMessage(Message message) {
		byte[] payloadBytes = dataSerializer.payloadToBytes(
				message.getType().payloadTypes, message.getPayload());
		int len = INTEGER_SIZE * 2 + SUM_SIZE + payloadBytes.length;

		ByteArrayOutputStream byteBuilder = new ByteArrayOutputStream();

		try {
			byteBuilder
					.write(dataSerializer.intToBytes(message.getType().headerId));
			byteBuilder.write(dataSerializer.intToBytes(len));
			byteBuilder.write(payloadBytes);
			byteBuilder.write(countCheckSum(
					dataSerializer.intToBytes(message.getType().headerId),
					dataSerializer.intToBytes(len), payloadBytes));

		} catch (IOException e) {
			e.printStackTrace();
		}

		return byteBuilder.toByteArray();
	}

	private byte countCheckSum(byte[] header, byte[] len,
			byte[] payload) {
		int sum = 0;
		for (int i = 0; i < header.length; i++) {
			sum = (sum + header[i]) % 128;
		}
		
		for (int i = 0; i < len.length; i++) {
			sum = (sum + len[i]) % 128;
		}
		
		for (int i = 0; i < payload.length; i++) {
			sum = (sum + payload[i]) % 128;
		}
		return (byte) (128 - (sum % 128)); 
		
	}

}

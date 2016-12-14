package cz.sedivy.ups.networking;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import cz.sedivy.ups.networking.errors.BadPayloadLengthException;
import cz.sedivy.ups.networking.errors.BadPayloadTypeException;

public class DataSerializer {
	private static final int INTEGER_SIZE = 4;

	public Integer bytesToInt(byte[] bytes) {
		return new Integer(bytes[3]); // TODO: dodelat
	}

	public byte[] intToBytes(int value) {
		byte[] bytes = new byte[4];
		bytes[3] = (byte) value;
		return bytes;
	}

	public List<Object> bytesToPayload(
			@SuppressWarnings("rawtypes") Class[] classes, byte[] bytes) {
		try {
			List<Object> list = new ArrayList<Object>();
			int pointer = 0;

			for (Class<?> c : classes) {
				if (c == Integer.class) {
					list.add(bytesToInt(Arrays.copyOfRange(bytes, pointer,
							pointer + INTEGER_SIZE)));
					pointer += INTEGER_SIZE;
				} else if (c == String.class) {
					int len = bytesToInt(Arrays.copyOfRange(bytes, pointer,
							pointer + INTEGER_SIZE));
					pointer += INTEGER_SIZE;
					list.add(bytesToString(Arrays.copyOfRange(bytes, pointer,
							pointer + len)));
					pointer += len;
				}
			}

			if (pointer != bytes.length)
				throw new BadPayloadLengthException();
			return list;
		} catch (IndexOutOfBoundsException e) {
			throw new BadPayloadLengthException();
		}

	}

	public byte[] payloadToBytes(@SuppressWarnings("rawtypes") Class[] classes,
			List<Object> payload) {
		if(classes == null) return new byte[0];
		
		if (classes.length != payload.size())
			throw new BadPayloadLengthException();

		ByteArrayOutputStream byteBuilder = new ByteArrayOutputStream();
		try {
			for (int i = 0; i < classes.length; i++) {
				if (classes[i] != payload.get(i).getClass()) {
					throw new BadPayloadTypeException();
				}
					
				if (classes[i] == String.class) {
					String value = (String) payload.get(i);
					byteBuilder.write(intToBytes(value.length()));
					byteBuilder.write(stringToBytes(value));

				} else if (classes[i] == Integer.class) {
					byteBuilder.write(intToBytes((Integer) payload.get(i)));

				} else
					throw new BadPayloadTypeException();

			}
		} catch (IOException e) {
			e.printStackTrace();
		}

		return byteBuilder.toByteArray();
	}

	private byte[] stringToBytes(String value) {
		return value.getBytes();
	}

	private Object bytesToString(byte[] bytes) {
		return new String(bytes);
	}
}

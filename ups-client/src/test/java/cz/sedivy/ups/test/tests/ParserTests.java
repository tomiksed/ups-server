package cz.sedivy.ups.test.tests;

import static org.junit.Assert.*;

import java.util.Arrays;

import org.junit.Before;
import org.junit.Test;

import cz.sedivy.ups.networking.Message;
import cz.sedivy.ups.networking.MessageSerializer;
import cz.sedivy.ups.networking.MessageType;
import cz.sedivy.ups.networking.errors.BadCheckSumException;
import cz.sedivy.ups.networking.errors.BadHeaderException;
import cz.sedivy.ups.networking.errors.BadLengthException;
import cz.sedivy.ups.networking.errors.BadPayloadLengthException;
import cz.sedivy.ups.networking.errors.TooShortException;

public class ParserTests {

	private byte[][] okMessages = { { 0,0,0,  1,   0,0,0, 9,   118 },
									{ 0,0,0,101,   0,0,0,13,   0,0,0,15,   127 },
									{ 0,0,0,102,   0,0,0,17,   0,0,0,15,   0,0,0,25,   97 },
									{ 0,0,0,103,   0,0,0,17,   0,0,0,4,    65, 66, 99, 100,   58 },
									{ 0,0,0,104,   0,0,0,23,   0,0,0,4,    65, 66, 99, 100,   0,0,0,2,   66, 65,   46 },
									{ 0,0,0,105,   0,0,0,21,   0,0,0,4,    65, 66, 99, 100,   0,0,0,2,   50 },
				
	};

	private byte[] shortMessage = {  0,0,0,0,   0,0,0 };

	private byte[] badLenMessage ={  0,0,0,1,   0,0,0, 5,   115 };

	private byte[] badSumMessage ={  0,0,0,1,   0,0,0,9,   100 };
	
	private byte[] badHeader     ={  0,0,0,99,  0,0,0,9,   20 };

	private byte[] tooLongPayload={ 0,0,0,103,  0,0,0,17,   0,0,0,6,    65, 66, 99, 100,  56 };
	
	private byte[] tooshortPayload={0,0,0,103,  0,0,0,17,   0,0,0,2,    65, 66, 99, 100,  60 };
	

	
	private MessageSerializer parser;

	@Before
	public void setUp() throws Exception {
		parser = new MessageSerializer();
	}

	
	@Test
	public void twoWayTests1() {
		Message m = new Message();
		m.setType(MessageType.PARSING_TEST0);
		
		byte[] data = parser.serializeMessage(m);
		
		parser.checkMessage(data);
		Message newMessage = parser.initMessage(data);
		parser.deserializeMessage(newMessage);
	}
	
	@Test
	public void twoWayTests2() {
		for (byte[] bs : okMessages) {
			parser.checkMessage(bs);
			Message message = parser.initMessage(bs);
			parser.deserializeMessage(message);
			
			byte[] newBytes =  parser.serializeMessage(message);
			
			if(bs.length != newBytes.length) {
				System.out.println("OLD: " + Arrays.toString(bs));
				System.out.println("NEW: " + Arrays.toString(newBytes));
				fail("Pole nejsou stejne dlouha");
			}
			
			for (int i = 0; i < newBytes.length; i++) {
				if(bs[i] != newBytes[i]) {
					System.out.println("OLD: " + Arrays.toString(bs));
					System.out.println("NEW: " + Arrays.toString(newBytes));
					fail("Pole se nerovnaji");
				}
			}
		}
	}
	
	
	@Test
	public void okMessageTests() {
		for (byte[] bs : okMessages) {
			parser.checkMessage(bs);
			Message message = parser.initMessage(bs);
			parser.deserializeMessage(message);

			
		}

	}

	@Test(expected = TooShortException.class)
	public void shortMessageTests() {
		parser.checkMessage(shortMessage);
		Message message = parser.initMessage(shortMessage);
		parser.deserializeMessage(message);

		System.out.println(message);

	}
	
	@Test(expected = BadLengthException.class)
	public void badLenMessageTests() {
		parser.checkMessage(badLenMessage);
		Message message = parser.initMessage(badLenMessage);
		parser.deserializeMessage(message);

		System.out.println(message);

	}
	
	@Test(expected = BadCheckSumException.class)
	public void badCheckSumMessageTests() {
		parser.checkMessage(badSumMessage);
		Message message = parser.initMessage(badSumMessage);
		parser.deserializeMessage(message);

		System.out.println(message);

	}
	
	@Test(expected = BadHeaderException.class)
	public void badHeaderMessageTests() {
		parser.checkMessage(badHeader);
		Message message = parser.initMessage(badHeader);
		parser.deserializeMessage(message);

		System.out.println(message);
	}
	
	@Test(expected = BadPayloadLengthException.class)
	public void tooLongPayloadMessageTests() {
		parser.checkMessage(tooLongPayload);
		Message message = parser.initMessage(tooLongPayload);
		parser.deserializeMessage(message);

		System.out.println(message);
	}
	
	@Test(expected = BadPayloadLengthException.class)
	public void tooShortPayloadMessageTests() {
		parser.checkMessage(tooshortPayload);
		Message message = parser.initMessage(tooshortPayload);
		parser.deserializeMessage(message);

		System.out.println(message);
	}
	
	

}

package cz.sedivy.ups.networking;

public enum MessageType {
	PARSING_TEST0(1, null),
	PARSING_TEST1(101, new Class<?>[]{Integer.class}),
	PARSING_TEST2(102, new Class<?>[]{Integer.class, Integer.class}),
	PARSING_TEST3(103, new Class<?>[]{String.class}),
	PARSING_TEST4(104, new Class<?>[]{String.class, String.class}),
	PARSING_TEST5(105, new Class<?>[]{String.class, Integer.class});
	
	int headerId;
	Class<?>[] payloadTypes;
	
	MessageType(int headerId, Class<?>[] payloadTypes) {
		this.headerId = headerId;
		this.payloadTypes = payloadTypes;
	}
}

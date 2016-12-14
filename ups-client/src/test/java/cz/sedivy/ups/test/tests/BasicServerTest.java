package cz.sedivy.ups.test.tests;

import static org.junit.Assert.*;

import org.junit.Test;

import cz.sedivy.ups.app.ServiceLocator;
import cz.sedivy.ups.networking.Message;
import cz.sedivy.ups.networking.NetworkManager;
import cz.sedivy.ups.networking.NetworkManagerCallback;
import cz.sedivy.ups.test.servers.EchoServer;

public class BasicServerTest {

	public void baseRawEcho() throws InterruptedException {
		int PORT = 2121;
		String HOST = "localhost";

		EchoServer echoserver = new EchoServer();
		echoserver.init(PORT);

		Thread.sleep(1000);

		NetworkManager nm = ServiceLocator.instance().getNetworkManager();

		nm.init(new NetworkManagerCallback() {

			@Override
			public void rcvRawMessage(byte[] bytes) {
				System.out.println("[TEST] prijimam: " + new String(bytes));

			}

			@Override
			public void rcvMessage(Message message) {
				// TODO Auto-generated method stub

			}

			@Override
			public void rcvBadMsg(Exception e) {
				// TODO Auto-generated method stub
				
			}

		});

		nm.config(HOST, PORT);
		nm.setSerializing(false);
		nm.connect();
		Thread.sleep(1000);

		System.out.println("[TEST] odesilam: " + "ABC");
		nm.sendRawMessage("ABcd".getBytes());

		Thread.sleep(1000);

		nm.disconnect();

	}

	public void cRawEcho() throws InterruptedException {
		NetworkManager nm = ServiceLocator.instance().getNetworkManager();

		nm.init(new NetworkManagerCallback() {

			@Override
			public void rcvRawMessage(byte[] bytes) {
				System.out.println("[TEST] prijimam: " + new String(bytes));

			}

			@Override
			public void rcvMessage(Message message) {
				// TODO Auto-generated method stub

			}

			@Override
			public void rcvBadMsg(Exception e) {
				// TODO Auto-generated method stub
				
			}

		});

		nm.config("10.0.6.115", 55555);
		nm.connect();
		Thread.sleep(10000);

		byte[] bytes = { 1, 2, 0, 3, 4 };
		nm.sendRawMessage(bytes);
		nm.sendRawMessage(bytes);

		Thread.sleep(100000);

		nm.disconnect();

	}

	@Test
	public void test1() throws InterruptedException {
		NetworkManager nm = ServiceLocator.instance().getNetworkManager();

		nm.init(new NetworkManagerCallback() {

			@Override
			public void rcvRawMessage(byte[] bytes) {
				System.out.println("Prijat blok dat");

			}

			@Override
			public void rcvMessage(Message message) {
				System.out.println("Naparsovana zprava");

			}

			@Override
			public void rcvBadMsg(Exception e) {
				System.out.println("Prisla zprava ale spatna protoze: " + e.getClass().toString());
				
			}

		});

		nm.config("10.0.6.115", 55555);
		nm.connect();

		byte[] bytes = { 1, 2, 0, 3, 4 };
		nm.sendRawMessage(bytes);
		nm.sendRawMessage(bytes);

		Thread.sleep(100000);

		nm.disconnect();
	}

}

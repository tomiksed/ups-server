package cz.sedivy.ups.test.runs;

import cz.sedivy.ups.test.servers.EchoServer;

public class EchoServerRun {
	public static void main(String[] args) throws InterruptedException {
		EchoServer server = new EchoServer();
		server.init(1212);
	}
}

package cz.sedivy.ups.app;

import cz.sedivy.ups.networking.NetworkManager;

public class ServiceLocator {
	private static ServiceLocator instance;
	private ServiceLocator() {
		networkManager = new NetworkManager();
	};
	
	public static ServiceLocator instance() {
		if(instance == null) {
			instance = new ServiceLocator();
		}
		return instance;
	}
	
	
	private NetworkManager networkManager;
	public NetworkManager getNetworkManager() {
		return networkManager;
	}
	
	
	
			
	
	
}

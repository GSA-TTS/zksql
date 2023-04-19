package org.vaultdb.config;

import java.sql.*;
import java.util.Properties;
import java.util.logging.Logger;

import org.vaultdb.config.SystemConfiguration;
import org.vaultdb.util.Utilities;

public class WorkerConfiguration  {

	public String workerId;
	public int dbId;
	
	
	public String hostname;

	// psql config
	public String dbName;
	public String user; 
	public String password;
	public String vaultDBRoot;
	public int dbPort;
	public int empPort = 0;

	
	transient Connection  dbConnection = null; // psql connection, will be reconstructed for remote runs
	
	// Plaintext channel: port for client commands (e.g., running execution steps) and passing around serialized objects
	//	public int clientPort; 

	public WorkerConfiguration(String worker, 
			String h, int p, String dbName, String user, String pass, String vaultdbRoot)  // psql
					throws Exception {
		workerId = worker;
		hostname = h;
		dbPort = p;
		this.dbName = dbName;
		this.user = user;
		password = pass;
		this.vaultDBRoot = vaultdbRoot.replace("$VAULTDB_ROOT", Utilities.getVaultDBRoot()); // replace pointer

		
	}

	
	// from configuration file
	public WorkerConfiguration(String c) throws Exception {
		
		
		String[] tokens = c.split("=");
		workerId = tokens[0];
		
		String details = tokens[1];

		String[] parsed = details.split(":|,");
		hostname = parsed[0];
		
		dbPort = Integer.parseInt(parsed[1]);
		dbName = parsed[2];
		
		
		vaultDBRoot = parsed[3];
		vaultDBRoot = vaultDBRoot.replace("$VAULTDB_ROOT", Utilities.getVaultDBRoot()); // replace pointer
		empPort = Integer.parseInt(parsed[4]);
		
		dbId = Integer.parseInt(parsed[5]);

		SystemConfiguration globalConf = SystemConfiguration.getInstance();
		user = globalConf.getProperty("psql-user");
		password = globalConf.getProperty("psql-password");

		
	}

	@Override 
	public String toString() {
		return workerId + "@" + hostname + " psql(" + dbName + ":" + dbPort + ")" ;
	}

	public Connection getDbConnection() throws Exception {
		if(dbConnection == null) {
			Class.forName("org.postgresql.Driver");
			String url = "jdbc:postgresql://" + hostname + ":" + dbPort + "/" + dbName;
			Properties props = new Properties();
			props.setProperty("user", user);
			props.setProperty("password",password);
			dbConnection = DriverManager.getConnection(url, props);

		}
		return dbConnection;
	}
	
	
	public void closeConnection() throws Exception {
		if(dbConnection != null) {
			Logger logger = SystemConfiguration.getInstance().getLogger();
			dbConnection.close();
		}
	}
	
}

//Adam Jaworski
//ajj23

//Steven Turner
//srt31

//Team14 / Team $WAG

//CS1555 Term Project Java Phase

import java.util.Scanner;
import java.util.ArrayList;
import java.util.HashMap;
import java.sql.*;

public class team142
{

	Scanner scan = new Scanner(System.in);	//global variables
	String username = "";
	String password = "";
	String databaseUsername = "";
	String databasePassword = "";
	int loggedIn = 0;
	
	private Connection connection; //used to hold the jdbc connection to the DB
	private Statement statement; //used to create an instance of the connection
	private ResultSet resultSet; //used to hold the result of your query (if one exists)
	private ResultSet resultSet2;
	private String query;  //this will hold the query we are using
	private String query2;
	
	public team142()	//constructor gets the ball rolling
	{
		try{
			  //Register the oracle driver.  This needs the oracle files provided
			  //in the oracle.zip file, unzipped into the local directory and 
			  //the class path set to include the local directory
			  DriverManager.registerDriver (new oracle.jdbc.driver.OracleDriver());
			  //This is the location of the database.  This is the database in oracle
			  //provided to the class
			  String url = "jdbc:oracle:thin:@class3.cs.pitt.edu:1521:dbclass"; 
			  databaseUsername = "srt31";
			  databasePassword = "3637885";
			  connection = DriverManager.getConnection(url, databaseUsername, databasePassword); 
			  //create a connection to DB on class3.cs.pitt.edu
			}
			catch(Exception Ex)  //What to do with any exceptions
			{
				System.out.println("Error connecting to database.  Machine Error: " + Ex.toString());
				Ex.printStackTrace();
			}
		doOpeningStuff();
	}
	
	public void doOpeningStuff()	//where the user selects whether they're an Administrator or a Customer
	{
		System.out.println("Now doing opening stuff");
		int validInput = 0;	
		loggedIn = 0;	//logs the user out (of course if the program just started they won't be logged in to begin with)
		while (true)	//loop until the user selects something from the menu
		{
			System.out.println("\nAre you an administrator or a customer? (press corresponding number, and press 'enter')");
			System.out.println("1: Administrator");
			System.out.println("2: Customer");
			System.out.println("3: Exit program");
			System.out.println("4: Test stuff");
			String input = scan.nextLine();
			if (input.equals("1"))
			{
				validInput = 1;
				System.out.println("You are an administrator");
				doAdministratorStuff();	//off to do administrator stuff
			}
			else if (input.equals("2"))
			{
				validInput = 1;
				System.out.println("You are a customer");
				doCustomerStuff();	//off to do customer stuff
			}
			else if (input.equals("3"))	//exits the program
			{
				validInput = 1;
				print("See ya later!");
				System.exit(0);
			}
			else if (input.equals("4"))
			{
				validInput = 1;
				doTestStuff();
			}
			else	//gives the user an error message
			{
				System.out.println("\nInvalid selection. Please try again.\n");
			}
		}
	}
	
	public void doAdministratorStuff()	//the user is an administrator
	{
		print("Now doing administrator stuff");
		int validInput = 0;
		String input = "";
		if (loggedIn == 0)	//if the user is not already logged in
		{
			boolean okay = verifyLoginInfoA();
			if (!okay)
			{
				print("Invalid login credentials");
				return;
			}
			loggedIn = 1;
		}
		while (true)	//menu in a loop, as are all the menus
		{
			print("\nPlease select an option (press corresponding number, and press 'enter')");
			print("1: Register new user");
			print("2: Update system time");
			print("3: Product statistics");
			print("4: Statistics");
			print("5: Log out");
			print("6: Exit program");
			input = scan.nextLine();
			if (input.equals("1"))
			{
				validInput = 1;
				doA1Stuff();
			}
			else if (input.equals("2"))
			{
				validInput = 1;
				doA2Stuff();
			}
			else if (input.equals("3"))
			{
				validInput = 1;
				doA3Stuff();
			}
			else if (input.equals("4"))
			{
				validInput = 1;
				doA4Stuff();
			}
			else if (input.equals("5"))	//go back to opening menu (logs the user out)
			{
				validInput = 1;
				loggedIn = 0;
				return;
			}
			else if (input.equals("6")) //exits the program
			{
				validInput = 1;
				print("See ya later!");
				System.exit(0);
			}
			else
			{
				print("Invalid selection. Please try again.");
			}
		}
	}
	
	public void doA1Stuff()	//new customer registration
	{
		print("Now doing A1 stuff");
		String input = "";
		print("\nPlease select an option (press corresponding number, and press 'enter')");
		print("1: Register new customer");
		print("2: Register new administrator");
		input = scan.nextLine();
		if (input.equals("1"))
		{
			registerNewCustomer();
		}
		else if (input.equals("2"))
		{
			registerNewAdministrator();
		}
		else
		{
			print("Invalid selection");
		}
	}
	
	public void registerNewCustomer()	//register a new customer
	{
		String input = "";
		try
		{
			statement = connection.createStatement(); //create an instance
			query = "SELECT * FROM Customer"; //read in all current customers, so we'll know if the login is a repeat

			resultSet = statement.executeQuery(query); //run the query on the DB table
			ArrayList<String> logins = new ArrayList<String>();
			while(resultSet.next())
			{
				print(resultSet.getString(1));	
				logins.add(resultSet.getString(1));	//add each login to the ArrayList
			}
			print("Please enter a login for the new customer:");	//prompt for information
			String loginc = scan.nextLine();
			print("Please enter a password for the new customer:");
			String passwordc = scan.nextLine();
			print("Please enter a name for the new customer:");
			String namec = scan.nextLine();
			print("Please enter an address for the new customer:");
			String addressc = scan.nextLine();
			print("Please enter an email for the new customer:");
			String emailc = scan.nextLine();
			if (logins.contains(loginc))	//if we already have the login
			{
				print("Unable to add new customer. Login already exists.");	//fail to add
			}
			else	//otherwise, insert the new customer
			{
				query = "insert into Customer values (?,?,?,?,?)";
     
				PreparedStatement updateStatement = connection.prepareStatement(query);
				updateStatement.setString(1, loginc);
				updateStatement.setString(2,passwordc);
				updateStatement.setString(3,namec);
				updateStatement.setString(4,addressc);
				updateStatement.setString(5,emailc);
				
				updateStatement.executeUpdate();
			}
		}
		catch(Exception e)
		{
			print("Error: " + e.toString());
		}
	}
	
	public void registerNewAdministrator()
	{
		String input = "";
		try
		{
			statement = connection.createStatement(); //create an instance
			query = "SELECT * FROM Administrator"; //check logins of all existing administrators

			resultSet = statement.executeQuery(query); //run the query on the DB table
			ArrayList<String> logins = new ArrayList<String>();
			while(resultSet.next())
			{
				print(resultSet.getString(1));
				logins.add(resultSet.getString(1)); //add each login to the arraylist
			}
			print("Please enter a login for the new administrator:");	//prompt for information
			String loginc = scan.nextLine();
			print("Please enter a password for the new administrator:");
			String passwordc = scan.nextLine();
			print("Please enter a name for the new administrator:");
			String namec = scan.nextLine();
			print("Please enter an address for the new administrator:");
			String addressc = scan.nextLine();
			print("Please enter an email for the new administrator:");
			String emailc = scan.nextLine();
			if (logins.contains(loginc))	//if we already have that login
			{
				print("Unable to add new administrator. Login already exists.");	//fail to add
			}
			else	//otherwise, insert the new administrator
			{
				query = "insert into Administrator values (?,?,?,?,?)";
     
				PreparedStatement updateStatement = connection.prepareStatement(query);
				updateStatement.setString(1, loginc);
				updateStatement.setString(2,passwordc);
				updateStatement.setString(3,namec);
				updateStatement.setString(4,addressc);
				updateStatement.setString(5,emailc);
				
				updateStatement.executeUpdate();
			}
		}
		catch(Exception e)
		{
			print("Error: " + e.toString());
		}
	}
	
	public void doA2Stuff() //update system date
	{
		print("Now doing A2 stuff");
		updateDate();	//I got a little carried away with the modularity
	}
	
	public void updateDate()	//updates the system date
	{
		print("Please enter the new date in the following format: mm-dd-yyyy");	//prompt for date, in a very specific format
		String input = scan.nextLine();
		String[] pieces = input.split("-");	//split on dashes
		if (!(pieces.length == 3))	//if there weren't exactly 3 pieces
		{
			print("Date invalid.");	//invalid date
			return;
		}
		String day = pieces[1];
		String month = pieces[0];
		String year = pieces[2];
		print("Day: " + day + " Month: " + month + " Year: " + year);
		int iday = 0;
		int imonth = 0;
		int iyear = 0;
		try	//turn the three parts into numbers
		{
			iday = Integer.parseInt(day);
			imonth = Integer.parseInt(month);
			iyear = Integer.parseInt(year);
		}
		catch(Exception e)	//at least one of the three parts wasn't numeric
		{
			print("Those weren't numbers");
			return;
		}
		boolean validDate = validateDate(iday, imonth, iyear);	//function to test whether the date is valid
		if (validDate)	//if the date was valid
		{
			print("Date checks out");
			print(monthToString(imonth));	//turn 1 to "jan", 2 to "feb", ...
			month = monthToString(imonth);
			print("Please enter the hour (12, 1, ... 11, 12, 1 ...)");	//prompt for time info
			String hour = scan.nextLine();
			int ihour = Integer.parseInt(hour);
			print("Please enter the number of minutes (0-59)");
			String minutes = scan.nextLine();
			int iminutes = Integer.parseInt(minutes);
			print("Please enter the number of seconds (0-59)");
			String seconds = scan.nextLine();
			int iseconds = Integer.parseInt(seconds);
			print("Please enter either 'am' or 'pm'");
			String ampm = scan.nextLine();
			boolean validTime = validateTime(ihour, iminutes, iseconds, ampm);	//validate the time information
			if (validTime)	//time information was valid
			{
				print("Time checks out");
				seconds = checkZeroes(seconds);	//turn 5 to 05
				minutes = checkZeroes(minutes);
				hour = checkZeroes(hour);
				day = checkZeroes(day);
				try	//do the update
				{
				statement = connection.createStatement(); //create an instance
				query = "update ourSysDATE set c_date = to_date('" + day + "-" + month + "-" + year + "/" + hour + ":" + minutes + ":" + seconds + ampm + "', 'DD-MON-YYYY/HH:MI:SSAM')";
				statement.executeQuery(query);
				print("It didn't break...");
				}
				catch(Exception e)
				{
					print("Error: " + e.toString());
				}
			}
			else
			{
				return;
			}
		}
		else 
		{
			return;
		}
	}
	
	public String checkZeroes(String s)	//will turn "7" into "07"
	{
		int number = Integer.parseInt(s);
		int size = s.length();
		if (number < 10 && size == 1)	//if it should have a leading zero, but doesn't
		{
			String s2 = ("0" + s);	//add a leading zero
			return s2;
		}
		return s;	//return unmodified string
	}
	
	public boolean validateTime(int hours, int minutes, int seconds, String ampm)	//makes sure time information isn't stupid
	{
		if (hours > 12)
		{
			print("Hours too high. Please ensure the value is 1-12.");
			return false;
		}
		if (hours < 1)
		{
			print("Hours too low. Please ensure the value is 1-12.");
			return false;
		}
		if (minutes < 0)
		{
			print("Minutes too low. Please ensure the value is 0-59.");
			return false;
		}
		if (minutes > 59)
		{
			print("Minutes too high. Please ensure the value is 0-59.");
			return false;
		}
		if (seconds < 0)
		{
			print("Seconds too low. Please ensure the value is 0-59.");
			return false;
		}
		if (seconds > 59)
		{
			print("Seconds too high. Please ensure the value is 0-59.");
			return false;
		}
		if (!(ampm.equals("am") || ampm.equals("pm")))
		{
			print("am/pm invalid. Please ensure the letters are lower-cased.");
			return false;
		}
		return true;
	}
	
	public boolean validateDate(int day, int month, int year)	//makes sure date information isn't stupid
	{
		if (year < 2015)	//can't set to the past
		{
			print("Invalid year");
			return false;
		}
		if (month < 0 || month > 12)	//month not in the range of months
		{
			print("Invalid month");
			return false;
		}
		if (day < 1 || day > 31)
		{
			print("Invalid day");
			return false;
		}
		if (year > 2099)	//too far in the future
		{
			print("Year too high");
			return false;
		}
		boolean leap = false;
		if (year%4 == 0)	//since leap years are every 4 years throughout our year window
			leap = true;
		if (day > 30 && (month == 2 || month == 4 || month == 6 || month == 9 || month == 11))	//more than 30 days in a month with 30
		{
			print("Invalid date");
			return false;
		}
		if (day > 29 && month == 2 && leap)	//more than 29 in February in a leap year
		{
			print("Invalid date");
			return false;
		}
		if (day > 28 && month == 2 && !leap)	//more than 28 days in February in a non leap year
		{
			print("Invalid date");
			return false;
		}
		return true;
	}
	
	public String monthToString(int month)	//1 to "jan"
	{
		if (month == 1)
			return "jan";
		else if (month == 2)
			return "feb";
		else if (month == 3)
			return "mar";
		else if (month == 4)
			return "apr";
		else if (month == 5)
			return "may";
		else if (month == 6)
			return "jun";
		else if (month == 7)
			return "jul";
		else if (month == 8)
			return "aug";
		else if (month == 9)
			return "sep";
		else if (month == 10)
			return "oct";
		else if (month == 11)
			return "nov";
		else if (month == 12)
			return "dec";
		return "derpMonth";
	}
	
	public void doA3Stuff() //product statistics
	{
		print("Now doing A3 stuff");
		print("Do you want info on all products, or just from a specific customer?");
		print("1. All products");
		print("2. Specific customer");
		String input = "";
		input = scan.nextLine();
		if (input.equals("1"))	//all products
		{
			productStatsAll();
		}
		else if (input.equals("2"))	//specific seller
		{
			productStatsSpecific();
		}
		else
		{
			print("Invalid selection");
		}
	}
	
	public void productStatsAll()
	{
		ArrayList<Integer> ids = new ArrayList<Integer>();
		try
		{
			statement = connection.createStatement(); //create an instance
			query = "select P.auction_id, name, seller, status, buyer, P.amount, B.amount, bidder, highestBid from Product P left outer join Bidlog B on P.auction_id = B.auction_id left outer join (select auction_id, MAX(amount) as highestBid from Bidlog group by auction_id) T on P.auction_id = T.auction_id";
			resultSet = statement.executeQuery(query); //run the query on the DB table
			while(resultSet.next())
			{
				if (!(ids.contains((Integer)resultSet.getInt(1))))	//so that we only print the info for each auction_id (product) once
				{
					print("");
					//print("AuctionID: " + resultSet.getInt(1));
					print("Name: " + resultSet.getString(2));
					//print("Seller: " + resultSet.getString(3));
					print("Status: " + resultSet.getString(4));
					if (resultSet.getString(4).equals("sold"))	//if the product was sold
					{
						print("Buyer: " + resultSet.getString(5));
						print("Sold for: " + resultSet.getInt(6));
					}
					else	//the product was not sold
					{
						print("Highest Bid: " + resultSet.getInt(9));
						//statement2 = connection.createStatement();
						query2 = ("select bidder from bidlog where amount = " + resultSet.getInt(9) + " and auction_id =  " + resultSet.getInt(1));
						resultSet2 = statement.executeQuery(query2);
						while(resultSet2.next())
						{
							print("Highest Bidder: " + resultSet2.getString(1));
						}
					}
					//print("Buyer: " + resultSet.getString(5));
					//print("Product Amount: " + resultSet.getInt(6));
					//print("Bid Amount: " + resultSet.getInt(7));
					//print("Bidder: " + resultSet.getString(8));
					//print("Highest Bid: " + resultSet.getInt(9));
					print("");
					Integer i = new Integer(resultSet.getInt(1));
					ids.add(i);
				}
			}
		}
		catch(Exception e)
		{
			print("Error: " + e.toString());
		}
	}
	
	public void productStatsSpecific()
	{
		String loginc = "";
		print("What seller do you want to see products for?");
		loginc = scan.nextLine();
		ArrayList<Integer> ids = new ArrayList<Integer>();
		int resultCount = 0;
		try
		{
			statement = connection.createStatement(); //create an instance
			query = "select P.auction_id, name, seller, status, buyer, P.amount, B.amount, bidder, highestBid from Product P left outer join Bidlog B on P.auction_id = B.auction_id left outer join (select auction_id, MAX(amount) as highestBid from Bidlog group by auction_id) T on P.auction_id = T.auction_id where seller = '" + loginc + "'";
			resultSet = statement.executeQuery(query); //run the query on the DB table
			while(resultSet.next())
			{
				if (!(ids.contains((Integer)resultSet.getInt(1))))	//so that we only print the info for each auction_id (product) once
				{
					print("");
					//print("AuctionID: " + resultSet.getInt(1));
					print("Name: " + resultSet.getString(2));
					//print("Seller: " + resultSet.getString(3));
					print("Status: " + resultSet.getString(4));
					if (resultSet.getString(4).equals("sold"))	//if the product was sold
					{
						print("Buyer: " + resultSet.getString(5));
						print("Sold for: " + resultSet.getInt(6));
					}
					else	//the product was not sold
					{
						print("Highest Bid: " + resultSet.getInt(9));
						//statement2 = connection.createStatement();
						query2 = ("select bidder from bidlog where amount = " + resultSet.getInt(9) + " and auction_id =  " + resultSet.getInt(1));
						resultSet2 = statement.executeQuery(query2);
						while(resultSet2.next())
						{
							print("Highest Bidder: " + resultSet2.getString(1));
						}
					}
					//print("Buyer: " + resultSet.getString(5));
					//print("Product Amount: " + resultSet.getInt(6));
					//print("Bid Amount: " + resultSet.getInt(7));
					//print("Bidder: " + resultSet.getString(8));
					//print("Highest Bid: " + resultSet.getInt(9));
					print("");
					Integer i = new Integer(resultSet.getInt(1));
					ids.add(i);
				}
				resultCount += 1;
			}
			if (resultCount == 0)
			{
				print("No results found");
			}
		}
		catch(Exception e)
		{
			print("Error: " + e.toString());
		}
	}
	
	public void doA4Stuff()  //statistics
	{
		print("Now doing A4 stuff");
		int validInput = 0;
		String input = "";
		print("1. Highest volume categories (leaf categories)");
		print("2. Highest volume categories (root categories)");
		print("3. Most active bidders");
		print("4. Most active buyers");
		input = scan.nextLine();
		if (input.equals("1"))
		{
			leafVolume();
		}
		else if (input.equals("2"))
		{
			rootVolume();
		}
		else if (input.equals("3"))
		{
			activeBidders();
		}
		else if (input.equals("4"))
		{
			activeBuyers();
		}
		else
		{
			print("Invalid selection");
		}
	}
	
	public void leafVolume()
	{
		int backMonths = 0;
		int top = 0;
		String input = "";
		String input2 = "";
		print("Get top how many categories? (enter a number)");
		input = scan.nextLine();
		print("Over the past how many months? (enter a number)");
		input2 = scan.nextLine();
		try
		{
			top = Integer.parseInt(input);
			backMonths = Integer.parseInt(input2);
		}
		catch(Exception e)
		{
			print("Those weren't numbers");
		}
		if (backMonths < 1 || backMonths > 120)
		{
			print("Months value invalid");
			return;
		}
		if (top < 1)
		{
			print("Top number invalid");
			return;
		}
		try
		{
			statement = connection.createStatement(); //create an instance
			query = "select * from (select name, Product_Count(name, " + backMonths + ") as products_sold from Category order by products_sold desc) where rownum <= " + top + " order by rownum";
			resultSet = statement.executeQuery(query); //run the query on the DB table
			while(resultSet.next())
			{
				print("Category: " + resultSet.getString(1));
				print("Products sold: " + resultSet.getInt(2));
			}
		}
		catch(Exception e)
		{
			print("Error: " + e.toString());
		}
	}
	
	public void rootVolume()
	{
		int backMonths = 0;
		int top = 0;
		String input = "";
		String input2 = "";
		print("Get top how many categories? (enter a number)");
		input = scan.nextLine();
		print("Over the past how many months? (enter a number)");
		input2 = scan.nextLine();
		try
		{
			top = Integer.parseInt(input);
			backMonths = Integer.parseInt(input2);
		}
		catch(Exception e)
		{
			print("Those weren't numbers");
		}
		if (backMonths < 1 || backMonths > 120)
		{
			print("Months value invalid");
			return;
		}
		if (top < 1)
		{
			print("Top number invalid");
			return;
		}
		try
		{
			statement = connection.createStatement(); //create an instance
			query = "select * from (select name, Product_Count(name, " + backMonths + ") as products_sold from Category order by products_sold desc) where rownum <= " + top + " order by rownum";
			resultSet = statement.executeQuery(query); //run the query on the DB table
			while(resultSet.next())
			{
				print("Category: " + resultSet.getString(1));
				print("Products sold: " + resultSet.getInt(2));
			}
		}
		catch(Exception e)
		{
			print("Error: " + e.toString());
		}
	}
	
	public void activeBidders()
	{
		int backMonths = 0;
		int top = 0;
		String input = "";
		String input2 = "";
		print("Get top how many bidders? (enter a number)");
		input = scan.nextLine();
		print("Over the past how many months? (enter a number)");
		input2 = scan.nextLine();
		try
		{
			top = Integer.parseInt(input);
			backMonths = Integer.parseInt(input2);
		}
		catch(Exception e)
		{
			print("Those weren't numbers");
		}
		if (backMonths < 1 || backMonths > 120)
		{
			print("Months value invalid");
			return;
		}
		if (top < 1)
		{
			print("Top number invalid");
			return;
		}
		try
		{
			statement = connection.createStatement(); //create an instance
			query = "select * from (select login, Bid_Count(login, " + backMonths + ") as bids_placed from Customer order by bids_placed desc) where rownum <= " + top + " order by rownum";
			resultSet = statement.executeQuery(query); //run the query on the DB table
			while(resultSet.next())
			{
				print("User: " + resultSet.getString(1));
				print("Bid Count: " + resultSet.getInt(2));
			}
		}
		catch(Exception e)
		{
			print("Error: " + e.toString());
		}
	}
	
	public void activeBuyers()
	{
		int backMonths = 0;
		int top = 0;
		String input = "";
		String input2 = "";
		print("Get top how many buyers? (enter a number)");
		input = scan.nextLine();
		print("Over the past how many months? (enter a number)");
		input2 = scan.nextLine();
		try
		{
			top = Integer.parseInt(input);
			backMonths = Integer.parseInt(input2);
		}
		catch(Exception e)
		{
			print("Those weren't numbers");
		}
		if (backMonths < 1 || backMonths > 120)
		{
			print("Months value invalid");
			return;
		}
		if (top < 1)
		{
			print("Top number invalid");
			return;
		}
		try
		{
			statement = connection.createStatement(); //create an instance
			query = "select * from (select login, Buying_Amount(login, " + backMonths + ") as buying_amount from Customer order by buying_amount desc) where rownum <= " + top + " order by rownum";
			resultSet = statement.executeQuery(query); //run the query on the DB table
			while(resultSet.next())
			{
				print("User: " + resultSet.getString(1));
				print("Purchase amount: " + resultSet.getInt(2));
			}
		}
		catch(Exception e)
		{
			print("Error: " + e.toString());
		}
	}
	
	public void doCustomerStuff() //the user is a Customer
	{
		print("Now doing customer stuff");
		int validInput = 0;
		String input = "";
		if (loggedIn == 0) //if the user is not logged in
		{
			boolean okay = verifyLoginInfoC();
			if (!okay)
			{
				print("Invalid login credentials");
				return;
			}
			loggedIn = 1;
		}
		while (true)
		{
			print("\nPlease select an option (press corresponding number, and press 'enter')");
			print("1: Browse products by category");
			print("2: Search for product by text");
			print("3: List product for auction");
			print("4: Bid on product");
			print("5: Sell product");
			print("6: Suggestions");
			print("7: Log out");
			print("8: Exit program");
			input = scan.nextLine();
			if (input.equals("1"))
			{
				validInput = 1;
				doC1Stuff();
			}
			else if (input.equals("2"))
			{
				validInput = 1;
				doC2Stuff();
			}
			else if (input.equals("3"))
			{
				validInput = 1;
				doC3Stuff();
			}
			else if (input.equals("4"))
			{
				validInput = 1;
				doC4Stuff();
			}
			else if (input.equals("5"))
			{
				validInput = 1;
				doC5Stuff();
			}
			else if (input.equals("6"))
			{
				validInput = 1;
				doC6Stuff();
			}
			else if (input.equals("7"))	//back to opening menu (logs user out)
			{
				validInput = 1;
				loggedIn = 0;
				return;
			}
			else if (input.equals("8")) //exits program
			{
				validInput = 1;
				print("See ya later!");
				System.exit(0);
			}
			else
			{
				print("Invalid selection. Please try again.");
			}
		}
	}
	
	public void doC1Stuff() //Browse by category
	{
		print("Now doing C1 stuff");
		ArrayList<String> catList = new ArrayList<String>();
		int validInput = 0;
		String input = "";
		int counter = 0;
		while (validInput == 0)
		{
			print("\nPlease select an option (press corresponding number, and press 'enter')");
			print("1: Previous menu");
			print("2: Show categories");
			input = scan.nextLine();
			if (input.equals("1"))
			{
				validInput = 1;
				return;
			}
			else if (input.equals("2"))
			{
				validInput = 1;
				try
				{
					statement = connection.createStatement(); //create an instance
					query = "SELECT * FROM Category"; //sample query one
			  		resultSet = statement.executeQuery(query);
			  		while(resultSet.next())
					{
			  			catList.add(resultSet.getString(1));
						System.out.println("Category " + (counter+1) + ": " + catList.get(counter));
						counter++;
			  		}
			  	} 
				catch(Exception e)
				{
					System.out.println("Error running the sample queries.  Machine Error: " + e.toString());
			  	}

			  	System.out.print("Please type a category number: ");
			  	int cat = Integer.parseInt(scan.nextLine());
			  	if(cat <= catList.size()+1)
				{
			  		print("1: Sort by product name");
					print("2: Sort by highest bid");
					input = scan.nextLine();
					if(input.equals("1"))
					{
						try
						{
							query = "SELECT auction_id, name, amount FROM (SELECT * FROM product NATURAL JOIN BelongsTo) WHERE category ='" + catList.get(cat-1) + "' ORDER BY name ASC";
							print(query);
							resultSet = statement.executeQuery(query);
			  				while(resultSet.next())
							{
			  					System.out.println(resultSet.getString(1) + " " + resultSet.getString(2) + " " + resultSet.getString(3));
							}
						}
						catch(Exception e)
						{
							System.out.println("Error running the sample queries.  Machine Error: " + e.toString());
						}
					} 
					 
					else if (input.equals("2"))
					{
						try
						{
							query = "SELECT auction_id, name, amount FROM (SELECT * FROM product NATURAL JOIN BelongsTo) WHERE category ='" + catList.get(cat-1) + "' ORDER BY amount DESC";
							while(resultSet.next())
							{
								System.out.println(resultSet.getString(1) + " " + resultSet.getString(2) + " " + resultSet.getString(3));
							}
						} 
						catch(Exception e)
						{
							System.out.println("Error running the sample queries.  Machine Error: " + e.toString());
						}
					} 
					else 
					{
						print("Wrong input");
					}

			  	}

			}
			else
			{
				print("Invalid selection. Please try again.");
			}
		}
	}
	
	public void doC2Stuff() //Browse by keyword
	{
		print("Now doing C2 stuff");
		int validInput = 0;
		String input = "";
		String keyword1 = "";
		String keyword2 = "";
		print("Please enter a keyword to search for:");
		
	}
	
	public void doC3Stuff() //Put for auction
	{
		print("Now doing C3 stuff");
		int validInput = 0;
		String input = "";
		while (validInput == 0)
		{
			print("\nPlease select an option (press corresponding number, and press 'enter')");
			print("1: Previous menu");
			input = scan.nextLine();
			if (input.equals("1"))
			{
				validInput = 1;
				return;
			}
			else
			{
				print("Invalid selection. Please try again.");
			}
		}
	}
	
	public void doC4Stuff()	//place bid
	{
		print("Now doing C4 stuff");
		int validInput = 0;
		String input = "";
		while (validInput == 0)
		{
			print("\nPlease select an option (press corresponding number, and press 'enter')");
			print("1: Previous menu");
			input = scan.nextLine();
			if (input.equals("1"))
			{
				validInput = 1;
				return;
			}
			else
			{
				print("Invalid selection. Please try again.");
			}
		}
	}
	
	public void doC5Stuff()	//sell product
	{
		print("Now doing C5 stuff");
		int validInput = 0;
		String input = "";
		while (validInput == 0)
		{
			print("\nPlease select an option (press corresponding number, and press 'enter')");
			print("1: Previous menu");
			input = scan.nextLine();
			if (input.equals("1"))
			{
				validInput = 1;
				return;
			}
			else
			{
				print("Invalid selection. Please try again.");
			}
		}
	}
	
	public void doC6Stuff()	//suggestions
	{
		print("Now doing C6 stuff");
		int validInput = 0;
		String input = "";
		while (validInput == 0)
		{

			print("\nPlease select an option (press corresponding number, and press 'enter')");
			print("1: Previous menu");
			input = scan.nextLine();
			if (input.equals("1"))
			{
				validInput = 1;
				return;
			}
			else
			{
				print("Invalid selection. Please try again.");
			}
		}
	}
	
	public boolean verifyLoginInfoA()
	{
		try
		{
			statement = connection.createStatement(); //create an instance
			query = "SELECT * FROM Administrator"; //sample query one

			resultSet = statement.executeQuery(query); //run the query on the DB table
			HashMap<String,String> mappy = new HashMap<String,String>();
			while(resultSet.next())
			{
				mappy.put(resultSet.getString(1), resultSet.getString(2));
			}
			print("Please enter your login name:");
			String loginc = scan.nextLine();
			print("Please enter your password:");
			String passwordc = scan.nextLine();
			if (passwordc.equals(mappy.get(loginc)))
			{
				print("Match found");
				return true;
			}
			else
			{
				print("Match not found");
				return false;
			}
		}
		catch(Exception e)
		{
			print("Error: " + e.toString());
		}
		return false;
	}
	
	public boolean verifyLoginInfoC()
	{
		try
		{
			statement = connection.createStatement(); //create an instance
			query = "SELECT * FROM Customer"; //sample query one

			resultSet = statement.executeQuery(query); //run the query on the DB table
			HashMap<String,String> mappy = new HashMap<String,String>();
			while(resultSet.next())
			{
				mappy.put(resultSet.getString(1), resultSet.getString(2));
			}
			print("Please enter your login name:");
			String loginc = scan.nextLine();
			print("Please enter your password:");
			String passwordc = scan.nextLine();
			if (passwordc.equals(mappy.get(loginc)))
			{
				print("Match found");
				return true;
			}
			else
			{
				print("Match not found");
				return false;
			}
		}
		catch(Exception e)
		{
			print("Error: " + e.toString());
		}
		return false;
	}
	
	public void doTestStuff()
	{
		int counter = 1;
		/*We will now perform a simple query to the database, asking it for all the
		records it has.  For your project, performing queries will be similar*/
		try{
		  statement = connection.createStatement(); //create an instance
		  query = "SELECT * FROM product"; //sample query one

		  resultSet = statement.executeQuery(query); //run the query on the DB table
		  /*the results in resultSet have an odd quality.  The first row in result
		  set is not relevant data, but rather a place holder.  This enables us to
		  use a while loop to go through all the records.  We must move the pointer
		  forward once using resultSet.next() or you will get errors*/

		  while(resultSet.next()) //this not only keeps track of if another record
								  //exists but moves us forward to the first record
		  {
			System.out.println("Record " + counter + ": " +
				 resultSet.getInt(1));   
			counter++;
		  }
		}
		catch(Exception Ex)
		{
		  System.out.println("Error running the sample queries.  Machine Error: " +
				Ex.toString());
		}
	}

	
	public void print(String s) //to make it easier on my fingers to print stuff
	{
		System.out.println(s);
	}
	
	public void print(int i)
	{
		System.out.println(i);
	}

	public static void main(String[] args)	//the beginning of the program
	{
		team142 team$WAG = new team142();	//just do the "constructor"
	}

}

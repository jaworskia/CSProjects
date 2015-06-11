import java.util.Random;
import java.io.PrintWriter;
import java.io.FileNotFoundException;

public class tracemaker
{
	public static void main(String[] args)
	{
		int lines = Integer.parseInt(args[0]);
		int i = 0;
		try
		{
			PrintWriter writer = new PrintWriter("trace");
			while (i < lines)
			{
				String line = "";
				for (int k = 0; k < 8; k++)
				{
					line += getThing();
				}
				line += " ";
				line += getOtherThing();
				writer.println(line);
				System.out.println(line);
				i += 1;
			}
			writer.close();
		}
		catch(FileNotFoundException a)
		{
			System.out.println("Whatever");
		}
	}
	
	private static String getThing()
	{
		Random generator = new Random();
		int random = generator.nextInt(16);
		if (random == 0)
			return "0";
		else if (random == 1)
			return "1";
		else if (random == 2)
			return "2";
		else if (random == 3)
			return "3";
		else if (random == 4)
			return "4";
		else if (random == 5)
			return "5";
		else if (random == 6)
			return "6";
		else if (random == 7)
			return "7";
		else if (random == 8)
			return "8";
		else if (random == 9)
			return "9";
		else if (random == 10)
			return "a";
		else if (random == 11)
			return "b";
		else if (random == 12)
			return "c";
		else if (random == 13)
			return "d";
		else if (random == 14)
			return "e";
		else if (random == 15)
			return "f";
		else
			return "X";
	}
	
	private static String getOtherThing()
	{
		Random generator = new Random();
		int random = generator.nextInt(2);
		if (random == 0)
			return "R";
		else if (random == 1)
			return "W";
		else
			return "X";
	}
}
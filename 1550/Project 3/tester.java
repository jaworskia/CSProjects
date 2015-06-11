public class tester
{
	public static void main(String[] args)
	{
		Entry one = new Entry(1);
		Entry two = new Entry(2);
		Entry three = new Entry(3);
		Entry four = new Entry(4);
		Entry five = new Entry(5);
		ClockList clock = new ClockList();
		print("Empty clock:");
		print(clock);
		print("Adding 1:");
		clock.add(one);
		print(clock);
		print("Adding 2:");
		clock.add(two);
		print(clock);
		print("Adding 3:");
		clock.add(three);
		print(clock);
		print("Adding 4:");
		clock.add(four);
		print(clock);
		print("Adding 5:");
		clock.add(five);
		print(clock);
		print("Removing Curr:");
		clock.removeCurr();
		print(clock);
		print("Advancing:");
		clock.advance();
		print(clock);
		print("Removing Curr:");
		clock.removeCurr();
		print(clock);
	}

	public static void print(ClockList c)
	{
		System.out.println(c);
	}
	public static void print(String s)
	{
		System.out.println(s);
	}
}
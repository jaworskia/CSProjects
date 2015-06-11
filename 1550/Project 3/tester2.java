

public class tester2
{
	public static void main(String[] args)
	{
		UsageList helper = new UsageList();
		print("Initializing:");
		print(helper);
		print("Adding 16:");
		helper.add(16);
		print(helper);
		print("Adding 29:");
		helper.add(29);
		print(helper);
		print("Adding 58:");
		helper.add(58);
		print(helper);
		print("Adding 71:");
		helper.add(71);
		print(helper);
		print("Adding 87:");
		helper.add(87);
		print(helper);
		print("Getting next after 5:");
		print(helper.getNext(5));
		print("Getting next after 16:");
		print(helper.getNext(16));
		print("Getting next after 80:");
		print(helper.getNext(80));
		print("Getting next after 84:");
		print(helper.getNext(84));
		print("Getting next after 99:");
		print(helper.getNext(99));
	}
	public static void print(UsageList u)
	{
		System.out.println(u);
	}
	public static void print(String s)
	{
		System.out.println(s);
	}
	public static void print(int i)
	{
		System.out.println(i);
	}
}
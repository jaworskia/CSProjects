//Adam Jaworski
//CS 1550
//Project 3

public class UsageNode		//used in the UsageList class, simple Node class for usages
{
	int usage;			//the Node's data is a usage time
	UsageNode next;			//next Node
	UsageNode previous;		//previous Node
	public UsageNode(int u, UsageNode n, UsageNode p)		//constructor
	{
		usage = u;
		next = n;
		previous = p;
	}
}
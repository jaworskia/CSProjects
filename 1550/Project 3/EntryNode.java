//Adam Jaworski
//CS 1550
//Project 3

public class EntryNode		//used in the ClockList class, simple Node class for Entry objects
{
	Entry entry;			//the Node's data is an Entry
	EntryNode next;			//next Node
	EntryNode previous;		//previous Node
	public EntryNode(Entry e, EntryNode n, EntryNode p)		//constructor
	{
		entry = e;
		next = n;
		previous = p;
	}
}
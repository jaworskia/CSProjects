//Adam Jaworski
//CS 1550
//Project 3

public class ClockList			//for implementing the "clock" algorithm
{
	EntryNode curr;				//where we are in the clock
	int count = 0;				//how many Entries are in the clock
	public ClockList(Entry e)	//constructor with an Entry
	{
		curr = new EntryNode(e, null, null);
		curr.next = curr;
		curr.previous = curr;
		count += 1;
	}
	public ClockList()			//constructor with no Entry
	{
		curr = null;
	}
	public Entry getCurr()		//returns the current Node's Entry
	{
		return curr.entry;
	}
	public Entry evict()			//determines which entry to evict, and does the eviction
	{
		while (curr.entry.getR())	//while the current entry's referenced bit is true
		{
			advance();				//move to next entry, wiping out the current entry's referenced bit
		}
		Entry evicted = curr.entry;	//curr should now be the oldest unreferenced page
		removeCurr();				//evict it
		return evicted;				//return the page
	}
	public Entry advance()
	{
		curr.entry.clearR();	//wipe out its referenced bit before moving on
		curr = curr.next;		//move on
		return curr.entry;		//returns the new current Entry
	}
	public Entry advance2()		//for advancing without clearing references
	{
		curr = curr.next;		//move one
		return curr.entry;		//returns the new current Entry
	}
	public void removeCurr()	//removes the current Entry
	{
		if (count <= 0)			//only if there's something to remove
		{
			System.out.println("Nothing to remove!");
		}
		else
		{
			curr.previous.next = curr.next;			//skip over curr forwards
			curr.next.previous = curr.previous;		//skip over curr backwards
			curr = curr.next;						//curr advances by default
			count -= 1;								//now one less Entry in the clock
		}
	}
	public void add(Entry e)						//adds a Node corresponding to Entry e
	{
		if (curr == null)							//currently nothing in the clock
		{
			curr = new EntryNode(e, null, null);	//curr is e
			curr.next = curr;						//links to itself forwards
			curr.previous = curr;					//links to itself backwards
			count += 1;								//now one more Entry in the clock
		}
		else
		{
			EntryNode added = new EntryNode(e, null, null);		//creates an EntryNode corresponding to Entry e
			added.next = curr;									//links forward to curr (so it's added at the back)
			added.previous = curr.previous;						//links backward to the old end
			curr.previous.next = added;							//old end links forward to added
			curr.previous = added;								//curr links backward to added
			count += 1;											//now one more Entry in the clock
		}
	}
	public void reference(Entry e)								//references Entry e if it's in the clock
	{
		EntryNode curr2 = curr;									//iterate without changing curr
		for (int i = 0; i < count; i++)							//for each Entry
		{
			if (e.page == curr2.entry.page)	//we've found the page
			{
				curr2.entry.setR();			//reference it
				break;						//can stop looping
			}
			curr2 = curr2.next;				//go to next Entry
		}
	}
	public void dirty(Entry e)									//dirties Entry e if it's in the clock
	{
		EntryNode curr2 = curr;									//iterate without changing curr
		for (int i = 0; i < count; i++)							//for each Entry
		{
			if (e.page == curr2.entry.page)	//we've found the page
			{
				curr2.entry.setD();			//dirty it
				break;						//can stop looping
			}
			curr2 = curr2.next;				//go to next Entry
		}
	}
	public String toString()				//for debugging
	{
		String response = "";
		for (int i = 0; i < count; i++)		//for each Entry
		{
			response += ("Index " + i + ": ");
			response += curr.entry.page;
			response += "\n";
			advance2();						//advance without clearing referenced bits
		}
		if (count > 0)						//if the clock is not empty
			response += ("Re: Index 0: " + curr.entry.page + "\n");	//verifies we're back at the beginning
		return response;
	}
}
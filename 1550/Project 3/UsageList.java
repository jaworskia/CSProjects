//Adam Jaworski
//CS 1550
//Project 3

public class UsageList
{
	UsageNode curr;
	UsageNode first;
	UsageNode last;
	int count = 0;
	public UsageList()								//constructor takes no parameters
	{
		curr = null;
		first = null;
		last = null;
	}
	public void add(int i)							//add usage i to list
	{
		if (count == 0)								//if the list is currently empty
		{
			first = new UsageNode(i, null, null);	//initialize the first node
			curr = first;							//curr will start here
			last = first;							//both first and last
			count += 1;								//now one more usage in list
		}
		else										//the list is not empty
		{
			UsageNode added = new UsageNode(i, null, null);		//initialize a new node
			added.previous = last;					//have it link backwards to old last
			last.next = added;						//old last links forwards to added
			last = added;							//added becomes new last
			count += 1;								//now one more usage in list
		}
	}
	public int getNext(int access)
	{
		boolean notUsed = false;
		while (access > curr.usage)		//while we're ahead of the current node's usage line
		{
			curr = curr.next;			//go to next usage
			if (curr == null)			//end of the list
			{
				notUsed = true;
				break;					//exit the loop
			}
		}
		if (notUsed)					//return -1 if the page will not be used again
			return -1;
		return curr.usage;				//otherwise return the next usage
	}
	public String toString()			//for debugging purposes
	{
		String response = "";
		UsageNode curr2 = first;
		for (int i = 0; i < count; i++)
		{
			response += (i + ": " + curr2.usage + "\n");
			curr2 = curr2.next;
		}
		return response;
	}
}
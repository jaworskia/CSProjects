//Adam Jaworski
//CS 1550
//Project 3

public class Entry			//represents a page in a frame
{
	public long page;		//the page number
	boolean rbit;			//referenced bit
	boolean dbit;			//dirty bit
	
	public Entry(long p)	//constructor takes the page number
	{
		page = p;
		rbit = true;	//always referenced on creation
		dbit = false;	//always clean on creation
	}
	
	public void clearR()	//unreference
	{
		rbit = false;
	}
	
	public void setR()		//reference
	{
		rbit = true;
	}
	
	public void setD()		//mark as dirty
	{
		dbit = true;
	}
	
	public boolean getD()	//get dirty status
	{
		return dbit;
	}
	
	public boolean getR()	//get reference status
	{
		return rbit;
	}
	
	public long getPage()	//get the page number
	{
		return page;
	}
	
	public String toString()	//for debugging
	{
		return ("Page: " + page + "\nR: " + rbit + "\nD: " + dbit);
	}
	
	public boolean equals(Object x)		//for ArrayList's 'contains' method
	{
		if (x == null)
			return false;
		if (this.getClass() != x.getClass())
			return false;
		if (((Entry)x).page == this.page)
			return true;
		else
			return false;
	}

}
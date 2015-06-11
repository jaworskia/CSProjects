import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.File;
import java.util.Scanner;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Random;

//Adam Jaworski
//CS 1550
//Project 3
//to compile: javac vmsim.java
//to run (example): java vmsim -n 8 -a opt -r 1024 bzip.trace
//NOTE: FOR DRAMATICALLY FASTER EXECUTION, COMMENT OUT PRINT STATEMENT ON LINE 239

public class vmsim
{
	public static void main(String[] args)
	{
		if (args.length != 7)		//print an error if the number of arguments is incorrect
		{
			print("Improper number of command line arguments.");
			print("Here is the expected command line:"); 
			print("java vmsim -n <numframes> -a <opt|clock|nru|rand> -r <refresh> <tracefile>");
			System.exit(0);
		}
		int numFrames = Integer.parseInt(args[1]);
		String algorithm = args[3];
		int refresh = Integer.parseInt(args[5]);
		String fileName = args[6];
		if (!(algorithm.equals("rand") || algorithm.equals("nru") || algorithm.equals("clock") || algorithm.equals("opt")))
		{
			print("The algorithm parameter is invalid: must be 'rand', 'nru', 'clock', or 'opt'");	//notify the user if the algorithm isn't correct
			System.exit(0);
		}
		//print("The number of frames is: " + numFrames);
		//print("The algorithm is: " + algorithm);
		//print("The refresh is: " + refresh);
		//print("The file name is: " + fileName);
		ArrayList<Entry> frames = new ArrayList(numFrames);			//the pages in frames
		ClockList clock = new ClockList();							//used in implementation of clock algorithm
		UsageList[] helper = new UsageList[1048576];				//a UsageList to correspond to each possible page
		int inFrames = 0;		//counter for how many pages are in frames
		int accesses = 0;		//counter for the total number of accesses
		int faults = 0;			//counter for the total number of page faults
		int diskWrites = 0;		//counter for the total number of disk writes
		File file = new File(fileName);
		if (algorithm.equals("opt"))		//if it's the opt algorithm
		{
			try								//we'll read through the entire file once at the start to set up our UsageLists
			{
				Scanner reader = new Scanner(new FileReader(file));		//to read the file
				String line = reader.nextLine();
				int number = 0;
				while (1==1)								//infinite loop
				{
					String[] pieces = line.split(" ");		//to parse the line
					String addressString = pieces[0];
					String mode = pieces[1];
					long address = Long.parseLong(addressString, 16);	//get the address
					long page = address/4096;					//determine the corresponding page number
					if (helper[(int)page] == null)				//initialize that page's UsageList if it's not already
						helper[(int)page] = new UsageList();
					helper[(int)page].add(number);				//add the line number to the UsageList for that page
					if (reader.hasNextLine())	//get the next line if there is one
						line = reader.nextLine();
					else	//exit when there are no more lines in the trace file
						break;
					number += 1;				//going to the next line
				}
				reader.close();
			}
			catch (IOException e)				//if there was a problem with the file or something
			{
				e.printStackTrace();
			}
		}
		try 
		{
			Scanner reader = new Scanner(new FileReader(file));	//will read from the trace file
			String line = reader.nextLine();
			while (1 == 1)		//infinite loop
			{
				if (algorithm.equals("nru") && accesses%refresh == 0 && accesses != 0)	//if nru and time to refresh
				{
					for (Entry e : frames)	//for every page in a frame
					{
						e.clearR();			//clear its referenced bit
					}
				}
				String output = "";
				output += line;
				//print(line);
				String[] pieces = line.split(" ");
				String addressString = pieces[0];
				String mode = pieces[1];
				//print("The address is: " + addressString);
				//print("The mode is: " + mode);
				long address = Long.parseLong(addressString, 16);
				//print("The address value is: " + address);
				long page = address/4096;
				//long offset = address%4096;
				//print("The page is: " + page);
				//print("The offset is: " + offset);
				Entry curr = new Entry(page);
				if (frames.contains(curr))				//if the corresponding page is already in a frame
				{
					//print("\tHit");						//it was a hit, so no page fault
					output += ("\n\tHit");
					frames.get(frames.indexOf(curr)).setR();	//set its referenced bit
					clock.reference(curr);				//set its referenced bit in the clock as well
					if (mode.equals("W"))				//if it was a write
					{
						frames.get(frames.indexOf(curr)).setD();	//set its dirty bit
						clock.dirty(curr);							//set its dirty bit in the clock as well
					}
				}
				else		//we have a page fault
				{
					if (mode.equals("W"))				//if the access is a write
					{
						curr.setD();					//set curr's dirty bit
					}
					faults += 1;						//increment faults counter
					if (inFrames < numFrames)				//if we are not at capacity
					{
						frames.add(curr);			//add the current page
						clock.add(curr);			//add it to the clock as well
						inFrames += 1;				//now one more page in a frame
						output += ("\n\tPage Fault -- No Eviction");
						//print("\tPage Fault -- No Eviction");	//there was an open frame, so no eviction was necessary
					}
					else	//an eviction will be necessary
					{
						Entry evicted = new Entry(0);		//to keep java from yelling at me
						Entry planB = new Entry(0);
						Entry planC = new Entry(0);
						Entry planD = new Entry(0);
						boolean validA = false;
						boolean validB = false;
						boolean validC = false;
						boolean validD = false;
						if (algorithm.equals("rand"))		//if it's the random replacement algorithm
						{
							Random generator = new Random();
							int random = generator.nextInt(numFrames);	//pick a random index
							evicted = frames.get(random);			//get the page in that frame
							frames.remove(random);						//evict the page in that frame
						}
						else if (algorithm.equals("nru"))	//if it's the nru replacement algorithm
						{
							Random generator = new Random();
							ArrayList<Entry> aList = new ArrayList(numFrames);	//to store all unreferenced and clean pages
							ArrayList<Entry> bList = new ArrayList(numFrames);	//to store all unreferenced and dirty pages
							ArrayList<Entry> cList = new ArrayList(numFrames);	//to store all referenced and clean pages
							ArrayList<Entry> dList = new ArrayList(numFrames);	//to store all referenced and dirty pages
							for (Entry e : frames)					//for each page in a frame
							{
								if (e.getR() == false && e.getD() == false)	//the page is unreferenced and clean
								{
									aList.add(e);			//add it to the appropriate list
									validA = true;			//top priority
								}
								else if (e.getR() == false)					//the page is unreferenced
								{
									bList.add(e);				//add it to the appropriate list
									validB = true;			//second priority
								}
								else if (e.getD() == false)					//the page is clean but referenced
								{
									cList.add(e);				//add it to the appropriate list
									validC = true;			//third priority
								}
								else										//the page is dirty and referenced
								{
									dList.add(e);				//add it to the appropriate list
									validD = true;			//last resort
								}
							}
							if (validA)			//if there is at least one unreferenced and clean page
							{
								evicted = aList.get(generator.nextInt(aList.size()));	//pick one to evict
							}
							else if (!validA && validB)	//if planB is our best option
							{
								evicted = bList.get(generator.nextInt(bList.size()));	//we'll evict a page that's unreferenced and dirty
							}
							else if (!validA && !validB && validC)	//planC is our best option
							{
								evicted = cList.get(generator.nextInt(cList.size()));	//a page that's clean but referenced
							}
							else if (!validA && !validB && !validC && validD)	//planD is our best option
							{
								evicted = dList.get(generator.nextInt(dList.size()));	//a page that's referenced and dirty
							}
							frames.remove(evicted);		//remove the evicted page
						}
						else if (algorithm.equals("clock"))		//if it's the clock replacement algorithm
						{
							evicted = clock.evict();			//evicts a page from the clock (see ClockList.java)
							frames.remove(evicted);				//remove the evicted page
						}
						else if (algorithm.equals("opt"))		//if it's the opt replacement algorithm
						{
							int furthest = 0;
							int current = 0;
							for (Entry e : frames)	//for every page in a frame
							{
								current = helper[(int)e.page].getNext(accesses);	//get the next use after 'accesses' (the current access)
								if (current > furthest)						//if it's the highest we've seen
								{
									evicted = e;							//this will be the one we evict
									furthest = current;						//the new highest
								}
								if (current == -1)							//should be the case if it's never used again
								{
									evicted = e;							//can't do any better than that
									break;									//so we might as well just evict this one
								}
							}
							frames.remove(evicted);				//remove the evicted page
						}
						frames.add(curr);							//put the current page in
						clock.add(curr);							//put it in the clock as well
						if (evicted.getD())							//if the evicted page was dirty
						{
							diskWrites += 1;						//we'd write it out to disk
							output += "\n\tPage Fault -- Evict Dirty";
							//print("\tPage Fault -- Evict Dirty");
						}
						else										//otherwise it was clean
						{
							output += "\n\tPage Fault -- Evict Clean";
							//print("\tPage Fault -- Evict Clean");
						}
					}
				}
				print(output);	//HERE
				accesses += 1;			//increment the total number of accesses
				if (reader.hasNextLine())	//get the next line if there is one
					line = reader.nextLine();
				else	//exit when there are no more lines in the trace file
					break;
			}
			reader.close();
		} 
		catch (IOException e) 									//means there was a problem with the file or something
		{
			e.printStackTrace();
		}
		print("Number of frames: " + numFrames);				//print out the final counts
		print("Total memory accesses: " + accesses);
		print("Total page faults: " + faults);
		print("Total writes to disk: " + diskWrites);
	}

	private static void print(String thing)		//just an abbreviated print statement
	{
		System.out.println(thing);
	}
	private static void print(Entry thing)		//just an abbreviated print statement
	{
		System.out.println(thing);
	}
	private static void print(ClockList c)		//just an abbreviated print statement
	{
		System.out.println(c);
	}
}

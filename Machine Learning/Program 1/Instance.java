//Adam Jaworski
//CS 1675

public class Instance //this is just a data structure to hold the information for a single training/test instance
{
	
	int number; 
	boolean f1; //values for the 16 features
	boolean f2;
	boolean f3;
	boolean f4;
	boolean f5;
	boolean f6;
	boolean f7;
	boolean f8;
	boolean f9;
	boolean f10;
	boolean f11;
	boolean f12;
	boolean f13;
	boolean f14;
	boolean f15;
	boolean f16;
	boolean answer;  //whether it was positive or negative
	
	public Instance() //constructor, everything defaults to false
	{
		f1 = false;
		f2 = false;
		f3 = false;
		f4 = false;
		f5 = false;
		f6 = false;
		f7 = false;
		f8 = false;
		f9 = false;
		f10 = false;
		f11 = false;
		f12 = false;
		f13 = false;
		f14 = false;
		f15 = false;
		f16 = false;
		answer = false;
		number = 0;
	}
	
	public void setNumber(int n)  //a whole bunch of mutator and accessor methods
	{
		number = n;
	}
	
	public int getNumber()
	{
		return number;
	}
	
	public void setAnswer(boolean b)
	{
		answer = b;
	}
	
	public boolean getAnswer()
	{
		return answer;
	}
	
	public void setf1(boolean b)
	{
		f1 = b;
	}
	
	public void setf2(boolean b)
	{
		f2 = b;
	}
	
	public void setf3(boolean b)
	{
		f3 = b;
	}
	
	public void setf4(boolean b)
	{
		f4 = b;
	}
	
	public void setf5(boolean b)
	{
		f5 = b;
	}
	
	public void setf6(boolean b)
	{
		f6 = b;
	}
	
	public void setf7(boolean b)
	{
		f7 = b;
	}
	
	public void setf8(boolean b)
	{
		f8 = b;
	}
	
	public void setf9(boolean b)
	{
		f9 = b;
	}
	
	public void setf10(boolean b)
	{
		f10 = b;
	}
	
	public void setf11(boolean b)
	{
		f11 = b;
	}
	
	public void setf12(boolean b)
	{
		f12 = b;
	}
	
	public void setf13(boolean b)
	{
		f13 = b;
	}
	
	public void setf14(boolean b)
	{
		f14 = b;
	}
	public void setf15(boolean b)
	{
		f15 = b;
	}
	
	public void setf16(boolean b)
	{
		f16 = b;
	}
	
	public boolean getf1()
	{
		return f1;
	}
	
	public boolean getf2()
	{
		return f2;
	}
	
	public boolean getf3()
	{
		return f3;
	}
	
	public boolean getf4()
	{
		return f4;
	}
	
	public boolean getf5()
	{
		return f5;
	}
	
	public boolean getf6()
	{
		return f6;
	}
	
	public boolean getf7()
	{
		return f7;
	}
	
	public boolean getf8()
	{
		return f8;
	}
	
	public boolean getf9()
	{
		return f9;
	}
	
	public boolean getf10()
	{
		return f10;
	}
	
	public boolean getf11()
	{
		return f11;
	}
	
	public boolean getf12()
	{
		return f12;
	}
	
	public boolean getf13()
	{
		return f13;
	}
	
	public boolean getf14()
	{
		return f14;
	}
	
	public boolean getf15()
	{
		return f15;
	}
	
	public boolean getf16()
	{
		return f16;
	}
	
	public boolean[] getArray() //returns the features as an array
	{
		boolean[] array = new boolean[16];
		array[0] = f1;
		array[1] = f2;
		array[2] = f3;
		array[3] = f4;
		array[4] = f5;
		array[5] = f6;
		array[6] = f7;
		array[7] = f8;
		array[8] = f9;
		array[9] = f10;
		array[10] = f11;
		array[11] = f12;
		array[12] = f13;
		array[13] = f14;
		array[14] = f15;
		array[15] = f16;
		return array;
	}
	
	public String toString()
	{
		return (number + ": " + answer + ", " + f1 + ", " + f2 + ", " + f3 + ", " + f4 + ", " + f5 + ", " + f6 + ", " + f7 + ", " + f8 + ", " + f9 + ", " + f10 + ", " + f11 + ", " + f12 + ", " + f13 + ", " + f14 + ", " + f15 + ", " + f16);
	}

}
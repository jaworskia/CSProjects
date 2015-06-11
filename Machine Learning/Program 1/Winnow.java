import java.io.*;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Random;
import java.text.*;

//Adam Jaworski
//CS 1675

public class Winnow

{

	static int[] weights = new int[16];  //the weights for the hypothesis function (though I think I ended up using different ones)

	public static void main(String[] args) throws IOException
	{
		String dataFile = args[0]; //the file containing the data
		for (int i = 0; i < 16; i++) //weights for the features
		{
			weights[i] = 1;  //they get initialized to 1
		}
		DecimalFormat df = new DecimalFormat("#.##"); //used later to make the percentages look neater
		ArrayList<Instance> instances = new ArrayList<Instance>(); //stores all of the instances
		File filey = new File(dataFile);
		Scanner reader = new Scanner(filey); //to read the file
		while(reader.hasNext()) //while there is still more to read
		{
			String line = reader.nextLine(); //get the next line
			if (line.contains(",")) //i.e, if it's not an empty line
			{
				Instance curr = new Instance(); //create a new instance corresponding to the line
				String[] info = line.split(","); //split it up around the commas
				int size = info.length;
				for (int i = 0; i < size; i++) //for all components of the line
				{
					String f = info[i]; //isolate the component
					if (i == 0) //if it's the first time through the loop
					{
						curr.setNumber(Integer.parseInt(f)); //set the instance's number
					}
					else if (i == 1) //second time through
					{
						if (f.equals("y") || f.equals("democrat")) //so it's compatible with either data set (democrat corresponds to true)
							curr.setAnswer(true); //set the instance's answer
					}
					else if (i == 2) //third time through
					{
						if (f.equals("y"))    //if it it true
							curr.setf1(true); //set the first feature to true (false is the default)
					}
					else if (i == 3) //fourth time through
					{
						if (f.equals("y"))  //and so on for all 16 features
							curr.setf2(true);
					}
					else if (i == 4)
					{
						if (f.equals("y"))
							curr.setf3(true);
					}
					else if (i == 5)
					{
						if (f.equals("y"))
							curr.setf4(true);
					}
					else if (i == 6)
					{
						if (f.equals("y"))
							curr.setf5(true);
					}
					else if (i == 7)
					{
						if (f.equals("y"))
							curr.setf6(true);
					}
					else if (i == 8)
					{
						if (f.equals("y"))
							curr.setf7(true);
					}
					else if (i == 9)
					{
						if (f.equals("y"))
							curr.setf8(true);
					}
					else if (i == 10)
					{
						if (f.equals("y"))
							curr.setf9(true);
					}
					else if (i == 11)
					{
						if (f.equals("y"))
							curr.setf10(true);
					}
					else if (i == 12)
					{
						if (f.equals("y"))
							curr.setf11(true);
					}
					else if (i == 13)
					{
						if (f.equals("y"))
							curr.setf12(true);
					}
					else if (i == 14)
					{
						if (f.equals("y"))
							curr.setf13(true);
					}
					else if (i == 15)
					{
						if (f.equals("y"))
							curr.setf14(true);
					}
					else if (i == 16)
					{
						if (f.equals("y"))
							curr.setf15(true);
					}
					else if (i == 17)
					{
						if (f.equals("y"))
							curr.setf16(true);
					}
				}
				instances.add(curr); //add completed instance to the ArrayList
			}
		}

		
		ArrayList<Instance> set1 = new ArrayList<Instance>(); //the ten partitions of the data
		ArrayList<Instance> set2 = new ArrayList<Instance>();
		ArrayList<Instance> set3 = new ArrayList<Instance>();
		ArrayList<Instance> set4 = new ArrayList<Instance>();
		ArrayList<Instance> set5 = new ArrayList<Instance>();
		ArrayList<Instance> set6 = new ArrayList<Instance>();
		ArrayList<Instance> set7 = new ArrayList<Instance>();
		ArrayList<Instance> set8 = new ArrayList<Instance>();
		ArrayList<Instance> set9 = new ArrayList<Instance>();
		ArrayList<Instance> set10 = new ArrayList<Instance>();
		
		ArrayList<Instance> rest1 = new ArrayList<Instance>(); //the training cases for the test set set1
		ArrayList<Instance> rest2 = new ArrayList<Instance>(); //and so on for all ten partitions
		ArrayList<Instance> rest3 = new ArrayList<Instance>();
		ArrayList<Instance> rest4 = new ArrayList<Instance>();
		ArrayList<Instance> rest5 = new ArrayList<Instance>();
		ArrayList<Instance> rest6 = new ArrayList<Instance>();
		ArrayList<Instance> rest7 = new ArrayList<Instance>();
		ArrayList<Instance> rest8 = new ArrayList<Instance>();
		ArrayList<Instance> rest9 = new ArrayList<Instance>();
		ArrayList<Instance> rest10 = new ArrayList<Instance>();
		
		for (Instance instance: instances) //for each instance we've read from the file
		{
			rest1.add(instance);  //add it to each of the training sets
			rest2.add(instance);
			rest3.add(instance);
			rest4.add(instance);
			rest5.add(instance);
			rest6.add(instance);
			rest7.add(instance);
			rest8.add(instance);
			rest9.add(instance);
			rest10.add(instance);
		}
		
		
		Random rand = new Random();
		int n = instances.size();
		int fraction = n/10; //about 10% will be in each partition
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n); //pick an instance at random
			Instance curr = instances.get(random);
			set1.add(curr); //add it to test set set1
			instances.remove(curr); //remove from instances, so we don't pick it again
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n); //and so on for all ten partitions
			Instance curr = instances.get(random);
			set2.add(curr);
			instances.remove(curr);
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n);
			Instance curr = instances.get(random);
			set3.add(curr);
			instances.remove(curr);
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n);
			Instance curr = instances.get(random);
			set4.add(curr);
			instances.remove(curr);
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n);
			Instance curr = instances.get(random);
			set5.add(curr);
			instances.remove(curr);
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n);
			Instance curr = instances.get(random);
			set6.add(curr);
			instances.remove(curr);
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n);
			Instance curr = instances.get(random);
			set7.add(curr);
			instances.remove(curr);
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n);
			Instance curr = instances.get(random);
			set8.add(curr);
			instances.remove(curr);
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n);
			Instance curr = instances.get(random);
			set9.add(curr);
			instances.remove(curr);
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n);
			Instance curr = instances.get(random);
			set10.add(curr);
			instances.remove(curr);
		}
		
		int[] weights1 = new int[16]; //the weights for trial 1
		for (int i = 0; i < 16; i++)
		{
			weights1[i] = 1; //initialize all the weights to 1
		}
		
		for (Instance instance: set1) //for each instance in test set set1
		{
			if (rest1.contains(instance)) //remove it from training set rest1
				rest1.remove(instance);
		}
		for (Instance instance: rest1) //for each instance in rest1
		{
			boolean prediction = predict(instance, weights1); //get prediction
			boolean answer = instance.getAnswer(); //and the answer
			boolean[] array = instance.getArray(); //and the instance's features
			if (!prediction && answer) //predict false, answer was true
			{
				for (int i = 0; i < 16; i++) //for all 16 features
				{
					if (array[i]) //if the feature was present
						weights1[i] = weights1[i] * 2; //double its weight
				}
			}
			else if (prediction && !answer) //predicted true, answer was false
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the feature was present
						weights1[i] = 0; //zero out its weight
				}
			}
		}
		double count1 = 0; //number of test cases for trial 1 (will actually be the same for all trials, though)
		double correct1 = 0; //the number of correct predictions for trial 1

		for (Instance instance: set1) //for each instance in test set set1
		{
			boolean prediction = predict(instance, weights1); //get prediction as before
			boolean answer = instance.getAnswer(); //and the answer
			boolean[] array = instance.getArray(); //and its features
			if (!prediction && answer) //predicted false, answer was true
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i]) //feature was present
						weights1[i] = weights1[i] * 2; //double its weight
				}
			}
			else if (prediction && !answer) //predicted true, answer was false
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the feature was present
						weights1[i] = 0; //zero out its weight
				}
			}
			else //answer and prediction must have been the same
			{
				correct1 += 1; //increment correct
			}
			count1 += 1; //increment count
		}
		
		System.out.println("Count 1: " + count1); //output results
		System.out.println("Correct 1: " + correct1);
		System.out.println("Accuracy: " + df.format((correct1/count1)*100) + "%");
		System.out.println();
		
		int[] weights2 = new int[16];  //AND SO ON FOR THE REST OF THE TRIALS
		for (int i = 0; i < 16; i++)
		{
			weights2[i] = 1;
		}
		
		for (Instance instance: set2)
		{
			if (rest2.contains(instance))
				rest2.remove(instance);
		}
		for (Instance instance: rest2) 
		{
			boolean prediction = predict(instance, weights2);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights2[i] = weights2[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights2[i] = 0; //zero out its weight
				}
			}
		}
		double count2 = 0;
		double correct2 = 0;

		for (Instance instance: set2)
		{
			boolean prediction = predict(instance, weights2);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights2[i] = weights2[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights2[i] = 0; //zero out its weight
				}
			}
			else
			{
				correct2 += 1;
			}
			count2 += 1;
		}
		
		System.out.println("Count 2: " + count2);
		System.out.println("Correct 2: " + correct2);
		System.out.println("Accuracy: " + df.format((correct2/count2)*100) + "%");
		System.out.println();
		
		
		int[] weights3 = new int[16];
		for (int i = 0; i < 16; i++)
		{
			weights3[i] = 1;
		}
		
		for (Instance instance: set3)
		{
			if (rest3.contains(instance))
				rest3.remove(instance);
		}
		for (Instance instance: rest3) 
		{
			boolean prediction = predict(instance, weights3);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights3[i] = weights3[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights3[i] = 0; //zero out its weight
				}
			}
		}
		double count3 = 0;
		double correct3 = 0;

		for (Instance instance: set3)
		{
			boolean prediction = predict(instance, weights3);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights3[i] = weights3[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights3[i] = 0; //zero out its weight
				}
			}
			else
			{
				correct3 += 1;
			}
			count3 += 1;
		}
		
		System.out.println("Count 3: " + count3);
		System.out.println("Correct 3: " + correct3);
		System.out.println("Accuracy: " + df.format((correct3/count3)*100) + "%");
		System.out.println();
		
		int[] weights4 = new int[16];
		for (int i = 0; i < 16; i++)
		{
			weights4[i] = 1;
		}
		
		for (Instance instance: set4)
		{
			if (rest4.contains(instance))
				rest4.remove(instance);
		}
		for (Instance instance: rest4) 
		{
			boolean prediction = predict(instance, weights4);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights4[i] = weights4[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights4[i] = 0; //zero out its weight
				}
			}
		}
		double count4 = 0;
		double correct4 = 0;

		for (Instance instance: set4)
		{
			boolean prediction = predict(instance, weights4);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights4[i] = weights4[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights4[i] = 0; //zero out its weight
				}
			}
			else
			{
				correct4 += 1;
			}
			count4 += 1;
		}
		
		System.out.println("Count 4: " + count4);
		System.out.println("Correct 4: " + correct4);
		System.out.println("Accuracy: " + df.format((correct4/count4)*100) + "%");
		System.out.println();
		
		int[] weights5 = new int[16];
		for (int i = 0; i < 16; i++)
		{
			weights5[i] = 1;
		}
		
		for (Instance instance: set5)
		{
			if (rest5.contains(instance))
				rest5.remove(instance);
		}
		for (Instance instance: rest5) 
		{
			boolean prediction = predict(instance, weights5);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights5[i] = weights5[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights5[i] = 0; //zero out its weight
				}
			}
		}
		double count5 = 0;
		double correct5 = 0;

		for (Instance instance: set5)
		{
			boolean prediction = predict(instance, weights5);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights5[i] = weights5[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights5[i] = 0; //zero out its weight
				}
			}
			else
			{
				correct5 += 1;
			}
			count5 += 1;
		}
		
		System.out.println("Count 5: " + count5);
		System.out.println("Correct 5: " + correct5);
		System.out.println("Accuracy: " + df.format((correct5/count5)*100) + "%");
		System.out.println();
		
		int[] weights6 = new int[16];
		for (int i = 0; i < 16; i++)
		{
			weights6[i] = 1;
		}
		
		for (Instance instance: set6)
		{
			if (rest6.contains(instance))
				rest6.remove(instance);
		}
		for (Instance instance: rest6) 
		{
			boolean prediction = predict(instance, weights6);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights6[i] = weights6[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights6[i] = 0; //zero out its weight
				}
			}
		}
		double count6 = 0;
		double correct6 = 0;

		for (Instance instance: set6)
		{
			boolean prediction = predict(instance, weights6);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights6[i] = weights6[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights6[i] = 0; //zero out its weight
				}
			}
			else
			{
				correct6 += 1;
			}
			count6 += 1;
		}
		
		System.out.println("Count 6: " + count6);
		System.out.println("Correct 6: " + correct6);
		System.out.println("Accuracy: " + df.format((correct6/count6)*100) + "%");
		System.out.println();
		
		int[] weights7 = new int[16];
		for (int i = 0; i < 16; i++)
		{
			weights7[i] = 1;
		}
		
		for (Instance instance: set7)
		{
			if (rest7.contains(instance))
				rest7.remove(instance);
		}
		for (Instance instance: rest7) 
		{
			boolean prediction = predict(instance, weights7);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights7[i] = weights7[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights7[i] = 0; //zero out its weight
				}
			}
		}
		double count7 = 0;
		double correct7 = 0;

		for (Instance instance: set7)
		{
			boolean prediction = predict(instance, weights7);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights7[i] = weights7[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights7[i] = 0; //zero out its weight
				}
			}
			else
			{
				correct7 += 1;
			}
			count7 += 1;
		}
		
		System.out.println("Count 7: " + count7);
		System.out.println("Correct 7: " + correct7);
		System.out.println("Accuracy: " + df.format((correct7/count7)*100) + "%");
		System.out.println();
		
		int[] weights8 = new int[16];
		for (int i = 0; i < 16; i++)
		{
			weights8[i] = 1;
		}
		
		for (Instance instance: set8)
		{
			if (rest8.contains(instance))
				rest8.remove(instance);
		}
		for (Instance instance: rest8) 
		{
			boolean prediction = predict(instance, weights8);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights8[i] = weights8[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights8[i] = 0; //zero out its weight
				}
			}
		}
		double count8 = 0;
		double correct8 = 0;

		for (Instance instance: set8)
		{
			boolean prediction = predict(instance, weights8);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights8[i] = weights8[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights8[i] = 0; //zero out its weight
				}
			}
			else
			{
				correct8 += 1;
			}
			count8 += 1;
		}
		
		System.out.println("Count 8: " + count8);
		System.out.println("Correct 8: " + correct8);
		System.out.println("Accuracy: " + df.format((correct8/count8)*100) + "%");
		System.out.println();
		
		int[] weights9 = new int[16];
		for (int i = 0; i < 16; i++)
		{
			weights9[i] = 1;
		}
		
		for (Instance instance: set9)
		{
			if (rest9.contains(instance))
				rest9.remove(instance);
		}
		for (Instance instance: rest9) 
		{
			boolean prediction = predict(instance, weights9);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights9[i] = weights9[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights9[i] = 0; //zero out its weight
				}
			}
		}
		double count9 = 0;
		double correct9 = 0;

		for (Instance instance: set9)
		{
			boolean prediction = predict(instance, weights9);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights9[i] = weights9[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights9[i] = 0; //zero out its weight
				}
			}
			else
			{
				correct9 += 1;
			}
			count9 += 1;
		}
		
		System.out.println("Count 9: " + count9);
		System.out.println("Correct 9: " + correct9);
		System.out.println("Accuracy: " + df.format((correct9/count9)*100) + "%");
		System.out.println();
		
		
		int[] weights10 = new int[16];
		for (int i = 0; i < 16; i++)
		{
			weights10[i] = 1;
		}
		
		for (Instance instance: set10)
		{
			if (rest10.contains(instance))
				rest10.remove(instance);
		}
		for (Instance instance: rest10) 
		{
			boolean prediction = predict(instance, weights10);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights10[i] = weights10[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights10[i] = 0; //zero out its weight
				}
			}
		}
		double count10 = 0;
		double correct10 = 0;

		for (Instance instance: set10)
		{
			boolean prediction = predict(instance, weights10);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			if (!prediction && answer)
			{
				for (int i = 0; i < 16; i++)
				{
					if (array[i])
						weights10[i] = weights10[i] * 2;
				}
			}
			else if (prediction && !answer)
			{
				for (int i = 0; i < 16; i++) //for each feature
				{
					if (array[i]) //if the value was true
						weights10[i] = 0; //zero out its weight
				}
			}
			else
			{
				correct10 += 1;
			}
			count10 += 1;
		}
		
		System.out.println("Count 10: " + count10);
		System.out.println("Correct 10: " + correct10);
		System.out.println("Accuracy: " + df.format((correct10/count10)*100) + "%");
		

	}

	public static boolean predict(Instance instance, int[] w) //method to predict an instance's answer
	{
		boolean[] array = instance.getArray(); //gets the array's features
		int[] numbers = new int[16];
		int sum = 0;
		for (int i = 0; i < 16; i++) //turns boolean feature array into ints
		{
			if (array[i])
				numbers[i] = 1;
			else
				numbers[i] = 0;
		}
		for (int i = 0; i < 16; i++) //computes dot product of weights and features
		{
			sum += (numbers[i] * w[i]);
		}
		if (sum > 8) //with 16 features, n/2 = 8
			return true;
		else
			return false;
	}


}
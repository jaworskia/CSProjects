import java.io.*;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Random;
import java.text.*;

//Adam Jaworski
//CS 1675

public class Perceptron

{

	static double[] weights = new double[16]; //Again I don't think I ended up using these
	static double weight0;

	public static void main(String[] args) throws IOException
	{
		String dataFile = args[0]; //the file containing the data
		for (int i = 0; i < 16; i++) //weights for the features
		{
			weights[i] = 0; //all initialized to 0
		}
		weight0 = 0;
		ArrayList<Instance> instances = new ArrayList<Instance>(); //stores all of the instances
		DecimalFormat df = new DecimalFormat("#.##");
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
		
		ArrayList<Instance> set1 = new ArrayList<Instance>();   //test sets
		ArrayList<Instance> set2 = new ArrayList<Instance>();
		ArrayList<Instance> set3 = new ArrayList<Instance>();
		ArrayList<Instance> set4 = new ArrayList<Instance>();
		ArrayList<Instance> set5 = new ArrayList<Instance>();
		ArrayList<Instance> set6 = new ArrayList<Instance>();
		ArrayList<Instance> set7 = new ArrayList<Instance>();
		ArrayList<Instance> set8 = new ArrayList<Instance>();
		ArrayList<Instance> set9 = new ArrayList<Instance>();
		ArrayList<Instance> set10 = new ArrayList<Instance>();
		
		ArrayList<Instance> rest1 = new ArrayList<Instance>();  //training set for test set set1
		ArrayList<Instance> rest2 = new ArrayList<Instance>(); //and so on
		ArrayList<Instance> rest3 = new ArrayList<Instance>();
		ArrayList<Instance> rest4 = new ArrayList<Instance>();
		ArrayList<Instance> rest5 = new ArrayList<Instance>();
		ArrayList<Instance> rest6 = new ArrayList<Instance>();
		ArrayList<Instance> rest7 = new ArrayList<Instance>();
		ArrayList<Instance> rest8 = new ArrayList<Instance>();
		ArrayList<Instance> rest9 = new ArrayList<Instance>();
		ArrayList<Instance> rest10 = new ArrayList<Instance>();
		
		for (Instance instance: instances) //for each instance we read from the file
		{
			rest1.add(instance); //add it to each of the training sets
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
			int random = rand.nextInt(n); //pick an instance at random
			Instance curr = instances.get(random);
			set2.add(curr); 
			instances.remove(curr); //remove from instances, so we don't pick it again
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n); //pick an instance at random
			Instance curr = instances.get(random);
			set3.add(curr); 
			instances.remove(curr); //remove from instances, so we don't pick it again
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n); //pick an instance at random
			Instance curr = instances.get(random);
			set4.add(curr); 
			instances.remove(curr); //remove from instances, so we don't pick it again
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n); //pick an instance at random
			Instance curr = instances.get(random);
			set5.add(curr); 
			instances.remove(curr); //remove from instances, so we don't pick it again
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n); //pick an instance at random
			Instance curr = instances.get(random);
			set6.add(curr); 
			instances.remove(curr); //remove from instances, so we don't pick it again
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n); //pick an instance at random
			Instance curr = instances.get(random);
			set7.add(curr); 
			instances.remove(curr); //remove from instances, so we don't pick it again
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n); //pick an instance at random
			Instance curr = instances.get(random);
			set8.add(curr); 
			instances.remove(curr); //remove from instances, so we don't pick it again
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n); //pick an instance at random
			Instance curr = instances.get(random);
			set9.add(curr); 
			instances.remove(curr); //remove from instances, so we don't pick it again
		}
		for (int i = 0; i < fraction; i++)
		{
			n = instances.size();
			int random = rand.nextInt(n); //pick an instance at random
			Instance curr = instances.get(random);
			set10.add(curr); 
			instances.remove(curr); //remove from instances, so we don't pick it again
		}
		
		
		
		double[] weights1 = new double[16];  //weights for trial 1
		double weight01 = 0;   //weight 0 for trial 1
		for (int i = 0; i < 16; i++)
		{
			weights1[i] = 0; //initialize weights to 0
		}
		for (Instance instance: set1) //for each instance in the test set
		{
			if (rest1.contains(instance))  //remove it from the training set
				rest1.remove(instance);
		}
		
		for (Instance instance: rest1) //for each instance in the training set
		{
			boolean prediction = predict(instance, weights1, weight01); //get prediction
			boolean answer = instance.getAnswer();  //get answer
			boolean[] array = instance.getArray();  //get the features
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)  //true corresponds to 1, false to -1
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights1[i] = (weights1[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight01 = weight01 + (.05)*(ans - pred); //update weight 0
			}
		}
		double count1 = 0;  //total number of test cases in trial 1 (will be the same for each trial)
		double correct1 = 0;  //number of correct predictions made on trial 1

		for (Instance instance: set1) //for each instance in the test set
		{
			boolean prediction = predict(instance, weights1, weight01); //get the prediction
			boolean answer = instance.getAnswer(); //and the answer
			boolean[] array = instance.getArray(); //and the features
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction) //true is 1, false is -1
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights1[i] = (weights1[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight01 = weight01 + (.05)*(ans - pred); //update weight 0
			}
			else //prediction must have been correct
			{
				correct1 += 1; //increment correct
			}
			count1 += 1; //increment count
		}
		
		System.out.println("Count 1: " + count1);  //output results
		System.out.println("Correct 1: " + correct1);
		System.out.println("Accuracy: " + df.format((correct1/count1)*100) + "%");
		System.out.println();
		
		double[] weights2 = new double[16];    //AND SO ON FOR THE REST OF THE TRIALS
		double weight02 = 0;
		for (int i = 0; i < 16; i++)
		{
			weights2[i] = 0;
		}
		for (Instance instance: set2)
		{
			if (rest2.contains(instance))
				rest2.remove(instance);
		}
		
		for (Instance instance: rest2) 
		{
			boolean prediction = predict(instance, weights2, weight02);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights2[i] = (weights2[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight02 = weight02 + (.05)*(ans - pred); //update weight 0
			}
		}
		double count2 = 0;
		double correct2 = 0;

		for (Instance instance: set2)
		{
			boolean prediction = predict(instance, weights2, weight02);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights2[i] = (weights2[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight02 = weight02 + (.05)*(ans - pred); //update weight 0
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
		
		double[] weights3 = new double[16];
		double weight03 = 0;
		for (int i = 0; i < 16; i++)
		{
			weights3[i] = 0;
		}
		for (Instance instance: set3)
		{
			if (rest3.contains(instance))
				rest3.remove(instance);
		}
		
		for (Instance instance: rest3) 
		{
			boolean prediction = predict(instance, weights3, weight03);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights3[i] = (weights3[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight03 = weight03 + (.05)*(ans - pred); //update weight 0
			}
		}
		double count3 = 0;
		double correct3 = 0;

		for (Instance instance: set3)
		{
			boolean prediction = predict(instance, weights3, weight03);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights3[i] = (weights3[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight03 = weight03 + (.05)*(ans - pred); //update weight 0
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
		
		double[] weights4 = new double[16];
		double weight04 = 0;
		for (int i = 0; i < 16; i++)
		{
			weights4[i] = 0;
		}
		for (Instance instance: set4)
		{
			if (rest4.contains(instance))
				rest4.remove(instance);
		}
		
		for (Instance instance: rest4) 
		{
			boolean prediction = predict(instance, weights4, weight04);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights4[i] = (weights4[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight04 = weight04 + (.05)*(ans - pred); //update weight 0
			}
		}
		double count4 = 0;
		double correct4 = 0;

		for (Instance instance: set4)
		{
			boolean prediction = predict(instance, weights4, weight04);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights4[i] = (weights4[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight04 = weight04 + (.05)*(ans - pred); //update weight 0
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
		
		double[] weights5 = new double[16];
		double weight05 = 0;
		for (int i = 0; i < 16; i++)
		{
			weights5[i] = 0;
		}
		for (Instance instance: set5)
		{
			if (rest5.contains(instance))
				rest5.remove(instance);
		}
		
		for (Instance instance: rest5) 
		{
			boolean prediction = predict(instance, weights5, weight05);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights5[i] = (weights5[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight05 = weight05 + (.05)*(ans - pred); //update weight 0
			}
		}
		double count5 = 0;
		double correct5 = 0;

		for (Instance instance: set5)
		{
			boolean prediction = predict(instance, weights5, weight05);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights5[i] = (weights5[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight05 = weight05 + (.05)*(ans - pred); //update weight 0
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
		
		double[] weights6 = new double[16];
		double weight06 = 0;
		for (int i = 0; i < 16; i++)
		{
			weights6[i] = 0;
		}
		for (Instance instance: set6)
		{
			if (rest6.contains(instance))
				rest6.remove(instance);
		}
		
		for (Instance instance: rest6) 
		{
			boolean prediction = predict(instance, weights6, weight06);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights6[i] = (weights6[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight06 = weight06 + (.05)*(ans - pred); //update weight 0
			}
		}
		double count6 = 0;
		double correct6 = 0;

		for (Instance instance: set6)
		{
			boolean prediction = predict(instance, weights6, weight06);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights6[i] = (weights6[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight06 = weight06 + (.05)*(ans - pred); //update weight 0
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
		
		double[] weights7 = new double[16];
		double weight07 = 0;
		for (int i = 0; i < 16; i++)
		{
			weights7[i] = 0;
		}
		for (Instance instance: set7)
		{
			if (rest7.contains(instance))
				rest7.remove(instance);
		}
		
		for (Instance instance: rest7) 
		{
			boolean prediction = predict(instance, weights7, weight07);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights7[i] = (weights7[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight07 = weight07 + (.05)*(ans - pred); //update weight 0
			}
		}
		double count7 = 0;
		double correct7 = 0;

		for (Instance instance: set7)
		{
			boolean prediction = predict(instance, weights7, weight07);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights7[i] = (weights7[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight07 = weight07 + (.05)*(ans - pred); //update weight 0
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
		
		double[] weights8 = new double[16];
		double weight08 = 0;
		for (int i = 0; i < 16; i++)
		{
			weights8[i] = 0;
		}
		for (Instance instance: set8)
		{
			if (rest8.contains(instance))
				rest8.remove(instance);
		}
		
		for (Instance instance: rest8) 
		{
			boolean prediction = predict(instance, weights8, weight08);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights8[i] = (weights8[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight08 = weight08 + (.05)*(ans - pred); //update weight 0
			}
		}
		double count8 = 0;
		double correct8 = 0;

		for (Instance instance: set8)
		{
			boolean prediction = predict(instance, weights8, weight08);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights8[i] = (weights8[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight08 = weight08 + (.05)*(ans - pred); //update weight 0
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
		
		double[] weights9 = new double[16];
		double weight09 = 0;
		for (int i = 0; i < 16; i++)
		{
			weights9[i] = 0;
		}
		for (Instance instance: set9)
		{
			if (rest9.contains(instance))
				rest9.remove(instance);
		}
		
		for (Instance instance: rest9) 
		{
			boolean prediction = predict(instance, weights9, weight09);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights9[i] = (weights9[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight09 = weight09 + (.05)*(ans - pred); //update weight 0
			}
		}
		double count9 = 0;
		double correct9 = 0;

		for (Instance instance: set9)
		{
			boolean prediction = predict(instance, weights9, weight09);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights9[i] = (weights9[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight09 = weight09 + (.05)*(ans - pred); //update weight 0
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
		
		double[] weights10 = new double[16];
		double weight010 = 0;
		for (int i = 0; i < 16; i++)
		{
			weights10[i] = 0;
		}
		for (Instance instance: set10)
		{
			if (rest10.contains(instance))
				rest10.remove(instance);
		}
		
		for (Instance instance: rest10) 
		{
			boolean prediction = predict(instance, weights10, weight010);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights10[i] = (weights10[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight010 = weight010 + (.05)*(ans - pred); //update weight 0
			}
		}
		double count10 = 0;
		double correct10 = 0;

		for (Instance instance: set10)
		{
			boolean prediction = predict(instance, weights10, weight010);
			boolean answer = instance.getAnswer();
			boolean[] array = instance.getArray();
			int[] numbers = new int[16];
			for (int i = 0; i < 16; i++) //turns boolean feature array into ints
			{
				if (array[i])
					numbers[i] = 1;
				else
					numbers[i] = 0;
			}
			int pred = 0;
			int ans = 0;
			if (prediction)
				pred = 1;
			else
				pred = -1;
			if (answer)
				ans = 1;
			else
				ans = -1;
			if (prediction != answer) //if the prediction was not the same as the answer
			{
				for (int i = 0; i < 16; i++) //for each feature/weight
				{
					weights10[i] = (weights10[i] + (.05)*(ans - pred)*(numbers[i])); //update the weight (learning rate is .05)
				}
				weight010 = weight010 + (.05)*(ans - pred); //update weight 0
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

	public static boolean predict(Instance instance, double[] w, double weighta) //method to predict an instance's answer
	{
		boolean[] array = instance.getArray(); //gets features
		int[] numbers = new int[16];
		double sum = 0;
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
		sum += weighta; //add weight 0
		if (sum > 0) //return true if the sum is positive
			return true;
		else //otherwise return false
			return false;
	}


}
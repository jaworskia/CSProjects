Adam Jaworski
Machine Learning HW #2
README file for Winnow and Perceptron implementations

DEPENDENCY: Instance.java
this is just a data structure to hold information about an instance (training/test
case)

To run Winnow.java, obviously first compile it, then do:
java Winnow <datafile>

for example,
java Winnow ArtificialData.txt


Similarly, for Perceptron.java, do:
java Perceptron <datafile>

as in,
java Perceptron VotesData.txt


The data file needs to be a .txt file, and they need to be formatted as they are
in the two files I've included (VotesData.txt and ArtificialData.txt).

The format is:
instance number,answer,feature1,feature2,...feature16
(where the boolean answer and features are represented as either 'y' or 'n')

The programs should work for any such data set with 16 features. However, in 
the case of VotesData.txt, the answer is represented as either "democrat" or
"republican". So the programs interpret "democrat" as true (y) and "republican" as
false (n).

IMPORTANT NOTE: These programs were tailored for the two provided data sets
(each with 16 features). Data sets with a different number of features won't work.

The programs are largely built in the same way (indeed, they share a lot of code
that's exactly the same) to handle reading the file and setting up the ten trials.
The only real differences are in how they make predictions and update their 
weights.

Notably the programs only take one data file as an argument, instead of two
(one for training and one for testing).The programs first read in all instances
(cases) from the data file. Then they're dynamically and randomly split up into
ten even partitions. These partitions are the test sets for each of the ten trials.

For the training sets, the full set of instances is copied ten times into a
training set for each test set. Then, all of the instances in test set k are
removed from training set k (so that they're not seen twice).

Each trial has its own array of weights, and variables for other information like
number of correct predictions. For trial k, the system processes all instances
in training set k. Then it's shown the instances in test set k, keeping track of
how many instances there are and how many it got correct. For each trial, the
programs output the number of cases in the training set, the number of correct
predictions, and the accuracy as a percentage.

The programs don't do anything for the statistical analysis, they just output the
results of the trials. I did the statistics stuff by hand.

Finally, since the partitions are determined dynamically and randomly, different
runs of the programs will yield different outputs. The overall results are always
similar though.

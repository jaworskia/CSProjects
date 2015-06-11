from pybrain.structure import FeedForwardNetwork
from pybrain.structure import LinearLayer, SigmoidLayer, SoftmaxLayer
from pybrain.structure import FullConnection
from pybrain.datasets import SupervisedDataSet
from pybrain.supervised.trainers import BackpropTrainer
from pybrain.datasets            import ClassificationDataSet
from pybrain.utilities           import percentError
from pybrain.tools.shortcuts     import buildNetwork
from pybrain.structure.modules   import SoftmaxLayer
from pylab import ion, ioff, figure, draw, contourf, clf, show, hold, plot
from scipy import diag, arange, meshgrid, where
from numpy.random import multivariate_normal				#import everything we should need

#Adam Jaworski
#CS 1675
#Assignment 3


with open("data.txt") as f:			#reads in all the lines from "data.txt"
	content = f.readlines()
	
alldata = ClassificationDataSet(64, 1, nb_classes=10)
	
for line in content:           #for every line we've read in
	list = line.split(",")		#it's a sample, so add it to the data set
	alldata.addSample((list[0], list[1], list[2], list[3], list[4], list[5], list[6], list[7], list[8], list[9], list[10], list[11], list[12], list[13], list[14], list[15], list[16], list[17], list[18], list[19], list[20], list[21], list[22], list[23], list[24], list[25], list[26], list[27], list[28], list[29], list[30], list[31], list[32], list[33], list[34], list[35], list[36], list[37], list[38], list[39], list[40], list[41], list[42], list[43], list[44], list[45], list[46], list[47], list[48], list[49], list[50], list[51], list[52], list[53], list[54], list[55], list[56], list[57], list[58], list[59], list[60], list[61], list[62], list[63]),(list[64]))

tstdata, trndata = alldata.splitWithProportion( 0.25 )			#25% of the data will be used for testing, 75% for training

trndata._convertToOneOfMany( )
tstdata._convertToOneOfMany( )		#the tutorial said I should do this

print "Number of training patterns: ", len(trndata)						#some information about the data set
print "Input and output dimensions: ", trndata.indim, trndata.outdim
print "First sample (input, target, class):"
print trndata['input'][0], trndata['target'][0], trndata['class'][0]


noHidden = FeedForwardNetwork()
inLayerNoHidden = LinearLayer(64)			#64-unit input layer
outLayerNoHidden = LinearLayer(10)			#10-unit output layer (one unit for each class)
noHidden.addInputModule(inLayerNoHidden)
noHidden.addOutputModule(outLayerNoHidden)		#add the layers to the network
noHidden.addConnection(FullConnection(inLayerNoHidden, outLayerNoHidden))		#no hidden layers, so hook input up to output
noHidden.sortModules()

trainer1 = BackpropTrainer(noHidden, trndata)

print "No Hidden"

for i in xrange(25):					#prints results of testing for network with no hidden layers
	trainer1.trainEpochs(1)
	tstresult1 = percentError(trainer1.testOnClassData(dataset=tstdata), tstdata['class'])
	print "epoch: %4d" % trainer1.totalepochs, " test error: %5.2f%%" % tstresult1
	
oneHidden = FeedForwardNetwork()
inLayerOne1 = LinearLayer(16)
inLayerOne2 = LinearLayer(16)
inLayerOne3 = LinearLayer(16)
inLayerOne4 = LinearLayer(16)					#inputs are divided into four
hiddenLayerOne1 = SigmoidLayer(10)				#each goes to hidden unit
outLayerOne = LinearLayer(10)					#the output layer, all hidden units go to it
oneHidden.addInputModule(inLayerOne1)
oneHidden.addInputModule(inLayerOne2)
oneHidden.addInputModule(inLayerOne3)
oneHidden.addInputModule(inLayerOne4)
oneHidden.addModule(hiddenLayerOne1)
oneHidden.addOutputModule(outLayerOne)				#adds all the modules
oneHidden.addConnection(FullConnection(inLayerOne1, hiddenLayerOne1))
oneHidden.addConnection(FullConnection(inLayerOne2, hiddenLayerOne1))
oneHidden.addConnection(FullConnection(inLayerOne3, hiddenLayerOne1))
oneHidden.addConnection(FullConnection(inLayerOne4, hiddenLayerOne1))
oneHidden.addConnection(FullConnection(hiddenLayerOne1, outLayerOne))
oneHidden.sortModules()			

trainer2 = BackpropTrainer(oneHidden, trndata)				#backpropagation trainer

print ("One Hidden")

for i in xrange(25):						#prints results of testing for network with one hidden layer
	trainer2.trainEpochs(1)
	tstresult2 = percentError(trainer2.testOnClassData(dataset=tstdata), tstdata['class'])
	print "epoch: %4d" % trainer2.totalepochs, " test error: %5.2f%%" % tstresult2

	
creative = FeedForwardNetwork()
cin1 = LinearLayer(8)
cin2 = LinearLayer(8)
cin3 = LinearLayer(8)
cin4 = LinearLayer(8)
cin5 = LinearLayer(8)
cin6 = LinearLayer(8)
cin7 = LinearLayer(8)
cin8 = LinearLayer(8)				#inputs divided into 8 sets, each corresponding to a row
h1 = SigmoidLayer(32)				#3 hidden layers
h2 = SigmoidLayer(16)
h3 = SigmoidLayer(10)
cout = LinearLayer(10)				#output layer
creative.addInputModule(cin1)		
creative.addInputModule(cin2)
creative.addInputModule(cin3)
creative.addInputModule(cin4)
creative.addInputModule(cin5)
creative.addInputModule(cin6)
creative.addInputModule(cin7)
creative.addInputModule(cin8)
creative.addModule(h1)
creative.addModule(h2)
creative.addModule(h3)
creative.addOutputModule(cout)						#add all the modules
creative.addConnection(FullConnection(cin1, h1))		
creative.addConnection(FullConnection(cin2, h1))
creative.addConnection(FullConnection(cin3, h1))
creative.addConnection(FullConnection(cin4, h1))
creative.addConnection(FullConnection(cin5, h1))
creative.addConnection(FullConnection(cin6, h1))
creative.addConnection(FullConnection(cin7, h1))
creative.addConnection(FullConnection(cin8, h1))		#all 8 input sets connect to the first hidden layer
creative.addConnection(FullConnection(h1, h2))			#first to second	
creative.addConnection(FullConnection(h2, h3))			#second to third		
creative.addConnection(FullConnection(h3, cout))		#third to output	
creative.sortModules()

trainer3 = BackpropTrainer(creative, trndata)

print "Creative"

for i in xrange(25):								#prints results for three hidden layers				
	trainer3.trainEpochs(1)
	tstresult3 = percentError(trainer3.testOnClassData(dataset=tstdata), tstdata['class'])
	print "epoch: %4d" % trainer3.totalepochs, " test error: %5.2f%%" % tstresult3




	




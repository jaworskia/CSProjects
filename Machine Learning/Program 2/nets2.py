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

for i in xrange(5):					#prints results of testing for network with no hidden layers
	trainer1.trainEpochs(1)
	tstresult1 = percentError(trainer1.testOnClassData(dataset=tstdata), tstdata['class'])
	print "epoch: %4d" % trainer1.totalepochs, " test error: %5.2f%%" % tstresult1
	
	
OneLayer = buildNetwork(64, 1, 10, outclass=SoftmaxLayer)
trainer2 = BackpropTrainer(OneLayer, trndata)

print "One Layer"

for i in xrange(5):
	trainer2.trainEpochs(1)
	tstresult2 = percentError(trainer2.testOnClassData(dataset=tstdata), tstdata['class'])
	print "epoch: %4d" % trainer2.totalepochs, "test error: %5.2f%%" % tstresult2

FiveLayers = buildNetwork(64, 5, 10, outclass=SoftmaxLayer)
trainer3 = BackpropTrainer(FiveLayers, trndata)

print "Five Layers"

for i in xrange(5):
	trainer3.trainEpochs(1)
	tstresult3 = percentError(trainer3.testOnClassData(dataset=tstdata), tstdata['class'])
	print "epoch: %4d" % trainer3.totalepochs, "test error: %5.2f%%" % tstresult3
	
TwelveLayers = buildNetwork(64, 12, 10, outclass=SoftmaxLayer)
trainer4 = BackpropTrainer(TwelveLayers, trndata)

print "Twelve Layers"

for i in xrange(5):
	trainer4.trainEpochs(1)
	tstresult4 = percentError(trainer4.testOnClassData(dataset=tstdata), tstdata['class'])
	print "epoch: %4d" % trainer4.totalepochs, "test error: %5.2f%%" % tstresult4
	




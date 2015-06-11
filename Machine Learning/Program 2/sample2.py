from pybrain.structure import FeedForwardNetwork
n = FeedForwardNetwork()
from pybrain.structure import LinearLayer, SigmoidLayer, SoftmaxLayer
inLayer1 = LinearLayer(2)
hiddenLayer1 = SigmoidLayer(3)
outLayer = LinearLayer(1)
inLayer2 = LinearLayer(2)
hiddenLayer2 = SigmoidLayer(3)
n.addInputModule(inLayer1)
n.addInputModule(inLayer2)
n.addModule(hiddenLayer1)
n.addModule(hiddenLayer2)
n.addOutputModule(outLayer)
from pybrain.structure import FullConnection
n.addConnection(FullConnection(inLayer1, hiddenLayer1))
n.addConnection(FullConnection(inLayer2, hiddenLayer2))
n.addConnection(FullConnection(hiddenLayer1, outLayer))
n.addConnection(FullConnection(hiddenLayer2, outLayer))
n.sortModules()												#this is all unchanged from before; it's the same network as sample.py

noHiddenLayerNetwork = FeedForwardNetwork()
inputLayer = LinearLayer(64)
outputLayer = LinearLayer(1)
noHiddenLayerNetwork.addInputModule(inputLayer)
noHiddenLayerNetwork.addOutputModule(outputLayer)
noHiddenLayerNetwork.addConnection(FullConnection(inputLayer, outputLayer))
noHiddenLayerNetwork.sortModules()

from pybrain.datasets import SupervisedDataSet
ds = SupervisedDataSet(4, 1)			#I think 4 is the size of the input vector, and 1 is the size of the output
										#I'll want 64, 1
										#and 3823 total

ds2 = SupervisedDataSet(64, 1)

ds.addSample((0, 0, 0, 0), (0,))	#a vector of four zeroes, and the answer is 0
ds.addSample((0, 0, 1, 1), (1,))
ds.addSample((0, 0, 0, 1), (0,))
ds.addSample((0, 1, 1, 0), (1,))

number = 0

with open("data.txt") as f:
	content = f.readlines()

for line in content:
	list = line.split(",")
	ds2.addSample((list[0], list[1], list[2], list[3], list[4], list[5], list[6], list[7], list[8], list[9], list[10], list[11], list[12], list[13], list[14], list[15], list[16], list[17], list[18], list[19], list[20], list[21], list[22], list[23], list[24], list[25], list[26], list[27], list[28], list[29], list[30], list[31], list[32], list[33], list[34], list[35], list[36], list[37], list[38], list[39], list[40], list[41], list[42], list[43], list[44], list[45], list[46], list[47], list[48], list[49], list[50], list[51], list[52], list[53], list[54], list[55], list[56], list[57], list[58], list[59], list[60], list[61], list[62], list[63]),(list[64]))
		


print "Before training"

sq_err = []
for data in ds2:				#for I guess every entry in the data set
    input_entry = data[0]		#the vector
    output_entry = data[1]		#the answer
    pred_entry = noHiddenLayerNetwork.activate(input_entry)	#I guess makes the prediction	(it's a real value; I guess we actually want it to be discrete for classification)
    #print 'Actual:', output_entry, 'Predicted', pred_entry
    #print (pred_entry - output_entry)**2
    sq_err.append((pred_entry - output_entry)**2)	#still in the loop; adds the squared difference to the array
print "RMSE no hidden pre train: %.2f" % (sum(sq_err) / len(sq_err))		#calculates and prints the average squared difference (or error)


from pybrain.supervised.trainers import BackpropTrainer		#just imports it
trainer = BackpropTrainer(n, ds)							#gives it both the network and the dataset
trainer.trainUntilConvergence()								#I...guess...this...trains...

print														#I guess just gives a newline

print "After training"								

sq_err = []
for data in ds2:											#same as before training
    input_entry = data[0]
    output_entry = data[1]
    pred_entry = noHiddenLayerNetwork.activate(input_entry)
    #print 'Actual:', output_entry, 'Predicted', pred_entry
    #print (pred_entry - output_entry)**2
    sq_err.append((pred_entry - output_entry)**2)
print "RMSE no hidden post train: %.2f" % (sum(sq_err) / len(sq_err))

#for line in content:
	#print line
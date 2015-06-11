from pybrain.structure import FeedForwardNetwork
n = FeedForwardNetwork()
from pybrain.structure import LinearLayer, SigmoidLayer, SoftmaxLayer

print("It's Working!")

# Creating the nodes
inLayer1 = LinearLayer(2) # input 1 and input 2
hiddenLayer1 = SigmoidLayer(3) # the three circle nodes on the top

inLayer2 = LinearLayer(2) # input 3 and input 4
hiddenLayer2 = SigmoidLayer(3) # the three circle nodes on the bottom

outLayer = LinearLayer(1) # the output node

# Adding the nodes into our network
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
n.sortModules()
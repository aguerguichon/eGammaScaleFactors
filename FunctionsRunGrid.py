import os
from string import *
import numpy as np

def GetDataFiles( inputs, scale, doScale, eleID, electronID, outFilePrefix ) :
    outFilePrefix.append( "Data_13TeV_Zee")
    dq2Command = 'cat datasets_data15.csv'
    datasets = os.popen( dq2Command ).read().split( '\n' )
    datasets.pop()
    inputs.append( datasets )

    doScale.append( scale )
    electronID.append( eleID )


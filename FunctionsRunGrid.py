import os
from string import *
import numpy as np

def GetDataFiles( inputs, scale, doScale, eleID, electronID, inFileIndex, outFilePrefix ) :

    fileList = [
        'DatasetList/Data_13TeV_Zee_25ns.csv',
        'DatasetList/Data_13TeV_Zee_50ns.csv',
        'DatasetList/MC_13TeV_Zee_25ns.csv',
        'DatasetList/MC_13TeV_Zee_50ns.csv'
        ]

    chosenFile = fileList[inFileIndex]
    dq2Command = 'cat ' + chosenFile 
    datasets = os.popen( dq2Command ).read().split( '\n' )
    datasets.pop()
    inputs.append( datasets )

    doScale.append( scale )
    electronID.append( eleID )

    if ( chosenFile.rfind( '.' ) != -1 ) : 
        chosenFile = chosenFile[0:chosenFile.rfind( '.' )]
    if ( chosenFile.rfind( '/' ) != -1 ) :
        chosenFile = chosenFile[chosenFile.rfind( '/' )+1:len( chosenFile )]
    outFilePrefix.append( chosenFile )


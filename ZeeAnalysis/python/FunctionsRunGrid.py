import os
from string import *
import numpy as np


def GetDataFiles( inputs, scale, doScale, eleID, electronID, inFileIndex, outFilePrefix, esModel, ptCut, ptCutVect ) :

    path= '/afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/DatasetList/'
    fileList = [
        ['Data_13TeV_Zee_25ns.csv', 'es2015PRE'],
        ['Data_13TeV_Zee_50ns.csv', 'es2015PRE'],
        ['MC_13TeV_Zee_25ns.csv', 'es2015PRE'],
        ['MC_13TeV_Zee_50ns.csv', 'es2015PRE'],
        ['Data_8TeV_Zee.csv', 'es2012c'],
        ['MC_8TeV_Zee_DiLepton.csv', 'es2012c'],
        ['MC_8TeV_Zee_1Lepton.csv' 'es2012c']
        ]

    chosenFile = path + fileList[inFileIndex][0]
    dq2Command = 'cat ' + chosenFile 
    datasets = os.popen( dq2Command ).read().split( '\n' )
    
    tmplist =[]
    tmpEsModel = fileList[inFileIndex][1]
#remove comments and empty lines from datasets
    for line  in datasets :
        if line == '' or '#' in line : continue
        tmplist.append( line )


    inputs.append( tmplist )
    doScale.append( scale )
    electronID.append( eleID )
    esModel.append( tmpEsModel )

    if ( chosenFile.rfind( '.' ) != -1 ) : 
        chosenFile = chosenFile[0:chosenFile.rfind( '.' )]
    if ( chosenFile.rfind( '/' ) != -1 ) :
        chosenFile = chosenFile[chosenFile.rfind( '/' )+1:len( chosenFile )]
    outFilePrefix.append( chosenFile )

    ptCutVect.append( ptCut )

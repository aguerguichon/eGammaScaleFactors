import os
from string import *
import numpy as np

def GetDataFiles( inputs, scale, doScale, eleID, electronID, inFileIndex, outFilePrefix, esModel, ptCut, ptCutVect ) :

    fileList = [
        'DatasetList/Data_13TeV_Zee_25ns.csv',
        'DatasetList/Data_13TeV_Zee_50ns.csv',
        'DatasetList/MC_13TeV_Zee_25ns.csv',
        'DatasetList/MC_13TeV_Zee_50ns.csv',
        'DatasetList/Data_8TeV_Zee.csv',
        'DatasetList/MC_8TeV_Zee_DiLepton.csv',
        'DatasetList/MC_8TeV_Zee_1Lepton.csv'
        ]

    chosenFile = fileList[inFileIndex]
    dq2Command = 'cat ' + chosenFile 
    datasets = os.popen( dq2Command ).read().split( '\n' )
    
    tmplist =[]
    tmpEsModel = ''
#remove comments and empty lines from datasets
    for line  in datasets :
        if 'esModel' in line :
            tmpEsModel = line
        if line == '' or '#' in line :
            continue
        tmplist.append( line )

    tmpEsModel = tmpEsModel.split( ' ' )[1]

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

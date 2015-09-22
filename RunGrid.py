import os
import sys
import numpy as np
from FunctionsRunGrid import *

mode = 1
inputs = []
options=""
doScale = []
electronID = []
outFilePrefix = []
esModel = []

GetDataFiles( inputs, 1, doScale, 1, electronID, 0, outFilePrefix, esModel)

print inputs

if len( inputs ) != len( doScale ) or len( inputs ) != len( outFilePrefix ) or len( inputs ) != len( esModel ) :
    print( "Wrong tabular sizes" )
    exit(0)

tab = np.genfromtxt( "Version.txt", dtype='S100', delimiter=' ' )

#=============================
for iFile in range( 0, len( inputs ) ) :
    version=0

    outFileName = outFilePrefix[iFile]

    electronIDTitle = ""
    if ( electronID[iFile] / 3 < 1 ) : 
        electronIDTitle += "Lkh"
    else :
        electronIDTitle += "CB"
    electronIDTitle += str( electronID[iFile] % 3 )
    outFileName += "_" + electronIDTitle  

    if doScale[iFile] :
        outFileName += "_scaled"
    
    for iName in tab :
        if  iName[0] == outFileName  :
            iName[1]=str( int(iName[1] ) + 1)
            version=iName[1]

    if  version == 0 :
        tab = np.resize( tab, (len(tab)+1, 2))
        tab[ len( tab )-1 ] = [ outFileName, 0]

    datasetList = ""
    for iDataset in range( 0, len( inputs[iFile] ) ):
        print iDataset
        print inputs[iFile][iDataset]
        if ( iDataset ) :
            datasetList += ','
        datasetList += inputs[iFile][iDataset]

    outFileName +=  "_" + str( version )
    print outFileName

    commandLine = 'prun'
#    commandLine += ' --exec "RunZee %IN --outName Ntuple.root '
    commandLine += ' --exec "RunZee \`echo %IN | sed \'s/,/ /g\'\` --outName Ntuple.root '
    commandLine += ' --doScale ' + str( doScale[iFile] ) + ' --doSmearing ' + str( doScale[iFile] ) 
    commandLine += ' --electronID ' + str( electronID[iFile] )
    commandLine += ' --esModel ' + esModel[iFile]
    commandLine += '"'
    commandLine += ' --outDS user.cgoudet.' + outFileName
    commandLine += ' --inDS ' + datasetList
    commandLine += ' --outputs Ntuple.root '
    commandLine += ( ' --useRootCore --extFile=lumicalc_histograms_None_200842-215643.root '
                     + '--extFile=ilumicalc_histograms_None_13TeV_25ns.root '
                     + '--extFile=ilumicalc_histograms_None_13TeV_50ns.root '
                     + options )
    
    os.system( commandLine )
    print commandLine
np.savetxt( 'Version.txt', tab, delimiter=' ', fmt='%s')  




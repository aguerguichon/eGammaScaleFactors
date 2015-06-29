import os
from string import *
import numpy as np

def GetDataFiles( inputs, scale, doScale, eleID, electronID, outFilePrefix ) :
    prefix = "Data_13TeV_Zee_"
    dq2Command = 'dq2-ls data15_13TeV.*.physics_Main.merge.DAOD_EGAM6.*/ | awk -F ":" \'{print $2 }\''
    datasets = os.popen( dq2Command ).read().split( '\n' )
    datasets.pop()
    print datasets
    runNumbers = []
    for runN in datasets :
        if  runN.find( '.' ) != -1  :
            runN = runN[runN.find( '.' )+1:]

        if  runN.find( '.' ) != -1  :
            runN = runN[:runN.find( '.' )]
        runNumbers.append( runN )

    print runNumbers
    launchedDatasets = np.genfromtxt( "launchedDatasets.txt", dtype='S100', delimiter=' ' )

    for iRun in range( 0, len( datasets )) :
        exists = 0
        for line in launchedDatasets :
            if line[0]==runNumbers[iRun] and str(scale)==line[1] and str(eleID)==line[2] :
                exists = 1
        if exists  :
            continue
        
        doScale.append( scale )
        electronID.append( eleID )
        launchedDatasets = np.resize( launchedDatasets, (len(launchedDatasets)+1, 3 ))
        launchedDatasets[ len( launchedDatasets )-1 ] = [ runNumbers[iRun], str(scale), str(eleID)]
        inputs.append( [datasets[iRun]] )
        outFilePrefix.append( prefix + str( runNumbers[iRun] ) )
                              
    np.savetxt( 'launchedDatasets.txt', launchedDatasets, delimiter=' ', fmt='%s')  

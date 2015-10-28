import os
import sys
import numpy as np
from FunctionsRunGrid import *
import subprocess as sub
inputs = []
doScale = []
electronID = []
esModel = []
ptCutVect = []
outFilePrefix = []

if len( sys.argv ) == 1 :
    print 'An option is needed to run the program. \nPossibilities are : '
    print 'DATA25 to update 25ns datasets'
    print 'DATA50 to update 50ns datasets'
    print 'MC25'

for option in sys.argv:
    if option == 'RunGrid.py' : continue

    if option in ['DATA25', 'DATA50'] :
#Get the list of all the runs in the grl
        savingFile="/afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/DatasetList/Data_13TeV_Zee_" + ( '25' if option == 'DATA25' else '50' ) + "ns.csv"
        datasetList = np.genfromtxt( savingFile, dtype='S100', delimiter=' ' )
        runs = []
        grlList=os.popen( 'ls /afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/data15*').read().split()
        for grl  in grlList :
            file = open( grl )
            for line in file :
                if not 'RunList' in line : continue
                runs += line.split( '>' )[1].split('<')[0].split(',')
                
#Check if the latest version of the run dataset belong to the datasetList

        oldDataset=''
        currentDatasetIndex=-1
        runAll = 0
        addDataset=[]
        
        for run in runs :

            oldDataset = ''
            currentDatasetIndex=-1
    
            run = int( run )
            if option == 'DATA25' and ( run < 276262 or run == 276731 ): continue
            if option == 'DATA50' and run > 276261 and run != 276731 : continue
            if run != 280614 : continue
            print run                    
#look for the current file for this run 
            for iDataset in range(0, len( datasetList ) ) :
                if str( run ) not in datasetList[iDataset] : continue
                currentDatasetIndex = iDataset
                
            line='dq2-ls data15_13TeV.00'+ str( run )+'.physics_Main.merge.DAOD_EGAM1.*/'
            output = os.popen(line).read().split()

            for file in output :
                if oldDataset == '' : oldDataset = file
                if oldDataset.split('.')[1] != file.split('.')[1] or file==output[-1] :
                    if currentDatasetIndex == -1 : addDataset.append( oldDataset )
                    elif datasetList[currentDatasetIndex] != oldDataset : runAll=1; datasetList[currentDatasetIndex] = oldDataset
                    else : continue
                elif oldDataset < file : oldDataset = file 
        
        datasetList= list( datasetList) + list( addDataset )
        np.savetxt( savingFile, datasetList, delimiter=' ', fmt='%s')
        rangeMax = 2 if option=='DATA50' else (5 if runAll or len( addDataset ) else 0 )

        for iLaunch in range( 0, rangeMax ) :
            outFilePrefix.append( 'Data_13TeV_Zee_25ns' if runAll else 'Data_13TeV_Zee_25ns' )
            esModel.append( 'es2015PRE' )
            inputs.append( datasetList if runAll else addDataset )
            electronID.append( 1 if iLaunch<4 else 2 )
            doScale.append( 1 if iLaunch==1 else 0 )
            if iLaunch == 3 : ptCutVect.append( 35 )
            elif iLaunch==2 : ptCutVect.append( 30 )
            else : ptCutVect.append( 27 )

    if 'MC25' == option :
        GetDataFiles( inputs, 0, doScale, 2, electronID, 2, outFilePrefix, esModel, 27, ptCutVect)
        GetDataFiles( inputs, 0, doScale, 1, electronID, 2, outFilePrefix, esModel, 27, ptCutVect)
        GetDataFiles( inputs, 0, doScale, 1, electronID, 2, outFilePrefix, esModel, 30, ptCutVect)
        GetDataFiles( inputs, 0, doScale, 1, electronID, 2, outFilePrefix, esModel, 35, ptCutVect)

    if 'MC50' == option :
        GetDataFiles( inputs, 0, doScale, 2, electronID, 3, outFilePrefix, esModel, 27, ptCutVect)
        GetDataFiles( inputs, 0, doScale, 1, electronID, 3, outFilePrefix, esModel, 27, ptCutVect)

    if option=='download' :
        path='/sps/atlas/c/cgoudet/Calibration/DataxAOD/'
        gridJobIDList = np.genfromtxt( 'GridJobList.txt', dtype='S100', delimiter=' ' )
        for job in gridJobIDList :
            print job

# mode = 1

if len( inputs ) :

    if len( inputs ) != len( doScale ) or len( inputs ) != len( outFilePrefix ) or len( inputs ) != len( esModel ) :
        print( "Wrong tabular sizes" )
        exit(0)

    tab = np.genfromtxt( "Version.txt", dtype='S100', delimiter=' ' )

    jobIDList=[]
#=============================
    for iFile in range( 0, len( inputs ) ) :
        version=0
        
        outFileName = outFilePrefix[iFile]
        
        electronIDTitle = ""
        if ( electronID[iFile] / 3 < 1 ) : electronIDTitle += "Lkh"
        else : electronIDTitle += "CB"
        electronIDTitle += str( electronID[iFile] % 3 )
        outFileName += "_" + electronIDTitle  

        if doScale[iFile] : outFileName += "_scaled"
        if ptCutVect[iFile] != 27 : outFileName += '_pt' + str( ptCutVect[iFile] )    

        for iName in tab :
            if  iName[0] == outFileName  : version=str( int(iName[1] ) + 1)
        outFileName +=  "_" + str( version )

        if  version == 0 :
            tab = np.resize( tab, (len(tab)+1, 2))
            tab[ len( tab )-1 ] = [ outFileName, 0]

        datasetList = ""
        for dataset in inputs[iFile] : datasetList += dataset + ( ',' if dataset != inputs[iFile][-1] else '' )

        print outFileName
        
        commandLine = ( 'prun --exec "RunZee \`echo %IN | sed \'s/,/ /g\'\` --outName Ntuple.root '
                        + ' --doScale ' + str( doScale[iFile] ) + ' --doSmearing ' + str( doScale[iFile] ) 
                        + ' --electronID ' + str( electronID[iFile] )
                        + ' --esModel ' + esModel[iFile]
                        +  ( ( ' --ptCut ' + str( ptCutVect[iFile]*1e3 ) ) if ptCutVect[iFile] != 27 else '' )
                        + '"'
                        + ' --outDS user.cgoudet.' + outFileName + ' --inDS ' + datasetList + ' --outputs Ntuple.root '
                        
                        + ( ' --useRootCore '
                            + '--extFile=lumicalc_histograms_None_200842-215643.root,ilumicalc_histograms_None_13TeV_25ns.root,ilumicalc_histograms_None_13TeV_50ns.root,PileUpReweighting_25ns_prw.root,PileUpReweighting_50ns_prw.root '
                            )
                        )

        result = sub.check_output([commandLine], shell=1, stderr=sub.STDOUT)    
        for line in result.split() :
            if 'jediTaskID' in line : 
                jobIDList.append( [line.split('=')[1], 'user.cgoudet.' + outFileName+'_Ntuple.root' )
    np.savetxt( 'Version.txt', tab, delimiter=' ', fmt='%s')   
    with open( 'GridJobList.txt', 'a') as jobFile :
        for job in jobIDList : jobFile.write( job[0] + ' ' + job[1] + '\n' )





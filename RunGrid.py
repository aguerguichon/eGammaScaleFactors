import os
import sys
import numpy as np
from FunctionsRunGrid import *

mode = 2
inputs = []
options=""
doScale = []
electronID = []
outFilePrefix = []
if  mode == 0 :
    inputs = [ 
#        ['data15_13TeV.00276731.physics_Main.merge.AOD.f620_m1480']
        ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r6765_r6282']
#        ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r6993_r6282/'] 
       # ["mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r6765_r6282/"],
#        ["mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2576_s2132_r6630_r6264_p2395/"]
#        [ "data15_13TeV.periodA.physics_Main.PhysCont.AOD.repro18_v02/", "data15_13TeV.periodC.physics_Main.PhysCont.AOD.repro18_v02/" ]
        ]
    doScale = [  0 ]
    electronID = [ 1 ]
    esModel=[ "es2015PRE" ]

#Do not put usercgoudet nor version
#outDatasetName="MC_8TeV_Zee1Lepton_AOD20"

    outFilePrefix = [
        # "MC_13TeV_Zee", 
        "MC_13TeV_Zee_25ns"
 #       "Data_13TeV_Zee"
#        "Data_13TeV_Zee_276731"
        ]

elif mode ==1 :
    inputs = [
        ["mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2576_s2132_r6630_r6264_p2352/" ],
        ["data15_13TeV.*.physics_Main.merge.DAOD_EGAM6.f594_m1435_p2361/"]        ]
    doScale = [ 0 ]
    outFilePrefix = [ 
        "MC_13TeV_Zee"
        ]


elif mode == 2 :
    GetDataFiles( inputs, 1, doScale, 1, electronID, 3, outFilePrefix)
    esModel = [ "es2015PRE" ]
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
        if ( iDataset ) :
            datasetList += ','
        datasetList += inputs[iFile][iDataset]

    outFileName +=  "_" + str( version )
    print outFileName

    commandLine = 'prun'
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

np.savetxt( 'Version.txt', tab, delimiter=' ', fmt='%s')  




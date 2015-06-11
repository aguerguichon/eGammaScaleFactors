import os
import sys
import numpy as np

inputs = [ 
#    ["data12_8TeV.periodAllYear.physics_Egamma.PhysCont.DAOD_ZEE.repro17_v01/"],
 #   ["mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r6241_r6197/"]
    # ["mc14_8TeV.129685.PowhegPythia8_AU2CT10_Zee_Exactly1LeptonFilter.merge.AOD.e2095_s1933_s1911_r6241_r6197/"],
    # ["data12_8TeV.periodAllYear.physics_Egamma.PhysCont.DAOD_ZEE.repro17_v01/"],
     ["mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r6241_r6197/"]
#     ["mc14_8TeV.129685.PowhegPythia8_AU2CT10_Zee_Exactly1LeptonFilter.merge.AOD.e2095_s1933_s1911_r6241_r6197/"]
    # ["mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2576_s2132_r6630_r6264_p2352/" ],
    # ["data15_13TeV.*.physics_Main.merge.DAOD_EGAM6.f594_m1435_p2361/"],
    # ["mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2576_s2132_r6630_r6264_p2352/" ],
    # ["data15_13TeV.*.physics_Main.merge.DAOD_EGAM6.f594_m1435_p2361/"]
    ]


doScale = [  0]
doSmearing = [0]
options=""

#Do not put usercgoudet nor version
#outDatasetName="MC_8TeV_Zee1Lepton_AOD20"
outFilePrefix = [
#    "Data_8TeV_Zee",
    "MC_8TeV_ZeeDiLepton"
    # "MC_8TeV_Zee1Lepton",
    # "Data_8TeV_Zee_scaled",
    # "MC_8TeV_ZeeDiLepton_smeared",
#     "MC_8TeV_Zee1Lepton_smeared"
    # "MC_13TeV_Zee",
    # "Data_13TeV_Zee",
    # "MC_13TeV_Zee_smeared",
    # "Data_13TeV_Zee_scaled"
]


if len( inputs ) != len( doScale ) or len( inputs ) != len( doSmearing ) or len( inputs ) != len( outFilePrefix ) :
    exit(0)

tab = np.genfromtxt( "Version.txt", dtype='S100', delimiter=' ' )

#=============================
for iFile in range( 0, len( inputs ) ) :
    version=0
    
    for iName in tab :
        if  iName[0] == outFilePrefix[iFile]  :
            iName[1]=str( int(iName[1] ) + 1)
            version=iName[1]

    if  version == 0 :
        tab = np.resize( tab, (len(tab)+1, 2))
        tab[ len( tab )-1 ] = [ outFilePrefix[iFile], 0]

    datasetList = ""
    for iDataset in range( 0, len( inputs[iFile] ) ):
        if ( iDataset ) :
            datasetList += ','
        datasetList += inputs[iFile][iDataset]

    outFileName = outFilePrefix[iFile] + "_" + str( version ) 
    print outFileName
    
    commandLine = 'prun'
    commandLine += ' --exec "RunZee \`echo %IN | sed \'s/,/ /g\'\` --outName Ntuple.root '
    commandLine += ' --doScale ' + str( doScale[iFile] ) + ' --doSmearing ' + str( doSmearing[iFile] ) + '"'
    commandLine += ' --outDS user.cgoudet.' + outFileName
    commandLine += ' --inDS ' + datasetList
    commandLine += ' --outputs Ntuple.root '
    commandLine += ' --useRootCore --extFile=lumicalc_histograms_None_200842-215643.root ' + options

    print commandLine
    os.system( commandLine )

np.savetxt( 'Version.txt', tab, delimiter=' ', fmt='%s')  




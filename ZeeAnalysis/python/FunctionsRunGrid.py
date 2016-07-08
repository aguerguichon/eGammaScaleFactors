import os
from string import *
import numpy as np

#def GetDataFiles( inputs, scale, doScale, eleID, electronID, inFileIndex, outFilePrefix, esModel, ptCut, ptCutVect ) :
def GetDataFiles( inputs, inFileIndex, options, configuration=1  ) :

    path= '/afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/DatasetList/'
    fileList = {}
    fileList['MC_13TeV_Zee_25nsa'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r6765_r6282/', 'es2015PRE', 1, 'PileUpReweighting_25nsa_prw.root' ]
    fileList['MC_13TeV_Zee_25nsb'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2576_s2132_r7267_p2470/', 'es2015PRE', 1, 'PileUpReweighting_25nsb_prw.root' ]
    fileList['MC_13TeV_Zee_25nsb_IBL'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2804_r7585_r7576_p2470/', 'es2015PRE', 1, 'PileUpReweighting_25nsb_prw.root' ]

    fileList['MC_13TeV_Zee_25ns_geo02'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2763_r7359_r6282_p2470/', 'es2015PRE', 1, 'PileUpReweighting_25nsb_prw.root']
    fileList['MC_13TeV_Zee_25ns_geo11'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2764_r7359_r6282_p2470/', 'es2015PRE', 1, 'PileUpReweighting_25nsb_prw.root' ]
    fileList['MC_13TeV_Zee_25ns_geo12'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2765_r7359_r6282_p2470/', 'es2015PRE', 1, 'PileUpReweighting_25nsb_prw.root' ]
    fileList['MC_13TeV_Zee_25ns_geo13'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2766_r7359_r6282_p2470/', 'es2015PRE', 1, 'PileUpReweighting_25nsb_prw.root' ]
    fileList['MC_13TeV_Zee_25ns_geo14'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2767_r7359_r6282_p2470/', 'es2015PRE', 1, 'PileUpReweighting_25nsb_prw.root' ]
    fileList['MC_13TeV_Zee_25ns_geo15'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2768_r7359_r6282_p2470/', 'es2015PRE', 1, 'PileUpReweighting_25nsb_prw.root' ]
    fileList['MC_13TeV_Zee_50ns'] = [ 'mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2576_s2132_r6630_r6264_p2395/', 'es2015PRE', 1, 'PileUpReweighting_50ns_prw.root' ]
    fileList['Data_8TeV_Zee'] = ['data12_8TeV.periodAllYear.physics_Egamma.PhysCont.DAOD_ZEE.repro17_v01/', 'es2012c']
    fileList['MC_8TeV_Zee_DiLepton'] = [ 'mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r6241_r6197/', 'es2012c']
    fileList['MC_8TeV_Zee_1Lepton'] = ['mc14_8TeV.129685.PowhegPythia8_AU2CT10_Zee_Exactly1LeptonFilter.merge.AOD.e2095_s1933_s1911_r6241_r6197/', 'es2012c']
    fileList['MC_13TeV_Ztautau_25nsb'] = ['mc15_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.DAOD_STDM3.e3601_s2576_s2132_r7267_r6282_p2470/', 'es2015PRE', 4.3862, 'PileUpReweighting_Ztautau_prw.root' ]
    fileList['MC_13TeV_Ztautau_25ns'] = ['mc15_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.DAOD_STDM3.e3601_s2726_r7772_r7676_p2616/', 'es2015cPRE', 4.3862, 'PileUpReweighting_Ztautau_prw.root' ]
    fileList['MC_13TeV_Zttbar_25nsb'] = ['mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_STDM2.e3698_s2608_s2183_r7267_r6282_p2482/', 'es2015PRE', 0.237, 'PileUpReweighting_Zttbar_prw.root' ]
    fileList['MC_13TeV_Zttbar_25ns'] = ['mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_STDM2.e3698_s2608_s2183_r7725_r7676_p2596/', 'es2015cPRE', 0.237, 'PileUpReweighting_Zttbar_prw.root' ]

    fileList['MC_13TeV_Zee_25ns'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r7773_r7676/', 'es2015cPRE', 1, 'PileUpReweighting_25nsbc_prw.root' ]

    fileList['MC_13TeV_Zee_2015c'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2576_s2132_r7725_r7676_p2666/', 'ZeeAnalysis/Config/MC2015c.boost' ]
    fileList['MC_13TeV_Zee_2015b'] = ['mc15_13TeV:mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2576_s2132_r7773_r7676_p2666/', 'ZeeAnalysis/Config/MC2015b.boost' ]

    fileList['Data_13TeV_Zee_2016'] = ['data16_13TeV.*.physics_Main.merge.DAOD_EGAM1.*p2667/', 'ZeeAnalysis/Config/Data2016.boost' ]
    fileList['Data_13TeV_Zee_2015'] = ['data15_13TeV.*.physics_Main.merge.DAOD_EGAM1.*p2667/', 'ZeeAnalysis/Config/Data2015.boost' ]

#    if not "esModel" in options : options["esModel"] = fileList[inFileIndex][1];
    options["outName"] = inFileIndex;
    # if len( fileList[inFileIndex] ) > 2 : options['datasetWeight'] = fileList[inFileIndex][2]
    # if len( fileList[inFileIndex] ) > 3 : options['pileupFile'] = fileList[inFileIndex][3]
    if len ( fileList[inFileIndex] ) > 1 : options['configFile'] = fileList[inFileIndex][1]
    if len ( fileList[inFileIndex] ) > 2 : options['extFile'] = fileList[inFileIndex][2]

# electronID doScale pt
# 0 : 1 0  27
# 1 : 1 1  27
# 2 : 1 0  30
# 3 : 1 0  35
# 4 : 1 0  20
# 5 : 2 0  27
# 6 fBrem
# 7 noIso

    minRange=0 if configuration>=0 else -configuration
    for iLaunch in range( minRange, abs(configuration)+1 ) :
        tmpOptions = options.copy()
        tmpOptions['electronID'] = ( 2 if iLaunch==5 else 1 )
        tmpOptions['doScale'] = ( 1 if iLaunch==1 else 0 )
        if iLaunch == 3 : tmpOptions["ptCut"] =  35
        elif iLaunch==2 : tmpOptions["ptCut"] =  30
        elif iLaunch==4 : tmpOptions["ptCut"] =  20
        else : tmpOptions["ptCut"] =  27
        if iLaunch == 6 : tmpOptions['fBremCut'] = 0.7
        if iLaunch == 7 : tmpOptions['doIso']=0
        inputs.append( [[fileList[inFileIndex][0]], tmpOptions] )

def StripName( line, doPrefix = 1, doSuffix = 1 ) :
    if ( line.rfind( '.' ) != -1 and doSuffix ) : 
        line = line[0:line.rfind( '.' )]

    if ( line.rfind( '/' ) != -1 and doPrefix ) :
        line = line[line.rfind( '/' )+1:len( line )]

    return line

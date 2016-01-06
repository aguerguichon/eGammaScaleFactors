import os
from string import *
import numpy as np


def GetDataFiles( inputs, scale, doScale, eleID, electronID, inFileIndex, outFilePrefix, esModel, ptCut, ptCutVect ) :

    path= '/afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/DatasetList/'
    fileList = {}
    fileList['MC_13TeV_Zee_25nsa'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r6765_r6282/', 'es2015PRE']
    fileList['MC_13TeV_Zee_25ns'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2576_s2132_r7267_p2470', 'es2015PRE']
    fileList['MC_13TeV_Zee_25ns_geo02'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2763_r7359_r6282_p2470/', 'es2015PRE' ]
    fileList['MC_13TeV_Zee_25ns_geo11'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2764_r7359_r6282_p2470/', 'es2015PRE' ]
    fileList['MC_13TeV_Zee_25ns_geo12'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2765_r7359_r6282_p2470/', 'es2015PRE' ]
    fileList['MC_13TeV_Zee_25ns_geo13'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2766_r7359_r6282_p2470/', 'es2015PRE' ]
    fileList['MC_13TeV_Zee_25ns_geo14'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2767_r7359_r6282_p2470/', 'es2015PRE' ]
    fileList['MC_13TeV_Zee_25ns_geo15'] = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2768_r7359_r6282_p2470/', 'es2015PRE' ]
    fileList['MC_13TeV_Zee_50ns'] = [ 'mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2576_s2132_r6630_r6264_p2395/', 'es2015PRE']
    fileList['Data_8TeV_Zee'] = ['data12_8TeV.periodAllYear.physics_Egamma.PhysCont.DAOD_ZEE.repro17_v01/', 'es2012c']
    fileList['MC_8TeV_Zee_DiLepton'] = [ 'mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r6241_r6197/', 'es2012c']
    fileList['MC_8TeV_Zee_1Lepton'] = ['mc14_8TeV.129685.PowhegPythia8_AU2CT10_Zee_Exactly1LeptonFilter.merge.AOD.e2095_s1933_s1911_r6241_r6197/', 'es2012c']

    inputs.append( fileList[inFileIndex][0] )
#remove comments and empty lines from datasets
    doScale.append( scale )
    electronID.append( eleID )
    esModel.append( fileList[inFileIndex][1] )
    outFilePrefix.append( inFileIndex )
    ptCutVect.append( ptCut )

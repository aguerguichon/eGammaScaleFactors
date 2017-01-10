import os
import subprocess as sub

def Shorten(inStr):
    inStr= inStr[:inStr.find(".e")]+inStr[inStr.find("_p"):]
    return inStr[:-1]

#2016
#datasetVect = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r7725_r7676/']

#Crock-check new geom Guillaume
datasetVect = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2984_r8585_r7676/']

#2015
#datasetVect = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r7773_r7676/']

# 'mc15_13TeV.361607.PowhegPy8EG_CT10nloME_AZNLOCTEQ6L1_WZqqll_mll20.merge.log.e4711_s2726_r7772_r7676_p2719/', 'mc15_13TeV.361610.PowhegPy8EG_CT10nloME_AZNLOCTEQ6L1_ZZqqll_mqq20mll20.merge.log.e4711_s2726_r7772_r7676_p2719/'


#result = sub.check_output(['ls -d /sps/atlas/c/cgoudet/Calibration/DataxAOD/user.cgoudet.*prw*/'], shell=1, stderr=sub.STDOUT).split('\n')
commandLine=''
#commandLine='asetup AthAnalysisBase,2.4.9,here\n'
commandLine+='\n'.join( [ 'pathena PileupReweighting/generatePRW_jobOptions.py --inDS="' + dataset + '" --outDS="user.aguergui.'+dataset[:-1]+'_prw_0" --tmpDir /tmp' for dataset in datasetVect ] )
os.system( commandLine )


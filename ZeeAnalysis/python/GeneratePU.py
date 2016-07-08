import os
import subprocess as sub

datasetVect = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r7773_r7676/'
           ,'mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r7725_r7676/'
           ]
#result = sub.check_output(['ls -d /sps/atlas/c/cgoudet/Calibration/DataxAOD/user.cgoudet.*prw*/'], shell=1, stderr=sub.STDOUT).split('\n')
commandLine=''
#commandLine='asetup AthAnalysisBase,2.4.9,here\n'
commandLine+='\n'.join( [ 'pathena PileupReweighting/generatePRW_jobOptions.py --inDS="' + dataset + '" --outDS="user.cgoudet.'+dataset[:-1]+'_prw_1" --tmpDir /tmp' for dataset in datasetVect ] )
os.system( commandLine )

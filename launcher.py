import os

commandLine = 'RunZee '
#commandLine += '/sps/atlas/c/cgoudet/Calibration/DataxAOD/mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2576_s2132_r7267_p2470_tid07239521_00/* --outName /sps/atlas/c/cgoudet/Calibration/Test/TestAnalysisNom.root '
commandLine += '/sps/atlas/c/cgoudet/Calibration/DataxAOD/mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2804_r7585_r7576_p2470_tid07678642_00/* --outName /sps/atlas/c/cgoudet/Calibration/Test/TestAnalysisIBL.root '
#commandLine += '/sps/atlas/c/cgoudet/Calibration/DataxAOD/
commandLine += '--esModel es2015PRE '
commandLine += '--pileupFile PileUpReweighting_25nsb_prw.root'
print commandLine
os.system( commandLine )

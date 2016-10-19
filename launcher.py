import os

commandLine = 'RunZee '
#commandLine += '/sps/atlas/c/cgoudet/Calibration/DataxAOD/mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2576_s2132_r7267_p2470_tid07239521_00/* --outName /sps/atlas/c/cgoudet/Calibration/Test/TestAnalysisNom.root '
#commandLine += '/sps/atlas/c/cgoudet/Calibration/DataxAOD/mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2576_s2132_r7773_r7676_p2666_tid08602137_00/* --outName /sps/atlas/c/cgoudet/Calibration/Test/TestAnalysisMC16.root --config /afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/ZeeAnalysis/Config/MC2015b.boost '

#commandLine += '/sps/atlas/c/cgoudet/Calibration/DataxAOD/mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.DAOD_EGAM1.e3601_s2576_s2132_r7725_r7676_p2666_tid08601523_00/* --outName /sps/atlas/c/cgoudet/Calibration/Test/TestAnalysisMC16.root --config /afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/ZeeAnalysis/Config/MC2015c.boost '
#commandLine += '/sps/atlas/c/cgoudet/Calibration/DataxAOD/data16_13TeV.00300863.physics_Main.merge.DAOD_EGAM1.f708_m1606_p2667_tid08654640_00/DAOD_EGAM1.08654640._000121.pool.root.1 --outName /sps/atlas/c/cgoudet/Calibration/Test/TestAnalysisData16.root --config /afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/ZeeAnalysis/Config/Data2016.boost '
#commandLine += ' --pileupFile PileUpReweighting_25nsb_prw.root '

#commandLine+= '/afs/cern.ch/work/k/kgrevtso/public/Calib_files/Data2015/Data/data15_13TeV.00283155.physics_Main.merge.DAOD_EGAM1.r7562_p2521_p2582_tid08024275_00/DAOD_EGAM1.08024275._000006.pool.root.1 --outName /sps/atlas/a/aguerguichon/Calibration/Test/TestCutFlow.root --config /afs/in2p3.fr/home/a/aguergui/public/eGammaScaleFactors/ZeeAnalysis/Config/Data2015.boost'

commandLine+= 'data16_13TeV.00307656.physics_Main.merge.DAOD_EGAM1.f739_m1673_p2785_tid09397978_00/*pool* --outName ./TestAnalysis.root --config /afs/in2p3.fr/home/a/aguergui/public/eGammaScaleFactors/ZeeAnalysis/Config/Data2016.boost'

commandLine+= ' --doScale '
print commandLine
os.system( commandLine )

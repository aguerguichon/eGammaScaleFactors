import os

commandLine = 'RunZee '
#commandLine+= '/afs/cern.ch/work/k/kgrevtso/public/Calib_files/Data2015/Data/data15_13TeV.00283155.physics_Main.merge.DAOD_EGAM1.r7562_p2521_p2582_tid08024275_00/DAOD_EGAM1.08024275._000006.pool.root.1 --outName /sps/atlas/a/aguerguichon/Calibration/Test/TestCutFlow.root --config /afs/in2p3.fr/home/a/aguergui/public/eGammaScaleFactors/ZeeAnalysis/Config/Data2015.boost'

commandLine+= '/sps/atlas/a/aguerguichon/Calibration/Test/data16_13TeV.00311481.physics_Main.merge.DAOD_EGAM1.f758_m1710_p2840_tid09767074_00/DAOD_EGAM1.09767074._000001.pool.root.1 --outName /sps/atlas/a/aguerguichon/Calibration/Test/TestAnalysis.root --config /afs/in2p3.fr/home/a/aguergui/public/eGammaScaleFactors/ZeeAnalysis/Config/Data2016.boost'

commandLine+= ' --electronID 1 --ptCut 27000'

print commandLine
os.system( commandLine )

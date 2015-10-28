import os
import sys
#from Functions_MeasureAlphaSigma import *

action= 1

path='/sps/atlas/c/cgoudet/Calibration/DataxAOD/'
# = os.popen( command ).read().split( '\n' )

toDownload = [
#    'Data_13TeV_Zee_25ns_Lkh1',
#    'Data_13TeV_Zee_25ns_Lkh1_pt30',
#    'Data_13TeV_Zee_25ns_Lkh1_pt35',
#    'Data_13TeV_Zee_25ns_Lkh1_scaled',
    'Data_13TeV_Zee_25ns_Lkh2',
    'Data_13TeV_Zee_25ns_Lkh2_scaled',
    # 'MC_13TeV_Zee_25ns_Lkh1',
    # 'MC_13TeV_Zee_25ns_Lkh1_pt30',
    # 'MC_13TeV_Zee_25ns_Lkh1_pt35',
    # 'MC_13TeV_Zee_25ns_Lkh1_scaled',
    # 'MC_13TeV_Zee_25ns_Lkh2'
    ]

missingFiles=[]

with open( '/afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/ZeeAnalysis/python/Version.txt' ) as versionFile :
    for line in versionFile :
        lineVect = line.split(' ')
        lineVect[1] = lineVect[1].replace('\n', "" )
        if lineVect[0] not in toDownload :
            continue
        os.chdir( path + lineVect[0] )
        dataset = 'user.cgoudet.' + lineVect[0] + '_' + lineVect[1] + '_Ntuple.root'
        if not action :
            nMiss = os.popen( 'rucio download '+ dataset ).read().split()[-1]
            os.system('rm OK' + lineVect[1] )
            if not int(nMiss) :
                os.system('touch OK' + lineVect[1] )
            missingFiles.append( [ dataset, nMiss ] )
        else :
            os.system( 'AddFiles ' + dataset + '/* --outName ' + lineVect[0] + '.root' )

if not action :
    print 'Missing data : '
    for line in missingFiles :
        if line[1] != '0' : 
            print line[0] + ' ' + line[1]

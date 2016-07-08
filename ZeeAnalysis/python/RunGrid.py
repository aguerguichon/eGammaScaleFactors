import os
import sys
import numpy as np
from FunctionsRunGrid import *
import subprocess as sub
from Test import *
inputs = []
doScale = []
electronID = []
esModel = []
ptCutVect = []
outFilePrefix = []

if len( sys.argv ) == 1 :
    print 'An option is needed to run the program. \nPossibilities are : '
    print 'DATA25 to update 25ns datasets'
    print 'DATA50 to update 50ns datasets'
    print 'MC25'

launcherPath = os.popen( 'pwd' ).read().split()[0]+'/'

for option in sys.argv:
    if option == 'RunGrid.py' : continue
    datasetsPath = '/sps/atlas/c/cgoudet/Calibration/DataxAOD/'
    if option in ['DATA25', 'DATA50'] :

#Get the list of all the runs in the grl
        path='/afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/'
        savingFile="DatasetList/Data_13TeV_Zee_" + ( '25' if option == 'DATA25' else '50' ) + "ns.csv"
        datasetList = np.genfromtxt( path + savingFile, dtype='S100', delimiter=' ' )
        runs = []
        grlList=os.popen( 'ls /afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/data15*v75*').read().split()
        for grl  in grlList :
            file = open( grl )
            for line in file :
                if not 'RunList' in line : continue
                runs += line.split( '>' )[1].split('<')[0].split(',')
                
#Check if the latest version of the run dataset belong to the datasetList

        runAll = 0
        runs = sorted( set( runs ), reverse=True)
        print runs
        treeDatasets=node( "/" )        
        for run in runs :

            run = int( run )
            if option == 'DATA25' and ( run < 276262 or run == 276731 ): continue
            if option == 'DATA50' and run > 276261 and run != 276731 : continue
            print run

            line='dq2-ls data15_13TeV.00'+ str( run )+'.physics_Main.merge.DAOD_EGAM1.*p2470/'#*p2582/'
            output = os.popen(line).read().split()
            print line
            for file in output :
                print file
                file = file.split(':')[1]
                treeDatasets.Insert( file.split('.')[-1].split('/')[0].split('_'), file.split('.')[1] )
                pass        
            pass

        datasets=[]
        treeDatasets.CreateTag('')
        treeDatasets.FillDatasets( datasets )
 
        np.savetxt( path + savingFile, datasetList, delimiter=' ', fmt='%s')
        rangeMax = 8

    if 'DATA2016' == option : GetDataFiles( inputs, 'Data_13TeV_Zee_2016', {}, 1 )     
    if 'DATA2015' == option : GetDataFiles( inputs, 'Data_13TeV_Zee_2015', {}, 1 )     
    if 'MC2015c' == option : GetDataFiles( inputs, 'MC_13TeV_Zee_2015c', {}, 1 )     
    if 'MC2015b' == option : GetDataFiles( inputs, 'MC_13TeV_Zee_2015b', {}, 0 )     



    if 'MC25' == option :
        # GetDataFiles( inputs, 'MC_13TeV_Zee_25ns', {'doScale' : 0, 'electronID' : 1} )     
        GetDataFiles( inputs, 'MC_13TeV_Zee_25ns', {'doScale' : 0, 'electronID' : 1, 'fBremCut' : 0.7 } )     
        # GetDataFiles( inputs, 'MC_13TeV_Zee_25ns', {'doScale' : 0, 'electronID' : 2} )     
#        GetDataFiles( inputs, 'MC_13TeV_Zee_25ns', {'doScale' : 1, 'electronID' : 1} )     
        # GetDataFiles( inputs, 'MC_13TeV_Zee_25ns', {'doScale' : 0, 'electronID' : 1, 'ptCut' : 30} )     
        # GetDataFiles( inputs, 'MC_13TeV_Zee_25ns', {'doScale' : 0, 'electronID' : 1, 'ptCut' : 20} )     
        # GetDataFiles( inputs, 'MC_13TeV_Zee_25ns', {'doScale' : 0, 'electronID' : 1, 'ptCut' : 35} )     
        # GetDataFiles( inputs, 'MC_13TeV_Zee_25ns', {'doScale' : 0, 'electronID' : 1, 'scaleSyst' : 1 } )     
        # GetDataFiles( inputs, 'MC_13TeV_Zee_25ns', {'doScale' : 0, 'electronID' : 1, 'scaleSyst' : 2 } )     
        # GetDataFiles( inputs, 'MC_13TeV_Zee_25ns', {'doScale' : 0, 'electronID' : 1, 'scaleSyst' : 3 } )     
        # GetDataFiles( inputs, 'MC_13TeV_Zee_25ns', {'doScale' : 0, 'electronID' : 1, 'doIso' : 0} )     
        # print inputs

    if 'MC25_dis' == option :
        # GetDataFiles( inputs, 0, doScale, 1, electronID, 'MC_13TeV_Zee_25ns_geo02', outFilePrefix, esModel, 27, ptCutVect)
        # GetDataFiles( inputs, 0, doScale, 1, electronID, 'MC_13TeV_Zee_25ns_geo11', outFilePrefix, esModel, 27, ptCutVect)
        # GetDataFiles( inputs, 0, doScale, 1, electronID, 'MC_13TeV_Zee_25ns_geo12', outFilePrefix, esModel, 27, ptCutVect)
        # GetDataFiles( inputs, 0, doScale, 1, electronID, 'MC_13TeV_Zee_25ns_geo13', outFilePrefix, esModel, 27, ptCutVect)
        # GetDataFiles( inputs, 0, doScale, 1, electronID, 'MC_13TeV_Zee_25ns_geo14', outFilePrefix, esModel, 27, ptCutVect)
        # GetDataFiles( inputs, 0, doScale, 1, electronID, 'MC_13TeV_Zee_25ns_geo15', outFilePrefix, esModel, 27, ptCutVect)
        GetDataFiles( inputs, 'MC_13TeV_Zee_25nsb', {'doScale' : 0, 'electronID' : 1} )     
        GetDataFiles( inputs, 'MC_13TeV_Zee_25nsb_IBL', {'doScale' : 0, 'electronID' : 1} )     



    if 'MC50' == option :
        GetDataFiles( inputs, 0, doScale, 2, electronID, 'MC_13TeV_Zee_50ns', outFilePrefix, esModel, 27, ptCutVect)
        GetDataFiles( inputs, 0, doScale, 1, electronID, 'MC_13TeV_Zee_50ns', outFilePrefix, esModel, 27, ptCutVect)

    if 'MC8' == option :
        GetDataFiles( inputs, 0, doScale, 1, electronID, 'MC_8TeV_Zee_1Lepton', outFilePrefix, esModel, 27, ptCutVect)
        GetDataFiles( inputs, 0, doScale, 1, electronID, 'MC_8TeV_Zee_DiLepton', outFilePrefix, esModel, 27, ptCutVect)
        GetDataFiles( inputs, 1, doScale, 1, electronID, 'MC_8TeV_Zee_1Lepton', outFilePrefix, esModel, 27, ptCutVect)
        GetDataFiles( inputs, 1, doScale, 1, electronID, 'MC_8TeV_Zee_DiLepton', outFilePrefix, esModel, 27, ptCutVect)

    if 'DATA8' == option :
        GetDataFiles( inputs, 0, doScale, 1, electronID, 'Data_8TeV_Zee', outFilePrefix, esModel, 27, ptCutVect)
        GetDataFiles( inputs, 1, doScale, 1, electronID, 'Data_8TeV_Zee', outFilePrefix, esModel, 27, ptCutVect)

    if 'BKG' == option : 
#        GetDataFiles( inputs, 'MC_13TeV_Ztautau_25ns', {'doScale' : 0, 'electronID' : 1} )     
#        GetDataFiles( inputs, 'MC_13TeV_Ztautau_25ns', {'doScale' : 1, 'electronID' : 1} )     
        GetDataFiles( inputs, 'MC_13TeV_Zttbar_25ns', {'doScale' : 0, 'electronID' : 1} )     
 #       GetDataFiles( inputs, 'MC_13TeV_Zttbar_25ns', {'doScale' : 1, 'electronID' : 1} )     


    if option=='clean' :

        datasets = os.popen( 'ls ' + datasetsPath ).read().split()
        for file in datasets : 
            if '.root' in file : continue
            if 'MC_' not in file and 'Data_' not in file : continue
            os.chdir( datasetsPath + file )            
            versions = os.popen( 'ls ' ).read().split()
            print file
            versions[:] = [value for value in versions if 'user' in value ]
            versions.sort()
            while len( versions ) > 2 :
                os.popen( 'rm -r ' + versions[0] )
                versions.remove( versions[0] )


##==================================================================
    if option=='download' :
        missingFiles=[]

#Get the list of jobs and datasets from the txt file
        gridJobIDList = np.genfromtxt( 'GridJobList.txt', dtype='S100', delimiter=' ' )
        toRemove=[]
        for job in gridJobIDList : 
            if job[1] == 'Test' : continue #First line is used to be sure to have a 2D vector
            line='http://bigpanda.cern.ch/task/\?jeditaskid\='+ job[0] + '\&display_limit\=200'

#read the web page were the info oof the jobs are stored
            output = os.popen( 'curl '+ line).read().split('\n')
            status=''
            for htmlLine in output :
                if not 'status' in htmlLine : continue
#check if the job is in the 'done' state
                if 'class=\'done_fill\'' in htmlLine : status='done'; break
            print status
#create the name of the repository were the dataset must be downloaded
            directory = job[1].replace( 'user.cgoudet.', '' ).replace('_Ntuple.root', '' )
            directory = directory[:directory.rfind('_')]
            print directory
            os.chdir( datasetsPath + directory )

#download the dataset with rucio and get the number of failed download
            nMiss = os.popen( 'rucio download '+ job[1] ).read().split()[-1]
            os.system('rm OK*')

#if done and missing files print it and do noting 
# if done and no missing file mark the job for removal and print and OK file in the directory
            if status == 'done' :
                if not int(nMiss)  :
                    os.system('touch OK' + job[1].split('.')[2].split('_')[-2] )
                    os.system('rm ' +job[1] + '/*.part' )
                    addFileLine= 'AddFiles ' + job[1] + '/* --outName ' + directory + '.root'
                    print addFileLine
                    os.system( addFileLine )
#create boost file to ease plotting
                    boostFile=open( os.popen( 'pwd' ).read().split()[0]+'/' + directory+'.boost', 'w')
                    rootFiles = os.popen( 'ls ' + directory + '*.root' ).read().split('\n')
                    rootFiles.remove('')
                    rootFiles = [ os.popen( 'pwd' ).read().split()[0]+'/' + f for f in rootFiles ]
                    boostFile.write( 'rootFileName=' + ' '.join( rootFiles ) + '\n' )
                    rootFiles = [ StripName( x ) + '_selectionTree' for x in rootFiles]
                    boostFile.write( 'objName=' + ' '.join( rootFiles ) + '\n' )
                    boostFile.close();
                    toRemove.append( job[0] )
                else :
                    missingFiles.append( [ job[1], nMiss ] )
            elif status == 'broken' :
                toRemove.apend( job[0] )
                pass

        
#cannot delete from gridJobIDList so I have to create a new one and add only interesting bins
            newGridJobList=[]
            for job in gridJobIDList : 
                if job[0] in toRemove : continue
                newGridJobList.append( job )
                
                os.chdir( launcherPath )
                np.savetxt( 'GridJobList.txt', newGridJobList, delimiter=' ', fmt='%s')   

#===================================================
if len( inputs ) :
    os.chdir( '/afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/' )
    jobIDList=[]

    for iFile in range( 0, len( inputs ) ) :
        tab = np.genfromtxt( "/afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/ZeeAnalysis/python/Version.txt", dtype='S100', delimiter=' ' )
        version=0
        
        optionLine=""
        outFileName = inputs[iFile][1]['outName']

        optionLine += ' '.join( [
                '--'+ label + ' ' + inputs[iFile][1][label] 
                for label in ['esModel', 'configFile', 'pileupFile', 'datasetWeight' ]  if label in inputs[iFile][1] ] )

        if  'electronID' in inputs[iFile][1] :
            electronIDTitle = ""
            if ( inputs[iFile][1]['electronID'] / 3 < 1 ) : electronIDTitle += "Lkh"
            else : electronIDTitle += "CB"
            electronIDTitle += str( inputs[iFile][1]['electronID'] % 3 )
            outFileName += "_" + electronIDTitle  
            optionLine += ' --electronID ' + str( inputs[iFile][1]['electronID'] )
            pass
             
        if "doScale" in inputs[iFile][1] : 
            if ( inputs[iFile][1]['doScale'] ) : outFileName += "_scaled"; optionLine += ' --doScale ' 

        if "scaleSyst" in inputs[iFile][1] and inputs[iFile][1]['scaleSyst'] : 
            optionLine += ' --scaleSyst ' + str( inputs[iFile][1]['scaleSyst' ] )
            systName=''
            if inputs[iFile][1]['scaleSyst']==1 : systName = 'iso'
            elif inputs[iFile][1]['scaleSyst']==2 : systName = 'reco'
            elif inputs[iFile][1]['scaleSyst']==3 : systName = 'ID'
            outFileName += '_' + systName + 'Syst'
            pass


        if "ptCut" in inputs[iFile][1] :
            if inputs[iFile][1]['ptCut'] != 27 : outFileName += '_pt' + str( inputs[iFile][1]['ptCut'] )    
            optionLine += ' --ptCut ' + str( inputs[iFile][1]['ptCut']*1000 )
            pass

        if "fBremCut" in inputs[iFile][1] :
            if inputs[iFile][1]['fBremCut'] != 1 : outFileName += '_fBrem' + str( int( inputs[iFile][1]['fBremCut']*100) )    
            optionLine += ' --fBremCut ' + str( inputs[iFile][1]['fBremCut'] )
            pass

        if 'doIso' in inputs[iFile][1] and inputs[iFile][1]['doIso'] != 1 : 
            optionLine += ' --doIso ' + str( inputs[iFile][1]['doIso'] ) 
            outFileName += "_doIso" + str( inputs[iFile][1]['doIso'] ) 

        for iName in tab :
            if  iName[0] == outFileName  : version=str( int(iName[1] ) + 1); iName[1]=str(version)

        if  version == 0 :
            tab = np.resize( tab, (len(tab)+1, 2))
            tab[ len( tab )-1 ] = [ outFileName, 0]
            
        outFileName +=  "_" + str( version )

        extFileLine=''
        if 'configFile' in inputs[iFile][1] :
            configFile = open( inputs[iFile][1]['configFile'], 'r' )
            extFileLine= ','.join( [ line.split('\n')[0].split('=')[1] for line in configFile if 'ilumCalc' in line or 'pileupFile' in line ] )
            configFile.close()
        if extFileLine != '' : extFileLine = '--extFile='+ extFileLine
        print outFileName


        datasetList = ""
        for dataset in inputs[iFile][0] : datasetList += dataset + ( ',' if dataset != inputs[iFile][0][-1] else '' )

#        print outFileName
        
        commandLine = ( 'prun --exec "RunZee \`echo %IN | sed \'s/,/ /g\'\` --outName Ntuple.root '
                        + optionLine 
                        + '"'
                        + ' --outDS user.cgoudet.' + outFileName + ' --inDS ' + datasetList + ' --outputs Ntuple.root '
                        + ( ' --useRootCore '
                            + extFileLine
                            + ' --tmpDir /tmp '
                            )
                        )

#        print( commandLine )
        # exit(0)
        result = sub.check_output([commandLine], shell=1, stderr=sub.STDOUT)    
        jobIDList=[]
        for line in result.split() :
            if 'jediTaskID' in line : 
                jobIDList.append( [line.split('=')[1], 'user.cgoudet.' + outFileName+'_Ntuple.root'] )

        np.savetxt( '/afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/ZeeAnalysis/python/Version.txt', tab, delimiter=' ', fmt='%s')   
        with open( '/afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/ZeeAnalysis/python/GridJobList.txt', 'a') as jobFile :
            for job in jobIDList : jobFile.write( job[0] + ' ' + job[1] + '\n' )





import os
import sys
import subprocess

# 
# 

oldDataset=''
with open( 'dumFile.txt', 'w+') as outFile:
    with open( 'dum.txt', 'r' ) as inFile :
        l=list(inFile)
        for runNumber in l:
            if runNumber == '\n' or runNumber == '':
                continue
            runNumber=runNumber.split()[0]
            line='dq2-ls data15_13TeV.00'+str(runNumber)+'.physics_Main.merge.DAOD_EGAM1.*/'
            print line
            output = subprocess.check_output(line, shell=True).split()
            if str(runNumber)  == '280614' :
                print output
            for dataset in output:
                dataset = dataset.split(':')[1]
                
                if oldDataset == '' : 
                    oldDataset = dataset
                else :
                    if oldDataset.split('.')[1] != dataset.split('.')[1] :
                        outFile.write( oldDataset + '\n' )
                        oldDataset=dataset
                        
                    else :
                        if oldDataset < dataset :
                            oldDataset = dataset
                print oldDataset
        outFile.write( oldDataset + '\n' )


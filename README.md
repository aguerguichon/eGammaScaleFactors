/**

**_In the different mentionned files, be careful to change the paths and names "user.aguergui" (in RunGrid.py especially)_**


How to make a Ntuple campaign
-----------------------------

###1 : Generate MC Pileup File###

- In ZeeAnalysis/python/GeneratePU.py, add the MC AOD dataset(s) corresponding to the DAOD which is used
- Check the latest athena release with the command 
```
  lsetup panda
  showVersions --show=athena
```
- Setup Athena (always use the latest release) in eGammaScaleFactors
```  
  asetup AthAnalysisBase,X.Y.Z,here 
```
- Launch your job
```  
  python GeneratePU.py
```

Once the job is finished, download `rucio download fileName` and merge all the output files `hadd fileName fileName_prw.root`. The merged file will then be the input file of RunZee.


###2 : Running on data###

- In eGammaScaleFactors/ZeeAnalysis/python/FunctionsRunGrid.py:
   * create a new entry in fileList (don't forget to put the '/' at the end of your dataset to work on containers), the shortcut name will be the name of the directory where data will be uploaded once the job is finished: 
```
fileList['yourShortcut']=['yourDataset/', 'yourConfigFile']
```
 
- In eGammaScaleFactors/ZeeAnalysis/python/RunGrid.py:
   * create a new key as: 
```
if yourKey == option: GetDataFiles(inputs, 'yourShortcut', {options}, nOptions)
```  
  -> each GetDataFiles() line corresponds to one job  
  -> more information on the different nOptions in FunctionsRunGrid.py l.70  
  -> if nOptions > 0, all the selections corresponding to 0 -> nOptions are performed  
  -> if nOptions < 0, only the selection corresponding to fabs(nOptions) is performed (baseline is 1)  

   ex: 
   * nOptions= 1: 2 different jobs are launched, the 1st one with electronID=1 doScale=0 ptCut=27 and the 2nd one with electronID=1 doScale=1 ptCut=27  
   * nOptions=-1: only one job is launched with electronID=1 doScale=1 ptCut=27

- In eGammaScaleFactors/ZeeAnalysis/Config/:
   * create the config file with:  
**for MC**: esModel, trigger, dataPUSF, pileupFile, ilumCalc  
**for data**: esModel, trigger, dataPUSF, grl  
**_NB_**: dataPUSF=1.09 (converted into 1/1.09 in the Analysis.cxx code)

- Setup: 
```
rcSetup 
lsetup panda
lsetup rucio
python RunGrid.py yourKey
```
  
NB: GridJobList.txt is a list of all jobs which output files haven't been downloaded yet

###Reading datasets from a csv file - creating csv file with datasets###

- It it possible to create a csv file with the dataset corresponding to each run number available in a given grl.  
For that, add the `makecsv` in your line command (works only for DATA15 and DATA16 keys for now):    
```
python RunGrid.py makecsv DATA15 DATA16
```  
In FunctionsRunGrid.py, instead of writing the full name of the datasets in the fileList map, just indicate the `list` key as follows:  
```
fileList['yourShortcut']=['list', 'yourConfigFile']
```
- To read datasets from a csv file, just put the name (with absolute path) of your csv file in the fileList map of FunctionsRunGrid.py
```
fileList['yourShortcut']=['yourcsvFile', 'yourConfigFile']
```


###3 : Downloading output files###

```
python RunGrid.py download
```
- This option of RunGrid.py downloads the output files, merge them into one or several (if there are a lot of events) root files and updates the GridJobList.txt file. It allows to keep track of the different versions
- It is also possible to just manually download output files with 'rucio download' and merge files with 'hadd'


Customize the selection and the variables
-------------------------------------------

###Adding variables to be stored in the ntuple###


Knowing what information is stored in the DAOD:
https://twiki.cern.ch/twiki/bin/viewauth/AtlasComputing/SoftwareTutorialxAODAnalysisInROOT#Knowing_what_information_is_in_t

To add variables, you have to modify Analysis::FillSelectionTree() in ZeeAnalysis/Root/Analysis.cxx by adding a new line:
```
m_mapVar['name you want to give to your variable in the tree']= 'the variable'
```

###Adding cuts and selection steps###

In `Analysis::PassSelection()` are the cuts and selection at the event level.  
In `Analysis::MakeElectronCut()` are the cuts and selection at the electron level.  

To include the new selection in the cut flow: 
- add the key name of the selection in the vector cutFlowNames in the Analysis constructor
- add the following line after your selection:
```
m_mapHist["cutFlow"]->Fill('yourKey')
```


Tools
-----

Tools are declared as attributes of the Analysis class. They are initialized in Analysis::InitializeTools()

*/
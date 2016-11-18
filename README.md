/**

\mainpage


# eGammaScaleFactors

**_In the different mentionned files, be careful to change the paths and names "user.aguergui" (in RunGrid.py and GeneratePU.py especially)_**  

**An example is given at each step for 2015 data. It will be wrapped within a block quote as follows:**

> Here is the example.  


___


eGammaScaleFactors is a package designed in the ROOT Analysis framework allowing to:
- get the PileUp-reweighting files from a MC AOD
- run on DxAOD and get a smaller Ntuple to work with  

After running on the CERN GRID, the output Ntuple basically contains:
- Z->ee kinematic information
- electons fBrem
- gain information


Download and setup
------------------

Download the eGammaScaleFactors package and setup RootCore.
```
git clone git@github.com:aguerguichon/eGammaScaleFactors.git  
cd eGammaScaleFactors
rcSetup -r //to check the latest version of RootCore available  
rcSetup Base,2.4.X  
rc find_packages  
rc compile  
```


How to make a Ntuple campaign
-----------------------------

###1 : Generate MC Pileup File###

- In ZeeAnalysis/python/GeneratePU.py, add the MC AOD dataset(s) corresponding to the DAOD which is used

>      datasetVect = ['mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r7725_r7676/']

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

>      cd /sps/atlas/a/aguerguichon/Calibration/DataxAOD/  
>      rucio download user.aguergui.mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r7725_r7676_prw_0_METADATA/  
>      cd ~/public/eGammaScaleFactors/  
>      hadd mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r7725_r7676_prw.root  user.aguergui.mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r7725_r7676_prw_0_METADATA/  

###2 : Running on data###

- In eGammaScaleFactors/ZeeAnalysis/python/FunctionsRunGrid.py:
   * create a new entry in fileList (don't forget to put the '/' at the end of your dataset to work on containers), the shortcut name will be the name of the directory where data will be uploaded once the job is finished: 
```
fileList['yourShortcut']=['yourDataset/', 'yourConfigFile']  
```
 
>      fileList['Data15_13TeV_Zee'] = ['data15_13TeV.*.physics_Main.merge.DAOD_EGAM1.r7562_p2521_p2667/', 'ZeeAnalysis/Config/Data2015.boost' ]   

- In eGammaScaleFactors/ZeeAnalysis/python/RunGrid.py:
   * create a new key as: 
```
if yourKey == option: GetDataFiles(inputs, 'yourShortcut', {options}, mode)
```  

>     if 'DATA15' == option : GetDataFiles( inputs, 'Data15_13TeV_Zee', {}, 1 )  

  -> each GetDataFiles() line corresponds to one job  
  -> more information on the different modes in FunctionsRunGrid.py l.70  
  -> if mode > 0, all the selections corresponding to 0 -> mode are performed  
  -> if mode < 0, only the selection corresponding to fabs(mode) is performed (baseline is mode=1)  

> mode= 1: 2 different jobs are launched, the 1st one with electronID=1 doScale=0 ptCut=27 and the 2nd one with electronID=1 doScale=1 ptCut=27    
> mode=-1: only one job is launched with electronID=1 doScale=1 ptCut=27  

- In eGammaScaleFactors/ZeeAnalysis/Config/:
   * create the config file with:  
**for MC**: esModel, trigger, dataPUSF, pileupFile, ilumCalc  
**for data**: esModel, trigger, dataPUSF, grl  
**_NB_**: dataPUSF=1.09 (converted into 1/1.09 in the Analysis.cxx code)


> **ZeeAnalysis/Config/Data2015.boost example:**  
  
>      esModel=es2015c_summer  
>      trigger=HLT_2e17_lhvloose.*  
>      dataPUSF=1.09  
>      grl=data15_13TeV.periodAllYear_DetStatus-v75-repro20-01_DQDefects-00-02-02_PHYS_StandardGRL_All_Good_25ns.xml  

> **ZeeAnalysis/Config/MC2015c.boost example:**  

>      esModel=es2016PRE  
>      trigger=HLT_2e17_lhvloose_nod0.*  
>      dataPUSF=1.09  
>      pileupFile=mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r7725_r7676_prw.root  
>      ilumCalc=ilumicalc_histograms_None_297730-311481_OflLumi-13TeV-005.root   


- Setup: 
```
rcSetup 
lsetup panda
lsetup rucio
python RunGrid.py yourKeys
```
  
NB: GridJobList.txt is a list of all jobs which output files haven't been downloaded yet

###Testing the code before running on the grid###
If you want to test the Analysis code before running on the entire dataset on, you can do so in interactive mode. For that, you just need to run the `eGammaScaleFactors/launcher.py` macro with a small file. In this macro, it is possible to test different options, config files... This should take only a few minutes.

>     commandLine+= '/sps/atlas/a/aguerguichon/Calibration/Test/data16_13TeV.00311481.physics_Main.merge.DAOD_EGAM1.f758_m1710_p2840_tid09767074_00/DAOD_EGAM1.09767074._000001.pool.root.1 --outName /sps/atlas/a/aguerguichon/Calibration/Test/TestAnalysis.root --config /afs/in2p3.fr/home/a/aguergui/public/eGammaScaleFactors/ZeeAnalysis/Config/Data2016.boost'  

>     commandLine+= ' --electronID 1 --ptCut 27000'  



###Reading datasets from a csv file - creating csv file with datasets###

- It it possible to create a csv file with the dataset corresponding to each run number available in a given grl.  
For that, add the `makecsv` in your line command (works only for DATA15 and DATA16 keys for now):    

>     python RunGrid.py makecsv DATA15 DATA16

In FunctionsRunGrid.py, instead of writing the full name of the datasets in the fileList map, just indicate the `list` key.  

>     fileList['Data15_13TeV_Zee']=['list', 'ZeeAnalysis/Config/Data2015.boost']

- To read datasets from a csv file, just put the name (with absolute path) of your csv file in the fileList map of FunctionsRunGrid.py

>     fileList['Data15_13TeV_Zee'] = ['/afs/in2p3.fr/home/a/aguergui/public/eGammaScaleFactors/DatasetList/Data15_13TeV_Zee.csv', 'ZeeAnalysis/Config/Data2015.boost' ]


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

To add variables, you have to modify Analysis::FillSelectionTree() in ZeeAnalysis/Root/Analysis.cxx by adding a new line

>     m_mapVar[string(TString::Format( "pt_%d", iEl+1 ))] = m_veGood[iEl]->pt();  

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
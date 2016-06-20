How to make a Ntuple campaign
====================================

1 : Generate MC Pileup File 
isolate the MC AOD dataset which correspond to the DAOD which is used.
Check the latest athena release with the command 'showVersions --show=athena'
-
asetup AthAnalysisBase,X.Y.Z,here (always use the latest release)
pathena PileupReweighting/generatePRW_jobOptions.py --inDS="<datasetName>" --outDS="<outFileName>" --tmpDir /tmp
-
Once finished, merge all the output files. The merged file will then be the input file of RunZee


2 : Data

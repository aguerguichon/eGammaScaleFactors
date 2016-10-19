import csv

inFile=open("WeightEW2016.csv", "rb")
reader=csv.reader( inFile, delimiter=' ')

#row[0]=Sample
#row[1]=Dataset
#row[6]=datasetWeight

for row in reader:
	if row[0]!="Sample":
		configFile=open(row[0]+"2015c.boost", "w")
		configFile.write("esModel=es2016PRE \ntrigger=HLT_2e17_lhvloose.* \ndataPUSF=0.8621 \nilumCalc=ilumicalc_histograms_None_297730-300908.root \n")
		configFile.write("pileupFile="+row[1]+" \ndatasetWeight="+row[6]+ "\n")
		configFile.close()

inFile.close()


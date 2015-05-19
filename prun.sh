input=mc14_8TeV.129685.PowhegPythia8_AU2CT10_Zee_Exactly1LeptonFilter.merge.AOD.e2095_s1933_s1911_r6241_r6197/
#input=mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r6241_r6197/
#input=data12_8TeV.periodAllYear.physics_Egamma.PhysCont.DAOD_ZEE.repro17_v01/
#input=mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.AOD.e3601_s2576_s2132_r6630_r6264/
outFileName=Ntuple.root
version=9

doScale=0
doSmearing=0
options=""
#Do not put usercgoudet nor version
outDatasetName="MC_8TeV_Zee1Lepton_Tight_AOD20"
#outDatasetName="MC_8TeV_ZeeDiLepton_AOD20"
#outDatasetName="Data_8TeV_Zee_DAOD20"
outDatasetName="MC_13TeV_Zee_AOD"


#=============================
if [[ ${outDatasetName} == "" ]];
then
    if [[ ${input} == data12* ]];
    then 
	outDatasetName="Data"
    else
	outDatasetName="MC"
    fi
    outDatasetName="${outDatasetName}_8TeV"

    if [[ ${input} == mc*DAOD* ]];
    then 
	outDatasetName="${outDatasetName}_DZee"
    else 
	outDatasetName="${outDatasetName}_Zee"
    fi

    if [[ ${doScale} != 0 ]];
    then
	outDatasetName="${outDatasetName}_scale"
    fi
    
    if [[ ${doSmearing} != 0 ]];
    then
	outDatasetName="${outDatasetName}_smearing"
    fi
fi

echo ${outDatasetName}

prun  \
--exec "RunZee \`echo %IN | sed 's/,/ /g'\` --outName ${outFileName} --doScale ${doScale} --doSmearing ${doSmearing}"  \
--outDS user.cgoudet.${outDatasetName}_${version}/ \
--inDS ${input} \
--outputs ${outFileName} \
--useRootCore \
--extFile=lumicalc_histograms_None_200842-215643.root \
${options}


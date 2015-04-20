#input=data12_8TeV.periodB.physics_Egamma.PhysCont.AOD.repro16_v05/
input=data12_8TeV.periodAllYear.physics_Egamma.PhysCont.DAOD_ZEE.repro17_v01/

outFileName=Ntuple.root
version=2
doScale=0
doSmearing=0
options=""
#Do not put usercgoudet nor version
outDatasetName="data_8TeV_Zee_v05"
#MC_8TeV_DZee"
#data_8TeV_DZee_rel20"


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
${options}


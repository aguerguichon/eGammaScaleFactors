input=data12_8TeV.periodB.physics_Egamma.PhysCont.AOD.repro17_v01/
outFileName=Ntuple.root
version=1

doScale=0
doSmearing=0
options=""
#Do not put usercgoudet nor version
outDatasetName="Data_8TeV_Zee_AOD20"



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


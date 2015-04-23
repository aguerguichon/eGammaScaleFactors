input=mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r6241_r6197/
outFileName=Ntuple.root
version=3

doScale=0
doSmearing=0
options=""
#Do not put usercgoudet nor version
outDatasetName="Data_8TeV_Zee_Dilepton_Rel20"



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
--inDS `cat lineFile.txt` \
--outputs ${outFileName} \
--useRootCore \
${options}

#--inDS ${input} \
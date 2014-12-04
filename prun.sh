input=data12_8TeV.periodB.physics_Egamma.PhysCont.AOD.repro16_v01/ 
#input=mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r5591_r5625/ 

outFileName=ZeeNtuple.root
version=5
doScale=1
doSmearing=1
options=""

outDataSetName=""

#=============================
if [[ ${outDataSetName} == "" ]];
then
    if [[ ${input} == data12* ]];
    then 
	outDatasetName="Data"
    else
	outDatasetName="MC"
    fi
    outDatasetName="${outDatasetName}_8TeV_Zee"

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


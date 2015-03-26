input=data12_8TeV.periodB.physics_Egamma.PhysCont.AOD.repro16_v05/
#input=data12_8TeV.00204158.physics_Egamma.merge.DAOD_ZEE.r4065_p1346_r6469_p2298/
<<<<<<< HEAD
#input=data12_8TeV.periodB.physics_Egamma.PhysCont.AOD.repro16_v05/
#input=mc14_8TeV.147806.PowhegPythia8_AU2CT10_Zee.merge.AOD.e1169_s1896_s1912_r5591_r5625/
=======
input=mc14_8TeV.147806.PowhegPythia8_AU2CT10_Zee.merge.AOD.e1169_s1896_s1912_r5591_r5625/
>>>>>>> Rel19
#input=mc14_8TeV.147806.PowhegPythia8_AU2CT10_Zee.merge.DAOD_EGAM1.e1169_s1896_s1912_r5591_r5625_p1796/
#input=data12_8TeV.periodAllYear.physics_Egamma.PhysCont.DAOD_ZEE.repro16_v02

outFileName=ZeeNtuple.root
version=20
doScale=0
doSmearing=0
options=""
#Do not put usercgoudet nor version
outDatasetName=""
#MC_8TeV_DZee"
#data_8TeV_DZee_rel20"


#=============================
if [[ ${outDatasetName} == "" ]];
then
    echo inloop
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


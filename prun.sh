#input=data12_8TeV.periodB.physics_Egamma.PhysCont.AOD.repro16_v05/
input=data12_8TeV.00204158.physics_Egamma.merge.DAOD_ZEE.r4065_p1346_r6469_p2298/
#input=mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r5591_r5625/ 
#input=mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.DAOD_EGAM1.e1861_s1933_s1911_r5591_r5625_p1796/
#input=mc14_8TeV.147806.PowhegPythia8_AU2CT10_Zee.merge.AOD.e1169_s1896_s1912_r5591_r5625/
#input=mc14_8TeV.147806.PowhegPythia8_AU2CT10_Zee.merge.DAOD_EGAM1.e1169_s1896_s1912_r5591_r5625_p1796/

outFileName=ZeeNtuple.root
version=2
doScale=0
doSmearing=0
options=""
#Do not put usercgoudet nor version
outDatasetName="data_8TeV_DZee_rel20"
#MC_8TeV_DZee"

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


#input=data12_8TeV.periodB.physics_Egamma.PhysCont.AOD.repro16_v01/ 
input=mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r5591_r5625/ \

prun  \
--exec "RunZee \`echo %IN | sed 's/,/ /g'\` --outName ZeeMC.root"  \
--outDS user.cgoudet.MC_TestCalib_0/ \
--inDS ${input} \
--outputs ZeeMC.root \
--nFiles=200 \
--useRootCore


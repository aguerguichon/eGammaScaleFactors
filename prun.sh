prun  \
--exec "RunZee \`echo %IN | sed 's/,/ /g'\` --outName ZeeMC.root"  \
--outDS user.cgoudet.MC_8TeV_Zee/ \
--inDS mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r5591_r5625/ \
--outputs ZeeMC.root \
--useRootCore
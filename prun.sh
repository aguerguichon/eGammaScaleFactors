prun  \
--exec "RunZee `echo %IN | sed 's/,/ /g'` --outName TestOneFile.root"  \
--outDS user.cgoudet.testRunZee12 \
--inDS mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r5591_r5625/ \
--outputs TestOneFile.root \
--nFiles 1 \
--useRootCore
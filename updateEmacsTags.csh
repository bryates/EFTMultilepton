set tagFile = ${CMSSW_BASE}/src/EFTMultilepton/TemplateMakers/bean.tags 

ctags -e -R -f $tagFile ${CMSSW_BASE}/src/EFTMultilepton/
ctags -e -R -a -f $tagFile  ${CMSSW_BASE}/src/BEAN

echo "Done updating $tagFile"

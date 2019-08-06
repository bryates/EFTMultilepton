#!/bin/sh

cat <<EOF
========================================================================
This script is creating a working directory for ntuplizing and plotting
the EFT multilepton analysis in 94X
Output is in setup.log
========================================================================
EOF

(
   set -e
   set -o xtrace

   export SCRAM_ARCH=slc7_amd64_gcc630
   scramv1 project CMSSW CMSSW_9_4_6
   cd CMSSW_9_4_6/src
   set +o xtrace
   eval $(scramv1 runtime -sh)
   set -o xtrace

   git cms-init > /dev/null

   git git@github.com:cms-govner/MiniAOD.git
   git git@github.com:cms-govner/ttH-LeptonID.git ttH/LeptonID
   git git@github.com:cms-govner/EFTMultilepton.git
   echo "NOTE: If setup.sh is ever updated to CMSSW_9_4_13 or higher, the following command can be omitted."
   cp -r /afs/crc.nd.edu/user/g/gsmith15/Public/additional_code_for_EFT/L1Prefiring . ## Note: if not running this on earth, change to scp -r

   scram b -j 8

) > setup.log


cat <<EOF
========================================================================
Output is in setup.log
========================================================================
EOF

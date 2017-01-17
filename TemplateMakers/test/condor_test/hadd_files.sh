#!/bin/bash
#usage ./hadd_files.sh <prefix>
#      ./hadd_files.sh tth_powheg_old

prefix=$1

target_dir="/scratch365/cmuelle2/extraction_trees/jan15_ICHEP_trees_withFactorizedRecoBdt_fastPerm_yesHiggsLoop_genFilterWeights/";

files="$target_dir/$prefix*.root" 

#postfix="_genFilterTraining_2lss.root"
postfix="_2lss_extraction.root"
#postfix="_2lss_selection.root"
#postfix="_2lss_trainingSelection.root"
output="$target_dir/../$prefix$postfix"

export SCRAM_ARCH=slc6_amd64_gcc530
eval `scramv1 runtime -sh`

if  [ $prefix ]
then
    hadd $output $files
    if [ "$?" = "0" ]
    then
	rm -rf $files
	mv $output "$target_dir/"
    else
	rm -rf $output
	echo "$files not found"
	exit 1
    fi
else
    echo "$prefix is not a valid argument"
fi
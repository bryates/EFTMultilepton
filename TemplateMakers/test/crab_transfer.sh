#!/bin/sh

# Script for use on lxplus to transfer output of completed crab jobs to ndpc disk. Need to source crab environment first.
# usage: ./crab_transfer.sh

user="gesmith"                                      # your user name
ndpc="ndpc5"                                        # the ndpc you are saving to
disk="disk1"                                        # disk on the ndpc
outputSubDir="crab/ttH_test_1_11_17"               # subdirectory to save output to on ndpc disk. crab will create these if they don't exist. Note: this is the end of the path; i.e. crab will not create any other sub-directories).
crabDir="crab_project_dir/crab_ttH_test_1_11_17"   # crab project directory for these jobs


## commands - only edit if you know what you are doing ##
whatToMount="$ndpc$disk/$user"
mkdir -p $whatToMount
sshfs -o cache=yes -o kernel_cache -o compression=no -o large_read -o Ciphers=arcfour $user@$ndpc:/mnt/$whatToMount/ $whatToMount
crab getoutput -d $crabDir --outputpath=$whatToMount/$outputSubDir/
fusermount -u $whatToMount

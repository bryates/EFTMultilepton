#! /bin/tcsh

#root -b -q 'wrapper_lobster.C+("tllq_multidim","inputfiles__tllq_multidim.txt")'
#root -b -q 'wrapper_lobster.C+("tllq_multidim","inputfiles__TEST_tllq_multidim.txt","TEST_tllq_output.root")'
root -b -q 'wrapper_lobster.C+("ttH_multidim","inputfiles__TEST_ttH_multidim.txt","TEST_ttH_output.root")'
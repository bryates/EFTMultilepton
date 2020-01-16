import subprocess
import datetime
import sys
import os

from EFTMultilepton.TemplateMakers.utils import get_files,move_files,clean_dir
from make_html import make_html

# Python wrapper to the check_anaTrees script when trying to run by-hand (instead of via lobster)

# Alias function
pjoin = os.path.join

USER_DIR = os.path.expanduser('~')
CMSSW_DIR = os.environ['CMSSW_BASE']

TIMESTAMP = datetime.datetime.now().strftime('%Y-%m-%d_%H%M')
TIMESTAMP2 = datetime.datetime.now().strftime('%Y_%m_%d')
HPATH = '/hadoop/store/user'
SFILES = os.path.expandvars('/store/smallfiles/$USER/anatest_files')

fpath_a15 = pjoin(SFILES,"TOP-19-001_unblinded_v1.root")
fpath_a16 = pjoin(SFILES,"anatest16.root")
fpath_a17 = pjoin(SFILES,"anatest17.root")
fpath_a18 = pjoin(SFILES,"anatest18.root")
fpath_a19 = pjoin(SFILES,"anatest19.root")
fpath_a20 = pjoin(SFILES,"anatest20.root")
fpath_a21 = pjoin(SFILES,"anatest21.root")
fpath_a22 = pjoin(SFILES,"anatest22.root")
fpath_a23 = pjoin(SFILES,"anatest23_v3.root")
fpath_a24 = pjoin(SFILES,"anatest24.root")
fpath_a25 = pjoin(SFILES,"anatest25.root")
fpath_a26 = pjoin(SFILES,"anatest26.root")
fpath_a27 = pjoin(SFILES,"anatest27.root")
fpath_a28 = pjoin(SFILES,"anatest28.root")
fpath_a29 = pjoin(SFILES,"anatest29.root")
fpath_a30 = pjoin(SFILES,"anatest30.root")
fpath_a31 = pjoin(SFILES,"anatest31.root")
fpath_a32 = pjoin(SFILES,"anatest32.root")

fpath_R6B1 = pjoin(SFILES,"private_ttH-ttZ-ttW_HanV4_R6B1Only.root")
fpath_R6B1_NSH = pjoin(SFILES,"private_ttH-ttZ-ttW_HanV4_R6B1Only_NSH.root")
fpath_R6e2_NSH = pjoin(SFILES,"private_tZq_HanV4_R6-extra2Only_NSH.root")

fpath_tllq_R6B1 = pjoin(SFILES,"private_sgnl_tllq_R6B1.root")
fpath_tllq_R6B2 = pjoin(SFILES,"private_sgnl_tllq_R6B2.root")
fpath_tllq_R6B3 = pjoin(SFILES,"private_sgnl_tllq_R6B3.root")

fpath_a28_redoFull = pjoin(SFILES,"anatest28_redoFullWF-NoStreaming.root")
fpath_a29_redoFull = pjoin(SFILES,"private_sgnl_redoTrees-a29_NoStreaming.root")
fpath_a29_redoFull_v2 = pjoin(SFILES,"private_sgnl_redoTrees-a29_NoStreaming_v2.root")
fpath_a29_noDupes = pjoin(SFILES,"anatest29_NoDuplicates.root")
fpath_a29_noDupes_v2 = pjoin(SFILES,"anatest29_NoDuplicatesV2.root")

fpath_a28_fixedErrors = pjoin(SFILES,"anatest28_fixedErrors.root")
fpath_a31_fixedErrors = pjoin(SFILES,"anatest31_fixedErrors.root")

fpath_a32_copyChanges = pjoin(SFILES,"anatest32_copyChanges.root")

fpath_a32_tllq_SM_Q2RF_check = pjoin(SFILES,"anatest32_tllq_SM_Q2RF_check.root")

fpath_HanV4SMChecks = pjoin(SFILES,"private_sgnl_HanV4_SMCheck.root")
fpath_HanOrigSMChecks = pjoin(SFILES,"private_sgnl_HanOriginal_SMCheck.root")

class Sample(object):
    def __init__(self,name,xsec,dirs=[]):
        self.__name = name
        self.__xsec = xsec      # Should come from the rateinfo.h file in MakeGoodPlot2
        self.__dirs = []
        for d in dirs: self.addDirectory(d)

    def name(self):
        return self.__name

    def xsec(self):
        return self.__xsec

    def list(self):
        return self.__dirs

    def addDirectory(self,*args):
        if len(args) == 0:
            return
        elif len(args) == 1:
            d = args[0]
        else:
            d = pjoin(*args)
        if d in self.list():
            return
        self.__dirs.append(d)

# Pipes subprocess messages to STDOUT
def run_process(inputs,verbose=True,indent=0):
    # Note: This will hold the main thread and wait for the subprocess to complete
    indent_str = "\t"*indent
    p = subprocess.Popen(inputs,stdout=subprocess.PIPE)
    stdout = []
    while True:
        l = p.stdout.readline()
        if l == '' and p.poll() is not None:
            break
        if l:
            stdout.append(l.rstrip())
            if verbose: print indent_str+l.rstrip()
    return stdout

def getFiles(tdir,ext=''):
    if not os.path.exists(tdir): return []
    lst = []
    for fn in sorted(os.listdir(tdir)):
        fpath = pjoin(tdir,fn)
        if not os.path.isfile(fpath):
            continue
        base,tail = fn.rsplit('.',1)
        if ext and tail != ext:
            continue
        lst.append(fpath)
    return lst

# Wrapper to run the 'check_anaTrees.C' macro interactively
def check_anaTrees():
    print "Running check_anaTrees()..."
    max_files = 10
    events = 500#75000
    skim = False
    testing = False

    # label_suffix = "v14-v1_Full"
    label_suffix = ""

    spath = 'awightma/analysisTrees'
    tllq4f_JetMax1 = Sample('tllq4f_JetMax1',xsec=0.0942)
    tllq4f_JetMax2 = Sample('tllq4f_JetMax2',xsec=0.0942)
    tllq4f_JetMax2_NSH = Sample('tllq4f_JetMax2_NoSkipHiggs',xsec=0.0942)

    tllq4f_0p_NoMerge = Sample('tllq4f_0p_NoMerge',xsec=0.0942)
    tllq4f_0p_JetMax1 = Sample('tllq4f_0p_JetMax1',xsec=0.0942)
    tllq4f_0p_JetMax2 = Sample('tllq4f_0p_JetMax2',xsec=0.0942)

    central_tZq_NSH = Sample('central_tZq_NoSkipHiggs',xsec=0.0942)
    central_tZq = Sample('central_tZq',xsec=0.0942)
    central_ttH = Sample('central_ttH',xsec=0.2151)

    tllq4f_JetMax1.addDirectory(HPATH,spath,'special/tllq4f_EFT_t-channelJets_2019_08_07/v1/tllq_multidim_b1')
    tllq4f_JetMax1.addDirectory(HPATH,spath,'special/tllq4f_EFT_t-channelJets_2019_08_07/v1/tllq_multidim_b2')

    # tllq4f_JetMax2.addDirectory(HPATH,spath,'special/tllq4f_EFT_t-channel_01j_2019_09_04_JetMax2/v1/tllq_multidim')
    tllq4f_JetMax2.addDirectory(HPATH,spath,'special/tllq4f_EFT_t-channel_01j_2019_09_20_JetMax2/v1/tllq_multidim')

    tllq4f_JetMax2_NSH.addDirectory(HPATH,spath,'special/tllq4f_EFT_t-channel_01j_2019_09_20_JetMax2_NoSkipHiggs/v1/tllq_multidim')

    tllq4f_0p_NoMerge.addDirectory(HPATH,spath,'special/tllq4f_EFT_t-channelNoJets_2019_10_01_HadFilterCheck/v1/tllq_multidim_NoMerge')
    tllq4f_0p_JetMax1.addDirectory(HPATH,spath,'special/tllq4f_EFT_t-channelNoJets_2019_10_01_HadFilterCheck/v1/tllq_multidim_JetMax1')
    tllq4f_0p_JetMax2.addDirectory(HPATH,spath,'special/tllq4f_EFT_t-channelNoJets_2019_10_01_HadFilterCheck/v1/tllq_multidim_JetMax2')

    # central_tZq_NSH.addDirectory(HPATH,spath,'special/central_tZq_2019_09_23_v14-v2_NoSkipHiggs/v1/tZq')
    central_tZq_NSH.addDirectory(HPATH,spath,'special/central_tZq_2019_09_23_v14-v1_NoSkipHiggs/v1/tZq')

    # central_tZq.addDirectory(HPATH,spath,'special/central_tZq_2019_09_20_v1/v1/tZq')
    central_tZq.addDirectory(HPATH,spath,'central_sgnl_2019_07_31/v1/tZq')

    central_ttH.addDirectory(HPATH,spath,'central_sgnl_2019_07_31/v1/ttH')

    ###################
    # anatest24 samples
    ###################
    a24_private_ttH   = Sample('private_ttH',xsec=0.2151)
    a24_private_tHq   = Sample('private_tHq',xsec=7.7129e-2)
    a24_private_tllq  = Sample('private_tllq',xsec=0.0942)
    a24_private_ttll  = Sample('private_ttll',xsec=0.2529)
    a24_private_ttlnu = Sample('private_ttlnu',xsec=0.2043)

    a24_private_ttH.addDirectory(HPATH,spath,'private_sgnl_2019_10_09/v1/ttH_multidim_b1')
    a24_private_tHq.addDirectory(HPATH,spath,'private_sgnl_2019_10_09/v1/tHq_multidim_b1')
    a24_private_tllq.addDirectory(HPATH,spath,'private_sgnl_2019_10_09/v1/tllq_multidim_b1')
    a24_private_tllq.addDirectory(HPATH,spath,'private_sgnl_2019_10_09/v1/tllq_multidim_b2')
    a24_private_ttll.addDirectory(HPATH,spath,'private_sgnl_2019_10_09/v1/ttll_multidim_b1')
    a24_private_ttll.addDirectory(HPATH,spath,'private_sgnl_2019_10_09/v1/ttll_multidim_b2')
    a24_private_ttlnu.addDirectory(HPATH,spath,'private_sgnl_2019_10_09/v1/ttlnu_multidim_b1')

    ###################
    # anatest25 samples
    ###################
    a25_private_ttH   = Sample('private_ttH',xsec=0.2151)
    a25_private_tHq   = Sample('private_tHq',xsec=7.7129e-2)
    a25_private_tllq  = Sample('private_tllq',xsec=0.0942)
    a25_private_ttll  = Sample('private_ttll',xsec=0.2529)
    a25_private_ttlnu = Sample('private_ttlnu',xsec=0.2043)
    
    a25_private_ttH.addDirectory(HPATH,spath,'private_sgnl_2019_10_11/v1/ttH_multidim_b1')
    a25_private_tHq.addDirectory(HPATH,spath,'private_sgnl_2019_10_11/v1/tHq_multidim_b1')
    a25_private_tllq.addDirectory(HPATH,spath,'private_sgnl_2019_10_11/v1/tllq_multidim_b1')
    a25_private_tllq.addDirectory(HPATH,spath,'private_sgnl_2019_10_11/v1/tllq_multidim_b2')
    a25_private_ttll.addDirectory(HPATH,spath,'private_sgnl_2019_10_11/v1/ttll_multidim_b1')
    a25_private_ttll.addDirectory(HPATH,spath,'private_sgnl_2019_10_11/v1/ttll_multidim_b2')
    a25_private_ttlnu.addDirectory(HPATH,spath,'private_sgnl_2019_10_11/v1/ttlnu_multidim_b1')

    a25_central_ttH = Sample('central_ttH',xsec=0.2151)
    a25_central_tHq = Sample('central_tHq',xsec=7.7129e-2)
    a25_central_tZq = Sample('central_tZq',xsec=0.0942)
    a25_central_ttZ = Sample('central_ttZ',xsec=0.2529)
    a25_central_ttW = Sample('central_ttW',xsec=0.2043)

    a25_central_tZq.addDirectory(HPATH,spath,'central_sgnl_2019_10_11/v1/tZq')
    a25_central_ttZ.addDirectory(HPATH,spath,'central_sgnl_2019_10_11/v1/ttZ')
    a25_central_ttW.addDirectory(HPATH,spath,'central_sgnl_2019_10_11/v1/ttW')
    a25_central_tHq.addDirectory(HPATH,spath,'central_sgnl_2019_10_11/v1/tHq')
    a25_central_ttH.addDirectory(HPATH,spath,'central_sgnl_2019_10_11/v1/ttH')

    # samples = [tllq4f_JetMax2]
    samples = [
        a25_private_ttH,

        # a25_central_ttH,
        # a25_central_tHq,
        # a25_central_tZq,
        # a25_central_ttZ,
        # a25_central_ttW,
    ]
    for samp in samples:
        label = samp.name()
        xsec = samp.xsec()
        label_name = "{label}".format(label=label)
        if len(label_suffix):
            label_name = "{label}_{suffix}".format(label=label,suffix=label_suffix)
        label_name += "_{tstamp}".format(tstamp=TIMESTAMP)
        print "Processing: {l}".format(l=label_name)
        file_list = []
        print "Building file list:"
        for idx,fdir in enumerate(samp.list()):
            print "\t[{0:0>{w}}/{1}] {dirpath}".format(idx+1,len(samp.list()),w=1,dirpath=fdir)
            files = getFiles(fdir,'root')
            file_list.extend(files)
        print "Found {} file(s)".format(len(file_list))
        if len(file_list) == 0:
            print "Skipping sample {l} with no files".format(l=label_name)
            continue
        inf = 'infiles_{l}.txt'.format(l=label_name)
        with open(inf,'w') as fd:
            for idx,fn in enumerate(file_list):
                if max_files > 0 and idx >= max_files:
                    print "Only processing {0} of {1} total files".format(max_files,len(file_list))
                    break
                fd.write('{fpath}\n'.format(fpath=fn))
        outf = "output_{l}.root".format(l=label_name)
        if testing:
            outf = ""
        cmd = ["root","-b","-l","-q"]
        cmd_args = "\"{outf}\",\"{inf}\",{evts},{xsec},{skim}".format(outf=outf,inf=inf,evts=events,xsec=xsec,skim=int(skim))
        # cmd.extend(["check_anaTrees.C(\"{outf}\", \"{inf}\", {evts}, {skim})".format(args=cmd_args)])
        cmd.extend(["check_anaTrees.C({args})".format(args=cmd_args)])
        subprocess.check_call(cmd)

#TODO: Switch the inputs to the 'all_samples' run
# Wrapper to run the 'read_anaTreeChecks.C' macro
def read_anaTreeChecks():
    print "Running read_anaTreeChecks()..."
    # NOTE: I re-named some of the pt/eta histograms, so most of the samples in 'checkAnaTrees'
    #       don't work (before 2019-11-12)

    # in pb
    tZq_xsec = 0.2529
    ttH_xsec = 0.2151
    tHq_xsec = 7.7129e-2
    ttZ_xsec = 0.2529
    ttW_xsec = 0.2043

    ##############################
    # ana28/ana29 samples
    ##############################
    # spath = "awightma/checkAnaTrees/ana28-priv_ana29-priv_NoStreaming_2019_11_11/v1"
    spath = "awightma/checkAnaTrees/ana25-priv_ana25-cent_ana28-priv_ana29-priv_NoStreaming_2019_11_13/v1"

    a25_private_ttH = Sample('a25priv_ttH',xsec=ttH_xsec)
    a25_private_tHq = Sample('a25priv_tHq',xsec=tHq_xsec)
    a25_private_tZq = Sample('a25priv_tllq',xsec=tZq_xsec)
    a25_private_ttZ = Sample('a25priv_ttll',xsec=ttZ_xsec)
    a25_private_ttW = Sample('a25priv_ttlnu',xsec=ttW_xsec)

    a25_central_ttH = Sample('a25cent_ttH',xsec=ttH_xsec)
    a25_central_tHq = Sample('a25cent_tHq',xsec=tHq_xsec)
    a25_central_tZq = Sample('a25cent_tllq',xsec=tZq_xsec)
    a25_central_ttZ = Sample('a25cent_ttll',xsec=ttZ_xsec)
    a25_central_ttW = Sample('a25cent_ttlnu',xsec=ttW_xsec)

    a28_private_ttH = Sample('a28priv_ttH',xsec=ttH_xsec)
    a28_private_ttW = Sample('a28priv_ttlnu',xsec=ttW_xsec)
    a28_private_ttZ = Sample('a28priv_ttll',xsec=ttZ_xsec)
    a28_private_tZq = Sample('a28priv_tllq',xsec=tZq_xsec)
    a28_private_tHq = Sample('a28priv_tHq',xsec=tHq_xsec)
    
    a29_private_ttH = Sample('a29priv_ttH',xsec=ttH_xsec)
    a29_private_ttW = Sample('a29priv_ttlnu',xsec=ttW_xsec)
    a29_private_ttZ = Sample('a29priv_ttll',xsec=ttZ_xsec)
    a29_private_tZq = Sample('a29priv_tllq',xsec=tZq_xsec)
    a29_private_tHq = Sample('a29priv_tHq',xsec=tHq_xsec)

    a25_central_ttH.addDirectory(spath,'a25_central_ttH')
    a25_central_ttW.addDirectory(spath,'a25_central_ttW')
    a25_central_ttZ.addDirectory(spath,'a25_central_ttZ')
    a25_central_tZq.addDirectory(spath,'a25_central_tZq')
    a25_central_tHq.addDirectory(spath,'a25_central_tHq')

    a25_private_ttH.addDirectory(spath,'a25_private_ttH')
    a25_private_ttW.addDirectory(spath,'a25_private_ttW')
    a25_private_ttZ.addDirectory(spath,'a25_private_ttZ')
    a25_private_tZq.addDirectory(spath,'a25_private_tZq')
    a25_private_tHq.addDirectory(spath,'a25_private_tHq')

    a28_private_ttH.addDirectory(spath,'a28_private_ttH')
    a28_private_ttW.addDirectory(spath,'a28_private_ttW')
    a28_private_ttZ.addDirectory(spath,'a28_private_ttZ')
    a28_private_tZq.addDirectory(spath,'a28_private_tZq')
    a28_private_tHq.addDirectory(spath,'a28_private_tHq')

    a29_private_ttH.addDirectory(spath,'a29_private_ttH')
    a29_private_ttW.addDirectory(spath,'a29_private_ttW')
    a29_private_ttZ.addDirectory(spath,'a29_private_ttZ')
    a29_private_tZq.addDirectory(spath,'a29_private_tZq')
    a29_private_tHq.addDirectory(spath,'a29_private_tHq')

    ##############################
    # ana29 (no duplicates) and SMCheck samples
    ##############################
    spath = 'awightma/checkAnaTrees/ana29-priv-NoDuplicates_HanOrig-HanV4-SMCheck_2019_11_20/v1'
    a29noDupes_private_ttH = Sample('a29privNoDupe_ttH',xsec=ttH_xsec)
    a29noDupes_private_ttW = Sample('a29privNoDupe_ttW',xsec=ttW_xsec)
    a29noDupes_private_ttZ = Sample('a29privNoDupe_ttZ',xsec=ttZ_xsec)
    a29noDupes_private_tZq = Sample('a29privNoDupe_tZq',xsec=tZq_xsec)
    a29noDupes_private_tHq = Sample('a29privNoDupe_tHq',xsec=tHq_xsec)

    ttH_HanOrigSMCheck = Sample('ttH_HanOrigSMCheck',xsec=ttH_xsec)
    ttZ_HanOrigSMCheck = Sample('ttZ_HanOrigSMCheck',xsec=ttZ_xsec)
    ttW_HanOrigSMCheck = Sample('ttW_HanOrigSMCheck',xsec=ttW_xsec)

    ttH_HanV4SMCheck = Sample('ttH_HanV4SMCheck',xsec=ttH_xsec)
    ttZ_HanV4SMCheck = Sample('ttZ_HanV4SMCheck',xsec=ttZ_xsec)
    ttW_HanV4SMCheck = Sample('ttW_HanV4SMCheck',xsec=ttW_xsec)
    tZq_HanV4SMCheck = Sample('tZq_HanV4SMCheck',xsec=tZq_xsec)
    tHq_HanV4SMCheck = Sample('tHq_HanV4SMCheck',xsec=tHq_xsec)

    a29noDupes_private_ttH.addDirectory(spath,'a29_private_ttH')
    a29noDupes_private_ttW.addDirectory(spath,'a29_private_ttW')
    a29noDupes_private_ttZ.addDirectory(spath,'a29_private_ttZ')
    a29noDupes_private_tZq.addDirectory(spath,'a29_private_tZq')
    a29noDupes_private_tHq.addDirectory(spath,'a29_private_tHq')

    ttH_HanOrigSMCheck.addDirectory(spath,'ttH_HanOrigSMCheck')
    ttZ_HanOrigSMCheck.addDirectory(spath,'ttZ_HanOrigSMCheck')
    ttW_HanOrigSMCheck.addDirectory(spath,'ttW_HanOrigSMCheck')

    ttH_HanV4SMCheck.addDirectory(spath,'ttH_HanV4SMCheck')
    ttZ_HanV4SMCheck.addDirectory(spath,'ttZ_HanV4SMCheck')
    ttW_HanV4SMCheck.addDirectory(spath,'ttW_HanV4SMCheck')
    tZq_HanV4SMCheck.addDirectory(spath,'tZq_HanV4SMCheck')
    tHq_HanV4SMCheck.addDirectory(spath,'tHq_HanV4SMCheck')



    inputs = [
        # a25_private_ttH,
        # a25_private_ttW,
        # a25_private_ttZ,
        # a25_private_tZq,
        # a25_private_tHq,

        # a25_central_ttH,
        # a25_central_ttW,
        # a25_central_ttZ,
        # a25_central_tZq,
        # a25_central_tHq,

        a28_private_ttZ,
        a29_private_ttZ,
        a29noDupes_private_ttZ,
        ttZ_HanOrigSMCheck,
        ttZ_HanV4SMCheck,
        
        a28_private_tZq,
        a29_private_tZq,
        a29noDupes_private_tZq,
        tZq_HanV4SMCheck,

        a28_private_ttH,
        a29_private_ttH,
        a29noDupes_private_ttH,
        ttH_HanOrigSMCheck,
        ttH_HanV4SMCheck,

        a28_private_ttW,
        a29_private_ttW,
        a29noDupes_private_ttW,
        ttW_HanOrigSMCheck,
        ttW_HanV4SMCheck,

        # a28_private_tHq,
        # a29_private_tHq,
        # a29noDupes_private_tHq,
        # tHq_HanV4SMCheck,
    ]

    # dir_name = 'testing_{tstamp}'.format(tstamp=TIMESTAMP)
    # sub_dir = 'test'

    sub_dir = 'a28-a29_checks'
    dir_name = 'tZq_{tstamp}'.format(tstamp=TIMESTAMP2)

    move_output = False
    remake_merged_files = False # Force the recreation of the merged root files
    merge_lst = []
    for idx,samp in enumerate(inputs):
        name = samp.name()
        sub_path = samp.list()[0]
        xsec = samp.xsec()
        dir_path = pjoin(HPATH,sub_path)
        to_merge = getFiles(dir_path,ext='root')
        if len(to_merge) == 0:
            print "No files to merge!"
            continue
        merged_fname = "{fname}.root".format(fname=name)
        hadd_cmd = ['hadd','-f']
        hadd_cmd.extend([merged_fname])
        hadd_ops_split = len(hadd_cmd)  # Number of options before we start including the files to merge
        hadd_cmd.extend(to_merge)
        s1 = ' '.join(hadd_cmd[:hadd_ops_split])
        s2 = '\n\t'.join(hadd_cmd[hadd_ops_split:])
        # print "Merge command: {0}\n\t{1}".format(s1,s2)
        if remake_merged_files or not os.path.exists(merged_fname):
            run_process(hadd_cmd)
        if os.path.exists(merged_fname):
            merge_lst.append(merged_fname)
        else:
            print "Skipping missing input file: {fname}".format(fname=merged_fname)
    if len(merge_lst) == 0:
        print "No samples to run over!"
        return
    infs = "{{{infs}}}".format(infs=",".join('"{fn}"'.format(fn=fn) for fn in merge_lst))

    cmd = ["root","-b","-l","-q"]
    cmd.extend(["read_anaTreeChecks.C({infs},{xsec})".format(infs=infs,xsec=xsec)])
    subprocess.check_call(cmd)

    if move_output:
        output_dir = pjoin(USER_DIR,'www/eft_stuff/misc/anacheck_plots',sub_dir,dir_name)
        print "Output Dir: {dir}".format(dir=output_dir)
        if not os.path.exists(output_dir):
            # os.mkdir(output_dir)
            os.makedirs(output_dir)
        clean_dir(output_dir,["^.*\.png$","^index.html$"])
        imgs = get_files('.',targets=["^.*\.png$"])
        move_files(files=imgs,target=output_dir)
        make_html(output_dir)

def plot_systematic_variations():
    local_dir = pjoin(CMSSW_BASE,'src/EFTMultilepton/TemplateMakers/test/MakeGoodPlot2')

    fpath_testing = pjoin(local_dir,"testing.root")

    # fpath = fpath_a22
    # fpath = fpath_a29
    fpath = fpath_testing

    # As a reminder these should be unique over all lists, since they should be coming from the same file
    private_signal = ["tllq_16D","ttH_16D","ttll_16D","ttlnu_16D","tHq_16D"]
    central_signal = ["tZq","ttH","ttZ","ttW"]
    central_bkgd = ["ttGJets","WZ","WWW"]

    samples = []
    samples.extend(private_signal)
    samples.extend(central_signal)
    samples.extend(central_bkgd)

    # samples = ['ttlnu_16D']
    samples = ['ttW','ttlnu_16D']

    syst = "Q2RF"
    # syst = "PDF"

    move_output = False
    web_dir = "/afs/crc.nd.edu/user/a/awightma/www"
    # sub_dir = "eft_stuff/misc/anatest_plots/njet_plots/{syst}_variations/{tstamp}_from-anatest25".format(syst=syst,tstamp=TIMESTAMP2)
    # For testing
    sub_dir = "eft_stuff/misc/anatest_plots/njet_plots/testing/{tstamp}_{syst}".format(tstamp=TIMESTAMP,syst=syst)
    
    for sample in samples:
        name = sample

        cmd = ["root","-b","-l","-q"]
        cmd_args = "\"{fpath}\",\"{sample}\",\"{syst}\"".format(fpath=fpath,sample=name,syst=syst)
        cmd.extend(['quick_plots.C({args})'.format(args=cmd_args)])
        subprocess.check_call(cmd)

        if move_output:
            output_dir = "{name}".format(name=name)
            if name in central_bkgd:
                if name == "WZ":
                    output_dir = "bkgd_Diboson"
                elif name == "WWW":
                    output_dir = "bkgd_Triboson"
                else:
                    output_dir = "bkgd_{name}".format(name=name)
            elif name in central_signal:
                output_dir = "central_{name}".format(name=name)
            elif name in private_signal:
                tmp = name.split('_16D')[0] # Chop off the 16D part of the name
                output_dir = "private_{name}".format(name=tmp)
            else:
                raise RuntimeError("Unknown sample name: {name}".format(name=name))
            output_path = pjoin(web_dir,sub_dir,output_dir)
            print "Output: {path}".format(path=output_path)
            if not os.path.exists(output_dir):
                os.mkdir(output_dir)
            imgs = get_files('.',targets=["^.*\.png$"])

def compare_anatest_files():
    geoff_dir = "/afs/crc.nd.edu/user/g/gsmith15/Public/for_Tony/"
    tony_dir  = "/afs/crc.nd.edu/user/a/awightma/Public/for_tony/"

    cmssw_dirs = os.path.expanduser("~/CMSSW_Releases")
    hist_dir  = pjoin(cmssw_dirs,"CMSSW_8_1_0/src/CombineHarvester/TopEFT/hist_files/")
    local_dir = pjoin(cmssw_dirs,"from_govner/CMSSW_9_4_6/src/EFTMultilepton/TemplateMakers/test/MakeGoodPlot2/")

    merged_dir = "/tmpscratch/users/awightma/analysisWorkflow/mergedHists/"

    fname_data           = "temp_data.root"
    fname_MuonEG         = "temp_MuonEG.root"
    fname_DoubleMuon     = "temp_DoubleMuon.root"
    fname_SingleMuon     = "temp_SingleMuon.root"
    fname_SingleElectron = "temp_SingleElectron.root"
    fname_DoubleEG       = "temp_DoubleEG.root"

    fname_tZq_incl_higgs = "tZq_incl_higgs.root"
    fname_tZq_base = "tZq_base.root"
    fname_tZq_v14v1_NSH = "tZq_v14v1_NSH.root"  # This file is fucked up for some reason (empty histograms)

    fname_tllq = 'temp_tllq_multidim.root'

    fname_testing = 'testing.root'

    fpath_data_a26 = pjoin(merged_dir,"2019_07_08_from-standardhists_SRs_with_Round5_EFTsamps",fname_data)

    tmp_name = "2019_07_08_from-standardhists_SRs_with_Round5_EFTsamps"
    fpath_MuonEG_a26         = pjoin(merged_dir,tmp_name,fname_MuonEG)
    fpath_DoubleMuon_a26     = pjoin(merged_dir,tmp_name,fname_DoubleMuon)
    fpath_SingleMuon_a26     = pjoin(merged_dir,tmp_name,fname_SingleMuon)
    fpath_SingleElectron_a26 = pjoin(merged_dir,tmp_name,fname_SingleElectron)
    fpath_DoubleEG_a26       = pjoin(merged_dir,tmp_name,fname_DoubleEG)

    fpath_data_a27 = pjoin(merged_dir,"2019_10_22_data-nominal_newGT-94X_dataRun2_v11",fname_data)

    tmp_name = "2019_10_22_data-nominal_newGT-94X_dataRun2_v11"
    fpath_MuonEG_a27         = pjoin(merged_dir,tmp_name,fname_MuonEG)
    fpath_DoubleMuon_a27     = pjoin(merged_dir,tmp_name,fname_DoubleMuon)
    fpath_SingleMuon_a27     = pjoin(merged_dir,tmp_name,fname_SingleMuon)
    fpath_SingleElectron_a27 = pjoin(merged_dir,tmp_name,fname_SingleElectron)
    fpath_DoubleEG_a27       = pjoin(merged_dir,tmp_name,fname_DoubleEG)

    fpath_tZq_incl_higgs = pjoin(local_dir,fname_tZq_incl_higgs)
    fpath_tZq_base = pjoin(local_dir,fname_tZq_base)
    fpath_tZq_v14v1_NSH = pjoin(local_dir,fname_tZq_v14v1_NSH)
    fpath_testing = pjoin(local_dir,fname_testing)

    fpath_tllq_a25 = pjoin(merged_dir,"2019_10_13_full_MC",fname_tllq)
    fpath_tllq_a28 = pjoin(merged_dir,"2019_10_19_full_MC",fname_tllq)

    fpath = fpath_a26

    compare = [
        # (fpath_a15,fpath_a16),
        # (fpath_a15,fpath_a22),
        # (fpath_a16,fpath_a22),
        # (fpath_a22,fpath_a23),
        # (fpath_a23,fpath_a24),
        # (fpath_a26,fpath_a27),
        # (fpath_data_a26,fpath_data_a27),
        # (fpath_MuonEG_a26,fpath_MuonEG_a27),
        # (fpath_DoubleMuon_a26,fpath_DoubleMuon_a27),
        # (fpath_SingleMuon_a26,fpath_SingleMuon_a27),
        # (fpath_SingleElectron_a26,fpath_SingleElectron_a27),
        # (fpath_DoubleEG_a26,fpath_DoubleEG_a27),
        # (fpath_a22,fpath_a28),
        # (fpath_a16,fpath_a28),
        # (fpath_a16,fpath_tZq_incl_higgs),
        # (fpath_a28,fpath_tZq_incl_higgs),
        # (fpath_tZq_incl_higgs,fpath_tZq_base)
        # (fpath_tZq_base,fpath_a28),     # These are identical for tZq
        # (fpath_a16,fpath_tZq_v14v1_NSH),

        # (fpath_a23,fpath_a24),
        # (fpath_a24,fpath_a25),
        # (fpath_a25,fpath_a26),
        # (fpath_a26,fpath_a28),

        # (fpath_a24,fpath_a28),
        # (fpath_a25,fpath_a28),
        # (fpath_a26,fpath_a28),
        # (fpath_a28,fpath_a29),
        # (fpath_a29,fpath_a30),
        # (fpatha_28,fpath_a31),
        # (fpath_a28,fpath_a28_redoFull)

        # (fpath_a30,fpath_testing)

        # (fpath_a28_redoFull,fpath_a29_noDupes_v2),
        # (fpath_a29_noDupes,fpath_a29_noDupes_v2),
        # (fpath_a31,fpath_a29_noDupes_v2),
        # (fpath_R6B1,fpath_a29_noDupes_v2),
        # (fpath_R6B1_NSH,fpath_a29_noDupes_v2),
        # (fpath_R6e2_NSH,fpath_a29_noDupes_v2),

        # (fpath_tllq_R6B1,fpath_tllq_R6B2),
        # (fpath_tllq_R6B1,fpath_tllq_R6B3),
        # (fpath_tllq_R6B2,fpath_tllq_R6B3),

        # (fpath_a28_fixedErrors,fpath_a31_fixedErrors,"a28 vs. a31","a28-a31"),
        # (fpath_HanOrigSMChecks,fpath_HanV4SMChecks,"SM Orig vs. Hv4","SM_Check"),
        # (fpath_HanOrigSMChecks,fpath_a28_fixedErrors,"SM Orig vs. a28","SM-a28"),
        # (fpath_HanV4SMChecks,fpath_a31_fixedErrors,"SM Hv4 vs. a31","SM-a31"),


        # (fpath_HanOrigSMChecks,fpath_a28_fixedErrors,"SM Orig","a28","SMOrig-a28"),
        # (fpath_HanV4SMChecks,fpath_a31_fixedErrors  ,"SM Hv4" ,"a31","SMHv4-a31"),
        # (fpath_HanOrigSMChecks,fpath_HanV4SMChecks  ,"SM Orig","Hv4","SMOrig-SMHv4"),
        # (fpath_a28_fixedErrors,fpath_a31_fixedErrors,"a28"    ,"a31","a28-a31"),

        # (fpath_a28_fixedErrors,fpath_a28_fixedErrors,"a28","a28","central-a28"),
        # (fpath_a31_fixedErrors,fpath_a31_fixedErrors,"a31","a31","central-a31"),
        # (fpath_HanOrigSMChecks,fpath_HanOrigSMChecks,"SM Orig","SM Orig","central-SMOrig"),
        # (fpath_HanV4SMChecks,fpath_HanV4SMChecks    ,"Hv4","Hv4","central-SMHv4"),

        # (fpath_a31_fixedErrors,fpath_a32,"a31","a32","a31-a32"),
        # (fpath_a28_fixedErrors,fpath_a32,"a28","a32","a28-a32"),

        # (fpath_HanOrigSMChecks,fpath_a31_fixedErrors,"SM Orig","a31","SMOrig-a31"),
        # (fpath_HanV4SMChecks,fpath_a31_fixedErrors  ,"SM Hv4" ,"a31","SMHv4-a31"),

        # (fpath_HanOrigSMChecks,fpath_a32,"SM Orig","a32","SMOrig-a32"),
        # (fpath_HanV4SMChecks,fpath_a32,"SM Hv4","a32","SMHv4-a32"),

        # (fpath_a32,fpath_a32_copyChanges,"a32-base","a32-copy","a32-copyChanges"),
        # (fpath_a32,fpath_a32,"f1-title","f2-title",""),

        (fpath_a32,fpath_a32_tllq_SM_Q2RF_check,"f1","f2",""),
    ]

    for idx,tup in enumerate(compare):
        fpath1,fpath2,f1_title,f2_title,out_name = tup
        h,fname1 = os.path.split(fpath1)
        h,fname2 = os.path.split(fpath2)
        print "Comparing: {fn1} --> {fn2}".format(fn1=fname1,fn2=fname2)
        cmd = ["root","-b","-l","-q"]
        cmd_args = []
        cmd_args.append("\"{fp1}\",\"{fp2}\"".format(fp1=fpath1,fp2=fpath2))
        cmd_args.append("\"{fn1}\",\"{fn2}\"".format(fn1=f1_title,fn2=f2_title))
        cmd_args.append("\"{name}\"".format(name=out_name))
        cmd_args = ",".join(cmd_args)
        # cmd_args = "\"{fp1}\",\"{fp2}\",\"{fn1}\",\"{name}\"".format(
        #     fp1=fpath1,fp2=fpath2,
        #     title=cmp_title,name=out_name
        # )
        cmd.extend(['compare_anatest_files.C({args})'.format(args=cmd_args)])
        subprocess.check_call(cmd)
        # run_process(cmd,verbose=True,indent=0)
        print "#"*100

def make_yield_table():
    # fpath = fpath_a29_noDupes

    private_signal = ["tllq_16D","ttH_16D","ttll_16D","ttlnu_16D","tHq_16D"]
    central_signal = ["tZq","ttH","ttZ","ttW"]
    central_bkgd = ["ttGJets","WZ","WWW"]

    # Dictionary map to set the yield table name -- the (0,0) cell of the table
    fname_map = {
        'private_ttH-ttZ-ttW_HanV4_R6B1Only': 'R6B1Only',
        'private_ttH-ttZ-ttW_HanV4_R6B1Only_NSH': 'R6B1Only_NSH',
        'private_tZq_HanV4_R6-extra2Only_NSH': 'tllq_R6e2NSH',
        'private_sgnl_redoHistMaking-a29_NoStreaming': 'a29_redoHist',
        'private_sgnl_redoTrees-a29_NoStreaming': 'a29_redoFull',
        'private_sgnl_redoTrees-a29_NoStreaming_v2': 'a29_redoFull',
        'anatest28_redoFullWF-NoStreaming': 'a28_redoFull',
        'anatest29_NoDuplicates': 'a29_noDupes',
        'anatest29_NoDuplicatesV2': 'a29_noDupesV2',
        'private_sgnl_HanV4_SMCheck': 'HanV4SMCheck',
        'private_sgnl_HanOriginal_SMCheck': 'HanOrigSMChk',
        'private_sgnl_tllq_R6B1': 'tllq_R6B1',
        'private_sgnl_tllq_R6B2': 'tllq_R6B2',
        'private_sgnl_tllq_R6B3': 'tllq_R6B3',
        'anatest28_fixedErrors': 'a28_fixStats',
        'anatest31_fixedErrors': 'a31_fixStats',
        'anatest32_copyChanges': 'a32_copyCh',
    }

    lst = []
    # lst.extend([fpath_a28,fpath_a28_redoFull,fpath_a29,fpath_a29_noDupes,fpath_a31])
    lst.extend([
        # fpath_a28_redoFull,
        # fpath_a31,
        # fpath_a29_noDupes,
        # fpath_a29_noDupes_v2,
        # fpath_HanV4SMChecks,
        # fpath_HanOrigSMChecks,
        # fpath_a31_fixedErrors,
        fpath_a32,
        fpath_a32_copyChanges,
    ])
    # lst.extend([
    #     fpath_tllq_R6B1,
    #     fpath_tllq_R6B2,
    #     fpath_tllq_R6B3,
    # ])
    # lst.extend([fpath_R6B1,fpath_R6B1_NSH,fpath_R6e2_NSH])
    # lst.extend([fpath_a29_redoHist,fpath_a29_redoFull_v2])
    # lst.extend([
    #     # fpath_a28_redoFull,
    #     # fpath_a29_noDupes_v2,
    #     # fpath_a28_fixedErrors,
    #     # fpath_a31_fixedErrors,
    #     fpath_HanOrigSMChecks,
    #     fpath_HanV4SMChecks,
    # ])
    for fp in lst:
        h,t = os.path.split(fp)
        name = t.rsplit('.')[0]
        if fname_map.has_key(name):
            name = fname_map[name]
        print "Making yield table for {fp}".format(fp=fp)
        cmd = ["root","-b","-l","-q"]
        cmd_args = "\"{fp}\",\"{name}\"".format(fp=fp,name=name)
        cmd.extend(['make_yield_table.C({args})'.format(args=cmd_args)])
        subprocess.check_call(cmd)

def main():
    # check_anaTrees()
    # read_anaTreeChecks()
    # plot_systematic_variations()
    compare_anatest_files()
    # make_yield_table()


if __name__ == "__main__":
    main()
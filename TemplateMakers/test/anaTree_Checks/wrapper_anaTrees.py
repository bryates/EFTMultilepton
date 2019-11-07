import subprocess
import datetime
import sys
import os

from utils import get_files,move_files,clean_dir

# Python wrapper to the check_anaTrees script when trying to run by-hand (instead of via lobster)

USER_DIR = os.path.expanduser('~')

TIMESTAMP = datetime.datetime.now().strftime('%Y-%m-%d_%H%M')
TIMESTAMP2 = datetime.datetime.now().strftime('%Y_%m_%d')
HPATH = '/hadoop/store/user'

# Alias function
pjoin = os.path.join

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
            d = os.path.join(*args)
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
    events = 75000
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
        a25_central_ttH,
        # a25_central_tHq,
        a25_central_tZq,
        a25_central_ttZ,
        a25_central_ttW,
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

    # OLD METHOD (kept for historical referencing)
    inputs = [
        ##############################
        # Previous version of samples with old naming
        ##############################

        # ("m_centralv1","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/central_tZq_v14v1"),
        # ("m_centralv2","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/central_tZq_v14v2"),
        # ("m_JetMax1","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_JetMax1"),
        # ("m_JetMax2","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_JetMax2"),
        # ("m_0pNoMerge","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_NoMerge"),
        # ("m_0pJetMax1","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_JetMax1"),
        # ("m_0pJetMax2","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_JetMax2"),

        # ("m_centralv1_NSH","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/central_tZq_NoSkipHiggs_v14v1"),
        # ("m_centralv2_NSH","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/central_tZq_NoSkipHiggs_v14v2"),
        # ("m_JetMax1_NSH","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_JetMax1_NoSkipHiggs"),
        # ("m_JetMax2_NSH","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_JetMax2_NoSkipHiggs"),
        # ("m_0pNoMerge_NSH","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_NoMerge_NoSkipHiggs"),
        # ("m_0pJetMax1_NSH","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_JetMax1_NoSkipHiggs"),
        # ("m_0pJetMax2_NSH","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_JetMax2_NoSkipHiggs"),

        #NOTE: The directory says 'JetMax2' but the pythia setting was really 'JetMax1'
        # ('m_Match_JetMax1',"awightma/checkAnaTrees/tllq4f_MatchedNoHiggs0p_JetMax2_20191007_1044/v1/tllq4f_MatchedNoHiggs0p_JetMax2"),

        # ('m_NoMLM_NoMerge',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggs_0j_NoMerge"),
        # ('m_NoMLM_JetMax1',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggs_0j_JetMax1"),
        # ('m_NoMLM_JetMax2',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggs_0j_JetMax2"),
        # ('m_MLM_NoMerge',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggsMLM_0j_NoMerge"),
        # ('m_MLM_JetMax1',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggsMLM_0j_JetMax1"),
        # ('m_MLM_JetMax2',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggsMLM_0j_JetMax2"),

        ##############################
        # The most up-to-date samples with new naming
        ##############################

        # ("sp_JetMax2_1254","awightma/checkAnaTrees/test/lobster_test_20190927_1254/v1/tllq4f_JetMax2_FP"),
        # ("sp_JetMax2_1308","awightma/checkAnaTrees/test/lobster_test_20191001_1308/v1/tllq4f_JetMax2"),
        # ("sp_JetMax2_2005","awightma/checkAnaTrees/test/lobster_test_20191002_2005/v1/tllq4f_JetMax2"),
        # ("sp_JetMax2_1556","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_JetMax2"),

        # ("centralv1",       "awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/central_tZq_v14v1"),
        # ("centralv2",       "awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/central_tZq_v14v2"),
        # ("tchYesH_01p_JM1", "awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_JetMax1"),
        # ("tchYesH_01p_JM2", "awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_JetMax2"),
        # ("tchYesH_0p_NoMrg","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_NoMerge"),
        # ("tchYesH_0p_JM1",  "awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_JetMax1"),
        # ("tchYesH_0p_JM2",  "awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_JetMax2"),

        # ("centralv1_NSH",       "awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/central_tZq_NoSkipHiggs_v14v1"),
        # ("centralv2_NSH",       "awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/central_tZq_NoSkipHiggs_v14v2"),
        # ("tchYesH_01p_JM1_NSH", "awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_JetMax1_NoSkipHiggs"),
        # ("tchYesH_01p_JM2_NSH", "awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_JetMax2_NoSkipHiggs"),
        # ("tchYesH_0p_NoMrg_NSH","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_NoMerge_NoSkipHiggs"),
        # ("tchYesH_0p_JM1_NSH",  "awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_JetMax1_NoSkipHiggs"),
        # ("tchYesH_0p_JM2_NSH",  "awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_JetMax2_NoSkipHiggs"),

        #NOTE: The directory says 'JetMax2' but the pythia setting was really 'JetMax1'
        # ('MatchedNoH_JM1',"awightma/checkAnaTrees/tllq4f_MatchedNoHiggs0p_JetMax2_20191007_1044/v1/tllq4f_MatchedNoHiggs0p_JetMax2"),

        # ('tchNoH_0p_NoMrg',    "awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggs_0j_NoMerge"),
        # ('tchNoH_0p_JM1',      "awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggs_0j_JetMax1"),
        # ('tchNoH_0p_JM2',      "awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggs_0j_JetMax2"),
        # ('tchNoH_0p_MLM_NoMrg',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggsMLM_0j_NoMerge"),
        # ('tchNoH_0p_MLM_JM1',  "awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggsMLM_0j_JetMax1"),
        # ('tchNoH_0p_MLM_JM2',  "awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggsMLM_0j_JetMax2"),
        # ('tchNoH_0p_xq0_NoMrg',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_xqcut0_20191008_0959/v1/tllq4f_tch_NoHiggs_0j_xqcut0_NoMerge"),
        # ('tchNoH_0p_xq0_JM1',  "awightma/checkAnaTrees/tllq4f_tch_NoHiggs_xqcut0_20191008_0959/v1/tllq4f_tch_NoHiggs_0j_xqcut0_JetMax1"),
        # ('tchNoH_0p_xq0_JM2',  "awightma/checkAnaTrees/tllq4f_tch_NoHiggs_xqcut0_20191008_0959/v1/tllq4f_tch_NoHiggs_0j_xqcut0_JetMax2"),

        # ('tchNoH_0p_xq0_Stats',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_0j_xqcut0_2019-10-09_1808/v1/tllq4f_tch_NoHiggs_0j_xqcut0_NoMerge"),
        
        ##############################
        # ana24 samples
        ##############################
        # ('a24_private_ttH',  "awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a24_private_ttH"),
        # ('a24_private_tHq',  "awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a24_private_tHq"),
        # ('a24_private_tllq', "awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a24_private_tllq"),
        # ('a24_private_ttll', "awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a24_private_ttll"),
        # ('a24_private_ttlnu',"awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a24_private_ttlnu"),

        ##############################
        # ana25 samples
        ##############################
        # ('a25_private_ttH',  "awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a25_private_ttH"),
        # ('a25_private_tHq',  "awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a25_private_tHq"),
        # ('a25_private_tllq', "awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a25_private_tllq"),
        # ('a25_private_ttll', "awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a25_private_ttll"),
        # ('a25_private_ttlnu',"awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a25_private_ttlnu"),

        # ('a25_central_ttH',"awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a25_central_ttH"),
        # ('a25_central_tHq',"awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a25_central_tHq"),
        # ('a25_central_tZq',"awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a25_central_tZq"),
        # ('a25_central_ttZ',"awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a25_central_ttZ"),
        # ('a25_central_ttW',"awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1/a25_central_ttW"),
        ##############################
        # Custom ordering of the samples
        ##############################

        # ("centralv2","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/central_tZq_v14v2"),
        # ("tchYesH_01p_JM2","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_JetMax2"),
        # ("tchYesH_01p_JM2_NSH","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_JetMax2_NoSkipHiggs"),
        # ("tchYesH_0p_NoMrg","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_NoMerge"),
        # ("tchYesH_0p_NoMrg_NSH","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_NoMerge_NoSkipHiggs"),
        # ('MatchedNoH_JM1',"awightma/checkAnaTrees/tllq4f_MatchedNoHiggs0p_JetMax2_20191007_1044/v1/tllq4f_MatchedNoHiggs0p_JetMax2"),
        # ('tchNoH_0p_NoMrg',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggs_0j_NoMerge"),
        # ('tchNoH_0p_MLM_NoMrg',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggsMLM_0j_NoMerge"),
        # ('tchNoH_0p_xq0_NoMrg',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_xqcut0_20191008_0959/v1/tllq4f_tch_NoHiggs_0j_xqcut0_NoMerge"),
        # ('tchNoH_0p_xq0_Stats',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_0j_xqcut0_2019-10-09_1808/v1/tllq4f_tch_NoHiggs_0j_xqcut0_NoMerge"),
    ]

    spath = "awightma/checkAnaTrees/ana24-priv_ana25-priv_ana25-cent_2019-10-17_1436/v1"
    ##############################
    # ana24 samples
    ##############################
    a24_private_ttH   = Sample('a24priv_ttH',xsec=0.2151)
    a24_private_tHq   = Sample('a24priv_tHq',xsec=7.7129e-2)
    a24_private_tllq  = Sample('a24priv_tllq',xsec=0.0942)
    a24_private_ttll  = Sample('a24priv_ttll',xsec=0.2529)
    a24_private_ttlnu = Sample('a24priv_ttlnu',xsec=0.2043)

    a24_private_ttH.addDirectory(spath,"a24_private_ttH")
    a24_private_tHq.addDirectory(spath,"a24_private_tHq")
    a24_private_tllq.addDirectory(spath,"a24_private_tllq")
    a24_private_ttll.addDirectory(spath,"a24_private_ttll")
    a24_private_ttlnu.addDirectory(spath,"a24_private_ttlnu")

    ##############################
    # ana25 samples
    ##############################
    a25_private_ttH   = Sample('a25priv_ttH',xsec=0.2151)
    a25_private_tHq   = Sample('a25priv_tHq',xsec=7.7129e-2)
    a25_private_tllq  = Sample('a25priv_tllq',xsec=0.0942)
    a25_private_ttll  = Sample('a25priv_ttll',xsec=0.2529)
    a25_private_ttlnu = Sample('a25priv_ttlnu',xsec=0.2043)

    a25_private_ttH.addDirectory(spath,'a25_private_ttH')
    a25_private_tHq.addDirectory(spath,'a25_private_tHq')
    a25_private_tllq.addDirectory(spath,'a25_private_tllq')
    a25_private_ttll.addDirectory(spath,'a25_private_ttll')
    a25_private_ttlnu.addDirectory(spath,'a25_private_ttlnu')

    a25_central_ttH = Sample('a25cent_ttH',xsec=0.2151)
    a25_central_tHq = Sample('a25cent_tHq',xsec=7.7129e-2)
    a25_central_tZq = Sample('a25cent_tZq',xsec=0.0942)
    a25_central_ttZ = Sample('a25cent_ttZ',xsec=0.2529)
    a25_central_ttW = Sample('a25cent_ttW',xsec=0.2043)

    a25_central_ttH.addDirectory(spath,'a25_central_ttH')
    a25_central_tHq.addDirectory(spath,'a25_central_tHq')
    a25_central_tZq.addDirectory(spath,'a25_central_tZq')
    a25_central_ttZ.addDirectory(spath,'a25_central_ttZ')
    a25_central_ttW.addDirectory(spath,'a25_central_ttW')

    spath = "awightma/checkAnaTrees/ana26_PDF-fix_2019-10-20_1440/v1"
    ##############################
    # ana25 samples
    ##############################
    a26_private_tllq = Sample('a26priv_tllq',xsec=0.2529)
    a26_central_tZq  = Sample('a26cent_tZq',xsec=0.2529)

    a26_private_tllq.addDirectory(spath,'a26_private_tllq')
    a26_central_tZq.addDirectory(spath,'a26_central_tZq')


    inputs = [
        # a25_central_ttH,
        # a25_central_tHq,
        # a25_central_tZq,
        # a25_central_ttZ,
        # a25_central_ttW,

        # a24_private_ttH,
        # a24_private_tHq,
        # a24_private_tllq,
        # a24_private_ttll,
        # a24_private_ttlnu,

        # a25_private_ttH,
        # a25_private_tHq,
        # a25_private_tllq,
        # a25_private_ttll,
        # a25_private_ttlnu,

        a26_private_tllq,

        a26_central_tZq,
    ]

    # dir_name = 'testing_{tstamp}'.format(tstamp=TIMESTAMP)
    # sub_dir = 'test'

    sub_dir = 'anatest_checks'
    dir_name = 'ttH_{tstamp}'.format(tstamp=TIMESTAMP)

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
        hadd_ops_split = len(hadd_cmd)  # This is the number of options before we start including the files to merge
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
            os.mkdir(output_dir)
        clean_dir(output_dir,["^.*\.png$","^index.html$"])
        imgs = get_files('.',targets=["^.*\.png$"])
        move_files(files=imgs,target=output_dir)

# Untested!
def plot_systematic_variations():
    geoff_dir = "/afs/crc.nd.edu/user/g/gsmith15/Public/for_Tony/"
    tony_dir  = "/afs/crc.nd.edu/user/a/awightma/Public/for_tony/"
    hist_dir  = "/afs/crc.nd.edu/user/a/awightma/CMSSW_Releases/CMSSW_8_1_0/src/CombineHarvester/TopEFT/hist_files/"

    fname_a15   = "TOP-19-001_unblinded_v1.root"
    fname_a16   = "anatest16.root"
    fname_a17   = "anatest17.root"
    fname_a18   = "anatest18.root"
    fname_a19   = "anatest19.root"
    fname_a20   = "anatest20.root"
    fname_a21   = "anatest21.root"
    fname_a22   = "anatest22.root"
    fname_a23v3 = "anatest23_v3.root"
    fname_a24   = "anatest24.root"
    fname_a25   = "anatest25.root"
    fname_a26   = "anatest26.root"

    fname_a29   = "anatest29.root"
    fname_a30   = "anatest30.root"

    fpath_a15 = os.path.join(hist_dir,fname_a15)
    fpath_a16 = os.path.join(hist_dir,fname_a16)
    fpath_a17 = os.path.join(hist_dir,fname_a17)
    fpath_a18 = os.path.join(hist_dir,fname_a18)
    fpath_a19 = os.path.join(hist_dir,fname_a19)
    fpath_a20 = os.path.join(hist_dir,fname_a20)
    fpath_a22 = os.path.join(hist_dir,fname_a22)

    fpath_a21 = os.path.join(geoff_dir,fname_a21)

    fpath_a23 = os.path.join(tony_dir,fname_a23v3)
    fpath_a24 = os.path.join(tony_dir,fname_a24)
    fpath_a25 = os.path.join(tony_dir,fname_a25)
    fpath_a26 = os.path.join(tony_dir,fname_a26)
    fpath_a29 = os.path.join(tony_dir,fname_a29)
    fpath_a30 = os.path.join(tony_dir,fname_a30)

    testing_dir = "/afs/crc.nd.edu/user/a/awightma/CMSSW_Releases/from_govner/CMSSW_9_4_6/src/EFTMultilepton/TemplateMakers/test/MakeGoodPlot2"
    fpath_testing = os.path.join(testing_dir,"testing.root")

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

    # syst = "Q2RF"
    syst = "PDF"

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
            output_path = os.path.join(web_dir,sub_dir,output_dir)
            print "Output: {path}".format(path=output_path)
            if not os.path.exists(output_dir):
                os.mkdir(output_dir)
            imgs = get_files('.',targets=["^.*\.png$"])

def compare_anatest_files():
    geoff_dir = "/afs/crc.nd.edu/user/g/gsmith15/Public/for_Tony/"
    tony_dir  = "/afs/crc.nd.edu/user/a/awightma/Public/for_tony/"
    hist_dir  = "/afs/crc.nd.edu/user/a/awightma/CMSSW_Releases/CMSSW_8_1_0/src/CombineHarvester/TopEFT/hist_files/"
    local_dir = "/afs/crc.nd.edu/user/a/awightma/CMSSW_Releases/from_govner/CMSSW_9_4_6/src/EFTMultilepton/TemplateMakers/test/MakeGoodPlot2/"

    merged_dir = "/tmpscratch/users/awightma/analysisWorkflow/mergedHists/"

    fname_a15   = "TOP-19-001_unblinded_v1.root"
    fname_a16   = "anatest16.root"
    fname_a17   = "anatest17.root"
    fname_a18   = "anatest18.root"
    fname_a19   = "anatest19.root"
    fname_a20   = "anatest20.root"
    fname_a21   = "anatest21.root"
    fname_a22   = "anatest22.root"
    fname_a23v3 = "anatest23_v3.root"
    fname_a24   = "anatest24.root"
    fname_a25   = "anatest25.root"
    fname_a26   = "anatest26.root"
    fname_a27   = "anatest27.root"
    fname_a28   = "anatest28.root"
    fname_a29   = "anatest29.root"
    fname_a30   = "anatest30.root"

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

    fpath_a15 = os.path.join(hist_dir,fname_a15)
    fpath_a16 = os.path.join(hist_dir,fname_a16)
    fpath_a17 = os.path.join(hist_dir,fname_a17)
    fpath_a18 = os.path.join(hist_dir,fname_a18)
    fpath_a19 = os.path.join(hist_dir,fname_a19)
    fpath_a20 = os.path.join(hist_dir,fname_a20)
    fpath_a22 = os.path.join(hist_dir,fname_a22)

    fpath_a21 = os.path.join(geoff_dir,fname_a21)

    fpath_a23 = os.path.join(tony_dir,fname_a23v3)
    fpath_a24 = os.path.join(tony_dir,fname_a24)
    fpath_a25 = os.path.join(tony_dir,fname_a25)
    fpath_a26 = os.path.join(tony_dir,fname_a26)
    fpath_a27 = os.path.join(tony_dir,fname_a27)
    fpath_a28 = os.path.join(tony_dir,fname_a28)
    fpath_a29 = os.path.join(tony_dir,fname_a29)
    fpath_a30 = os.path.join(tony_dir,fname_a30)

    fpath_data_a26 = os.path.join(merged_dir,"2019_07_08_from-standardhists_SRs_with_Round5_EFTsamps",fname_data)

    fpath_MuonEG_a26         = os.path.join(merged_dir,"2019_07_08_from-standardhists_SRs_with_Round5_EFTsamps",fname_MuonEG)
    fpath_DoubleMuon_a26     = os.path.join(merged_dir,"2019_07_08_from-standardhists_SRs_with_Round5_EFTsamps",fname_DoubleMuon)
    fpath_SingleMuon_a26     = os.path.join(merged_dir,"2019_07_08_from-standardhists_SRs_with_Round5_EFTsamps",fname_SingleMuon)
    fpath_SingleElectron_a26 = os.path.join(merged_dir,"2019_07_08_from-standardhists_SRs_with_Round5_EFTsamps",fname_SingleElectron)
    fpath_DoubleEG_a26       = os.path.join(merged_dir,"2019_07_08_from-standardhists_SRs_with_Round5_EFTsamps",fname_DoubleEG)

    fpath_data_a27 = os.path.join(merged_dir,"2019_10_22_data-nominal_newGT-94X_dataRun2_v11",fname_data)

    fpath_MuonEG_a27         = os.path.join(merged_dir,"2019_10_22_data-nominal_newGT-94X_dataRun2_v11",fname_MuonEG)
    fpath_DoubleMuon_a27     = os.path.join(merged_dir,"2019_10_22_data-nominal_newGT-94X_dataRun2_v11",fname_DoubleMuon)
    fpath_SingleMuon_a27     = os.path.join(merged_dir,"2019_10_22_data-nominal_newGT-94X_dataRun2_v11",fname_SingleMuon)
    fpath_SingleElectron_a27 = os.path.join(merged_dir,"2019_10_22_data-nominal_newGT-94X_dataRun2_v11",fname_SingleElectron)
    fpath_DoubleEG_a27       = os.path.join(merged_dir,"2019_10_22_data-nominal_newGT-94X_dataRun2_v11",fname_DoubleEG)

    fpath_tZq_incl_higgs = os.path.join(local_dir,fname_tZq_incl_higgs)
    fpath_tZq_base = os.path.join(local_dir,fname_tZq_base)
    fpath_tZq_v14v1_NSH = os.path.join(local_dir,fname_tZq_v14v1_NSH)

    fpath_tllq_a25 = os.path.join(merged_dir,"2019_10_13_full_MC",fname_tllq)
    fpath_tllq_a28 = os.path.join(merged_dir,"2019_10_19_full_MC",fname_tllq)

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
        (fpath_a29,fpath_a30),
    ]

    for idx,tup in enumerate(compare):
        fpath1,fpath2 = tup
        h,fname1 = os.path.split(fpath1)
        h,fname2 = os.path.split(fpath2)
        print "Comparing: {fn1} --> {fn2}".format(fn1=fname1,fn2=fname2)
        cmd = ["root","-b","-l","-q"]
        cmd_args = "\"{fp1}\",\"{fp2}\"".format(fp1=fpath1,fp2=fpath2)
        cmd.extend(['compare_anatest_files.C({args})'.format(args=cmd_args)])
        subprocess.check_call(cmd)
        # run_process(cmd,verbose=True,indent=0)
        print "#"*100

def main():
    # check_anaTrees()
    # read_anaTreeChecks()
    # plot_systematic_variations()
    compare_anatest_files()


if __name__ == "__main__":
    main()
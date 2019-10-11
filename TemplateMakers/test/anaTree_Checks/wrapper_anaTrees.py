import subprocess
import datetime
import sys
import os

from utils import get_files,move_files,clean_dir

# Python wrapper to the check_anaTrees script when trying to run by-hand (instead of via lobster)

USER_DIR = os.path.expanduser('~')

TIMESTAMP = datetime.datetime.now().strftime('%Y-%m-%d_%H%M')
HPATH = '/hadoop/store/user'

# Alias function
pjoin = os.path.join

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
            stdout.append(l.strip())
            if verbose: print indent_str+l.strip()
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

# Wrapper to run the 'check_anaTrees.C' macro
def check_anaTrees():
    print "Running check_anaTrees()..."
    max_files = 3
    events = -1
    skim = True
    testing = False

    # label_suffix = "v14-v1_Full"
    label_suffix = ""

    spath = 'awightma/analysisTrees'
    tllq4f_JetMax2 = ('tllq4f_JetMax2',[
        # pjoin(hpath,spath,'special/tllq4f_EFT_t-channel_01j_2019_09_04_JetMax2/v1/tllq_multidim'),
        pjoin(HPATH,spath,'special/tllq4f_EFT_t-channel_01j_2019_09_20_JetMax2/v1/tllq_multidim'),
    ])

    tllq4f_JetMax2_NoSkipHiggs = ('tllq4f_JetMax2_NoSkipHiggs',[
        pjoin(HPATH,spath,'special/tllq4f_EFT_t-channel_01j_2019_09_20_JetMax2_NoSkipHiggs/v1/tllq_multidim'),
    ])

    tllq4f_JetMax1 = ('tllq4f_JetMax1',[
        pjoin(HPATH,spath,'special/tllq4f_EFT_t-channelJets_2019_08_07/v1/tllq_multidim_b1'),
        pjoin(HPATH,spath,'special/tllq4f_EFT_t-channelJets_2019_08_07/v1/tllq_multidim_b2')
    ])

    tllq4f_0p_NoMerge = ('tllq4f_0p_JetMax1',[
        pjoin(HPATH,spath,'special/tllq4f_EFT_t-channelNoJets_2019_10_01_HadFilterCheck/v1/tllq_multidim_NoMerge'),
    ])

    tllq4f_0p_JetMax1 = ('tllq4f_0p_JetMax1',[
        pjoin(HPATH,spath,'special/tllq4f_EFT_t-channelNoJets_2019_10_01_HadFilterCheck/v1/tllq_multidim_JetMax1'),
    ])

    tllq4f_0p_JetMax2 = ('tllq4f_0p_JetMax1',[
        pjoin(HPATH,spath,'special/tllq4f_EFT_t-channelNoJets_2019_10_01_HadFilterCheck/v1/tllq_multidim_JetMax2'),
    ])

    central_tZq = ('central_tZq',[
        # pjoin(HPATH,spath,'special/central_tZq_2019_09_20_v1/v1/tZq'),  # Uses v14-v1 sample
        pjoin(HPATH,spath,'central_sgnl_2019_07_31/v1/tZq'),            # Uses v14-v2 sample
    ])

    central_tZq_NoSkipHiggs = ('central_tZq_NoSkipHiggs',[
        pjoin(HPATH,spath,'special/central_tZq_2019_09_23_v14-v1_NoSkipHiggs/v1/tZq')
        # pjoin(hpath,spath,'special/central_tZq_2019_09_23_v14-v2_NoSkipHiggs/v1/tZq')
    ])

    central_ttH = ('central_ttH',[
        pjoin(HPATH,spath,'central_sgnl_2019_07_31/v1/ttH'),
    ])

    # samples = [tllq4f_JetMax2]
    samples = [central_tZq]
    for label,sample_dirs in samples:
        label_name = "{label}".format(label=label)
        if len(label_suffix):
            label_name = "{label}_{suffix}".format(label=label,suffix=label_suffix)
        label_name += "_{tstamp}".format(tstamp=TIMESTAMP)
        print "Processing: {l}".format(l=label_name)
        file_list = []
        print "Building file list:"
        for idx,fdir in enumerate(sample_dirs):
            print "\t[{0:0>{w}}/{1}] {dirpath}".format(idx+1,len(sample_dirs),w=1,dirpath=fdir)
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
        cmd_args = "\"{outf}\",\"{inf}\",{evts},{skim}".format(outf=outf,inf=inf,evts=events,skim=int(skim))
        # cmd.extend(["check_anaTrees.C(\"{outf}\", \"{inf}\", {evts}, {skim})".format(args=cmd_args)])
        cmd.extend(["check_anaTrees.C({args})".format(args=cmd_args)])
        subprocess.check_call(cmd)

#TODO: Switch the inputs to the 'all_samples' run
# Wrapper to run the 'read_anaTreeChecks.C' macro
def read_anaTreeChecks():
    print "Running read_anaTreeChecks()..."
    dir_name = 'main_comparisons'
    sub_dir = 'tllq4f_checks'
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
        # Custom ordering of the samples
        ##############################

        ("centralv2","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/central_tZq_v14v2"),
        ("tchYesH_01p_JM2","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_JetMax2"),
        ("tchYesH_01p_JM2_NSH","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_JetMax2_NoSkipHiggs"),
        ("tchYesH_0p_NoMrg","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_NoMerge"),
        ("tchYesH_0p_NoMrg_NSH","awightma/checkAnaTrees/test/lobster_test_20191003_1556/v1/tllq4f_0p_NoDim6_NoMerge_NoSkipHiggs"),
        ('MatchedNoH_JM1',"awightma/checkAnaTrees/tllq4f_MatchedNoHiggs0p_JetMax2_20191007_1044/v1/tllq4f_MatchedNoHiggs0p_JetMax2"),
        ('tchNoH_0p_NoMrg',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggs_0j_NoMerge"),
        # ('tchNoH_0p_MLM_NoMrg',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_20191007_2018/v1/tllq4f_tch_NoHiggsMLM_0j_NoMerge"),
        ('tchNoH_0p_xq0_NoMrg',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_xqcut0_20191008_0959/v1/tllq4f_tch_NoHiggs_0j_xqcut0_NoMerge"),
        ('tchNoH_0p_xq0_Stats',"awightma/checkAnaTrees/tllq4f_tch_NoHiggs_0j_xqcut0_2019-10-09_1808/v1/tllq4f_tch_NoHiggs_0j_xqcut0_NoMerge"),
    ]

    remake_merged_files = False # Force the recreation of the merged root files
    merge_lst = []
    for idx,tup in enumerate(inputs):
        name,sub_path = tup
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
    cmd.extend(["read_anaTreeChecks.C({infs})".format(infs=infs)])
    subprocess.check_call(cmd)

    return

    output_dir = pjoin(USER_DIR,'www/eft_stuff/misc/anacheck_plots',sub_dir,dir_name)
    print "Output Dir: {dir}".format(dir=output_dir)
    if not os.path.exists(output_dir):
        os.mkdir(output_dir)
    clean_dir(output_dir,["^.*\.png$","^index.html$"])
    imgs = get_files('.',targets=["^.*\.png$"])
    move_files(files=imgs,target=output_dir)

def main():
    # check_anaTrees()
    read_anaTreeChecks()


if __name__ == "__main__":
    main()
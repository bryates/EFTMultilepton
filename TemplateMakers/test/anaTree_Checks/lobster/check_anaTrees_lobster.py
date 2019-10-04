import datetime
from glob import glob
from lobster import cmssw
from lobster.core import AdvancedOptions, Category, Config, Dataset, StorageConfiguration, Workflow
import os

cmsswbase = os.environ['CMSSW_BASE']

tstamp1 = datetime.datetime.now().strftime('%Y%m%d_%H%M')
tstamp2 = datetime.datetime.now().strftime('%Y_%m_%d')
lobster_step = "checkAnaTrees"

master_label = 'EFT_T3_{step}_{tstamp}'.format(step=lobster_step,tstamp=tstamp1)
# master_label = 'EFT_CRC_{step}_{tstamp}'.format(step=lobster_step,tstamp=tstamp1)

ver = "v1"
tag = "test/lobster_test_{tstamp}".format(tstamp=tstamp1)

workdir_path = "{path}/{step}/{tag}/{ver}".format(step=lobster_step,tag=tag,ver=ver,path="/tmpscratch/users/$USER/analysisWorkflow")
plotdir_path = "{path}/{step}/{tag}/{ver}".format(step=lobster_step,tag=tag,ver=ver,path="~/www/lobster")
output_path  = "{path}/{step}/{tag}/{ver}".format(step=lobster_step,tag=tag,ver=ver,path="/store/user/$USER")

URI_HDFS   = "{scheme}://{host}:{port}".format(scheme="hdfs",host="eddie.crc.nd.edu",port="19000")
URI_ROOT   = "{scheme}://{host}/".format(scheme="root",host="deepthought.crc.nd.edu") # Note the extra slash after the hostname!
URI_GSIFTP = "{scheme}://{host}".format(scheme="gsiftp",host="T3_US_NotreDame")
URI_SRM    = "{scheme}://{host}".format(scheme="srm",host="T3_US_NotreDame")
URI_FILE   = "{scheme}://{host}/hadoop".format(scheme="file",host="")

input_path = "/store/user"
storage = StorageConfiguration(
    input=[
        "{uri}{path}".format(uri=URI_HDFS  ,path=input_path),
        "{uri}{path}".format(uri=URI_ROOT  ,path=input_path),
        "{uri}{path}".format(uri=URI_GSIFTP,path=input_path),
        "{uri}{path}".format(uri=URI_SRM   ,path=input_path),
    ],
    output=[
        "{uri}{path}".format(uri=URI_HDFS  ,path=output_path),
        "{uri}{path}".format(uri=URI_ROOT  ,path=output_path),
        "{uri}{path}".format(uri=URI_GSIFTP,path=output_path),
        "{uri}{path}".format(uri=URI_SRM   ,path=output_path),
        "{uri}{path}".format(uri=URI_FILE  ,path=output_path),
    ]
)

# NOTE1: The central_tZq files are ~250M each! The private are ~150M --> be careful about files_per_task
# NOTE2: The more recent private tllq samples are ~15M in size
processing = Category(
    name='processing',
    cores=1,
    memory=2500,
    disk=2900,
    # mode='fixed'
)

####################################################################################################
# This is where we define the locations of the input files. The input files should be the result
#   of running the analysisTree making code on some mAOD datasets/samples. The list part of the tuple
#   can be used to specify multiple directories to get the root files that should be run over (e.g.
#   in case of large stat samples produced over multiple lobster runs), just make sure that all of the
#   events were produced with the exact same setup!
####################################################################################################
sub_path = 'awightma/analysisTrees'
eft_fpt = 6
central_fpt = 10
eft_mergesize = '128M'  # EFT samples are ~15M per root file
central_mergesize = '256K'  # central samples are ~33K per root file
tllq4f_JetMax2 = ('tllq4f_JetMax2',eft_fpt,eft_mergesize,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax2_b1'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax2_b2'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax2_b3'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax2_b4'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax2_FPR5B3/v1/tllq_multidim_JetMax2'),
])

tllq4f_JetMax1 = ('tllq4f_JetMax1',eft_fpt,eft_mergesize,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax1_b1'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax1_b2'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax1_b3'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax1_b4'),
])

tllq4f_0p_NoMerge = ('tllq4f_0p_NoDim6_NoMerge',eft_fpt,central_mergesize,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelNoJets_2019_10_01_HadFilterCheck/v1/tllq_multidim_NoMerge'),
])

tllq4f_0p_JetMax1 = ('tllq4f_0p_NoDim6_JetMax1',eft_fpt,central_mergesize,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelNoJets_2019_10_01_HadFilterCheck/v1/tllq_multidim_JetMax1'),
])

tllq4f_0p_JetMax2 = ('tllq4f_0p_NoDim6_JetMax2',eft_fpt,central_mergesize,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelNoJets_2019_10_01_HadFilterCheck/v1/tllq_multidim_JetMax2'),
])

central_tZq_v1 = ('central_tZq_v14-v1',central_fpt,central_mergesize,[
    os.path.join(sub_path,'special/central_tZq_2019_09_20_v1/v1/tZq'),  # Uses v14-v1 sample
])

central_tZq_v2 = ('central_tZq_v14-v2',central_fpt,central_mergesize,[
    os.path.join(sub_path,'central_sgnl_2019_07_31/v1/tZq'),            # Uses v14-v2 sample
])
####################################################################################################
central_tZq_NoSkipHiggs_v1 = ('central_tZq_NoSkipHiggs_v14-v1',central_fpt,central_mergesize,[
    os.path.join(sub_path,'special/central_tZq_2019_09_23_v14-v1_NoSkipHiggs/v1/tZq')
])
central_tZq_NoSkipHiggs_v2 = ('central_tZq_NoSkipHiggs_v14-v2',central_fpt,central_mergesize,[
    os.path.join(sub_path,'special/central_tZq_2019_09_23_v14-v2_NoSkipHiggs/v1/tZq')
])
tllq4f_JetMax1_NoSkipHiggs = ('tllq4f_JetMax1_NoSkipHiggs',eft_fpt,eft_mergesize,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax1_b1'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax1_b2'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax1_b3'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax1_b4'),
])
tllq4f_JetMax2_NoSkipHiggs = ('tllq4f_JetMax2_NoSkipHiggs',eft_fpt,eft_mergesize,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax2_b1'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax2_b2'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax2_b3'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax2_b4'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax2_b5'),
])
tllq4f_0p_NoMerge_NoSkipHiggs = ('tllq4f_0p_NoDim6_NoMerge_NoSkipHiggs',eft_fpt,central_mergesize,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_0p_NoMerge'),
])
tllq4f_0p_JetMax1_NoSkipHiggs = ('tllq4f_0p_NoDim6_JetMax1_NoSkipHiggs',eft_fpt,central_mergesize,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_0p_JetMax1'),
])
tllq4f_0p_JetMax2_NoSkipHiggs = ('tllq4f_0p_NoDim6_JetMax2_NoSkipHiggs',eft_fpt,central_mergesize,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_0p_JetMax2'),
])
####################################################################################################
central_ttH = ('central_ttH',central_fpt,central_mergesize,[
    os.path.join(sub_path,'central_sgnl_2019_07_31/v1/ttH'),
])
####################################################################################################
samples = [
    tllq4f_JetMax1,
    tllq4f_JetMax2,
    central_tZq_v1,
    central_tZq_v2,
    tllq4f_0p_NoMerge,
    tllq4f_0p_JetMax1,
    tllq4f_0p_JetMax2,
    tllq4f_JetMax1_NoSkipHiggs,
    tllq4f_JetMax2_NoSkipHiggs,
    tllq4f_0p_NoMerge_NoSkipHiggs,
    tllq4f_0p_JetMax1_NoSkipHiggs,
    tllq4f_0p_JetMax2_NoSkipHiggs,
    central_tZq_NoSkipHiggs_v1,
    central_tZq_NoSkipHiggs_v2,
]

# Note: The 'interface' directory is automatically included
sandbox = cmssw.Sandbox(include=['EFTMultilepton/TemplateMakers/src'])

wf = []
for idx,tup in enumerate(samples):
    label,fpt,mergesize,dirs = tup

    print "[{n}/{tot}] Adding workflow: {label}".format(n=idx+1,tot=len(samples),label=label)
    for d in dirs:
        print "\tInput Dir: {dir}".format(dir=d)

    dataset=Dataset(
        files=dirs,
        files_per_task=fpt,
        patterns=["*.root"]
    )

    safe_label_name = label.replace('-','')
    output = Workflow(
        label=safe_label_name,
        dataset=dataset,
        category=processing,
        command='python check_anaTrees.py @inputfiles',
        extra_inputs=[
            'utils.h',
            'categories.h',
            'check_anaTrees.C',
            'check_anaTrees.py',
        ],
        outputs=['output.root'],
        cleanup_input=False,
        merge_size=mergesize,
        merge_command='hadd @outputfiles @inputfiles',
        sandbox=sandbox
    )
    wf.extend([output])

payload = max(25,len(samples)*15)
print "Payload Size: {0}".format(payload)
config = Config(
    label=master_label,
    workdir=workdir_path,
    plotdir=plotdir_path,
    storage=storage,
    workflows=wf,
    advanced=AdvancedOptions(
        payload=payload,
        bad_exit_codes=[127, 160],
        log_level=1,
        xrootd_servers=['ndcms.crc.nd.edu',
                       'cmsxrootd.fnal.gov',
                       'deepthought.crc.nd.edu']
    )
)
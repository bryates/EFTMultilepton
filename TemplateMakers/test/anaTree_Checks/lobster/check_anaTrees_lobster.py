import os
import sys

from datetime import datetime as dt
from lobster import cmssw
from lobster.core import AdvancedOptions, Category, Config, Dataset, StorageConfiguration, Workflow

cmsswbase = os.environ['CMSSW_BASE']

tstamp1 = dt.now().strftime('%Y%m%d_%H%M')
tstamp2 = dt.now().strftime('%Y_%m_%d')
tstamp3 = dt.now().strftime('%Y-%m-%d_%H%M')
lobster_step = "checkAnaTrees"

# master_label = 'EFT_T3_{step}_{tstamp}'.format(step=lobster_step,tstamp=tstamp1)
master_label = 'EFT_CRC_{step}_{tstamp}'.format(step=lobster_step,tstamp=tstamp1)

ver = "v1"
# tag = "test/lobster_test_{tstamp}".format(tstamp=tstamp1)
# tag = "tllq4f_tch_NoHiggs_0j_xqcut0_{tstamp}".format(tstamp=tstamp3)
# tag = "ana24-priv_ana25-priv_ana25-cent_{tstamp}".format(tstamp=tstamp3)
# tag = "ana26_PDF-fix_{tstamp}".format(tstamp=tstamp3)
# tag = "ana25-priv_ana25-cent_ana28-priv_ana29-priv_NoStreaming_{tstamp}".format(tstamp=tstamp2)
tag = "ana29-priv-NoDuplicates_HanOrig-HanV4-SMCheck_{tstamp}".format(tstamp=tstamp2)

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
    ],
    disable_input_streaming = True
)

# NOTE1: The central_tZq files are ~250M each! The private are ~150M --> be careful about files_per_task
# NOTE2: The more recent private tllq samples are ~15M in size
processing = Category(
    name='processing',
    cores=1,
    memory=2500,
    disk=2900,
    tasks_max=3000  # maximum tasks in the queue (running+waiting)
    # mode='fixed'
)

# From rateinfo.h file in MakeGoodPlot2
ttH_xsec = 0.2151
tHq_xsec = 7.7129e-2
tZq_xsec = 0.0942
ttZ_xsec = 0.2529
ttW_xsec = 0.2043

####################################################################################################
# This is where we define the locations of the input files. The input files should be the result
#   of running the analysisTree making code on some mAOD datasets/samples. The list part of the tuple
#   can be used to specify multiple directories to get the root files that should be run over (e.g.
#   in case of large stat samples produced over multiple lobster runs), just make sure that all of the
#   events were produced with the exact same setup!
####################################################################################################
sub_path = 'awightma/analysisTrees'
central_fpt = 6    # 10    # IMPORTANT: Use the smaller number when running with T3 resources!!
eft_mergesize = '1024M' #'256M'  # EFT samples are ~15M per root file
central_mergesize = '2048K' #'512K'  # central samples are ~33K per root file
tllq4f_JetMax2 = ('tllq4f_JetMax2',5,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax2_b1'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax2_b2'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax2_b3'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax2_b4'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax2_FPR5B3/v1/tllq_multidim_JetMax2'),
])
tllq4f_JetMax1 = ('tllq4f_JetMax1',5,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax1_b1'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax1_b2'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax1_b3'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelJets_2019_09_27_JetMax1-JetMax2_b1-b4/v1/tllq_multidim_JetMax1_b4'),
])
tllq4f_0p_NoMerge = ('tllq4f_0p_NoDim6_NoMerge',5,central_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelNoJets_2019_10_01_HadFilterCheck/v1/tllq_multidim_NoMerge'),
])
tllq4f_0p_JetMax1 = ('tllq4f_0p_NoDim6_JetMax1',5,central_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelNoJets_2019_10_01_HadFilterCheck/v1/tllq_multidim_JetMax1'),
])
tllq4f_0p_JetMax2 = ('tllq4f_0p_NoDim6_JetMax2',5,central_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channelNoJets_2019_10_01_HadFilterCheck/v1/tllq_multidim_JetMax2'),
])


tllq4f_tch_NoHiggs_0j_NoMerge = ('tllq4f_tch_NoHiggs_0j_NoMerge',3,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-ch_NoHiggs_0j_2019_10_07_NoMrg-JM1-JM2/v1/tllq_multidim_tch_NoHiggs_0j_NoMerge')
])
tllq4f_tch_NoHiggs_0j_JetMax1 = ('tllq4f_tch_NoHiggs_0j_JetMax1',3,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-ch_NoHiggs_0j_2019_10_07_NoMrg-JM1-JM2/v1/tllq_multidim_tch_NoHiggs_0j_JetMax1')
])
tllq4f_tch_NoHiggs_0j_JetMax2 = ('tllq4f_tch_NoHiggs_0j_JetMax2',3,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-ch_NoHiggs_0j_2019_10_07_NoMrg-JM1-JM2/v1/tllq_multidim_tch_NoHiggs_0j_JetMax2')
])

tllq4f_tch_NoHiggsMLM_0j_NoMerge = ('tllq4f_tch_NoHiggsMLM_0j_NoMerge',3,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-ch_NoHiggs_0j_2019_10_07_NoMrg-JM1-JM2/v1/tllq_multidim_tch_NoHiggsMLM_0j_NoMerge')
])
tllq4f_tch_NoHiggsMLM_0j_JetMax1 = ('tllq4f_tch_NoHiggsMLM_0j_JetMax1',3,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-ch_NoHiggs_0j_2019_10_07_NoMrg-JM1-JM2/v1/tllq_multidim_tch_NoHiggsMLM_0j_JetMax1')
])
tllq4f_tch_NoHiggsMLM_0j_JetMax2 = ('tllq4f_tch_NoHiggsMLM_0j_JetMax2',3,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-ch_NoHiggs_0j_2019_10_07_NoMrg-JM1-JM2/v1/tllq_multidim_tch_NoHiggsMLM_0j_JetMax2')
])

tllq4f_tch_NoHiggs_0j_xqcut0_NoMerge = ('tllq4f_tch_NoHiggs_0j_xqcut0_NoMerge',3,eft_mergesize,tZq_xsec,[
    #os.path.join(sub_path,'special/tllq4f_EFT_t-ch_NoHiggs_0j_2019_10_08_xqcut0_NoMrg-JM1-JM2/v1/tllq_multidim_tch_NoHiggs_0j_xqcut0_NoMerge')
    os.path.join(sub_path,'private_sgnl_2019_10_09/v1/tllq_multidim_b1'),
    os.path.join(sub_path,'private_sgnl_2019_10_09/v1/tllq_multidim_b2'),
])
tllq4f_tch_NoHiggs_0j_xqcut0_JetMax1 = ('tllq4f_tch_NoHiggs_0j_xqcut0_JetMax1',3,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-ch_NoHiggs_0j_2019_10_08_xqcut0_NoMrg-JM1-JM2/v1/tllq_multidim_tch_NoHiggs_0j_xqcut0_JetMax1')
])
tllq4f_tch_NoHiggs_0j_xqcut0_JetMax2 = ('tllq4f_tch_NoHiggs_0j_xqcut0_JetMax2',3,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-ch_NoHiggs_0j_2019_10_08_xqcut0_NoMrg-JM1-JM2/v1/tllq_multidim_tch_NoHiggs_0j_xqcut0_JetMax2')
])

#NOTE: This should have said 'JetMax1', but was mis-named when I ran over this sample
tllq4f_MatchedNoHiggs0p_JetMax2 = ('tllq4f_MatchedNoHiggs0p_JetMax2',3,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'private_sgnl_2019_08_05/v1/tllq_multidim_b1')
])

central_tZq_v1 = ('central_tZq_v14-v1',central_fpt,central_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/central_tZq_2019_09_20_v1/v1/tZq'),  # Uses v14-v1 sample
])
central_tZq_v2 = ('central_tZq_v14-v2',central_fpt,central_mergesize,tZq_xsec,[
    os.path.join(sub_path,'central_sgnl_2019_07_31/v1/tZq'),            # Uses v14-v2 sample
])
####################################################################################################
central_tZq_NoSkipHiggs_v1 = ('central_tZq_NoSkipHiggs_v14-v1',central_fpt,central_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/central_tZq_2019_09_23_v14-v1_NoSkipHiggs/v1/tZq')
])
central_tZq_NoSkipHiggs_v2 = ('central_tZq_NoSkipHiggs_v14-v2',central_fpt,central_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/central_tZq_2019_09_23_v14-v2_NoSkipHiggs/v1/tZq')
])
tllq4f_JetMax1_NoSkipHiggs = ('tllq4f_JetMax1_NoSkipHiggs',5,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax1_b1'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax1_b2'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax1_b3'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax1_b4'),
])
tllq4f_JetMax2_NoSkipHiggs = ('tllq4f_JetMax2_NoSkipHiggs',5,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax2_b1'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax2_b2'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax2_b3'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax2_b4'),
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_JetMax2_b5'),
])
tllq4f_0p_NoMerge_NoSkipHiggs = ('tllq4f_0p_NoDim6_NoMerge_NoSkipHiggs',5,central_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_0p_NoMerge'),
])
tllq4f_0p_JetMax1_NoSkipHiggs = ('tllq4f_0p_NoDim6_JetMax1_NoSkipHiggs',5,central_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_0p_JetMax1'),
])
tllq4f_0p_JetMax2_NoSkipHiggs = ('tllq4f_0p_NoDim6_JetMax2_NoSkipHiggs',5,central_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_EFT_t-channel_2019_10_03_NoSkipHiggs/v1/tllq_multidim_0p_JetMax2'),
])
####################################################################################################
central_ttH = ('central_ttH',central_fpt,central_mergesize,ttH_xsec,[
    os.path.join(sub_path,'central_sgnl_2019_07_31/v1/ttH'),
])
####################################################################################################
# anatest24 samples
####################################################################################################
a24_private_ttH   = ('a24_private_ttH'  ,3,eft_mergesize,ttH_xsec,[os.path.join(sub_path,'private_sgnl_2019_10_09/v1/ttH_multidim_b1')])
a24_private_tHq   = ('a24_private_tHq'  ,3,eft_mergesize,tHq_xsec,[os.path.join(sub_path,'private_sgnl_2019_10_09/v1/tHq_multidim_b1')])
a24_private_tllq  = ('a24_private_tllq' ,3,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'private_sgnl_2019_10_09/v1/tllq_multidim_b1'),
    os.path.join(sub_path,'private_sgnl_2019_10_09/v1/tllq_multidim_b2'),
])
a24_private_ttll  = ('a24_private_ttll' ,3,eft_mergesize,ttZ_xsec,[
    os.path.join(sub_path,'private_sgnl_2019_10_09/v1/ttll_multidim_b1'),
    os.path.join(sub_path,'private_sgnl_2019_10_09/v1/ttll_multidim_b2'),
])
a24_private_ttlnu = ('a24_private_ttlnu',3,eft_mergesize,ttW_xsec,[os.path.join(sub_path,'private_sgnl_2019_10_09/v1/ttlnu_multidim_b1')])

a24_central_ttH = ('a24_central_ttH',central_fpt,central_mergesize,ttH_xsec,[])
a24_central_tHq = ('a24_central_tHq',central_fpt,central_mergesize,tHq_xsec,[])
a24_central_tZq = ('a24_central_tZq',central_fpt,central_mergesize,tZq_xsec,[])
a24_central_ttZ = ('a24_central_ttZ',central_fpt,central_mergesize,ttZ_xsec,[])
a24_central_ttW = ('a24_central_ttW',central_fpt,central_mergesize,ttW_xsec,[])

####################################################################################################
# anatest25 samples
####################################################################################################
a25_private_ttH   = ('a25_private_ttH'  ,3,eft_mergesize,ttH_xsec,[os.path.join(sub_path,'private_sgnl_2019_10_11/v1/ttH_multidim_b1')])
a25_private_tHq   = ('a25_private_tHq'  ,3,eft_mergesize,tHq_xsec,[os.path.join(sub_path,'private_sgnl_2019_10_11/v1/tHq_multidim_b1')])
a25_private_tllq  = ('a25_private_tllq' ,3,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'private_sgnl_2019_10_11/v1/tllq_multidim_b1'),
    os.path.join(sub_path,'private_sgnl_2019_10_11/v1/tllq_multidim_b2'),
])
a25_private_ttll  = ('a25_private_ttll' ,3,eft_mergesize,ttZ_xsec,[
    os.path.join(sub_path,'private_sgnl_2019_10_11/v1/ttll_multidim_b1'),
    os.path.join(sub_path,'private_sgnl_2019_10_11/v1/ttll_multidim_b2'),
])
a25_private_ttlnu = ('a25_private_ttlnu',3,eft_mergesize,ttW_xsec,[os.path.join(sub_path,'private_sgnl_2019_10_11/v1/ttlnu_multidim_b1')])

a25_central_ttH = ('a25_central_ttH',central_fpt,central_mergesize,ttH_xsec,[os.path.join(sub_path,'central_sgnl_2019_10_11/v1/ttH')])
a25_central_tHq = ('a25_central_tHq',central_fpt,central_mergesize,tHq_xsec,[os.path.join(sub_path,'central_sgnl_2019_10_11/v1/tHq')])
a25_central_tZq = ('a25_central_tZq',central_fpt,central_mergesize,tZq_xsec,[os.path.join(sub_path,'central_sgnl_2019_10_11/v1/tZq')])
a25_central_ttZ = ('a25_central_ttZ',central_fpt,central_mergesize,ttZ_xsec,[os.path.join(sub_path,'central_sgnl_2019_10_11/v1/ttZ')])
a25_central_ttW = ('a25_central_ttW',central_fpt,central_mergesize,ttW_xsec,[os.path.join(sub_path,'central_sgnl_2019_10_11/v1/ttW')])

####################################################################################################
# anatest26 samples
#   Same as a25, except for tllq and tZq which used new PDFUP/PDFDOWN weights
####################################################################################################
a26_private_tllq  = ('a26_private_tllq' ,3,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,'special/tllq4f_2019_10_17_fixPDF/v1/tllq_multidim_b1'),
    os.path.join(sub_path,'special/tllq4f_2019_10_17_fixPDF/v1/tllq_multidim_b2'),
])
a26_central_tZq = ('a26_central_tZq',central_fpt,central_mergesize,tZq_xsec,[os.path.join(sub_path,'special/tZq_2019_10_17_fixPDF/v2/tZq')])

####################################################################################################
# anatest28 samples
####################################################################################################
sdir2 = 'special/private_sgnl_reprocFullWF-a28_NoStreaming_2019_11_09/v1'
a28_private_ttH = ('a28_private_ttH',1,eft_mergesize,ttH_xsec,[os.path.join(sub_path,sdir2,'ttH_multidim_b1')])
a28_private_tHq = ('a28_private_tHq',1,eft_mergesize,tHq_xsec,[os.path.join(sub_path,sdir2,'tHq_multidim_b1')])
a28_private_ttW = ('a28_private_ttW',1,eft_mergesize,ttW_xsec,[os.path.join(sub_path,sdir2,'ttlnu_multidim_b1')])
a28_private_ttZ = ('a28_private_ttZ',1,eft_mergesize,ttZ_xsec,[
    os.path.join(sub_path,sdir2,'ttll_multidim_b1'),
    os.path.join(sub_path,sdir2,'ttll_multidim_b2')
])
a28_private_tZq = ('a28_private_tZq',1,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,sdir2,'tllq_multidim_b1'),
    os.path.join(sub_path,sdir2,'tllq_multidim_b2')
])

# The central samples are mostly the same as was used in a25, except for ttH, which uses the 'new_pmx' version
a28_central_ttH = ([os.path.join(sub_path,'special/central_ttH-WW-WZ-ttGJets_new_pmx_2019_10_21/v3/ttH')])
a28_central_tHq = ([os.path.join(sub_path,'central_sgnl_2019_10_11/v1/tHq')])
a28_central_ttW = ([os.path.join(sub_path,'central_sgnl_2019_10_11/v1/ttW')])
a28_central_ttZ = ([os.path.join(sub_path,'central_sgnl_2019_10_11/v1/ttZ')])
a28_central_tZq = ([os.path.join(sub_path,'central_sgnl_2019_10_11/v1/tZq')])

####################################################################################################
# anatest29 samples
####################################################################################################
sdir2 = 'special/private_sgnl_reprocFullWF-a29_NoStreaming_2019_11_08/v1'
a29_private_ttH = ('a29_private_ttH',2,eft_mergesize,ttH_xsec,[
    os.path.join(sub_path,sdir2,'ttH_multidim_b1'),
    os.path.join(sub_path,sdir2,'ttH_multidim_b2'),
])
a29_private_tHq = ('a29_private_tHq',2,eft_mergesize,tHq_xsec,[os.path.join(sub_path,sdir2,'tHq_multidim_b1')])
a29_private_ttW = ('a29_private_ttW',2,eft_mergesize,ttW_xsec,[
    os.path.join(sub_path,sdir2,'ttlnu_multidim_b1'),
    os.path.join(sub_path,sdir2,'ttlnu_multidim_b2'),
])
a29_private_ttZ = ('a29_private_ttZ',2,eft_mergesize,ttZ_xsec,[
    os.path.join(sub_path,sdir2,'ttll_multidim_b1'),
    os.path.join(sub_path,sdir2,'ttll_multidim_b2'),
])
a29_private_tZq = ('a29_private_tZq',2,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,sdir2,'tllq_multidim_b1'),
    os.path.join(sub_path,sdir2,'tllq_multidim_b2'),
    os.path.join(sub_path,sdir2,'tllq_multidim_b3'),
])

####################################################################################################
# anatest29 samples (no duplicates)
####################################################################################################
sdir2 = 'special/private_sgnl_reprocFullWF-a29_NoDuplicates_2019_11_18/v1'
a29noDupes_private_ttH = ('a29noDupes_private_ttH',2,eft_mergesize,ttH_xsec,[
    os.path.join(sub_path,sdir2,'ttH_multidim_b1'),
    os.path.join(sub_path,sdir2,'ttH_multidim_b2'),
])
a29noDupes_private_tHq = ('a29noDupes_private_tHq',2,eft_mergesize,tHq_xsec,[os.path.join(sub_path,sdir2,'tHq_multidim_b1')])
a29noDupes_private_ttW = ('a29noDupes_private_ttW',2,eft_mergesize,ttW_xsec,[
    os.path.join(sub_path,sdir2,'ttlnu_multidim_b1'),
    os.path.join(sub_path,sdir2,'ttlnu_multidim_b2'),
])
a29noDupes_private_ttZ = ('a29noDupes_private_ttZ',2,eft_mergesize,ttZ_xsec,[
    os.path.join(sub_path,sdir2,'ttll_multidim_b1'),
    os.path.join(sub_path,sdir2,'ttll_multidim_b2'),
])
a29noDupes_private_tZq = ('a29noDupes_private_tZq',2,eft_mergesize,tZq_xsec,[
    os.path.join(sub_path,sdir2,'tllq_multidim_b1'),
    os.path.join(sub_path,sdir2,'tllq_multidim_b2'),
    os.path.join(sub_path,sdir2,'tllq_multidim_b3'),
])

####################################################################################################
# Dedicated SM check samples
####################################################################################################
sdir2 = 'special/ttXJet_HanModelOriginal_SMCheck_2019_11_18/v1'
HanOrig_private_ttH = ('ttH_HanOrigSMCheck',4,central_mergesize,ttH_xsec,[os.path.join(sub_path,sdir2,'ttH_multidim_b1')])
HanOrig_private_ttZ = ('ttZ_HanOrigSMCheck',4,central_mergesize,ttZ_xsec,[os.path.join(sub_path,sdir2,'ttll_multidim_b1')])
HanOrig_private_ttW = ('ttW_HanOrigSMCheck',4,central_mergesize,ttW_xsec,[os.path.join(sub_path,sdir2,'ttlnu_multidim_b1')])

sdir2 = 'special/ttXJet-tXq4f-HanV4SMCheck_b1-b2_2019_11_18/v1'
HanV4_private_ttH = ('ttH_HanV4SMCheck',4,central_mergesize,ttH_xsec,[
    os.path.join(sub_path,sdir2,'ttH_multidim_b1'),
    os.path.join(sub_path,sdir2,'ttH_multidim_b2'),
])
HanV4_private_ttZ = ('ttZ_HanV4SMCheck',4,central_mergesize,ttZ_xsec,[
    os.path.join(sub_path,sdir2,'ttll_multidim_b1'),
    os.path.join(sub_path,sdir2,'ttll_multidim_b2'),
])
HanV4_private_ttW = ('ttW_HanV4SMCheck',4,central_mergesize,ttW_xsec,[
    os.path.join(sub_path,sdir2,'ttlnu_multidim_b1'),
    os.path.join(sub_path,sdir2,'ttlnu_multidim_b2'),
])
HanV4_private_tZq = ('tZq_HanV4SMCheck',4,central_mergesize,tZq_xsec,[
    os.path.join(sub_path,sdir2,'tllq_multidim_b1'),
    os.path.join(sub_path,sdir2,'tllq_multidim_b2'),
])
HanV4_private_tHq = ('tHq_HanV4SMCheck',4,central_mergesize,tHq_xsec,[
    os.path.join(sub_path,sdir2,'tHq_multidim_b1'),
    os.path.join(sub_path,sdir2,'tHq_multidim_b2'),
])
####################################################################################################

samples = [
    # central_tZq_v1,
    # central_tZq_v2,
    # tllq4f_JetMax1,
    # tllq4f_JetMax2,
    # tllq4f_0p_NoMerge,
    # tllq4f_0p_JetMax1,
    # tllq4f_0p_JetMax2,
    # tllq4f_MatchedNoHiggs0p_JetMax2,
    # tllq4f_tch_NoHiggs_0j_NoMerge,
    # tllq4f_tch_NoHiggs_0j_JetMax1,
    # tllq4f_tch_NoHiggs_0j_JetMax2,
    # tllq4f_tch_NoHiggsMLM_0j_NoMerge,
    # tllq4f_tch_NoHiggsMLM_0j_JetMax1,
    # tllq4f_tch_NoHiggsMLM_0j_JetMax2,
    # tllq4f_tch_NoHiggs_0j_xqcut0_NoMerge,
    # tllq4f_tch_NoHiggs_0j_xqcut0_JetMax1,
    # tllq4f_tch_NoHiggs_0j_xqcut0_JetMax2,
    # tllq4f_JetMax1_NoSkipHiggs,
    # tllq4f_JetMax2_NoSkipHiggs,
    # tllq4f_0p_NoMerge_NoSkipHiggs,
    # tllq4f_0p_JetMax1_NoSkipHiggs,
    # tllq4f_0p_JetMax2_NoSkipHiggs,
    # central_tZq_NoSkipHiggs_v1,
    # central_tZq_NoSkipHiggs_v2,

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

    # a25_central_ttH,
    # a25_central_tHq,
    # a25_central_tZq,
    # a25_central_ttZ,
    # a25_central_ttW,

    # a26_private_tllq,
    # a26_central_tZq,

    # a28_private_ttH,
    # a28_private_tHq,
    # a28_private_ttW,
    # a28_private_ttZ,
    # a28_private_tZq,

    # a29_private_ttH,
    # a29_private_tHq,
    # a29_private_ttW,
    # a29_private_ttZ,
    # a29_private_tZq,

    a29noDupes_private_ttH,
    a29noDupes_private_tHq,
    a29noDupes_private_ttW,
    a29noDupes_private_ttZ,
    a29noDupes_private_tZq,

    HanOrig_private_ttH,
    HanOrig_private_ttZ,
    HanOrig_private_ttW,

    HanV4_private_ttH,
    HanV4_private_ttZ,
    HanV4_private_ttW,
    HanV4_private_tZq,
    HanV4_private_tHq,
]

# Note: The 'interface' directory is automatically included
sandbox = cmssw.Sandbox(include=['EFTMultilepton/TemplateMakers/src'])

wf = []
for idx,tup in enumerate(samples):
    label,fpt,mergesize,xsec,dirs = tup

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
        command='python check_anaTrees.py {xsec} @inputfiles'.format(xsec=xsec),
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
payload = min(250,payload)
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
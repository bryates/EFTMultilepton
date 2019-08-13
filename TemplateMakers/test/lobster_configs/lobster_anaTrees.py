import datetime
from glob import glob
from lobster import cmssw
from lobster.core import AdvancedOptions, Category, Config, Dataset, StorageConfiguration, Workflow
import os

isdata = False # remember to change osTwoLep_cfg.py
doeftsamps = True
singleSamp = None # default, to be overridden below if desired

tstamp1 = datetime.datetime.now().strftime('%Y%m%d_%H%M')
tstamp2 = datetime.datetime.now().strftime('%Y_%m_%d')
lobster_step = "analysisTrees"

#master_label = 'EFT_T3_anaTrees_{tstamp}'.format(tstamp=tstamp1)
master_label = 'EFT_CRC_anaTrees_{tstamp}'.format(tstamp=tstamp1)

ver = "v1"
#tag = "test/lobster_test_{tstamp}".format(tstamp=tstamp1)
tag = "special/tllq4f_SM_t-channel_{tstamp}_0partons".format(tstamp=tstamp2)
#tag = "central_sgnl_{tstamp}".format(tstamp=tstamp2)
#tag = "central_bkgd_{tstamp}".format(tstamp=tstamp2)
#tag = "private_sgnl_{tstamp}".format(tstamp=tstamp2)
#tag = "data2017_{tstamp}".format(tstamp=tstamp2)

workdir_path = "{path}/{step}/{tag}/{ver}".format(step=lobster_step,tag=tag,ver=ver,path="/tmpscratch/users/$USER")
plotdir_path = "{path}/{step}/{tag}/{ver}".format(step=lobster_step,tag=tag,ver=ver,path="~/www/lobster")
output_path  = "{path}/{step}/{tag}/{ver}".format(step=lobster_step,tag=tag,ver=ver,path="/store/user/$USER")


#### If you set singleSamp to ['label','dataset'] below, it will override all samples with a single sample
#singleSamp = ['ttH_NLO','/ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM']
#singleSamp = ['ttH_LO_old','/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM']
#singleSamp = ['ttJets','/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'] # couldn't find split decay samples
#singleSamp = ['ttH','/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM']
#singleSamp = ['ttH_multidim','mAOD_step_ttH_16DRobertSetupFullScanFullScan_run0/']
#singleSamp = ['tHq_multidim','mAOD_step_tHq_16DttllScanpoints_run1/']

cmsswbase = os.environ['CMSSW_BASE']
hadoop_loc = "" # Gets overwritten for private EFT samples

storage = StorageConfiguration(
    output=[
        "hdfs://eddie.crc.nd.edu:19000"  + output_path,
        # ND is not in the XrootD redirector, thus hardcode server.
        "root://deepthought.crc.nd.edu/" + output_path, # Note the extra slash after the hostname!
        "gsiftp://T3_US_NotreDame"       + output_path,
        "srm://T3_US_NotreDame"          + output_path,
        "file:///hadoop"                 + output_path,
    ]
)


processing = Category(
    name='processing',
    cores=1,
    #runtime=60 * 30,  # Time in seconds
    memory=2500,
    disk=2000
)

mysamples = []
lumimask = None
supplement = ""

## Add the MC ...
if ((not isdata) and (not doeftsamps)): # eventually rm the "not doeftsamps" here if running over all the mc at once
    #### 2016 ####
    # mysamples.append(['DYJets_M10to50','/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM'])
    # mysamples.append(['DYJets_M50','/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM'])
    # mysamples.append(['SingleTop_schan','/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'])
    # mysamples.append(['SingleTop_tWchan_antitop','/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'])
    # mysamples.append(['SingleTop_tWchan_top','/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'])
    # mysamples.append(['SingleTop_tchan_antitop','/ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-herwigpp/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'])
    # mysamples.append(['SingleTop_tchan_top','/ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-herwigpp/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'])
    # mysamples.append(['WJets','/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'])
    # mysamples.append(['WW','/WW_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'])
    # mysamples.append(['WZ','/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/MINIAODSIM'])
    # mysamples.append(['ZZ','/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'])
    # mysamples.append(['ttH','/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'])
    # mysamples.append(['ttJets_Dilept','/TTJets_Dilept_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'])
    # mysamples.append(['ttJets_SingleLeptFromT','/TTJets_SingleLeptFromT_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'])
    # mysamples.append(['ttJets_SingleLeptFromTbar','/TTJets_SingleLeptFromTbar_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'])
    # mysamples.append(['ttW','/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM'])
    # mysamples.append(['ttZ','/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM'])
    # mysamples.append(['WWW','/WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'])
    # mysamples.append(['WWZ','/WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'])
    # mysamples.append(['WZZ','/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'])
    # mysamples.append(['ZZZ','/ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'])
    # mysamples.append(['tZq','/tZq_ll_4f_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'])
    # mysamples.append(['tttt','/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'])
    # mysamples.append(['ttWW','/TTWW_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/MINIAODSIM'])
    # mysamples.append(['ttWZ','/TTWZ_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM'])
    
    #### 2017 ####
    # #mysamples.append(['DYJets_M50','/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM'])
    # mysamples.append(['DYJets_M50','/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM'])
    # #mysamples.append(['SingleTop_schan','/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM'])
    # mysamples.append(['SingleTop_schan','/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # #mysamples.append(['SingleTop_tWchan_antitop','/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM'])
    # mysamples.append(['SingleTop_tWchan_antitop','/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # #mysamples.append(['SingleTop_tWchan_top','/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM'])
    # mysamples.append(['SingleTop_tWchan_top','/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # #mysamples.append(['SingleTop_tchan_antitop','/ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM'])
    # mysamples.append(['SingleTop_tchan_antitop','/ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # #mysamples.append(['SingleTop_tchan_top','/ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM'])
    # mysamples.append(['SingleTop_tchan_top','/ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # #mysamples.append(['ZZ','/ZZTo4L_13TeV_powheg_pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10_ext1-v1/MINIAODSIM']) ## to update
    # mysamples.append(['ZZ','/ZZTo4L_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # #mysamples.append(['ttH','/ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM'])
    # #mysamples.append(['ttH','/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # mysamples.append(['ttH','/ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # 
    # #mysamples.append(['ttJets_Dilept','/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v11-v1/MINIAODSIM']) ## in production (?)
    # #mysamples.append(['ttJets_SingleLeptFromT','/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM'])    
    # #mysamples.append(['ttJets_SingleLeptFromTbar','/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v11-v1/MINIAODSIM']) ## in production
    # mysamples.append(['ttJets','/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM']) # couldn't find split decay samples    
    # 
    # #mysamples.append(['ttW','/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v2/MINIAODSIM'])
    # mysamples.append(['ttW','/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # #mysamples.append(['ttZ','/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM'])
    # mysamples.append(['ttZ','/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # #mysamples.append(['WWW','/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v11-v1/MINIAODSIM']) ## available ??
    # mysamples.append(['WWW','/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])     #a
    # #mysamples.append(['WWZ','/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v11-v1/MINIAODSIM']) ## available ??
    # mysamples.append(['WWZ','/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])     #a
    # #mysamples.append(['WZZ','/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v11-v1/MINIAODSIM'])
    # mysamples.append(['WZZ','/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])        #a
    # #mysamples.append(['ZZZ','/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v11-v1/MINIAODSIM'])
    # mysamples.append(['ZZZ','/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])        #a
    # #mysamples.append(['tZq','/tZq_ll_4f_ckm_NLO_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM'])
    # mysamples.append(['tZq','/tZq_ll_4f_ckm_NLO_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # #mysamples.append(['ttWW','/TTWW_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10_ext1-v2/MINIAODSIM'])
    # mysamples.append(['ttWW','/TTWW_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM'])
    # #mysamples.append(['ttWZ','/TTWZ_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v2/MINIAODSIM']) ## available ??
    # mysamples.append(['ttWZ','/TTWZ_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # 
    # mysamples.append(['WJets','/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'])
    # 
    # mysamples.append(['ttGJets','/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])            #a
    # mysamples.append(['ttGJets_ext','/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'])   #a 
    # 
    # # just added:
    # mysamples.append(['WW','/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # mysamples.append(['WZ','/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])                        #a
    
    
    #new:
    #/TTTW_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM
    
    #couldn't find PU2017_12Apr2018:
    #mysamples.append(['tttt','/TTTT_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v2/MINIAODSIM'])
    #mysamples.append(['WW','/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM']) ## to update
    #mysamples.append(['WZ','/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v2/MINIAODSIM'])
    #mysamples.append(['DYJets_M10to50','/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v2/MINIAODSIM'])
    
    
    
    
    
    # cherry-pick 2017, no ddbrs:
    # "b"
    #mysamples.append(['WW','/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    #mysamples.append(['ZZ','/ZZTo4L_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])    
    
    # "a"
    #mysamples.append(['WWW','/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    #mysamples.append(['WWZ','/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    #mysamples.append(['WZZ','/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    #mysamples.append(['ZZZ','/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    #mysamples.append(['WZ','/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    #mysamples.append(['ttGJets','/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    #mysamples.append(['ttGJets_ext','/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'])
    
    #mysamples.append(['ttWW','/TTWW_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM'])
    #mysamples.append(['ttWZ','/TTWZ_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])

    mysamples.append(['ttH','/ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])                 #a
    mysamples.append(['tHq','/THQ_4f_Hincl_13TeV_madgraph_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])                                           #whatever (doesn't have any pdf stuff)

    ## for preshower thing:
    mysamples.append(['ttW','/TTWJetsToLNu_TuneCP5_PSweights_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM'])     #a
    mysamples.append(['ttZ','/TTZToLLNuNu_M-10_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])                     #a
    mysamples.append(['tZq','/tZq_ll_4f_ckm_NLO_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/MINIAODSIM'])            #a

if ((not isdata) and doeftsamps):

    #mysamples.append(['tllq_cbW','mAOD_step_tllq_cbW_run2/'])           
    #mysamples.append(['tllq_cpQ3','mAOD_step_tllq_cpQ3_run2/'])          
    #mysamples.append(['tllq_cpQM','mAOD_step_tllq_cpQM_run2/'])          
    #mysamples.append(['tllq_cptb','mAOD_step_tllq_cptb_run2/'])          
    #mysamples.append(['tllq_cpt','mAOD_step_tllq_cpt_run2/'])          
    #mysamples.append(['tllq_cQe1','mAOD_step_tllq_cQe1_run2/'])         
    #mysamples.append(['tllq_ctG','mAOD_step_tllq_ctG_run2/'])          
    #mysamples.append(['tllq_ctl1','mAOD_step_tllq_ctl1_run2/'])         
    #mysamples.append(['tllq_ctp','mAOD_step_tllq_ctp_run2/'])
    #mysamples.append(['tllq_ctW','mAOD_step_tllq_ctW_run2/'])
    #mysamples.append(['tllq_ctZ','mAOD_step_tllq_ctZ_run2/'])
    #mysamples.append(['ttH_cbW','mAOD_step_ttH_cbW_run2/'])
    #mysamples.append(['ttH_cpQ3','mAOD_step_ttH_cpQ3_run2/'])
    #mysamples.append(['ttH_cpQM','mAOD_step_ttH_cpQM_run2/'])
    #mysamples.append(['ttH_cptb','mAOD_step_ttH_cptb_run2/'])
    #mysamples.append(['ttH_cpt','mAOD_step_ttH_cpt_run2/'])
    #mysamples.append(['ttH_cQe1','mAOD_step_ttH_cQe1_run2/'])
    #mysamples.append(['ttH_ctG','mAOD_step_ttH_ctG_run2/'])
    #mysamples.append(['ttH_ctl1','mAOD_step_ttH_ctl1_run2/'])
    #mysamples.append(['ttH_ctp','mAOD_step_ttH_ctp_run2/'])
    #mysamples.append(['ttH_ctW','mAOD_step_ttH_ctW_run2/'])  
    #mysamples.append(['ttH_ctZ','mAOD_step_ttH_ctZ_run2/'])  
    #mysamples.append(['ttll_cbW','mAOD_step_ttll_cbW_run2/']) 
    #mysamples.append(['ttll_cpQ3','mAOD_step_ttll_cpQ3_run2/'])
    #mysamples.append(['ttll_cpQM','mAOD_step_ttll_cpQM_run2/'])
    #mysamples.append(['ttll_cptb','mAOD_step_ttll_cptb_run2/'])
    #mysamples.append(['ttll_cpt','mAOD_step_ttll_cpt_run2/'])
    #mysamples.append(['ttll_cQe1','mAOD_step_ttll_cQe1_run2/'])
    #mysamples.append(['ttll_ctG','mAOD_step_ttll_ctG_run2/'])
    #mysamples.append(['ttll_ctl1','mAOD_step_ttll_ctl1_run2/'])
    #mysamples.append(['ttll_ctp','mAOD_step_ttll_ctp_run2/'])
    #mysamples.append(['ttll_ctW','mAOD_step_ttll_ctW_run2/']) 
    #mysamples.append(['ttll_ctZ','mAOD_step_ttll_ctZ_run2/'])  
    #mysamples.append(['ttlnu_cbW','mAOD_step_ttlnu_cbW_run2/']) 
    #mysamples.append(['ttlnu_cpQ3','mAOD_step_ttlnu_cpQ3_run2/'])
    #mysamples.append(['ttlnu_cpQM','mAOD_step_ttlnu_cpQM_run2/'])
    #mysamples.append(['ttlnu_cptb','mAOD_step_ttlnu_cptb_run2/'])
    #mysamples.append(['ttlnu_cpt','mAOD_step_ttlnu_cpt_run2/'])
    #mysamples.append(['ttlnu_cQe1','mAOD_step_ttlnu_cQe1_run2/'])
    #mysamples.append(['ttlnu_ctG','mAOD_step_ttlnu_ctG_run2/'])
    #mysamples.append(['ttlnu_ctl1','mAOD_step_ttlnu_ctl1_run2/'])
    #mysamples.append(['ttlnu_ctp','mAOD_step_ttlnu_ctp_run2/'])
    #mysamples.append(['ttlnu_ctW','mAOD_step_ttlnu_ctW_run2/'])
    #mysamples.append(['ttlnu_ctZ','mAOD_step_ttlnu_ctZ_run2/'])
    
    
    ### current round = round 4! Do not mix rounds! When in doubt, check the twiki and/or ask Andrew!
    # mysamples.append(['ttH_multidim_batch1','Batch1/postLHE_step/v1/mAOD_step_ttH_16DOldLimitsAxisScan_run1/'])
    # mysamples.append(['ttH_multidim_batch2','Batch2/postLHE_step/v1/mAOD_step_ttH_16DOldLimitsAxisScan_run1/'])
    # mysamples.append(['ttH_multidim_batch3','Batch3/postLHE_step/v1/mAOD_step_ttH_16DOldLimitsAxisScan_run1/'])
    # mysamples.append(['ttH_multidim_batch4','Batch4/postLHE_step/v1/mAOD_step_ttH_16DOldLimitsAxisScan_run1/'])
    # mysamples.append(['ttH_multidim_batch5','Batch5/postLHE_step/v1/mAOD_step_ttH_16DOldLimitsAxisScan_run1/'])
    # 
    # mysamples.append(['ttlnu_multidim_batch1','Batch1/postLHE_step/v1/mAOD_step_ttlnu_16DOldLimitsAxisScan_run1/'])
    # mysamples.append(['ttlnu_multidim_batch2','Batch2/postLHE_step/v1/mAOD_step_ttlnu_16DOldLimitsAxisScan_run1/'])
    # mysamples.append(['ttlnu_multidim_batch3','Batch3/postLHE_step/v1/mAOD_step_ttlnu_16DOldLimitsAxisScan_run1/'])
    # mysamples.append(['ttlnu_multidim_batch4','Batch4/postLHE_step/v1/mAOD_step_ttlnu_16DOldLimitsAxisScan_run1/'])
    # mysamples.append(['ttlnu_multidim_batch5','Batch5/postLHE_step/v1/mAOD_step_ttlnu_16DOldLimitsAxisScan_run1/'])
    # 
    # mysamples.append(['ttll_multidim_batch1','Batch1/postLHE_step/v1/mAOD_step_ttll_16DOldLimitsAxisScan_run1/'])
    # mysamples.append(['ttll_multidim_batch2','Batch2/postLHE_step/v1/mAOD_step_ttll_16DOldLimitsAxisScan_run1/'])
    # mysamples.append(['ttll_multidim_batch3','Batch3/postLHE_step/v1/mAOD_step_ttll_16DOldLimitsAxisScan_run1/'])
    # 
    # mysamples.append(['tllq_multidim_batch1','Batch1/postLHE_step/v1/mAOD_step_tllq_16DOldLimitsAxisScan_run1/'])
    # mysamples.append(['tllq_multidim_batch2','Batch2/postLHE_step/v1/mAOD_step_tllq_16DOldLimitsAxisScan_run1/'])
    # mysamples.append(['tllq_multidim_batch3','Batch3/postLHE_step/v1/mAOD_step_tllq_16DOldLimitsAxisScan_run1/'])
    # 
    # mysamples.append(['tHq_multidim_batch6','Batch6/postLHE_step/v1/mAOD_step_tHq_16DttllScanpoints_run1/'])

    ### Private EFT samples
    #hadoop_loc = "/store/user/awightma/FullProduction/Round5/"
    # ttH
    #mysamples.append(['ttH_multidim_b1','Batch1/postLHE_step/v1/mAOD_step_ttHJet_HanModel16DttllScanpoints_run1/'])
    # ttlnu
    #mysamples.append(['ttlnu_multidim_b1','Batch1/postLHE_step/v1/mAOD_step_ttlnuJet_HanModel16DttllScanpoints_run1/'])
    # ttll
    #mysamples.append(['ttll_multidim_b1','Batch1/postLHE_step/v1/mAOD_step_ttllNuNuJetNoHiggs_HanModel16DttllScanpoints_run1/'])
    #mysamples.append(['ttll_multidim_b2','Batch2/postLHE_step/v2/mAOD_step_ttllNuNuJetNoHiggs_HanModel16DttllScanpoints_run1/'])
    # tHq
    #mysamples.append(['tHq_multidim_b1','Batch1/postLHE_step/v1/mAOD_step_tHq4fMatched_HanModel16DttllScanpoints_run1/'])
    # tllq
    #mysamples.append(['tllq_multidim_b1','Batch1/postLHE_step/v1/mAOD_step_tllq4fMatchedNoHiggs_HanModel16DttllScanpoints_run1/'])

    hadoop_loc = '/store/user/awightma/'
    #mysamples.append(['tllq_multidim_b1','postLHE_step/2019_04_19/tllq4f_t-channelMatched_pythia-JetMax1/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10_run1/'])
    #mysamples.append(['tllq_multidim_b2','postLHE_step/2019_04_19/tllq4f_t-channelMatched_pythia-JetMax1_b2/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10_run1/'])

    mysamples.append(['tllq_multidim_0partons','postLHE_step/2019_04_19/tllq4f-NoDim6Diagrams/v2/mAOD_step_tllq4f_NoDim6NoSchanW_run0/'])
    #singleSamp = ['tllq_multidim_b1','postLHE_step/2019_04_19/tllq4f_t-channelMatched_pythia-JetMax1/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10_run1/']
    #singleSamp = ['tllq_multidim_b2','postLHE_step/2019_04_19/tllq4f_t-channelMatched_pythia-JetMax1_b2/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10_run1/']

## Add the data...
if (isdata):
    #### 2016 ####
    # mysamples.append(['SingleMuon_Run2016Bv1','/SingleMuon/Run2016B-07Aug17_ver1-v1/MINIAOD'])
    # mysamples.append(['SingleMuon_Run2016Bv2','/SingleMuon/Run2016B-07Aug17_ver2-v1/MINIAOD'])
    # mysamples.append(['SingleMuon_Run2016C','/SingleMuon/Run2016C-07Aug17-v1/MINIAOD'])
    # mysamples.append(['SingleMuon_Run2016D','/SingleMuon/Run2016D-07Aug17-v1/MINIAOD'])
    # mysamples.append(['SingleMuon_Run2016E','/SingleMuon/Run2016E-07Aug17-v1/MINIAOD'])
    # mysamples.append(['SingleMuon_Run2016F','/SingleMuon/Run2016F-07Aug17-v1/MINIAOD'])
    # mysamples.append(['SingleMuon_Run2016G','/SingleMuon/Run2016G-07Aug17-v1/MINIAOD'])
    # mysamples.append(['SingleMuon_Run2016H','/SingleMuon/Run2016H-07Aug17-v1/MINIAOD'])
    # 
    # mysamples.append(['DoubleMuon_Run2016H','/DoubleMuon/Run2016H-07Aug17-v1/MINIAOD'])
    # mysamples.append(['DoubleMuon_Run2016D','/DoubleMuon/Run2016D-07Aug17-v1/MINIAOD'])
    # mysamples.append(['DoubleMuon_Run2016E','/DoubleMuon/Run2016E-07Aug17-v1/MINIAOD'])
    # mysamples.append(['DoubleMuon_Run2016Bv2','/DoubleMuon/Run2016B-07Aug17_ver2-v1/MINIAOD'])
    # mysamples.append(['DoubleMuon_Run2016G','/DoubleMuon/Run2016G-07Aug17-v1/MINIAOD'])
    # mysamples.append(['DoubleMuon_Run2016C','/DoubleMuon/Run2016C-07Aug17-v1/MINIAOD'])
    # mysamples.append(['DoubleMuon_Run2016F','/DoubleMuon/Run2016F-07Aug17-v1/MINIAOD'])
    # mysamples.append(['DoubleMuon_Run2016Bv1','/DoubleMuon/Run2016B-07Aug17_ver1-v1/MINIAOD'])
    # 
    # mysamples.append(['SingleElectron_Run2016H','/SingleElectron/Run2016H-07Aug17-v1/MINIAOD'])
    # mysamples.append(['SingleElectron_Run2016D','/SingleElectron/Run2016D-07Aug17-v1/MINIAOD'])
    # mysamples.append(['SingleElectron_Run2016C','/SingleElectron/Run2016C-07Aug17-v1/MINIAOD'])
    # mysamples.append(['SingleElectron_Run2016G','/SingleElectron/Run2016G-07Aug17-v1/MINIAOD'])
    # mysamples.append(['SingleElectron_Run2016E','/SingleElectron/Run2016E-07Aug17-v1/MINIAOD'])
    # mysamples.append(['SingleElectron_Run2016Bv1','/SingleElectron/Run2016B-07Aug17_ver1-v1/MINIAOD'])
    # mysamples.append(['SingleElectron_Run2016F','/SingleElectron/Run2016F-07Aug17-v1/MINIAOD'])
    # mysamples.append(['SingleElectron_Run2016Bv2','/SingleElectron/Run2016B-07Aug17_ver2-v2/MINIAOD'])
    # 
    # mysamples.append(['DoubleEG_Run2016H','/DoubleEG/Run2016H-07Aug17-v1/MINIAOD'])
    # mysamples.append(['DoubleEG_Run2016F','/DoubleEG/Run2016F-07Aug17-v1/MINIAOD'])
    # mysamples.append(['DoubleEG_Run2016D','/DoubleEG/Run2016D-07Aug17-v1/MINIAOD'])
    # mysamples.append(['DoubleEG_Run2016E','/DoubleEG/Run2016E-07Aug17-v1/MINIAOD'])
    # mysamples.append(['DoubleEG_Run2016G','/DoubleEG/Run2016G-07Aug17-v1/MINIAOD'])
    # mysamples.append(['DoubleEG_Run2016Bv1','/DoubleEG/Run2016B-07Aug17_ver1-v1/MINIAOD'])
    # mysamples.append(['DoubleEG_Run2016C','/DoubleEG/Run2016C-07Aug17-v1/MINIAOD'])
    # mysamples.append(['DoubleEG_Run2016Bv2','/DoubleEG/Run2016B-07Aug17_ver2-v2/MINIAOD'])
    # 
    # mysamples.append(['MuonEG_Run2016Bv2','/MuonEG/Run2016B-07Aug17_ver2-v1/MINIAOD'])
    # mysamples.append(['MuonEG_Run2016H','/MuonEG/Run2016H-07Aug17-v1/MINIAOD'])
    # mysamples.append(['MuonEG_Run2016G','/MuonEG/Run2016G-07Aug17-v1/MINIAOD'])
    # mysamples.append(['MuonEG_Run2016C','/MuonEG/Run2016C-07Aug17-v1/MINIAOD'])
    # mysamples.append(['MuonEG_Run2016E','/MuonEG/Run2016E-07Aug17-v1/MINIAOD'])
    # mysamples.append(['MuonEG_Run2016F','/MuonEG/Run2016F-07Aug17-v1/MINIAOD'])
    # mysamples.append(['MuonEG_Run2016Bv1','/MuonEG/Run2016B-07Aug17_ver1-v1/MINIAOD'])
    # mysamples.append(['MuonEG_Run2016D','/MuonEG/Run2016D-07Aug17-v1/MINIAOD'])

    #### 2017 ####
    mysamples.append(['SingleElectron_Run2017B','/SingleElectron/Run2017B-31Mar2018-v1/MINIAOD'])  
    mysamples.append(['SingleElectron_Run2017C','/SingleElectron/Run2017C-31Mar2018-v1/MINIAOD'])
    mysamples.append(['SingleElectron_Run2017D','/SingleElectron/Run2017D-31Mar2018-v1/MINIAOD'])
    mysamples.append(['SingleElectron_Run2017E','/SingleElectron/Run2017E-31Mar2018-v1/MINIAOD'])
    mysamples.append(['SingleElectron_Run2017F','/SingleElectron/Run2017F-31Mar2018-v1/MINIAOD'])

    mysamples.append(['SingleMuon_Run2017B','/SingleMuon/Run2017B-31Mar2018-v1/MINIAOD'])
    mysamples.append(['SingleMuon_Run2017C','/SingleMuon/Run2017C-31Mar2018-v1/MINIAOD'])
    mysamples.append(['SingleMuon_Run2017D','/SingleMuon/Run2017D-31Mar2018-v1/MINIAOD'])
    mysamples.append(['SingleMuon_Run2017E','/SingleMuon/Run2017E-31Mar2018-v1/MINIAOD'])
    mysamples.append(['SingleMuon_Run2017F','/SingleMuon/Run2017F-31Mar2018-v1/MINIAOD'])
    mysamples.append(['SingleMuon_Run2017H','/SingleMuon/Run2017H-31Mar2018-v1/MINIAOD'])

    mysamples.append(['DoubleEG_Run2017B','/DoubleEG/Run2017B-31Mar2018-v1/MINIAOD'])
    mysamples.append(['DoubleEG_Run2017C','/DoubleEG/Run2017C-31Mar2018-v1/MINIAOD'])
    mysamples.append(['DoubleEG_Run2017D','/DoubleEG/Run2017D-31Mar2018-v1/MINIAOD'])
    mysamples.append(['DoubleEG_Run2017E','/DoubleEG/Run2017E-31Mar2018-v1/MINIAOD'])
    mysamples.append(['DoubleEG_Run2017F','/DoubleEG/Run2017F-31Mar2018-v1/MINIAOD'])

    mysamples.append(['DoubleMuon_Run2017B','/DoubleMuon/Run2017B-31Mar2018-v1/MINIAOD'])
    mysamples.append(['DoubleMuon_Run2017C','/DoubleMuon/Run2017C-31Mar2018-v1/MINIAOD'])
    mysamples.append(['DoubleMuon_Run2017D','/DoubleMuon/Run2017D-31Mar2018-v1/MINIAOD'])
    mysamples.append(['DoubleMuon_Run2017E','/DoubleMuon/Run2017E-31Mar2018-v1/MINIAOD'])
    mysamples.append(['DoubleMuon_Run2017F','/DoubleMuon/Run2017F-31Mar2018-v1/MINIAOD'])

    mysamples.append(['MuonEG_Run2017B','/MuonEG/Run2017B-31Mar2018-v1/MINIAOD'])
    mysamples.append(['MuonEG_Run2017C','/MuonEG/Run2017C-31Mar2018-v1/MINIAOD'])
    mysamples.append(['MuonEG_Run2017D','/MuonEG/Run2017D-31Mar2018-v1/MINIAOD'])
    mysamples.append(['MuonEG_Run2017E','/MuonEG/Run2017E-31Mar2018-v1/MINIAOD'])
    mysamples.append(['MuonEG_Run2017F','/MuonEG/Run2017F-31Mar2018-v1/MINIAOD'])

    # mysamples.append(['MET_Run2017B','/MET/Run2017B-31Mar2018-v1/MINIAOD'])
    # mysamples.append(['MET_Run2017C','/MET/Run2017C-31Mar2018-v1/MINIAOD'])
    # mysamples.append(['MET_Run2017D','/MET/Run2017D-31Mar2018-v1/MINIAOD'])
    # mysamples.append(['MET_Run2017E','/MET/Run2017E-31Mar2018-v1/MINIAOD'])
    # mysamples.append(['MET_Run2017F','/MET/Run2017F-31Mar2018-v1/MINIAOD'])




    ## lumimasks (data only!) ##
    ## golden json 2016:
    #lumimask = cmsswbase+'/src/EFTMultilepton/TemplateMakers/data/JSON/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
    ## golden json 2017:
    lumimask = cmsswbase+'/src/EFTMultilepton/TemplateMakers/data/JSON/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt'
    supplement = "_data"

if (singleSamp is not None):
    #if specified, overides all the stuff above with one sample
    mysamples = []
    print "overiding mysamples with: {0}".format(singleSamp[0])
    mysamples.append(singleSamp)
    supplement = '_'+singleSamp[0] 

workflows = []
mergesize = '0.05G'
for label,samp in mysamples:
    ds = None
    #if (samp[:5]=='Batch'):
    if (doeftsamps):
        # Note: don't know why this has to be in list form, since LOBSTER ONLY READS THE FIRST ELEMENT!!! @#$%!
        storage.input=[
            #"hdfs://eddie.crc.nd.edu:19000"  + hadoop_loc,
            "root://deepthought.crc.nd.edu/" + hadoop_loc,  # Note the extra slash after the hostname!
            #"gsiftp://T3_US_NotreDame"       + hadoop_loc,
            #"srm://T3_US_NotreDame"          + hadoop_loc,
        ]
        mergesize = -1
        ds = Dataset(
            files=samp,
            #files_per_task=10,
            patterns=["*.root"]
        )
    else:
        ds = cmssw.Dataset(
            dataset=samp,
            events_per_task=30000,
            lumi_mask=lumimask
        )
    cms_cmd = ['cmsRun','osTwoLep_cfg.py']
    if isdata:
        cms_cmd.extend(['data=True'])
    else:
        cms_cmd.extend(['data=False'])

    if 'ttll_multidim' in label or 'ttll_EFT' in label or 'tllq_multidim' in label or 'tllq_EFT' in label:
        # For private samples
        cms_cmd.extend(['skipHiggs=True'])
    elif 'ttZ' == label or 'tZq' == label:
        # For central samples
        cms_cmd.extend(['skipHiggs=True'])
    else:
        cms_cmd.extend(['skipHiggs=False'])
    print "adding workflow: {0}".format(label)
    print "\tCmd: {0}".format(' '.join(cms_cmd))
    wf = Workflow(
        label=label,
        dataset=ds,
        category=processing,
        #pset='osTwoLep_cfg.py',
        #arguments=['skim=True','jetCleanFakeable=True','data='+str(isdata)],
        #command='cmsRun osTwoLep_cfg.py',
        command=' '.join(cms_cmd),
        #extra_inputs=[cmsswbase+"/src/L1Prefiring/EventWeightProducer/files/L1PrefiringMaps_new.root"],
        merge_size=mergesize,
    )
    workflows.append(wf)

config = Config(
    label=master_label,
    workdir=workdir_path,
    plotdir=plotdir_path,
    storage=storage,
    workflows=workflows,
    advanced=AdvancedOptions(
        bad_exit_codes=[127, 160],
        log_level=1,
        xrootd_servers=['ndcms.crc.nd.edu',
                       'cmsxrootd.fnal.gov',
                       'deepthought.crc.nd.edu']
    )
)


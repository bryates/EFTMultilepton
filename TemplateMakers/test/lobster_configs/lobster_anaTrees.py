import datetime
from glob import glob
from lobster import cmssw
from lobster.core import AdvancedOptions, Category, Config, Dataset, StorageConfiguration, Workflow
import os

isdata = False # remember to change osTwoLep_cfg.py
doeftsamps = True
singleSamp = None # default, to be overridden below if desired
doskim = False

tstamp1 = datetime.datetime.now().strftime('%Y%m%d_%H%M')
tstamp2 = datetime.datetime.now().strftime('%Y_%m_%d')
lobster_step = "analysisTrees"

# master_label = 'EFT_T3_anaTrees_{tstamp}'.format(tstamp=tstamp1)
# master_label = 'EFT_CRC_anaTrees_{tstamp}'.format(tstamp=tstamp1)
master_label = 'EFT_anaWF_anaTrees_{tstamp}'.format(tstamp=tstamp1)

ver = "v1"
#tag = "test/lobster_test_{tstamp}".format(tstamp=tstamp1)
# tag = "special/tllq4f_EFT_MatchedNoHiggs_{tstamp}_NoMrg-JM1-JM2".format(tstamp=tstamp2)
#tag = "central_sgnl_{tstamp}".format(tstamp=tstamp2)
#tag = "central_bkgd_{tstamp}".format(tstamp=tstamp2)
tag = "private_sgnl_{tstamp}".format(tstamp=tstamp2)
#tag = "data2017_{tstamp}".format(tstamp=tstamp2)
# tag = "special/central_ttH-WW-WZ-ttGJets_new_pmx_{tstamp}".format(tstamp=tstamp2)


workdir_path = "{path}/{step}/{tag}/{ver}".format(step=lobster_step,tag=tag,ver=ver,path="/tmpscratch/users/$USER/analysisWorkflow")
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
    disk=2000,
    tasks_max=2000  # maximum tasks in the queue (running+waiting)
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
    # mysamples.append(['WW','/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # mysamples.append(['ZZ','/ZZTo4L_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])    
    
    # "a"
    # mysamples.append(['WWW','/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # mysamples.append(['WWZ','/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # mysamples.append(['WZZ','/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # mysamples.append(['ZZZ','/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # mysamples.append(['WZ','/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # mysamples.append(['ttGJets','/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # mysamples.append(['ttGJets_ext','/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'])
    
    #mysamples.append(['ttWW','/TTWW_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM'])
    #mysamples.append(['ttWZ','/TTWZ_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])

    # mysamples.append(['ttH','/ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])                 #a
    # mysamples.append(['tHq','/THQ_4f_Hincl_13TeV_madgraph_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])                                           #whatever (doesn't have any pdf stuff)

    ## for preshower thing:
    # mysamples.append(['ttW','/TTWJetsToLNu_TuneCP5_PSweights_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM'])     #a
    # mysamples.append(['ttZ','/TTZToLLNuNu_M-10_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])                     #a
    # mysamples.append(['tZq','/tZq_ll_4f_ckm_NLO_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/MINIAODSIM'])            #a

    # mysamples.append(['tZq','/tZq_ll_4f_ckm_NLO_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'])

    ## Newer samples with 'new_pmx'
    mysamples.append(['ttH','/ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    mysamples.append(['WZ','/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    mysamples.append(['ZZ','/ZZTo4L_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM'])
    # mysamples.append(['ttGJets','/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM'])          # This sample seems to outright fail when trying to access it currently

    ## Newer samples with 'v14-v2'
    # mysamples.append(['ttW','/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'])
    # mysamples.append(['ttZ','/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'])
    # mysamples.append(['ZZ','/ZZTo4L_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'])    # This is actually older then the 'new_pmx' version by ~1 month
    # mysamples.append(['WWW','/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'])
    # mysamples.append(['WWZ','/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'])
    # mysamples.append(['ZZZ','/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'])
    # mysamples.append(['ttGJets','/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'])

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

    ### Private EFT samples Round 5
    #hadoop_loc = '/store/user/awightma/'
    ## ttH
    #mysamples.append(['ttH_multidim_b1','FullProduction/Round5/Batch1/postLHE_step/v1/mAOD_step_ttHJet_HanModel16DttllScanpoints_run1/'])
    ## ttlnu
    #mysamples.append(['ttlnu_multidim_b1','FullProduction/Round5/Batch1/postLHE_step/v1/mAOD_step_ttlnuJet_HanModel16DttllScanpoints_run1/'])
    ## ttll
    #mysamples.append(['ttll_multidim_b1','FullProduction/Round5/Batch1/postLHE_step/v1/mAOD_step_ttllNuNuJetNoHiggs_HanModel16DttllScanpoints_run1/'])
    #mysamples.append(['ttll_multidim_b2','FullProduction/Round5/Batch2/postLHE_step/v2/mAOD_step_ttllNuNuJetNoHiggs_HanModel16DttllScanpoints_run1/'])
    ## tHq
    #mysamples.append(['tHq_multidim_b1','FullProduction/Round5/Batch1/postLHE_step/v1/mAOD_step_tHq4fMatched_HanModel16DttllScanpoints_run1/'])
    ## tllq
    #mysamples.append(['tllq_multidim_b1','Batch1/postLHE_step/v1/mAOD_step_tllq4fMatchedNoHiggs_HanModel16DttllScanpoints_run1/']) # OLD tllq SAMPLE, DO NOT USE!
    #mysamples.append(['tllq_multidim_b1','postLHE_step/2019_04_19/tllq4f-tch-NoHiggs_0partons_xqcut0/v2/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsXQCUT0NoMerge_run1'])
    #mysamples.append(['tllq_multidim_b2','postLHE_step/2019_04_19/tllq4f-tch-NoHiggs_0partons_xqcut0_extra/v2/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsXQCUT0NoMerge_run1'])
    
    ### Private EFT samples Round 6
    hadoop_loc = '/store/user/'
    ## ttH (Bad starting point)
    # mysamples.append(['ttH_multidim_b1','kmohrman/FullProduction/Round6/Batch1/postLHE_step/v1/mAOD_step_ttHJet_HanV4ttXJetStartPtChecks_run2'])  # 6.4M
    # mysamples.append(['ttH_multidim_b2','kmohrman/FullProduction/Round6/Batch2/postLHE_step/v1/mAOD_step_ttHJet_HanV4ttXJetStartPtChecks_run2'])  # 3.8M
    ## ttH (old/better starting point)
    mysamples.append(['ttH_multidim_b1','kmohrman/FullProduction/Round6/Batch7/postLHE_step/v2/mAOD_step_ttHJet_HanV4ttXJetStartPtChecks_run0'])    # 12.8M
    ## ttlnu
    mysamples.append(['ttlnu_multidim_b1','kmohrman/FullProduction/Round6/Batch1/postLHE_step/v1/mAOD_step_ttlnuJet_HanV4ttXJetStartPtChecks_run1'])    # 4.9M
    mysamples.append(['ttlnu_multidim_b2','kmohrman/FullProduction/Round6/Batch2/postLHE_step/v1/mAOD_step_ttlnuJet_HanV4ttXJetStartPtChecks_run1'])    # 2.9M
    mysamples.append(['ttlnu_multidim_b3','kmohrman/FullProduction/Round6/Batch5/postLHE_step/v1/mAOD_step_ttlnuJet_HanV4ttXJetStartPtChecks_run1'])    # 2.9M
    mysamples.append(['ttlnu_multidim_b4','kmohrman/FullProduction/Round6/Batch6/postLHE_step/v1/mAOD_step_ttlnuJet_HanV4ttXJetStartPtChecks_run1'])    # 4.9M
    ## ttll
    mysamples.append(['ttll_multidim_b1','kmohrman/FullProduction/Round6/Batch1/postLHE_step/v1/mAOD_step_ttllNuNuJetNoHiggs_HanV4ttXJetStartPtChecks_run2'])   # 5.6M
    mysamples.append(['ttll_multidim_b2','kmohrman/FullProduction/Round6/Batch2/postLHE_step/v1/mAOD_step_ttllNuNuJetNoHiggs_HanV4ttXJetStartPtChecks_run2'])   # 3.4M
    mysamples.append(['ttll_multidim_b3','kmohrman/FullProduction/Round6/Batch5/postLHE_step/v1/mAOD_step_ttllNuNuJetNoHiggs_HanV4ttXJetStartPtChecks_run2'])   # 3.4M
    mysamples.append(['ttll_multidim_b4','kmohrman/FullProduction/Round6/Batch6/postLHE_step/v2/mAOD_step_ttllNuNuJetNoHiggs_HanV4ttXJetStartPtChecks_run2'])   # 5.6M
    ## tHq
    mysamples.append(['tHq_multidim_b1','kmohrman/FullProduction/Round6/Batch3/postLHE_step/v1/mAOD_step_tHq4f_HanV4tHqStartPtChecksMatchOff_run2'])    # 10.0M
    ## tllq
    mysamples.append(['tllq_multidim_b1','awightma/postLHE_step/2019_04_19/tllq4f-tch-NoHiggs_0partons_xqcut0/v2/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsXQCUT0NoMerge_run1'])         # 0.5M
    mysamples.append(['tllq_multidim_b2','awightma/postLHE_step/2019_04_19/tllq4f-tch-NoHiggs_0partons_xqcut0_extra/v2/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsXQCUT0NoMerge_run1'])   # 2.0M
    mysamples.append(['tllq_multidim_b3','awightma/postLHE_step/2019_04_19/tllq4f-tch-NoHiggs_0partons_xqcut0_extra2/v1/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsXQCUT0NoMerge_run1'])  # 5.0M
    mysamples.append(['tllq_multidim_b4','kmohrman/FullProduction/Round6/Batch4/postLHE_step/v1/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsXQCUT0MatchOff_run1']) # 5.0M
    mysamples.append(['tllq_multidim_b5','kmohrman/FullProduction/Round6/Batch6/postLHE_step/v2/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsXQCUT0MatchOff_run1']) # 5.0M
    ## IN PROGRESS STILL
    
    ### Others from R6 era
    # mysamples.append(['ttH_multidim_b1','kmohrman/postLHE_step/2019_04_19/ttXJet-HanV4ttXJetSMCheck/v1/mAOD_step_ttHJet_HanV4ttXSMCheck_run0']) # 320k events
    # mysamples.append(['ttH_multidim_b2','kmohrman/postLHE_step/2019_04_19/ttXJet-HanV4tXq4fSMCheck-b2/v1/mAOD_step_ttHJet_HanV4ttXSMCheck_run0']) # 323k events

    # mysamples.append(['ttll_multidim_b1','kmohrman/postLHE_step/2019_04_19/ttXJet-HanV4ttXJetSMCheck/v1/mAOD_step_ttllNuNuJetNoHiggs_HanV4ttXSMCheck_run0']) # 325k events
    # mysamples.append(['ttll_multidim_b2','kmohrman/postLHE_step/2019_04_19/ttXJet-HanV4tXq4fSMCheck-b2/v1/mAOD_step_ttllNuNuJetNoHiggs_HanV4ttXSMCheck_run0']) # 326k events

    # mysamples.append(['ttlnu_multidim_b1','kmohrman/postLHE_step/2019_04_19/ttXJet-HanV4ttXJetSMCheck/v1/mAOD_step_ttlnuJet_HanV4ttXSMCheck_run0']) # 240k events
    # mysamples.append(['ttlnu_multidim_b2','kmohrman/postLHE_step/2019_04_19/ttXJet-HanV4tXq4fSMCheck-b2/v1/mAOD_step_ttlnuJet_HanV4ttXSMCheck_run0']) # 241k events

    # mysamples.append(['tHq_multidim_b1','kmohrman/postLHE_step/2019_04_19/tXq4f-HanV4tXq4fSMCheck/v1/mAOD_step_tHq4f_HanV4tXqSMCheckMatchOff_run0']) # 250k events
    # mysamples.append(['tHq_multidim_b2','kmohrman/postLHE_step/2019_04_19/tXq4f-HanV4tXq4fSMCheck-b2/v1/mAOD_step_tHq4f_HanV4tXqSMCheckMatchOff_run0']) # 250k events

    # mysamples.append(['tllq_multidim_b1','kmohrman/postLHE_step/2019_04_19/tXq4f-HanV4tXq4fSMCheck/v1/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4tXqSMCheckMatchOff_run0']) #250k events
    # mysamples.append(['tllq_multidim_b2','kmohrman/postLHE_step/2019_04_19/tXq4f-HanV4tXq4fSMCheck-b2/v1/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4tXqSMCheckMatchOff_run0']) #250k events

    ###

    # mysamples.append(['tllq_multidim_0partons','postLHE_step/2019_04_19/tllq4f-NoDim6Diagrams/v2/mAOD_step_tllq4f_NoDim6NoSchanW_run0/'])

    # mysamples.append(['tllq_multidim_JetMax1_b1','postLHE_step/2019_04_19/tllq4f_t-channelMatched_pythia-JetMax1-JetMax2_b1-b4/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10JetMax1b1_run1'])
    # mysamples.append(['tllq_multidim_JetMax1_b2','postLHE_step/2019_04_19/tllq4f_t-channelMatched_pythia-JetMax1-JetMax2_b1-b4/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10JetMax1b2_run1'])
    # mysamples.append(['tllq_multidim_JetMax1_b3','postLHE_step/2019_04_19/tllq4f_t-channelMatched_pythia-JetMax1-JetMax2_b1-b4/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10JetMax1b3_run1'])
    # mysamples.append(['tllq_multidim_JetMax1_b4','postLHE_step/2019_04_19/tllq4f_t-channelMatched_pythia-JetMax1-JetMax2_b1-b4/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10JetMax1b4_run1'])

    # mysamples.append(['tllq_multidim_JetMax2_b1','postLHE_step/2019_04_19/tllq4f_t-channelMatched_pythia-JetMax1-JetMax2_b1-b4/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10JetMax2b1_run1'])
    # mysamples.append(['tllq_multidim_JetMax2_b2','postLHE_step/2019_04_19/tllq4f_t-channelMatched_pythia-JetMax1-JetMax2_b1-b4/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10JetMax2b2_run1'])
    # mysamples.append(['tllq_multidim_JetMax2_b3','postLHE_step/2019_04_19/tllq4f_t-channelMatched_pythia-JetMax1-JetMax2_b1-b4/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10JetMax2b3_run1'])
    # mysamples.append(['tllq_multidim_JetMax2_b4','postLHE_step/2019_04_19/tllq4f_t-channelMatched_pythia-JetMax1-JetMax2_b1-b4/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10JetMax2b4_run1'])
    # mysamples.append(['tllq_multidim_JetMax2_b5','FullProduction/Round5/Batch3/postLHE_step/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10JetMax2_run1'])

    # mysamples.append(['tllq_multidim_0p_NoMerge','postLHE_step/2019_04_19/tllq4f_forHadFilterCheck_pythia-JetMax1-JetMax2-NoMerge/v2/mAOD_step_tllq4f_NoDim6NoSchanWNoMerge_run0'])
    # mysamples.append(['tllq_multidim_0p_JetMax1','postLHE_step/2019_04_19/tllq4f_forHadFilterCheck_pythia-JetMax1-JetMax2-NoMerge/v2/mAOD_step_tllq4f_NoDim6NoSchanWJetMax1_run0'])
    # mysamples.append(['tllq_multidim_0p_JetMax2','postLHE_step/2019_04_19/tllq4f_forHadFilterCheck_pythia-JetMax1-JetMax2-NoMerge/v2/mAOD_step_tllq4f_NoDim6NoSchanWJetMax2_run0'])

    # mysamples.append(['tllq_multidim_MatchedNoHiggs_NoMerge','postLHE_step/2019_04_19/tllq4f-tch-NoHiggs_0partons/v2/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsNoMerge_run1'])
    # mysamples.append(['tllq_multidim_MatchedNoHiggs_JetMax1','postLHE_step/2019_04_19/tllq4f-tch-NoHiggs_0partons/v2/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsJetMax1_run1'])
    # mysamples.append(['tllq_multidim_MatchedNoHiggs_JetMax2','postLHE_step/2019_04_19/tllq4f-tch-NoHiggs_0partons/v2/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsJetMax2_run1'])

    # mysamples.append(['tllq_multidim_MatchedNoHiggsMLM_NoMerge','postLHE_step/2019_04_19/tllq4f-tch-NoHiggs_0partons/v2/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsMLMNoMerge_run1'])
    # mysamples.append(['tllq_multidim_MatchedNoHiggsMLM_JetMax1','postLHE_step/2019_04_19/tllq4f-tch-NoHiggs_0partons/v2/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsMLMJetMax1_run1'])
    # mysamples.append(['tllq_multidim_MatchedNoHiggsMLM_JetMax2','postLHE_step/2019_04_19/tllq4f-tch-NoHiggs_0partons/v2/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsMLMJetMax2_run1'])

    # mysamples.append(['tllq_multidim_tch_NoHiggs_0j_xqcut0_NoMerge','postLHE_step/2019_04_19/tllq4f-tch-NoHiggs_0partons_xqcut0/v2/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsXQCUT0NoMerge_run1'])
    # mysamples.append(['tllq_multidim_tch_NoHiggs_0j_xqcut0_JetMax1','postLHE_step/2019_04_19/tllq4f-tch-NoHiggs_0partons_xqcut0/v2/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsXQCUT0JetMax1_run1'])
    # mysamples.append(['tllq_multidim_tch_NoHiggs_0j_xqcut0_JetMax2','postLHE_step/2019_04_19/tllq4f-tch-NoHiggs_0partons_xqcut0/v2/mAOD_step_tllq4fNoSchanWNoHiggs0p_HanV4Model16DttllScanpointsXQCUT0JetMax2_run1'])

    # singleSamp = ['tllq_multidim_b1','postLHE_step/2019_04_19/tllq4f_t-channelMatched_pythia-JetMax1/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10_run1/']
    # singleSamp = ['tllq_multidim_b2','postLHE_step/2019_04_19/tllq4f_t-channelMatched_pythia-JetMax1_b2/v1/mAOD_step_tllq4fMatchedNoSchanW_HanModel16DttllScanpointsXQCUT10_run1/']

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
mergesize = '64M'
if isdata:
    mergesize = '256M'
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
            files_per_task=2,
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

    if doeftsamps:
        cms_cmd.extend(['isPrivateSample=True'])

    if 'tllq_multidim' in label or 'tHq_multidim' in label or 'tZq' == label:
        # The sample uses the 4f parton PDF scheme
        cms_cmd.extend(['is4fScheme=True'])

    if doskim:
        cms_cmd.extend(['skim=True'])

    print "Adding workflow: {0}".format(label)
    print "\tSamples: {0}".format(samp)
    print "\tCmd: {0}".format(' '.join(cms_cmd))
    print "\tmerge: {0}".format(mergesize)
    wf = Workflow(
        label=label,
        dataset=ds,
        category=processing,
        #arguments=['skim=True','jetCleanFakeable=True','data='+str(isdata)],
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


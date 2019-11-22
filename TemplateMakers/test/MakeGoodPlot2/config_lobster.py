import datetime
from glob import glob
from lobster import cmssw
from lobster.core import AdvancedOptions, Category, Config, Dataset, StorageConfiguration, Workflow

version = datetime.datetime.now().strftime('%Y%m%d_%H%M')

#thisround = "lobster_trees__EFT_test_14_4_18/" # old Fall17 MINIAOD
#thisround = "lobster_trees__EFT_test_28_4_18/" # available new MINIAOD 
#thisround = "lobster_trees__EFT_SingleMu_only_test_10_5_18/" #SingleMu data and ttjets only
#thisround = "lobster_trees__EFT_noSkim_test_10_5_18/" #ttjets only
#thisround = "lobster_trees__EFT_for_trigSFs_15_5_18/"

tstamp1 = datetime.datetime.now().strftime('%Y%m%d_%H%M')
tstamp2 = datetime.datetime.now().strftime('%Y_%m_%d')
lobster_step = "histMaking"

# master_label = 'EFT_T3_histMaking_{tstamp}'.format(tstamp=tstamp1)
# master_label = 'EFT_CRC_histMaking_{tstamp}'.format(tstamp=tstamp1)
master_label = 'EFT_anaWF_histMaking_{tstamp}'.format(tstamp=tstamp1)

ver = "v1"
tag = "test/lobster_test_{tstamp}".format(tstamp=tstamp1)
# tag = "private_sgnl_{tstamp}".format(tstamp=tstamp2)
# tag = "special/geoff_inputfiles_central_bkgd_{tstamp}".format(tstamp=tstamp2)
# tag = "special/tllq4f_SM_t-channel_{tstamp}_0partons".format(tstamp=tstamp2)
# tag = "special/central_new_pmx_samples_{tstamp}".format(tstamp=tstamp2)

# tag = "special/data-nominal_newGT-94X_dataRun2_v11_{tstamp}".format(tstamp=tstamp2)
# tag = "special/data-ddbrs_newGT-94X_dataRun2_v11_{tstamp}".format(tstamp=tstamp2)
# tag = "special/data-fakes_newGT-94X_dataRun2_v11_{tstamp}".format(tstamp=tstamp2)
# tag = "special/data-QFs_newGT-94X_dataRun2_v11_{tstamp}".format(tstamp=tstamp2)

# tag = "full_data_{tstamp}".format(tstamp=tstamp2)
# tag = "full_MC_{tstamp}".format(tstamp=tstamp2)

workdir_path = "{path}/{step}/{tag}/{ver}".format(step=lobster_step,tag=tag,ver=ver,path="/tmpscratch/users/$USER/analysisWorkflow")
plotdir_path = "{path}/{step}/{tag}/{ver}".format(step=lobster_step,tag=tag,ver=ver,path="~/www/lobster")
output_path  = "{path}/{step}/{tag}/{ver}".format(step=lobster_step,tag=tag,ver=ver,path="/store/user/$USER")

URI_HDFS   = "{scheme}://{host}:{port}".format(scheme="hdfs",host="eddie.crc.nd.edu",port="19000")
URI_ROOT   = "{scheme}://{host}/".format(scheme="root",host="deepthought.crc.nd.edu") # Note the extra slash after the hostname!
URI_GSIFTP = "{scheme}://{host}".format(scheme="gsiftp",host="T3_US_NotreDame")
URI_SRM    = "{scheme}://{host}".format(scheme="srm",host="T3_US_NotreDame")
URI_FILE   = "{scheme}://{host}/hadoop".format(scheme="file",host="")

## current ##
isdata = False
#############

ddbr_or_nom = ['nom']
#thisround = "lobster_trees__lobster_trees__EFT_test_25_5_18/" # mc
#thisround = "lobster_trees__lobster_trees__EFT_test_14_6_18/" # EFT mc
#thisround = "/" # MD EFT mc

if isdata:
    #thisround = "lobster_trees__lobster_trees__EFT_test_20_5_18/" # data
    ddbr_or_nom=['nom','_Fakes','_QFs']
    #ddbr_or_nom=['_Fakes']
    #ddbr_or_nom=['_Fakes','_QFs']
    #ddbr_or_nom = ['nom']

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


mysamples = []
if (not isdata):
    ## Add the MC...
    # mysamples.append('DYJets_M50')
    # mysamples.append('SingleTop_schan')

    # mysamples.append('SingleTop_tWchan_antitop')
    # mysamples.append('SingleTop_tWchan_top')
    # mysamples.append('SingleTop_tchan_antitop')
    # mysamples.append('SingleTop_tchan_top')
    
    mysamples.append('WW')                                
    mysamples.append('WZ')  
    mysamples.append('ZZ')
    mysamples.append('WWW')
    mysamples.append('WWZ')
    mysamples.append('WZZ')
    mysamples.append('ZZZ')
    mysamples.append('ttGJets')
    
    # mysamples.append('ttJets')                              #<-- new in latest round
    ## #mysamples.append('ttJets_Dilept')                      #<-- not in latest round
    ## #mysamples.append('ttJets_SingleLeptFromT')             #<-- not in latest round
    ## #mysamples.append('ttJets_SingleLeptFromTbar')          #<-- not in latest round

    mysamples.append('ttH')
    mysamples.append('ttW')
    mysamples.append('ttZ')
    mysamples.append('tZq')
    # mysamples.append('tHq')

    # ## mysamples.append('ttWW')
    # ## mysamples.append('ttWZ')
    # mysamples.append('DYJets_M10to50')                    
    # mysamples.append('WJets')                             


    # EFT samps
    # ##mysamples.append('ttH_cbW')
    # mysamples.append('ttH_cpQ3') 
    # mysamples.append('ttH_cpQM') 
    # mysamples.append('ttH_cptb') 
    # mysamples.append('ttH_cpt')
    # mysamples.append('ttH_cQe1') 
    # mysamples.append('ttH_ctG')
    # mysamples.append('ttH_ctl1') 
    # mysamples.append('ttH_ctp')
    # mysamples.append('ttH_ctW')
    # mysamples.append('ttH_ctZ')
    # mysamples.append('tllq_cpQ3')
    # mysamples.append('tllq_cpQM')
    # mysamples.append('tllq_cptb')
    # mysamples.append('tllq_cpt') 
    # mysamples.append('tllq_cQe1')
    # mysamples.append('tllq_ctG') 
    # mysamples.append('tllq_ctl1')
    # mysamples.append('tllq_ctp') 
    # mysamples.append('tllq_ctW') 
    # mysamples.append('tllq_ctZ') 
    # ##mysamples.append('ttll_cbW') 
    # mysamples.append('ttll_cpQ3')
    # mysamples.append('ttll_cpQM')
    # mysamples.append('ttll_cptb')
    # mysamples.append('ttll_cpt') 
    # mysamples.append('ttll_cQe1')
    # mysamples.append('ttll_ctG') 
    # mysamples.append('ttll_ctl1')
    # mysamples.append('ttll_ctp') 
    # mysamples.append('ttll_ctW') 
    # mysamples.append('ttll_ctZ') 
    # ##mysamples.append('ttlnu_cbW')
    # mysamples.append('ttlnu_cpQ3')
    # mysamples.append('ttlnu_cpQM')
    # mysamples.append('ttlnu_cptb')
    # mysamples.append('ttlnu_cpt')
    # mysamples.append('ttlnu_cQe1')
    # mysamples.append('ttlnu_ctG')
    # mysamples.append('ttlnu_ctl1')
    # mysamples.append('ttlnu_ctp')
    # mysamples.append('ttlnu_ctW')
    # mysamples.append('ttlnu_ctZ')
    
    # This is the currently used naming for private EFT samples
    mysamples.append('ttH_multidim')
    mysamples.append('ttlnu_multidim')
    mysamples.append('ttll_multidim')
    mysamples.append('tllq_multidim')
    mysamples.append('tHq_multidim')

    #mysamples.append('ttH_EFT')
    #mysamples.append('ttlnu_EFT')
    #mysamples.append('ttll_EFT')
    #mysamples.append('tllq_EFT')
    #mysamples.append('tHq_EFT')

    # mysamples.append('ttlnu_no_rewgt_1Jet')
    # mysamples.append('ttlnu_no_rewgt_0Jet')
    
    

if isdata:
    ### Add the data...
    mysamples.append('DoubleEG')
    mysamples.append('DoubleMuon')
    mysamples.append('MuonEG')
    mysamples.append('SingleElectron')
    mysamples.append('SingleMuon')


## for trigger studies only:
#mysamples.append('MET')

### moving these here temporarily
## lobster_trees__EFT_test_14_4_18 samps
# mysamples.append('DYJets_M10to50')                     #<-- not in latest round
# mysamples.append('WJets')                              #<-- not in latest round
# mysamples.append('WW')                                 #<-- not in latest round
# mysamples.append('WZ')                                 #<-- not in latest round
# mysamples.append('tttt')                               #<-- not in latest round

#mysamples.append('ttJets_noSkim')

## ToDo: just update sample_names_reg to be the above.

data = []
for samp in mysamples:
    with open("inputfiles__{samp}.txt".format(samp=samp)) as f:
        lines = f.read().splitlines()
        lst = set([x.replace("/hadoop","").replace(input_path,"").rsplit("/",1)[0] for x in lines])
        lst = list(lst)
        data.append((samp,lst,len(lines)))
        print "{sample}: files={nfiles}".format(sample=samp,nfiles=len(lines))
        for dir_path in lst:
            print "{0:>4}{path}".format('',path=dir_path)


# This doesn't do what you would think:
#sandbox = cmssw.Sandbox(include=['EFTMultilepton/TemplateMakers/test/MakeGoodPlot2'])
sandbox = cmssw.Sandbox(include=['EFTMultilepton/TemplateMakers/src','EFTMultilepton/TemplateMakers/data'])
# Most stuff has to be added "by hand":
extra_inputs=[]
extra_inputs.append('../variables.h')
extra_inputs.append('../functionsTTH.cc')
extra_inputs.append('../csvSF_treeReader_13TeV.C')
extra_inputs.append('../EFT_code/helperToolsEFT.h')
headerfiles = glob("*.h")
extra_inputs.extend(headerfiles)
extra_inputs.append('wrapper_lobster.C')
extra_inputs.append('wrapper_lobster.py')

print " "
print "included files: {lst}".format(lst=extra_inputs)
print " "

# File Input Sizes
#   Data based: ~50-150M
#   EFT based: ~150-200M
#   Central bkgds: 100-300M
#   Central sgnls: 250-300M
#   Private tllq: ~50M
# Num File Inputs:
#   Data based: ~2300-23000 units
#   EFT based: ~600-1800 units
#   Central bkgds: ~10-650 units
#   Central sgnls: ~400-800 units
# File Output Sizes
#   Data based: ~11M (pre/post-merged)
#   EFT based: ~50-100M (pre-merged), ~100-150M (post-merged)
#   Central bkgds: ~10-15M (pre/post-merged)
#   Central sgnls: ~15M (pre/post-merged)

processing = Category(
    name='processing',
    cores=1,
    memory=3500,
    disk=4500
)

workflows = []
for thing in ddbr_or_nom:
    extlabel=''
    if thing != 'nom':
        extlabel=thing
    for label, dirs, nfiles in data:
        fpt=10
        #if (label=='tZq' or label=='ttW' or label=='ttZ' or label[:3]=='ttH' or label[:5]=='ttlnu' or label[:4]=='tllq' or label[:4]=='ttll'):
        if (label=='tZq' or label=='ttW' or label=='ttZ'):
            fpt=15
        if (label=='ttJets'):
            fpt=5
        if (label=='DoubleEG' or label=='DoubleMuon' or label=='MuonEG' or label=='SingleElectron' or label=='SingleMuon'):
            fpt=50  # maybe use 35?
        merge_size = '512M'
        if 'multidim' in label:
            fpt = min(int(nfiles/20),10)
            fpt = max(fpt,1)
            merge_size = '1G'
        elif label == 'tZq':
            merge_size = '128M'
        cmd = ['python','wrapper_lobster.py',label,thing,'@inputfiles']
        wf_label = "{label}{ext}".format(label=label,ext=extlabel)
        ttH = Workflow(
            label=wf_label,
            dataset=Dataset(
                files=dirs,
                files_per_task=fpt,
                patterns=["*.root"]
            ),
            category=processing,
            command=' '.join(cmd),
            extra_inputs=extra_inputs,
            publish_label='test',
            merge_command='hadd @outputfiles @inputfiles',
            merge_size=merge_size,
            outputs=['output.root'],
            sandbox=sandbox
        )

        workflows.append(ttH)
        print "Added workflow: {}".format(label+extlabel)
        print "\tfiles: {}".format(nfiles)
        print "\tfpt: {}".format(fpt)
        print "\tmerge: {}".format(merge_size)


config = Config(
    label=master_label,
    workdir=workdir_path,
    plotdir=plotdir_path,
    storage=storage,
    workflows=workflows,
    advanced=AdvancedOptions(
        bad_exit_codes=[127, 160],
        log_level=1
    )
)


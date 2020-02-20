import os
import shutil

from EFTMultilepton.TemplateMakers.utils import clean_dir

BASE_PATH = os.path.expandvars("/tmpscratch/users/$USER/analysisWorkflow/mergedHists")
MIXED_DIR = "mixed_samples"

class Mixin(object):
    def __init__(self,loc,*args):
        self.__dir = loc
        self.__samples = [x for x in args]
        self.__indent = ' '*4

    def dir(self):
        return self.__dir

    def indent(self,depth):
        return self.__indent*depth

    def samples(self):
        return self.__samples

    def addSample(self,sample):
        if sample in self.samples():
            return
        self.__samples.append(sample)

    def __str__(self):
        s = "Samples from '.../mergedHists/{}'".format(self.dir())
        if len(self.samples()):
            s += '\n{indent}{sample}'.format(indent=self.indent(1),sample=' '.join(self.samples()))
        return s

    def list(self):
        return [os.path.join(self.dir(),x) for x in self.samples()]

def check_inputs(lst):
    fnames = []
    for mixin in lst:
        for p in mixin.list():
            src_fpath = os.path.join(BASE_PATH,p)
            h,t = os.path.split(p)
            if not os.path.exists(src_fpath):
                err_str = "Target file does not exist: {}".format(src_fpath)
                raise RuntimeError(err_str)
            elif not os.path.isfile(src_fpath):
                err_str = "Target file is not a file: {}".format(src_fpath)
                raise RuntimeError(err_str)
            elif t in fnames:
                err_str = "Repeated file name found: {}".format(t)
                raise RuntimeError(err_str)
            fnames.append(t)

def main():
    standardHists = Mixin("2019_07_08_from-standardhists_SRs_with_Round5_EFTsamps")
    standardHists.addSample("temp_data.root")
    standardHists.addSample("temp_DoubleEG.root")
    standardHists.addSample("temp_DoubleMuon.root")
    standardHists.addSample("temp_Fakes.root")
    standardHists.addSample("temp_MuonEG.root")
    standardHists.addSample("temp_QFs.root")
    standardHists.addSample("temp_SingleElectron.root")
    standardHists.addSample("temp_SingleMuon.root")

    new_pmx = Mixin("2019_10_22_central_new_pmx")
    new_pmx.addSample("temp_ttH.root")
    new_pmx.addSample("temp_WZ.root")
    new_pmx.addSample("temp_ZZ.root")

    full_MC_2019_10_19 = Mixin("2019_10_19_full_MC")
    full_MC_2019_10_19.addSample("temp_ttGJets.root")
    full_MC_2019_10_19.addSample("temp_ttW.root")
    full_MC_2019_10_19.addSample("temp_ttZ.root")
    full_MC_2019_10_19.addSample("temp_tZq.root")
    full_MC_2019_10_19.addSample("temp_WW.root")
    full_MC_2019_10_19.addSample("temp_WWW.root")
    full_MC_2019_10_19.addSample("temp_WWZ.root")
    full_MC_2019_10_19.addSample("temp_WZZ.root")
    full_MC_2019_10_19.addSample("temp_ZZZ.root")

    a28_redo = Mixin("2019_11_11_private_sgnl_redoTrees-a28_NoStreaming")
    a28_redo.addSample("temp_tHq_multidim.root")
    a28_redo.addSample("temp_tllq_multidim.root")
    a28_redo.addSample("temp_ttH_multidim.root")
    a28_redo.addSample("temp_ttll_multidim.root")
    a28_redo.addSample("temp_ttlnu_multidim.root")

    a31 = Mixin("2019_11_09_private_sgnl_redoTrees-a29_NoStreaming_v2")
    a31.addSample("temp_tHq_multidim.root")
    a31.addSample("temp_tllq_multidim.root")
    a31.addSample("temp_ttH_multidim.root")
    a31.addSample("temp_ttll_multidim.root")
    a31.addSample("temp_ttlnu_multidim.root")

    a29_noDupes = Mixin("2019_11_15_private_sgnl_reprocFullWF-a29_NoDuplicates")
    a29_noDupes.addSample("temp_ttH_multidim.root")
    a29_noDupes.addSample("temp_tHq_multidim.root")
    a29_noDupes.addSample("temp_tllq_multidim.root")
    a29_noDupes.addSample("temp_ttll_multidim.root")
    a29_noDupes.addSample("temp_ttlnu_multidim.root")

    a29_noDupesV2 = Mixin("2019_11_19_private_sgnl_reprocFullWF-a29_NoDuplicates")
    a29_noDupesV2.addSample("temp_ttH_multidim.root")
    a29_noDupesV2.addSample("temp_tHq_multidim.root")
    a29_noDupesV2.addSample("temp_tllq_multidim.root")
    a29_noDupesV2.addSample("temp_ttll_multidim.root")
    a29_noDupesV2.addSample("temp_ttlnu_multidim.root")

    hanV4_SMCheck = Mixin("2019_11_18_private_sgnl_ttXJet-tXq4f_HanV4SMCheck_b1-b2")
    hanV4_SMCheck.addSample("temp_ttH_multidim.root")
    hanV4_SMCheck.addSample("temp_tHq_multidim.root")
    hanV4_SMCheck.addSample("temp_tllq_multidim.root")
    hanV4_SMCheck.addSample("temp_ttll_multidim.root")
    hanV4_SMCheck.addSample("temp_ttlnu_multidim.root")

    hanV4_SMCheck = Mixin("2019_11_18_private_sgnl_ttXJet-tXq4f_HanV4SMCheck_b1-b2")
    hanV4_SMCheck.addSample("temp_ttH_multidim.root")
    hanV4_SMCheck.addSample("temp_tHq_multidim.root")
    hanV4_SMCheck.addSample("temp_tllq_multidim.root")
    hanV4_SMCheck.addSample("temp_ttll_multidim.root")
    hanV4_SMCheck.addSample("temp_ttlnu_multidim.root")

    hanOrig_SMCheck = Mixin("2019_11_18_private_sgnl_ttXJet_HanOriginalSMCheck")
    hanOrig_SMCheck.addSample("temp_ttH_multidim.root")
    hanOrig_SMCheck.addSample("temp_ttll_multidim.root")
    hanOrig_SMCheck.addSample("temp_ttlnu_multidim.root")

    tllq_R6B1 = Mixin("2019_11_19_private_tllq_R6B1")
    tllq_R6B1.addSample("temp_tllq_multidim.root")

    tllq_R6B2 = Mixin("2019_11_19_private_tllq_R6B2")
    tllq_R6B2.addSample("temp_tllq_multidim.root")

    tllq_R6B3 = Mixin("2019_11_19_private_tllq_R6B3")
    tllq_R6B3.addSample("temp_tllq_multidim.root")

    ################################################################################################
    # These all use the new TH1EFT class that re-enables the storing of the stat errors
    ################################################################################################
    central_MC_fixedErrors = Mixin("2019_11_22_central_sgnl-bkgd_fixedErrors")
    central_MC_fixedErrors.addSample("temp_ttH.root")
    central_MC_fixedErrors.addSample("temp_ttW.root")
    central_MC_fixedErrors.addSample("temp_ttZ.root")
    central_MC_fixedErrors.addSample("temp_tZq.root")
    central_MC_fixedErrors.addSample("temp_WW.root")
    central_MC_fixedErrors.addSample("temp_WZ.root")
    central_MC_fixedErrors.addSample("temp_ZZ.root")
    central_MC_fixedErrors.addSample("temp_WWW.root")
    central_MC_fixedErrors.addSample("temp_WWZ.root")
    central_MC_fixedErrors.addSample("temp_WZZ.root")
    central_MC_fixedErrors.addSample("temp_ZZZ.root")
    central_MC_fixedErrors.addSample("temp_ttGJets.root")

    central_full_CR = Mixin("2019_12_06_central_full_CR")
    central_full_CR.addSample("temp_ttH.root")
    central_full_CR.addSample("temp_ttW.root")
    central_full_CR.addSample("temp_ttZ.root")
    central_full_CR.addSample("temp_tZq.root")
    central_full_CR.addSample("temp_WW.root")
    central_full_CR.addSample("temp_WZ.root")
    central_full_CR.addSample("temp_ZZ.root")
    central_full_CR.addSample("temp_WWW.root")
    central_full_CR.addSample("temp_WWZ.root")
    central_full_CR.addSample("temp_WZZ.root")
    central_full_CR.addSample("temp_ZZZ.root")
    central_full_CR.addSample("temp_ttGJets.root")

    full_MC_a32_CR_lepOnly = Mixin("2019_12_10_full_MC_CR_lepOnlySelection")
    full_MC_a32_CR_lepOnly.addSample("temp_ttH.root")
    full_MC_a32_CR_lepOnly.addSample("temp_ttW.root")
    full_MC_a32_CR_lepOnly.addSample("temp_ttZ.root")
    full_MC_a32_CR_lepOnly.addSample("temp_tZq.root")
    full_MC_a32_CR_lepOnly.addSample("temp_WW.root")
    full_MC_a32_CR_lepOnly.addSample("temp_WZ.root")
    full_MC_a32_CR_lepOnly.addSample("temp_ZZ.root")
    full_MC_a32_CR_lepOnly.addSample("temp_WWW.root")
    full_MC_a32_CR_lepOnly.addSample("temp_WWZ.root")
    full_MC_a32_CR_lepOnly.addSample("temp_WZZ.root")
    full_MC_a32_CR_lepOnly.addSample("temp_ZZZ.root")
    full_MC_a32_CR_lepOnly.addSample("temp_ttGJets.root")
    full_MC_a32_CR_lepOnly.addSample("temp_ttH_multidim.root")
    full_MC_a32_CR_lepOnly.addSample("temp_tHq_multidim.root")
    full_MC_a32_CR_lepOnly.addSample("temp_tllq_multidim.root")
    full_MC_a32_CR_lepOnly.addSample("temp_ttll_multidim.root")
    full_MC_a32_CR_lepOnly.addSample("temp_ttlnu_multidim.root")

    data_CR = Mixin("2019_12_09_data_CR_GT-94X_dataRun2_v6")
    data_CR.addSample("temp_data.root")
    data_CR.addSample("temp_Fakes.root")
    data_CR.addSample("temp_QFs.root")
    data_CR.addSample("temp_DoubleEG.root")
    data_CR.addSample("temp_DoubleMuon.root")
    data_CR.addSample("temp_MuonEG.root")
    data_CR.addSample("temp_SingleElectron.root")
    data_CR.addSample("temp_SingleMuon.root")

    data_CR_lepOnly = Mixin("2019_12_10_data_CR_lepOnlySelection_GT-94X_dataRun2_v6")
    data_CR_lepOnly.addSample("temp_data.root")
    data_CR_lepOnly.addSample("temp_Fakes.root")
    data_CR_lepOnly.addSample("temp_QFs.root")
    data_CR_lepOnly.addSample("temp_DoubleEG.root")
    data_CR_lepOnly.addSample("temp_DoubleMuon.root")
    data_CR_lepOnly.addSample("temp_MuonEG.root")
    data_CR_lepOnly.addSample("temp_SingleElectron.root")
    data_CR_lepOnly.addSample("temp_SingleMuon.root")

    private_sgnl_a28_fixedErrors = Mixin("2019_11_22_private_sgnl_a28_fixedErrors")
    private_sgnl_a28_fixedErrors.addSample("temp_ttH_multidim.root")
    private_sgnl_a28_fixedErrors.addSample("temp_tHq_multidim.root")
    private_sgnl_a28_fixedErrors.addSample("temp_tllq_multidim.root")
    private_sgnl_a28_fixedErrors.addSample("temp_ttll_multidim.root")
    private_sgnl_a28_fixedErrors.addSample("temp_ttlnu_multidim.root")

    private_sgnl_a31_fixedErrors = Mixin("2019_11_22_private_sgnl_a31_fixedErrors")
    private_sgnl_a31_fixedErrors.addSample("temp_ttH_multidim.root")
    private_sgnl_a31_fixedErrors.addSample("temp_tHq_multidim.root")
    private_sgnl_a31_fixedErrors.addSample("temp_tllq_multidim.root")
    private_sgnl_a31_fixedErrors.addSample("temp_ttll_multidim.root")
    private_sgnl_a31_fixedErrors.addSample("temp_ttlnu_multidim.root")

    private_sgnl_a32 = Mixin("2019_12_03_private_sgnl_a32")
    private_sgnl_a32.addSample("temp_ttH_multidim.root")
    private_sgnl_a32.addSample("temp_tHq_multidim.root")
    private_sgnl_a32.addSample("temp_tllq_multidim.root")
    private_sgnl_a32.addSample("temp_ttll_multidim.root")
    private_sgnl_a32.addSample("temp_ttlnu_multidim.root")

    private_sgnl_a32_CR = Mixin("2019_12_06_private_sgnl_CR")
    private_sgnl_a32_CR.addSample("temp_ttH_multidim.root")
    private_sgnl_a32_CR.addSample("temp_tHq_multidim.root")
    private_sgnl_a32_CR.addSample("temp_tllq_multidim.root")
    private_sgnl_a32_CR.addSample("temp_ttll_multidim.root")
    private_sgnl_a32_CR.addSample("temp_ttlnu_multidim.root")

    private_ttH_a32_CR_copyChanges = Mixin("2019_12_10_ttH_CR_check_copyChanges")
    private_ttH_a32_CR_copyChanges.addSample("temp_ttH_multidim.root")

    private_sgnl_a32_SR_copyChanges = Mixin("2019_12_11_full_MC_a32_SR_check_copyChanges")
    private_sgnl_a32_SR_copyChanges.addSample("temp_ttH_multidim.root")
    private_sgnl_a32_SR_copyChanges.addSample("temp_tHq_multidim.root")
    private_sgnl_a32_SR_copyChanges.addSample("temp_tllq_multidim.root")
    private_sgnl_a32_SR_copyChanges.addSample("temp_ttll_multidim.root")
    private_sgnl_a32_SR_copyChanges.addSample("temp_ttlnu_multidim.root")

    central_sgnl_a32_SR_copyChanges = Mixin("2019_12_11_full_MC_a32_SR_check_copyChanges")
    central_sgnl_a32_SR_copyChanges.addSample("temp_ttH.root")
    central_sgnl_a32_SR_copyChanges.addSample("temp_ttW.root")
    central_sgnl_a32_SR_copyChanges.addSample("temp_ttZ.root")
    central_sgnl_a32_SR_copyChanges.addSample("temp_tZq.root")

    central_bkgd_a32_SR_copyChanges = Mixin("2019_12_11_full_MC_a32_SR_check_copyChanges")
    central_bkgd_a32_SR_copyChanges.addSample("temp_WW.root")
    central_bkgd_a32_SR_copyChanges.addSample("temp_WZ.root")
    central_bkgd_a32_SR_copyChanges.addSample("temp_ZZ.root")
    central_bkgd_a32_SR_copyChanges.addSample("temp_WWW.root")
    central_bkgd_a32_SR_copyChanges.addSample("temp_WWZ.root")
    central_bkgd_a32_SR_copyChanges.addSample("temp_WZZ.root")
    central_bkgd_a32_SR_copyChanges.addSample("temp_ZZZ.root")
    central_bkgd_a32_SR_copyChanges.addSample("temp_ttGJets.root")

    data_SR_geoff_inputfiles_v1 = Mixin("2019_12_11_geoff_inputfiles_data_SR")
    data_SR_geoff_inputfiles_v1.addSample("temp_data.root")
    data_SR_geoff_inputfiles_v1.addSample("temp_DoubleEG.root")
    data_SR_geoff_inputfiles_v1.addSample("temp_DoubleMuon.root")
    data_SR_geoff_inputfiles_v1.addSample("temp_Fakes.root")
    data_SR_geoff_inputfiles_v1.addSample("temp_MuonEG.root")
    data_SR_geoff_inputfiles_v1.addSample("temp_QFs.root")
    data_SR_geoff_inputfiles_v1.addSample("temp_SingleElectron.root")
    data_SR_geoff_inputfiles_v1.addSample("temp_SingleMuon.root")

    data_CR_geoff_inputfiles_v1 = Mixin("2019_12_11_geoff_inputfiles_data_CR")
    data_CR_geoff_inputfiles_v1.addSample("temp_data.root")
    data_CR_geoff_inputfiles_v1.addSample("temp_DoubleEG.root")
    data_CR_geoff_inputfiles_v1.addSample("temp_DoubleMuon.root")
    data_CR_geoff_inputfiles_v1.addSample("temp_Fakes.root")
    data_CR_geoff_inputfiles_v1.addSample("temp_MuonEG.root")
    data_CR_geoff_inputfiles_v1.addSample("temp_QFs.root")
    data_CR_geoff_inputfiles_v1.addSample("temp_SingleElectron.root")
    data_CR_geoff_inputfiles_v1.addSample("temp_SingleMuon.root")

    full_MC_CR_a32_NewHistRanges = Mixin("2019_12_13_full_MC_a32_NewHistRanges")
    full_MC_CR_a32_NewHistRanges.addSample("temp_WW.root")
    full_MC_CR_a32_NewHistRanges.addSample("temp_WZ.root")
    full_MC_CR_a32_NewHistRanges.addSample("temp_ZZ.root")
    full_MC_CR_a32_NewHistRanges.addSample("temp_WWW.root")
    full_MC_CR_a32_NewHistRanges.addSample("temp_WWZ.root")
    full_MC_CR_a32_NewHistRanges.addSample("temp_WZZ.root")
    full_MC_CR_a32_NewHistRanges.addSample("temp_ZZZ.root")
    full_MC_CR_a32_NewHistRanges.addSample("temp_ttGJets.root")
    # full_MC_CR_a32_NewHistRanges.addSample("temp_ttH.root")
    # full_MC_CR_a32_NewHistRanges.addSample("temp_ttW.root")
    # full_MC_CR_a32_NewHistRanges.addSample("temp_ttZ.root")
    # full_MC_CR_a32_NewHistRanges.addSample("temp_tZq.root")
    full_MC_CR_a32_NewHistRanges.addSample("temp_ttH_multidim.root")
    full_MC_CR_a32_NewHistRanges.addSample("temp_tHq_multidim.root")
    full_MC_CR_a32_NewHistRanges.addSample("temp_tllq_multidim.root")
    full_MC_CR_a32_NewHistRanges.addSample("temp_ttll_multidim.root")
    full_MC_CR_a32_NewHistRanges.addSample("temp_ttlnu_multidim.root")

    data_CR_geoff_inputfiles_NewHistRanges = Mixin("2019_12_13_geoff_inputfiles_data_CR_NewHistRanges")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_data.root")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_DoubleEG.root")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_DoubleMuon.root")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_Fakes.root")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_MuonEG.root")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_QFs.root")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_SingleElectron.root")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_SingleMuon.root")

    reproc_data_CR_NewHistRanges = Mixin("2019_12_13_reproc-data_CR_NewHistRanges")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_data.root")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_DoubleEG.root")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_DoubleMuon.root")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_Fakes.root")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_MuonEG.root")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_QFs.root")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_SingleElectron.root")
    data_CR_geoff_inputfiles_NewHistRanges.addSample("temp_SingleMuon.root")

    private_sgnl_SM_ISR_FSR_Systematics = Mixin("2020_01_20_private_sgnl_SM_ISR_FSR_Systematics_Default-PSWeights_moreStats")
    private_sgnl_SM_ISR_FSR_Systematics.addSample("temp_ttH_multidim.root")
    private_sgnl_SM_ISR_FSR_Systematics.addSample("temp_ttll_multidim.root")
    private_sgnl_SM_ISR_FSR_Systematics.addSample("temp_ttlnu_multidim.root")
    private_sgnl_SM_ISR_FSR_Systematics.addSample("temp_tllq_multidim.root")
    private_sgnl_SM_ISR_FSR_Systematics.addSample("temp_tHq_multidim.root")

    private_tllq_SM_Q2RF_check = Mixin("2020_01_14_private_tllq_SM_Q2RF")
    private_tllq_SM_Q2RF_check.addSample("temp_tllq_multidim.root")

    private_sgnl_EFT_DataLimsHigh = Mixin("2020_02_19_private_sgnl_EFT_DataLimsHigh")
    private_sgnl_EFT_DataLimsHigh.addSample("temp_ttH_multidim.root")
    private_sgnl_EFT_DataLimsHigh.addSample("temp_ttll_multidim.root")
    private_sgnl_EFT_DataLimsHigh.addSample("temp_ttlnu_multidim.root")
    private_sgnl_EFT_DataLimsHigh.addSample("temp_tllq_multidim.root")
    private_sgnl_EFT_DataLimsHigh.addSample("temp_tHq_multidim.root")

    private_sgnl_EFT_DataLimsLow = Mixin("2020_02_19_private_sgnl_EFT_DataLimsLow")
    private_sgnl_EFT_DataLimsLow.addSample("temp_ttH_multidim.root")
    private_sgnl_EFT_DataLimsLow.addSample("temp_ttll_multidim.root")
    private_sgnl_EFT_DataLimsLow.addSample("temp_ttlnu_multidim.root")
    private_sgnl_EFT_DataLimsLow.addSample("temp_tllq_multidim.root")
    private_sgnl_EFT_DataLimsLow.addSample("temp_tHq_multidim.root")

    ################################################################################################

    lst = [
        # standardHists,
        # new_pmx,
        # full_MC_2019_10_19,
        # standardHists,
        # central_MC_fixedErrors,
        # private_sgnl_a28_fixedErrors,
        # private_sgnl_a31_fixedErrors,
        # private_sgnl_a32,

        # private_sgnl_a32_SR_copyChanges,
        # central_sgnl_a32_SR_copyChanges,
        # central_bkgd_a32_SR_copyChanges,
        # data_SR_geoff_inputfiles_v1,
        private_sgnl_SM_ISR_FSR_Systematics,
        # private_tllq_SM_Q2RF_check,
        #private_sgnl_EFT_DataLimsHigh,
        #private_sgnl_EFT_DataLimsLow,
    ]
    lst.extend([#CR stuff
        #full_MC_CR_a32_NewHistRanges,
        #data_CR_geoff_inputfiles_NewHistRanges,
        # reproc_data_CR_NewHistRanges,
    ])
    # lst.extend([#CR lepOnlySelection
    #     full_MC_a32_CR_lepOnly,
    #     data_CR_lepOnly
    # ])
    lst.extend([
        # a28_redo,
        # a31,
        # a29_noDupes,
        # hanV4_SMCheck,
        # hanOrig_SMCheck,
        # a29_noDupesV2,
        # tllq_R6B1,
        # tllq_R6B2,
        # tllq_R6B3,
    ])

    target_dir = os.path.join(BASE_PATH,MIXED_DIR)

    if not os.path.exists(target_dir):
        err_str = "Target directory does not exist: {}".format(target_dir)
        raise RuntimeError(err_str)
    elif not os.path.isdir(target_dir):
        err_str = "Target is not a directory: {}".format(target_dir)
        raise RuntimeError(err_str)

    check_inputs(lst)

    print "Target Dir: {path}".format(path=target_dir)
    clean_dir(target_dir,["^temp_.*\.root$"],dry_run=False)

    for mixin in lst:
        print "From: {}".format(mixin.dir())
        for p in mixin.list():
            src_fpath = os.path.join(BASE_PATH,p)
            print "{}Copying {}".format(mixin.indent(1),src_fpath)
            shutil.copy(src_fpath,target_dir)

    print ""
    for mixin in lst: print mixin

if __name__ == "__main__":
    main()

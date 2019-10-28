import os
import argparse
from collections import deque

EVT_WHITELIST = set([
    "297050:323:408105593","297050:72:89347145","297056:143:200191640","297056:81:107769775","297057:115:156905948","297057:259:334802554",
    "297057:599:726161783","297057:628:758179933","297100:305:545655694","297100:339:597485326","297100:96:173625979","297101:252:350659367",
    "297101:486:645229085","297113:117:221614204","297114:68:115709224","297114:96:164284883","297175:12:19508902","297175:13:21266934",
    "297176:171:262103435","297176:180:275490315","297176:92:134969605","297177:24:35603957","297177:63:94727009","297178:1231:1414096053",
    "297178:24:33006771","297178:398:525945373","297178:404:532365303","297178:410:540714706","297178:427:561087624","297178:50:69846775",
    "297178:794:976686981","297178:804:987739660","297178:988:1178283466","297219:1104:1883697243","297219:1295:2129927192","297219:1405:-2028095705",
    "297219:1424:-2006282231","297219:1605:-1789678756","297219:1746:-1632924068","297219:1992:-1373713471","297219:203:414880366",
    "297219:510:976935010","297219:569:1077403055","297219:720:1323943085","297219:974:1703345640","297224:53:40141241","297227:107:78672829",
    "297292:125:231410819","297292:245:446195429","297292:255:463989359","297292:283:510832716","297292:286:515175280","297292:40:74757126",
    "297292:448:808862566","297292:487:880295593","297292:633:1137514917","297293:114:177271990","297293:135:209604754","297296:266:385206686",
    "297296:380:540089031","297359:166:231867530","297411:157:230315191","297411:164:243225962","297411:373:602799384","297411:729:1216900765",
    "297411:755:1260995072","297411:84:95769051","297411:926:1536043925","297424:142:197453726","297424:86:110179175","297425:12:18893045",
    "297426:77:139309473","297426:83:149968961","297426:96:173708011","297429:10:15626966","297429:58:100441690","297435:35:50099371",
    "297435:41:58796209","297435:44:61964856","297435:4:5065396","297467:63:70015741","297467:96:133063449","297468:15:26755785",
    "297468:44:79468575","297468:60:108739752","297468:61:111028003","297469:14:24860851","297469:30:52690106","297483:170:227108419",
    "297483:43:17914094","297484:117:227176737","297484:143:275647649","297484:165:316788647","297484:170:324862003","297485:227:390437281",
    "297486:21:34911152","297486:337:553785055","297486:464:747288738","297486:565:892175398","297486:568:896683083","297487:309:406329701",
    "297487:32:44193638","297487:7:8457383","297488:150:173073013","297488:163:187301096","297488:251:282329234","297488:258:289130161",
    "297488:45:50693197","297503:234:429624597","297503:31:56979722","297503:524:961329606","297503:616:1126831422","297503:687:1251872746",
    "297505:348:514187873","297505:40:61426244","297505:78:117937426","297557:104:161165538","297557:225:365469941","297557:250:405958013",
    "297557:266:431986903","297557:280:453128434","297557:332:543664907","297557:333:546751891","297558:179:293080655","297558:182:297649664",
    "297558:256:420690670","297558:89:142416779","297563:89:124373608","297599:27:45061093","297599:350:636637568","297599:86:133622529",
    "297603:181:321372445","297603:74:125258830","297604:288:467029761","297605:55:86326076","297606:17:20788257","297606:206:259437168",
    "297606:210:264232500","297620:226:325527261","297620:243:352206487","297620:256:373630430","297620:295:432437043","297620:58:44448759",
    "297620:74:72402473","297620:90:99486740","297656:104:143411066","297656:273:446429978","297656:360:596520705","297656:94:123544816",
    "297665:176:293035398","297666:111:173942109","297674:116:188112236","297674:12:17971277","297674:130:209408549","297674:52:85285377",
    "297675:169:290985432","297675:253:435151865","297675:271:464519476","297675:2:2343724","297675:339:580687278","297675:345:591027840",
    "297675:416:707473586","297722:150:192907316","297722:160:211169494","297722:285:432005852","297723:147:273825669","297723:208:385362984",
    "299062:274:450716649","299065:173:265517996","299065:320:456297193","299067:206:261631583","299067:245:310219532","299067:344:429187079",
    "299067:456:551574278","299149:347:480138341","299149:470:681225833","299149:73:62288682","299149:74:63754832","299149:96:99096372",
    "299178:85:59745175","299180:46:74033838","299180:52:83881459","299180:53:87110842","299184:108:168577217","299184:145:228513125",
    "299184:196:305764702","299184:254:408820749","299329:102:171889557","299329:19:31609734","299329:19:33007614"
])

arg_parser = argparse.ArgumentParser(prog='parse_sync_files',
    description="Script for comparing differences between sync files produced by the pick_events.C root macro. This script is highly dependent on the output format of the pick_events.C macro"
)
arg_parser.add_argument('--whitelist',
    metvar='PATH',
    help='Specify a path to a whitelist file. Only events from this list will be considered for comparison between sync files. The file should contain one event entry per line in the form RUN:LS:EVENT'
)
arg_parser.add_argument('infiles'
    metvar='PATH',
    nargs='+',
    help='%(metavars)s are paths to sync files produced by the pick_events.C root macro'
)

class Event(object):
    # This is exact order as produced by the pick_events.C macro
    OBJ_INDEX = [
        ('Category'   ,0,3,"Cat",0),
        ('nTightLeps' ,0,3,"Lep",0),
        ('nTightEles' ,0,3,"Ele",0),
        ('nTightMuons',0,3,"Muo",0),
        ('nCleanJets' ,0,3,"Jet",0)
    ]
    LEP_INDEX = [
        ('pt'            ,0.0,6,"pt",0.01),
        ('eta'           ,0.0,5,"eta",0.01),
        ('charge'        ,0  ,2,"Q",0),
        ('relIso'        ,0.0,0,"rIso",0.01),
        ('miniIso'       ,0.0,0,"mIso",0.01),
        ('lepMVA'        ,0.0,0,"lMVA",0.03),
        ('miniIsoCharged',0.0,0,"isoQ",0.01),
        ('miniIsoNeutral',0.0,0,"isoN",0.01)
    ]
    JET_INDEX = [
        ('pt'     ,0.0,7,"pt",5.00),
        ('eta'    ,0.0,5,"eta",0.01),
        ('DeepCSV',0.0,5,"DCSV",0.01)
    ]

    UID_WIDTH = 6+1+4+1+11

    def __init__(self,input_row):
        self.__run = -1
        self.__ls = -1
        self.__event = -1

        self.__obj_counts_data = []
        self.__lep1_data = []
        self.__lep2_data = []
        self.__jet1_data = []
        self.__jet2_data = []

        self._parse(input_row)

    def __str__(self):
        delim = ' '
        s = '{uid:<{w}}: '.format(uid=self.uid(),w=self.UID_WIDTH)
        s += self._form_data(self.__obj_counts_data,self.OBJ_INDEX)
        s += self._form_data(self.__lep1_data,self.LEP_INDEX)
        s += self._form_data(self.__lep2_data,self.LEP_INDEX)
        s += self._form_data(self.__jet1_data,self.JET_INDEX)
        s += self._form_data(self.__jet2_data,self.JET_INDEX)
        return s

    @classmethod
    def header(cls):
        delim = ' '
        s = '{:<{w}}: '.format('RUN:LS:EVENT',w=cls.UID_WIDTH)
        for tup in cls.OBJ_INDEX:
            col_name = tup[3] if len(tup[3]) else tup[0]
            col_width = max(1,tup[2])
            s += '{:>{w}}'.format(col_name,w=col_width) + delim
        for tup in cls.LEP_INDEX:
            col_name = tup[3] if len(tup[3]) else tup[0]
            col_width = max(1,tup[2])
            s += '{:>{w}}'.format(col_name,w=col_width) + delim
        for tup in cls.LEP_INDEX:
            col_name = tup[3] if len(tup[3]) else tup[0]
            col_width = max(1,tup[2])
            s += '{:>{w}}'.format(col_name,w=col_width) + delim
        for tup in cls.JET_INDEX:
            col_name = tup[3] if len(tup[3]) else tup[0]
            col_width = max(1,tup[2])
            s += '{:>{w}}'.format(col_name,w=col_width) + delim
        for tup in cls.JET_INDEX:
            col_name = tup[3] if len(tup[3]) else tup[0]
            col_width = max(1,tup[2])
            s += '{:>{w}}'.format(col_name,w=col_width) + delim
        return s

    @classmethod
    def is_equal(cls,diffs):
        for k,v in diffs.iteritems():
            if len(v): return False
        return True

    def _form_data(self,lst,index_tuple):
        delim = ' '
        s = ''
        for idx,tup in enumerate(index_tuple):
            typ = tup[1]
            width = tup[2]
            alt = tup[3]
            val = lst[idx]
            if isinstance(typ,str):
                s += '{}'.format(val)
            elif isinstance(typ,int):
                s += '{:>{w}d}'.format(int(val),w=width)
            elif isinstance(typ,float):
                s += '{:>{w}.2f}'.format(float(val),w=width)
            s += delim
        return s

    def _conv(self,x,typ):
        if isinstance(typ,str):
            return str(x)
        elif isinstance(typ,int):
            return int(x)
        elif isinstance(typ,float):
            return float(x)
        else:
            return x

    def _parse(self,row):
        queue = deque(row.split())
        self.__run, self.__ls, self.__event = queue.popleft().split(':')[1:]
        for tup in self.OBJ_INDEX: self.__obj_counts_data.append(self._conv(queue.popleft(),tup[1]))
        for tup in self.LEP_INDEX: self.__lep1_data.append(self._conv(queue.popleft(),tup[1]))
        for tup in self.LEP_INDEX: self.__lep2_data.append(self._conv(queue.popleft(),tup[1]))
        for tup in self.JET_INDEX: self.__jet1_data.append(self._conv(queue.popleft(),tup[1]))
        for tup in self.JET_INDEX: self.__jet2_data.append(self._conv(queue.popleft(),tup[1]))

        # print "Counts: {length}".format(length=len(self.__obj_counts_data))
        # print "lep1: {}".format(len(self.__lep1_data))
        # print "lep2: {}".format(len(self.__lep2_data))
        # print "jet1: {}".format(len(self.__jet1_data))
        # print "jet2: {}".format(len(self.__jet2_data))

    def uid(self):
        s = "{run}:{ls}:{event}".format(run=self.__run,ls=self.__ls,event=self.__event)
        return s

    def run(self):
        return self.__run

    def ls(self):
        return self.__ls

    def evtnum(self):
        return self.__event

    def getCountsData(self,idx):
        if idx < 0 or idx >= len(self.OBJ_INDEX):
            raise IndexError("list index {} out of range".format(idx))
        return self.__obj_counts_data[idx]

    def getJetData(self,idx,rank):
        if idx < 0 or idx >= len(self.JET_INDEX):
            raise IndexError("list index {} out of range".format(idx))
        if rank == 1:
            return self.__jet1_data[idx]
        elif rank == 2:
            return self.__jet2_data[idx]
        else:
            raise IndexError("Invalid rank {}".format(rank))

    def getLepData(self,idx,rank):
        if idx < 0 or idx >= len(self.LEP_INDEX):
            raise IndexError("list index {} out of range".format(idx))
        if rank == 1:
            return self.__lep1_data[idx]
        elif rank == 2:
            return self.__lep2_data[idx]
        else:
            raise IndexError("Invalid rank {}".format(rank))

    def getDiffs(self,other):
        diffs = {
            'objs': [],
            'lep1': [],
            'lep2': [],
            'jet1': [],
            'jet2': []
        }
        for idx,tup in enumerate(self.OBJ_INDEX):
            elem1 = self.getCountsData(idx)
            elem2 = other.getCountsData(idx)
            if elem1 != elem2:
                diffs['objs'].append(idx)
        for idx,tup in enumerate(self.LEP_INDEX):
            typ = tup[1]
            threshold = tup[4]
            elem1 = self.getLepData(idx,1)
            elem2 = other.getLepData(idx,1)
            if isinstance(typ,float) or isinstance(typ,int):
                if abs(elem1 - elem2) > threshold:
                    diffs['lep1'].append(idx)
            else:
                if elem1 != elem2:
                    diffs['lep1'].append(idx)
        for idx,tup in enumerate(self.LEP_INDEX):
            typ = tup[1]
            threshold = tup[4]
            elem1 = self.getLepData(idx,2)
            elem2 = other.getLepData(idx,2)
            if isinstance(typ,float) or isinstance(typ,int):
                if abs(elem1 - elem2) > threshold:
                    diffs['lep2'].append(idx)
            else:
                if elem1 != elem2:
                    diffs['lep2'].append(idx)
        for idx,tup in enumerate(self.JET_INDEX):
            typ = tup[1]
            threshold = tup[4]
            elem1 = self.getJetData(idx,1)
            elem2 = other.getJetData(idx,1)
            if isinstance(typ,float) or isinstance(typ,int):
                if abs(elem1 - elem2) > threshold:
                    diffs['jet1'].append(idx)
            else:
                if elem1 != elem2:
                    diffs['jet1'].append(idx)
        for idx,tup in enumerate(self.JET_INDEX):
            typ = tup[1]
            threshold = tup[4]
            elem1 = self.getJetData(idx,2)
            elem2 = other.getJetData(idx,2)
            if isinstance(typ,float) or isinstance(typ,int):
                if abs(elem1 - elem2) > threshold:
                    diffs['jet2'].append(idx)
            else:
                if elem1 != elem2:
                    diffs['jet2'].append(idx)
        return diffs

def parseFile(fpath):
    events = {}
    uid_set = set()
    if not os.path.exists(fpath):
        raise RuntimeError("Unknown fpath: {}".format(fpath))
    with open(fpath) as f:
        for idx,l in enumerate(f):
            try:
                ev = Event(l.strip())
            except:
                print "ERROR: Failed parsing file at line {}".format(idx)
                raise
            uid = ev.uid()
            if uid in uid_set:
                print "WARNING: Duplicate event found {uid}".format(uid=uid)
                continue
            uid_set.add(uid)
            events[uid] = ev
    return events

# def compare_syncs(evts1,evts2):
def compare_syncs(fpath1,fpath2):
    # print "f1: sync_a26_SingleMuon_EraB.dat"
    # print "f2: sync_a27_SingleMuon_EraB.dat"
    print "f1: {}".format(fpath1)
    print "f2: {}".format(fpath2)

    evts1 = parseFile(fpath1)
    evts2 = parseFile(fpath2)

    s1 = set(evts1.keys())
    s2 = set(evts2.keys())

    wl_diff1 = EVT_WHITELIST - s1
    wl_diff2 = EVT_WHITELIST - s2
    wl_diff3 = wl_diff1 & wl_diff2

    if len(wl_diff1):
        print "Whitelist events missing from f1: {}/{}".format(len(wl_diff1),len(EVT_WHITELIST))
        for evt_uid in sorted(wl_diff1):
            print "\t{}".format(evt_uid)
        print ""

    if len(wl_diff2):
        print "Whitelist events missing from f2: {}/{}".format(len(wl_diff2),len(EVT_WHITELIST))
        for evt_uid in sorted(wl_diff2):
            print "\t{}".format(evt_uid)
        print ""

    if len(wl_diff3):
        print "Whitelist events missing from both f1 and f2: {}/{}".format(len(wl_diff3),len(EVT_WHITELIST))
        for evt_uid in sorted(wl_diff3):
            print "\t{}".format(evt_uid)
        print ""

    s1 = s1 & EVT_WHITELIST
    s2 = s2 & EVT_WHITELIST

    diff1 = s1 - s2
    diff2 = s2 - s1
    inter = s1 & s2

    # lumi_mask = {}
    # for uid in diff1:
    #     run,ls,evt = uid.split(':')
    #     if not lumi_mask.has_key(run):
    #         lumi_mask[run] = []
    #     if not ls in lumi_mask[run]:
    #         lumi_mask[run].append(ls)
    # mask_str = "{"
    # for run in sorted(lumi_mask.keys()):
    #     lst = lumi_mask[run]
    #     if len(lst) == 0: continue
    #     tmp_lst = [int(x) for x in lst]
    #     tmp_lst.sort()
    #     ls_start = tmp_lst[0]
    #     ls_prev = ls_start
    #     lumi_str = ""
    #     for idx,ls in enumerate(tmp_lst):
    #         if ls - ls_prev > 1:
    #             lumi_str += "[{},{}],".format(ls_start,ls_prev)
    #             ls_start = ls
    #         ls_prev = ls
    #     lumi_str += "[{},{}]".format(ls_start,ls_prev)
    #     mask_str += '"{run}": [{lumi_list}],'.format(run=run,lumi_list=lumi_str)
    # mask_str += "}"
    # print mask_str
    # return

    if len(diff1):
        print "Events in f1 missing from f2: {}/{}".format(len(diff1),len(s1))
        for evt_uid in sorted(diff1):
            print "\t{}".format(evt_uid)
        print ""

    if len(diff2):
        print "Events in f2 missing from f1: {}/{}".format(len(diff2),len(s2))
        for evt_uid in sorted(diff2):
            print "\t{}".format(evt_uid)
        print ""

    print "Events in f1 and f2: {}".format(len(inter))
    print ""

    unchanged_events = []
    changed_events = []
    for ev_uid in sorted(inter):
        evt1 = evts1[ev_uid]
        evt2 = evts2[ev_uid]

        diffs = evt1.getDiffs(evt2)
        if Event.is_equal(diffs):
            unchanged_events.append(ev_uid)
            continue
        changed_events.append(ev_uid)
        print Event.header()
        print evt1
        print evt2
        for k,lst in diffs.iteritems():
            if len(lst) == 0: continue
            index = []
            if k == 'objs':
                index = Event.OBJ_INDEX
            elif k == 'lep1':
                index = Event.LEP_INDEX
            elif k == 'lep2':
                index = Event.LEP_INDEX
            elif k == 'jet1':
                index = Event.JET_INDEX
            elif k == 'jet2':
                index = Event.JET_INDEX
            for idx in lst:
                name = index[idx][0]
                threshold = index[idx][4]
                print "\t{obj}: {col} > {thresh}".format(obj=k,col=name,thresh=threshold)
        print ""
    print "Changed events: {}/{}".format(len(changed_events),len(inter))
    print "Un-changed events: {}/{}".format(len(unchanged_events),len(inter))
    for ev_uid in unchanged_events:
        print "\t{}".format(ev_uid)

def main():
    # fpath = "ex_file.dat"
    # fpath1 = "sync_a26_SingleMuon_EraB.dat"
    # fpath2 = "sync_a27_SingleMuon_EraB.dat"

    # fpath1 = "sync_GTv6_SingleMuon_failed_events.dat"
    # fpath2 = "sync_GTv11_SingleMuon_failed_events.dat"

    fpath1 = "sync_GTv6_SingleMuon_failed_events_psleptons.dat"
    fpath2 = "sync_GTv11_SingleMuon_failed_events_psleptons.dat"

    #fpath1 = "/afs/crc.nd.edu/user/b/byates2/CMSSW_9_4_6/src/EFTMultilepton/TemplateMakers/test/anaTree_Checks/sync_a26_DoubleMuon_EraB.dat"
    #fpath2 = "/afs/crc.nd.edu/user/b/byates2/CMSSW_9_4_6/src/EFTMultilepton/TemplateMakers/test/anaTree_Checks/sync_a27_DoubleMuon_EraB.dat"

    #fpath1 = "sync_oldGT_skim.dat"
    #fpath2 = "sync_newGT_skim.dat"

    # fpath1 = "sync_a26_DoubleMuonB.dat"  # Calculated from Geoff's trees
    # fpath2 = "/afs/crc.nd.edu/user/b/byates2/CMSSW_9_4_6/src/EFTMultilepton/TemplateMakers/test/anaTree_Checks/sync_a26_DoubleMuon_EraB.dat"   # Calculated from my trees on the old GT

    compare_syncs(fpath1,fpath2)

if __name__ == "__main__":
    main()
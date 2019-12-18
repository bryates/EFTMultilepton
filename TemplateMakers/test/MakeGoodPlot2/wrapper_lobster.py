#### For use with config_lobster.py ####

import subprocess
import sys

thisamp = sys.argv[1]
#infiles = sys.argv[2:]
thing   = sys.argv[2]
infiles = sys.argv[3:]

print thisamp
print thing
print infiles

fname = "files.txt"
outf = "output.root"

with open(fname,'w') as fd:
    for fn in infiles:
        fd.write('{}\n'.format(fn))

#subprocess.check_call(["root", "-b", "-l", "-q", "wrapper_lobster.C+(\""+thisamp+"\", \"files.txt\", \"output.root\")"])

cmd_args = "\"{samp}\",\"{inf}\",\"{outf}\"".format(samp=thisamp,inf=fname,outf=outf)
if (thing=='nom'):
    print "nom"
    subprocess.check_call(["root","-b","-l","-q","wrapper_lobster.C+({args})".format(args=cmd_args)])
else:
    print "not nom!"
    #if (thisamp=='DoubleEG' or thisamp=='DoubleMuon' or thisamp=='MuonEG' or thisamp=='SingleElectron' or thisamp=='SingleMuon'):
    if (thing=='_QFs'):
        print "QFs"
        subprocess.check_call(["root","-b","-l","-q","wrapper_lobster.C+({args},true,false)".format(args=cmd_args)])
    elif (thing=='_Fakes'):
        print "Fakes"
        subprocess.check_call(["root","-b","-l","-q","wrapper_lobster.C+({args},false,true)".format(args=cmd_args)])

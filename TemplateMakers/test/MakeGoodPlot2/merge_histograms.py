import sys
import os
import glob
import subprocess
import argparse
import logging
import datetime

from EFTMultilepton.TemplateMakers.LvlFormatter import LvlFormatter

TIMESTAMP1 = datetime.datetime.now().strftime('%Y_%m_%d')
TIMESTAMP2 = datetime.datetime.now().strftime('%Y%m%d_%H%M')

MAX_MERGE = 50

USER_NAME = os.environ['USER']
USER_DIR = os.path.expanduser('~')
HOME_DIR = os.getcwd()
TMPSCRATCH_DIR = os.path.join('/tmpscratch/users',USER_NAME,'analysisWorkflow')
MERGED_DIR = os.path.join(TMPSCRATCH_DIR,'mergedHists')

if not os.path.exists(TMPSCRATCH_DIR):
    err = "[ERROR] {user} does not have a /tmpscratch/users/$USER area setup, please set one up before using this script!".format(user=USER_NAME)
    raise RuntimeError(err)

arg_parser = argparse.ArgumentParser(prog='merge_histograms.py',
    description="Script used to merge the final output of the lobster jobs from the histMaking step. To be used as a replacement to the 'haddhists.py' script. All output will be placed under /tmpscratch/users/$USER/analysisWorkflow/mergedHists"
)
arg_parser.add_argument('-o','--out-dir',
    metavar='DIR',
    default=os.path.join('merged_output'),
    help='Specify the name of the directory that the merged files will be placed in. This should be a simple name with no path specification'
)
arg_parser.add_argument('--no-timestamp',
    action='store_true',
    help='Do not prefix the output directory name with a timestamp')
arg_parser.add_argument('--prefix',
    metavar='NAME',
    default='temp',
    help='Prefix each merged sample output root file with %(metavar)s'
)
arg_parser.add_argument('--sub-dir',
    action='store_true',
    help='The merged outputs will be placed in their own sub-directories, based on sample name'
)
arg_parser.add_argument('-k','--keep-old',
    action='store_true',
    help='If an already existing output directory is used, will not raise an error. Instead will skip merging samples which already exist, except in the case that --is-data is specified'
)
arg_parser.add_argument('-f','--force',
    action='store_true',
    help='Remove existing output directory'
)
arg_parser.add_argument('-b','--input-base-path',
    metavar='PATH',
    help='Specify a base path that all input directory paths should be appended to'
)
arg_parser.add_argument('-j','--ncores',
    metavar='N',
    type=int,
    help='Tells hadd to use %(metavar)s cores'
)
arg_parser.add_argument('--is-data',
    action='store_true',
    help='All input directories are from data. This means that all specified input samples will be merged into the same root file'
)
arg_parser.add_argument('--testing',
    action='store_true',
    help='Run the code in test mode. Note: This will disregard the --out-dir and --no-timestamp options. Instead all output will be in a test directory named like test/merged_output_20XXYYZZ_HHMM'
)
arg_parser.add_argument('indirs',
    metavar='DIR',
    nargs='+',
    help='%(metavar)s are paths to input directories with root files produced by the HistMaking code to be finally merged. If --input-base-path is set, then these directories should all be relative to the specified base path'
)
args = arg_parser.parse_args()

frmt = LvlFormatter()
logging.getLogger().setLevel(logging.DEBUG)

# Configure logging to also output to stdout
console = logging.StreamHandler()
console.setLevel(logging.INFO)
console.setFormatter(frmt)
logging.getLogger('').addHandler(console)

def run_process(inputs,verbose=True,indent=0,prepend=''):
    indent_str = "\t"*indent
    p = subprocess.Popen(inputs,stdout=subprocess.PIPE)
    stdout = []
    while True:
        l = p.stdout.readline()
        if l == '' and p.poll() is not None:
            break
        if l:
            stdout.append(l.strip())
            s = indent_str+l.strip()
            if verbose:
                logging.info(s)
            else:
                logging.debug(s)
    return stdout

# TODO: Fix issue with directories specified with a trailing slash not naming merged output files correctly
def main():
    in_dirs = []
    for d in args.indirs:
        dpath = os.path.join(args.input_base_path,d) if args.input_base_path else d
        if not os.path.exists(dpath):
            err = "[ERROR] Unknown path {path}".format(path=dpath)
            raise RuntimeError(err)
        elif not os.path.isdir(dpath):
            err = "[ERROR] Input must be a directory: {path}".format(path=dpath)
            raise RuntimeError(err)
        in_dirs.append(dpath)

    out_dir_name = "{tstamp}_{dir}".format(tstamp=TIMESTAMP1,dir=args.out_dir)
    if args.no_timestamp:
        out_dir_name = "{dir}".format(dir=args.out_dir)
    out_dir = os.path.join(MERGED_DIR,out_dir_name)
    if args.testing:
        out_dir = os.path.join(MERGED_DIR,'test',"merged_output_{tstamp}".format(tstamp=TIMESTAMP2))

    if os.path.exists(out_dir) and not args.keep_old:
        err = "[ERROR] Output directory already exists: {path}".format(path=out_dir)
        raise RuntimeError(err)

    if not os.path.exists(out_dir):
        # pass
        os.makedirs(out_dir)
        # os.mkdir(out_dir)

    log_file = os.path.join(out_dir,'out.log')
    outlog = logging.FileHandler(filename=log_file,mode='w')
    outlog.setLevel(logging.DEBUG)
    outlog.setFormatter(frmt)
    logging.getLogger('').addHandler(outlog)

    logging.info("Command-line: {cmd}".format(cmd=' '.join(sys.argv)))
    logging.info("Output directory: {path}".format(path=out_dir))
    logging.info("Log file: {file}".format(file=log_file))
    if args.input_base_path:
        logging.info("Hadoop path: {path}".format(path=args.input_base_path))

    for in_dir in in_dirs:
        head,tail = os.path.split(in_dir)
        logging.info("Merging output: {dir}".format(dir=tail))
        sample = tail
        if args.is_data:
            if tail[-4:] == '_QFs':
                sample = 'data_QFs'
            elif tail[-6:] == '_Fakes':
                sample = 'data_Fakes'
            else:
                sample = 'data'
        sub_dir = out_dir
        if args.sub_dir:
            sub_dir = os.path.join(out_dir,sample)
            
        if not os.path.exists(sub_dir):
            logging.info("Creating sub-directory: {path}".format(path=sub_dir))
            os.mkdir(sub_dir)

        tar_file = os.path.join(sub_dir,"{prefix}_{samp}.root".format(prefix=args.prefix,samp=sample))
        if args.sub_dir:
            tar_file = os.path.join(sub_dir,"{prefix}.root".format(prefix=args.prefix))

        logging.info("Output file: {file}".format(file=tar_file))
        if os.path.exists(tar_file) and not args.is_data:
            logging.warning("Skipping sample that already exists in the output directory: {samp}".format(samp=sample))
            continue

        src_files = glob.glob(os.path.join(in_dir,"output_*.root"))
        if len(src_files) < 1:
            err = "Not enough files to merge!"
            logging.error(err)
            raise RuntimeError(err)
        elif len(src_files) > MAX_MERGE:
            err = "Trying to merge more than {max} files at once!".format(max=MAX_MERGE)
            logging.error(err)
            raise RuntimeError(err)

        base_cmd = ['hadd','-k']
        if args.is_data and os.path.exists(tar_file):
            base_cmd.extend(['-a'])
        else:
            base_cmd.extend(['-f'])
        if args.ncores:
            base_cmd.extend(['-j',args.cores])

        hadd_cmd = base_cmd + [tar_file]
        hadd_cmd.extend(src_files)

        # This is only to try and make the print statements more readable by pruning the absolute file paths
        if args.input_base_path:
            rel_srcs = ['\n\t(hadoop_path)/'+os.path.relpath(x,args.input_base_path) for x in src_files]
        else:
            rel_srcs = [x for x in src_files]
        cleaned_cmd = base_cmd + [tar_file]
        cleaned_cmd.extend(rel_srcs)
        logging.info("Merge target: {path}".format(path=tar_file))
        logging.info("Merge command: {cmd}".format(cmd=' '.join(cleaned_cmd)))
        run_process(hadd_cmd)
    logging.info("Finished!")


if __name__ == "__main__":
    main()

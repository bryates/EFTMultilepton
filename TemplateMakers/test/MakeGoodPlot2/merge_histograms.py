import sys
import os
import glob
import subprocess
import argparse

USER_DIR = os.path.expanduser('~')
HOME_DIR = os.getcwd()

MAX_MERGE = 50

arg_parser = argparse.ArgumentParser(prog='merge_histograms.py',
    description="Script used to merge the final output of the lobster job used to generate the analysis histograms. To be used as a replacement to the 'haddhists.py' script"
)
arg_parser.add_argument('-o','--out-dir',
    metavar='DIR',
    default=os.path.join(HOME_DIR,'merged_output'),
    help='Specify an output directory under which all merged content will be placed'
)
arg_parser.add_argument('--prefix',
    metavar='NAME',
    default='merged',
    help='Prefix each merged sample output root file with %(metavar)s'
)
arg_parser.add_argument('--sub-dir',
    action='store_true',
    help='The merged outputs will be placed in their own sub-directories, based on sample name'
)
arg_parser.add_argument('-k','--keep-old',
    action='store_true',
    help='If an already existing output directory is used, will not raise an error. Instead will skip merging samples which already exist, except in the case that --is-dir is specified'
)
arg_parser.add_argument('-f','--force',
    action='store_true',
    help='Remove existing output directory'
)
arg_parser.add_argument('-b','--base-path',
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
arg_parser.add_argument('indirs',
    metavar='DIR',
    nargs='+',
    help='%(metavar)s are paths to input directories with root files produced by the HistMaking code to be finally merged'
)
args = arg_parser.parse_args()

# Pipes subprocess messages to STDOUT
def run_process(inputs,verbose=True,indent=0,prepend=''):
    # Note: This will hold the main thread and wait for the subprocess to complete
    indent_str = "\t"*indent+prepend
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

# TODO: Fix issue with directories specified with a trailing slash not naming merged output files correctly
def main():
    in_dirs = []
    for d in args.indirs:
        dpath = os.path.join(args.base_path,d) if args.base_path else d
        if not os.path.exists(dpath):
            err = "[ERROR] Unknown path {path}".format(path=dpath)
            raise RuntimeError(err)
        elif not os.path.isdir(dpath):
            err = "[ERROR] Input must be a directory: {path}".format(path=dpath)
            raise RuntimeError(err)
        in_dirs.append(dpath)

    out_dir = args.out_dir
    if os.path.exists(out_dir) and not args.keep_old:
        err = "[ERROR] Output directory already exists: {path}".format(path=out_dir)
        raise RuntimeError(err)

    if args.base_path:
        print "Hadoop path: {path}".format(path=args.base_path)

    if not os.path.exists(out_dir):
        print "Creating output directory: {path}".format(path=out_dir)
        os.mkdir(out_dir)

    for in_dir in in_dirs:
        head,tail = os.path.split(in_dir)
        print "Merging output: {dir}".format(dir=tail)
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
            print "\tCreating sub-directory: {path}".format(path=os.path.relpath(sub_dir,HOME_DIR))
            os.mkdir(sub_dir)

        tar_file = os.path.join(sub_dir,"{prefix}_{samp}.root".format(prefix=args.prefix,samp=sample))
        if args.sub_dir:
            tar_file = os.path.join(sub_dir,"{prefix}.root".format(prefix=args.prefix))

        if os.path.exists(tar_file) and not args.is_data:
            print "[WARNING] Skipping sample that already exists in the output directory: {samp}".format(samp=sample)
            continue

        src_files = glob.glob(os.path.join(in_dir,"output_*.root"))
        if len(src_files) < 1:
            err = "[ERROR] Not enough files to merge!"
            raise RuntimeError(err)
        elif len(src_files) > MAX_MERGE:
            err = "[ERROR] Trying to merge more than {max} files at once!".format(max=MAX_MERGE)
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
        rel_tar = os.path.relpath(tar_file,HOME_DIR)
        if args.base_path:
            rel_srcs = ['(hadoop_path)/'+os.path.relpath(x,args.base_path) for x in src_files]
        else:
            rel_srcs = [x for x in src_files]
        cleaned_cmd = base_cmd + [rel_tar]
        cleaned_cmd.extend(rel_srcs)
        print "\tMerge target: {path}".format(path=rel_tar)
        print "\tMerge command: {cmd}".format(cmd=' '.join(cleaned_cmd))

        run_process(hadd_cmd)


if __name__ == "__main__":
    main()
    print "Finished!"

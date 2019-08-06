#! /usr/bin/env python
import os
import re
from EFTMultilepton.DrawPlots.utilities.prettytable import *
from argparse import ArgumentParser
import ROOT

parser = ArgumentParser(description='Dump event information from a summary tree.')
parser.add_argument('tree_file_name', help='ROOT file with tree to dump.')
parser.add_argument('--event_list', action='store', help='Dump only events listed in event list.')
parser.add_argument('--branches', nargs='+', help='Dump only these branches')
parser.add_argument('--max_events', help='Max events to run on')

args = parser.parse_args()
tree_file = ROOT.TFile(args.tree_file_name)
tree = tree_file.Get('summaryTree')
branches = [branch.GetName() for branch in tree.GetListOfBranches()]
if args.branches:
    branches = args.branches
dump_table = PrettyTable(branches)
dump_table.float_format = '4.2'

selected_run_lumi_events = []
if args.event_list:
    with open(args.event_list) as event_list:
        lines_with_numbers = [line for line in event_list.readlines() if re.search('\d', line)]
        for line in lines_with_numbers:
            if ',' in line or ':' in line:
                run_lumi_events = [entry.strip() for entry in re.split('[,:]', line.strip())] #can be comma or colon delineated
            else:
                run_lumi_events = [entry.strip() for entry in re.split('\w', line)]
            selected_run_lumi_events.append([int(entry) for entry in run_lumi_events[:3]]) #in case there are extra columns after run, lumi, event

for index, event in enumerate(tree):
    if args.max_events and index > args.max_events:
        continue
    if index % 20000 == 0 and index != 0:
        print '%.0f%% finished...' % (float(index) / tree.GetEntries() * 100)
    if [tree.eventInfo_run, tree.eventInfo_lumi, tree.eventInfo_evt] in selected_run_lumi_events or not args.event_list:
        dump_table.add_row([getattr(tree, branch) for branch in branches])

output_file_name = '%s_branch_value_dump.txt' % args.tree_file_name.replace('.root', '')
if '/' in output_file_name:
    output_file_name = output_file_name[output_file_name.rfind('/')+1:]
print 'Writing dump table to %s...' % output_file_name
with open(output_file_name, 'w+') as output_file:
    output_file.write(dump_table.get_string())

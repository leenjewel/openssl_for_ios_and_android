#!/usr/bin/env python
#
# Copyright (C) 2016 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

"""report_sample.py: report samples in the same format as `perf script`.
"""

from __future__ import print_function
import argparse
from simpleperf_report_lib import ReportLib


def report_sample(record_file, symfs_dir, kallsyms_file, show_tracing_data):
    """ read record_file, and print each sample"""
    lib = ReportLib()

    lib.ShowIpForUnknownSymbol()
    if symfs_dir is not None:
        lib.SetSymfs(symfs_dir)
    if record_file is not None:
        lib.SetRecordFile(record_file)
    if kallsyms_file is not None:
        lib.SetKallsymsFile(kallsyms_file)

    while True:
        sample = lib.GetNextSample()
        if sample is None:
            lib.Close()
            break
        event = lib.GetEventOfCurrentSample()
        symbol = lib.GetSymbolOfCurrentSample()
        callchain = lib.GetCallChainOfCurrentSample()

        sec = sample.time / 1000000000
        usec = (sample.time - sec * 1000000000) / 1000
        print('%s\t%d [%03d] %d.%06d:\t\t%d %s:' % (sample.thread_comm,
                                                    sample.tid, sample.cpu, sec,
                                                    usec, sample.period, event.name))
        print('%16x\t%s (%s)' % (sample.ip, symbol.symbol_name, symbol.dso_name))
        for i in range(callchain.nr):
            entry = callchain.entries[i]
            print('%16x\t%s (%s)' % (entry.ip, entry.symbol.symbol_name, entry.symbol.dso_name))
        if show_tracing_data:
            data = lib.GetTracingDataOfCurrentSample()
            if data:
                print('\ttracing data:')
                for key, value in data.items():
                    print('\t\t%s : %s' % (key, value))
        print('')


def main():
    parser = argparse.ArgumentParser(description='Report samples in perf.data.')
    parser.add_argument('--symfs',
                        help='Set the path to find binaries with symbols and debug info.')
    parser.add_argument('--kallsyms', help='Set the path to find kernel symbols.')
    parser.add_argument('record_file', nargs='?', default='perf.data',
                        help='Default is perf.data.')
    parser.add_argument('--show_tracing_data', action='store_true', help='print tracing data.')
    args = parser.parse_args()
    report_sample(args.record_file, args.symfs, args.kallsyms, args.show_tracing_data)


if __name__ == '__main__':
    main()

/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.simpleperf;

import android.system.Os;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * This class sets record options used by ProfileSession. The options are
 * converted to a string list in toRecordArgs(), which is then passed to
 * `simpleperf record` cmd. Run `simpleperf record -h` or
 * `run_simpleperf_on_device.py record -h` for help messages.
 * </p>
 *
 * <p>
 * Example:
 *   RecordOptions options = new RecordOptions();
 *   options.setDuration(3).recordDwarfCallGraph().setOutputFilename("perf.data");
 *   ProfileSession session = new ProfileSession();
 *   session.startRecording(options);
 * </p>
 */
public class RecordOptions {

    /**
     * Set output filename. Default is perf-<month>-<day>-<hour>-<minute>-<second>.data.
     * The file will be generated under simpleperf_data/.
     */
    public RecordOptions setOutputFilename(String filename) {
        outputFilename = filename;
        return this;
    }

    /**
     * Set event to record. Default is cpu-cycles. See `simpleperf list` for all available events.
     */
    public RecordOptions setEvent(String event) {
        this.event = event;
        return this;
    }

    /**
     * Set how many samples to generate each second running. Default is 4000.
     */
    public RecordOptions setSampleFrequency(int freq) {
        this.freq = freq;
        return this;
    }

    /**
     * Set record duration. The record stops after `durationInSecond` seconds. By default,
     * record stops only when stopRecording() is called.
     */
    public RecordOptions setDuration(double durationInSecond) {
        this.durationInSecond = durationInSecond;
        return this;
    }

    /**
     * Record some threads in the app process. By default, record all threads in the process.
     */
    public RecordOptions setSampleThreads(List<Integer> threads) {
        this.threads.addAll(threads);
        return this;
    }

    /**
     * Record dwarf based call graph. It is needed to get Java callstacks.
     */
    public RecordOptions recordDwarfCallGraph() {
        this.dwarfCallGraph = true;
        this.fpCallGraph = false;
        return this;
    }

    /**
     * Record frame pointer based call graph. It is suitable to get C++ callstacks on 64bit devices.
     */
    public RecordOptions recordFramePointerCallGraph() {
        this.fpCallGraph = true;
        this.dwarfCallGraph = false;
        return this;
    }

    /**
     * Trace context switch info to show where threads spend time off cpu.
     */
    public RecordOptions traceOffCpu() {
        this.traceOffCpu = true;
        return this;
    }

    /**
     * Translate record options into arguments for `simpleperf record` cmd.
     */
    public List<String> toRecordArgs() {
        ArrayList<String> args = new ArrayList<>();

        String filename = outputFilename;
        if (filename == null) {
            filename = getDefaultOutputFilename();
        }
        args.add("-o");
        args.add(filename);
        args.add("-e");
        args.add(event);
        args.add("-f");
        args.add(String.valueOf(freq));
        if (durationInSecond != 0.0) {
            args.add("--duration");
            args.add(String.valueOf(durationInSecond));
        }
        if (threads.isEmpty()) {
            args.add("-p");
            args.add(String.valueOf(Os.getpid()));
        } else {
            String s = "";
            for (int i = 0; i < threads.size(); i++) {
                if (i > 0) {
                    s += ",";
                }
                s += threads.get(i).toString();
            }
            args.add("-t");
            args.add(s);
        }
        if (dwarfCallGraph) {
            args.add("-g");
        } else if (fpCallGraph) {
            args.add("--call-graph");
            args.add("fp");
        }
        if (traceOffCpu) {
            args.add("--trace-offcpu");
        }
        return args;
    }

    private String getDefaultOutputFilename() {
        LocalDateTime time = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("'perf'-MM-dd-HH-mm-ss'.data'");
        return time.format(formatter);
    }

    private String outputFilename;
    private String event = "cpu-cycles";
    private int freq = 4000;
    private double durationInSecond = 0.0;
    private ArrayList<Integer> threads = new ArrayList<>();
    private boolean dwarfCallGraph = false;
    private boolean fpCallGraph = false;
    private boolean traceOffCpu = false;
}

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

import android.os.Build;
import android.system.OsConstants;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

/**
 * <p>
 * This class uses `simpleperf record` cmd to generate a recording file.
 * It allows users to start recording with some options, pause/resume recording
 * to only profile interested code, and stop recording.
 * </p>
 *
 * <p>
 * Example:
 *   RecordOptions options = new RecordOptions();
 *   options.setDwarfCallGraph();
 *   ProfileSession session = new ProfileSession();
 *   session.StartRecording(options);
 *   Thread.sleep(1000);
 *   session.PauseRecording();
 *   Thread.sleep(1000);
 *   session.ResumeRecording();
 *   Thread.sleep(1000);
 *   session.StopRecording();
 * </p>
 *
 * <p>
 * It throws an Error when error happens. To read error messages of simpleperf record
 * process, filter logcat with `simpleperf`.
 * </p>
 */
public class ProfileSession {
    private static final String SIMPLEPERF_PATH_IN_IMAGE = "/system/bin/simpleperf";

    enum State {
        NOT_YET_STARTED,
        STARTED,
        PAUSED,
        STOPPED,
    }

    private State state = State.NOT_YET_STARTED;
    private String appDataDir;
    private String simpleperfPath;
    private String simpleperfDataDir;
    private Process simpleperfProcess;
    private boolean traceOffcpu = false;

    /**
     * @param appDataDir the same as android.content.Context.getDataDir().
     *                   ProfileSession stores profiling data in appDataDir/simpleperf_data/.
     */
    public ProfileSession(String appDataDir) {
        this.appDataDir = appDataDir;
        simpleperfDataDir = appDataDir + "/simpleperf_data";
    }

    /**
     * ProfileSession assumes appDataDir as /data/data/app_package_name.
     */
    public ProfileSession() {
        String packageName = "";
        try {
            String s = readInputStream(new FileInputStream("/proc/self/cmdline"));
            for (int i = 0; i < s.length(); i++) {
                if (s.charAt(i) == '\0') {
                    s = s.substring(0, i);
                    break;
                }
            }
            packageName = s;
        } catch (IOException e) {
            throw new Error("failed to find packageName: " + e.getMessage());
        }
        if (packageName.isEmpty()) {
            throw new Error("failed to find packageName");
        }
        appDataDir = "/data/data/" + packageName;
        simpleperfDataDir = appDataDir + "/simpleperf_data";
    }

    /**
     * Start recording.
     * @param options RecordOptions
     */
    public void startRecording(RecordOptions options) {
        startRecording(options.toRecordArgs());
    }

    /**
     * Start recording.
     * @param args arguments for `simpleperf record` cmd.
     */
    public synchronized void startRecording(List<String> args) {
        if (state != State.NOT_YET_STARTED) {
            throw new AssertionError("startRecording: session in wrong state " + state);
        }
        for (String arg : args) {
            if (arg.equals("--trace-offcpu")) {
                traceOffcpu = true;
            }
        }
        simpleperfPath = findSimpleperf();
        checkIfPerfEnabled();
        createSimpleperfDataDir();
        createSimpleperfProcess(simpleperfPath, args);
        state = State.STARTED;
    }

    /**
     * Pause recording. No samples are generated in paused state.
     */
    public synchronized void pauseRecording() {
        if (state != State.STARTED) {
            throw new AssertionError("pauseRecording: session in wrong state " + state);
        }
        if (traceOffcpu) {
            throw new AssertionError(
                    "--trace-offcpu option doesn't work well with pause/resume recording");
        }
        sendCmd("pause");
        state = State.PAUSED;
    }

    /**
     * Resume a paused session.
     */
    public synchronized void resumeRecording() {
        if (state != State.PAUSED) {
            throw new AssertionError("resumeRecording: session in wrong state " + state);
        }
        sendCmd("resume");
        state = State.STARTED;
    }

    /**
     * Stop recording and generate a recording file under appDataDir/simpleperf_data/.
     */
    public synchronized void stopRecording() {
        if (state != State.STARTED && state != State.PAUSED) {
            throw new AssertionError("stopRecording: session in wrong state " + state);
        }
        if (Build.VERSION.SDK_INT == Build.VERSION_CODES.P + 1 &&
                simpleperfPath.equals(SIMPLEPERF_PATH_IN_IMAGE)) {
            // The simpleperf shipped on Android Q contains a bug, which may make it abort if
            // calling simpleperfProcess.destroy().
            destroySimpleperfProcessWithoutClosingStdin();
        } else {
            simpleperfProcess.destroy();
        }
        try {
            int exitCode = simpleperfProcess.waitFor();
            if (exitCode != 0) {
                throw new AssertionError("simpleperf exited with error: " + exitCode);
            }
        } catch (InterruptedException e) {
        }
        simpleperfProcess = null;
        state = State.STOPPED;
    }

    private void destroySimpleperfProcessWithoutClosingStdin() {
        // In format "Process[pid=? ..."
        String s = simpleperfProcess.toString();
        final String prefix = "Process[pid=";
        if (s.startsWith(prefix)) {
            int startIndex = prefix.length();
            int endIndex = s.indexOf(',');
            if (endIndex > startIndex) {
                int pid = Integer.parseInt(s.substring(startIndex, endIndex).trim());
                android.os.Process.sendSignal(pid, OsConstants.SIGTERM);
                return;
            }
        }
        simpleperfProcess.destroy();
    }

    private String readInputStream(InputStream in) {
        BufferedReader reader = new BufferedReader(new InputStreamReader(in));
        String result = reader.lines().collect(Collectors.joining("\n"));
        try {
            reader.close();
        } catch (IOException e) {
        }
        return result;
    }

    private String findSimpleperf() {
        // 1. Try /data/local/tmp/simpleperf. Probably it's newer than /system/bin/simpleperf.
        String simpleperfPath = findSimpleperfInTempDir();
        if (simpleperfPath != null) {
            return simpleperfPath;
        }
        // 2. Try /system/bin/simpleperf, which is available on Android >= Q.
        simpleperfPath = SIMPLEPERF_PATH_IN_IMAGE;
        if (isExecutableFile(simpleperfPath)) {
            return simpleperfPath;
        }
        throw new Error("can't find simpleperf on device. Please run api_profiler.py.");
    }

    private boolean isExecutableFile(String path) {
        File file = new File(path);
        return file.canExecute();
    }

    private String findSimpleperfInTempDir() {
        String path = "/data/local/tmp/simpleperf";
        File file = new File(path);
        if (!file.isFile()){
            return null;
        }
        // Copy it to app dir to execute it.
        String toPath = appDataDir + "/simpleperf";
        try {
            Process process = new ProcessBuilder()
                    .command("cp", path, toPath).start();
            process.waitFor();
        } catch (Exception e) {
            return null;
        }
        if (!isExecutableFile(toPath)) {
            return null;
        }
        // For apps with target sdk >= 29, executing app data file isn't allowed. So test executing
        // it.
        try {
            Process process = new ProcessBuilder()
                    .command(toPath).start();
            process.waitFor();
        } catch (Exception e) {
            return null;
        }
        return toPath;
    }

    private void checkIfPerfEnabled() {
        String value = "";
        Process process;
        try {
            process = new ProcessBuilder()
                    .command("/system/bin/getprop", "security.perf_harden").start();
        } catch (IOException e) {
            // Omit check if getprop doesn't exist.
            return;
        }
        try {
            process.waitFor();
        } catch (InterruptedException e) {
        }
        value = readInputStream(process.getInputStream());
        if (value.startsWith("1")) {
            throw new Error("linux perf events aren't enabled on the device." +
                            " Please run api_profiler.py.");
        }
    }

    private void createSimpleperfDataDir() {
        File file = new File(simpleperfDataDir);
        if (!file.isDirectory()) {
            file.mkdir();
        }
    }

    private void createSimpleperfProcess(String simpleperfPath, List<String> recordArgs) {
        // 1. Prepare simpleperf arguments.
        ArrayList<String> args = new ArrayList<>();
        args.add(simpleperfPath);
        args.add("record");
        args.add("--log-to-android-buffer");
        args.add("--log");
        args.add("debug");
        args.add("--stdio-controls-profiling");
        args.add("--in-app");
        args.add("--tracepoint-events");
        args.add("/data/local/tmp/tracepoint_events");
        args.addAll(recordArgs);

        // 2. Create the simpleperf process.
        ProcessBuilder pb = new ProcessBuilder(args).directory(new File(simpleperfDataDir));
        try {
            simpleperfProcess = pb.start();
        } catch (IOException e) {
            throw new Error("failed to create simpleperf process: " + e.getMessage());
        }

        // 3. Wait until simpleperf starts recording.
        String startFlag = readReply();
        if (!startFlag.equals("started")) {
            throw new Error("failed to receive simpleperf start flag");
        }
    }

    private void sendCmd(String cmd) {
        cmd += "\n";
        try {
            simpleperfProcess.getOutputStream().write(cmd.getBytes());
            simpleperfProcess.getOutputStream().flush();
        } catch (IOException e) {
            throw new Error("failed to send cmd to simpleperf: " + e.getMessage());
        }
        if (!readReply().equals("ok")) {
            throw new Error("failed to run cmd in simpleperf: " + cmd);
        }
    }

    private String readReply() {
        // Read one byte at a time to stop at line break or EOF. BufferedReader will try to read
        // more than available and make us blocking, so don't use it.
        String s = "";
        while (true) {
            int c = -1;
            try {
                c = simpleperfProcess.getInputStream().read();
            } catch (IOException e) {
            }
            if (c == -1 || c == '\n') {
                break;
            }
            s += (char)c;
        }
        return s;
    }
}

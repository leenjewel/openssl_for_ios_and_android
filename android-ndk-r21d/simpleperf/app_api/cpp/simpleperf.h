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

#pragma once
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <vector>

// A C++ API used to control simpleperf recording.
namespace simpleperf {

/**
 * RecordOptions sets record options used by ProfileSession. The options are
 * converted to a string list in toRecordArgs(), which is then passed to
 * `simpleperf record` cmd. Run `simpleperf record -h` or
 * `run_simpleperf_on_device.py record -h` for help messages.
 *
 * Example:
 *   RecordOptions options;
 *   options.setDuration(3).recordDwarfCallGraph().setOutputFilename("perf.data");
 *   ProfileSession session;
 *   session.startRecording(options);
 */
class RecordOptionsImpl;
class RecordOptions {
 public:
  RecordOptions();
  ~RecordOptions();
  /**
   * Set output filename. Default is perf-<month>-<day>-<hour>-<minute>-<second>.data.
   * The file will be generated under simpleperf_data/.
   */
  RecordOptions& SetOutputFilename(const std::string& filename);

  /**
   * Set event to record. Default is cpu-cycles. See `simpleperf list` for all available events.
   */
  RecordOptions& SetEvent(const std::string& event);

  /**
   * Set how many samples to generate each second running. Default is 4000.
   */
  RecordOptions& SetSampleFrequency(size_t freq);

  /**
   * Set record duration. The record stops after `durationInSecond` seconds. By default,
   * record stops only when stopRecording() is called.
   */
  RecordOptions& SetDuration(double duration_in_second);

  /**
   * Record some threads in the app process. By default, record all threads in the process.
   */
  RecordOptions& SetSampleThreads(const std::vector<pid_t>& threads);

  /**
   * Record dwarf based call graph. It is needed to get Java callstacks.
   */
  RecordOptions& RecordDwarfCallGraph();

  /**
   * Record frame pointer based call graph. It is suitable to get C++ callstacks on 64bit devices.
   */
  RecordOptions& RecordFramePointerCallGraph();

  /**
   * Trace context switch info to show where threads spend time off cpu.
   */
  RecordOptions& TraceOffCpu();

  /**
   * Translate record options into arguments for `simpleperf record` cmd.
   */
  std::vector<std::string> ToRecordArgs() const;

 private:
  RecordOptionsImpl* impl_;
};

/**
 * ProfileSession uses `simpleperf record` cmd to generate a recording file.
 * It allows users to start recording with some options, pause/resume recording
 * to only profile interested code, and stop recording.
 *
 * Example:
 *   RecordOptions options;
 *   options.setDwarfCallGraph();
 *   ProfileSession session;
 *   session.StartRecording(options);
 *   sleep(1);
 *   session.PauseRecording();
 *   sleep(1);
 *   session.ResumeRecording();
 *   sleep(1);
 *   session.StopRecording();
 *
 * It aborts when error happens. To read error messages of simpleperf record
 * process, filter logcat with `simpleperf`.
 */
class ProfileSessionImpl;
class ProfileSession {
 public:
  /**
   * @param appDataDir the same as android.content.Context.getDataDir().
   *                   ProfileSession stores profiling data in appDataDir/simpleperf_data/.
   */
  ProfileSession(const std::string& app_data_dir);

  /**
   * ProfileSession assumes appDataDir as /data/data/app_package_name.
   */
  ProfileSession();
  ~ProfileSession();

  /**
   * Start recording.
   * @param options RecordOptions
   */
  void StartRecording(const RecordOptions& options);

  /**
   * Start recording.
   * @param args arguments for `simpleperf record` cmd.
   */
  void StartRecording(const std::vector<std::string>& record_args);

  /**
   * Pause recording. No samples are generated in paused state.
   */
  void PauseRecording();

  /**
   * Resume a paused session.
   */
  void ResumeRecording();

  /**
   * Stop recording and generate a recording file under appDataDir/simpleperf_data/.
   */
  void StopRecording();
 private:
  ProfileSessionImpl* impl_;
};

}  // namespace simpleperf

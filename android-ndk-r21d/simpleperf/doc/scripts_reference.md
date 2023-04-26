# Scripts reference

## Table of Contents

- [app_profiler.py](#app_profilerpy)
    - [Profile from launch of an application](#profile-from-launch-of-an-application)
- [run_simpleperf_without_usb_connection.py](#run_simpleperf_without_usb_connectionpy)
- [binary_cache_builder.py](#binary_cache_builderpy)
- [run_simpleperf_on_device.py](#run_simpleperf_on_devicepy)
- [report.py](#reportpy)
- [report_html.py](#report_htmlpy)
- [inferno](#inferno)
- [pprof_proto_generator.py](#pprof_proto_generatorpy)
- [report_sample.py](#report_samplepy)
- [simpleperf_report_lib.py](#simpleperf_report_libpy)


## app_profiler.py

app_profiler.py is used to record profiling data for Android applications and native executables.

```sh
# Record an Android application.
$ python app_profiler.py -p com.example.simpleperf.simpleperfexamplewithnative

# Record an Android application with Java code compiled into native instructions.
$ python app_profiler.py -p com.example.simpleperf.simpleperfexamplewithnative --compile_java_code

# Record the launch of an Activity of an Android application.
$ python app_profiler.py -p com.example.simpleperf.simpleperfexamplewithnative -a .SleepActivity

# Record a native process.
$ python app_profiler.py -np surfaceflinger

# Record a native process given its pid.
$ python app_profiler.py --pid 11324

# Record a command.
$ python app_profiler.py -cmd \
    "dex2oat --dex-file=/data/local/tmp/app-profiling.apk --oat-file=/data/local/tmp/a.oat"

# Record an Android application, and use -r to send custom options to the record command.
$ python app_profiler.py -p com.example.simpleperf.simpleperfexamplewithnative \
    -r "-e cpu-clock -g --duration 30"

# Record both on CPU time and off CPU time.
$ python app_profiler.py -p com.example.simpleperf.simpleperfexamplewithnative \
    -r "-e task-clock -g -f 1000 --duration 10 --trace-offcpu"

# Save profiling data in a custom file (like perf_custom.data) instead of perf.data.
$ python app_profiler.py -p com.example.simpleperf.simpleperfexamplewithnative -o perf_custom.data
```

### Profile from launch of an application

Sometimes we want to profile the launch-time of an application. To support this, we added --app in
the record command. The --app option sets the package name of the Android application to profile.
If the app is not already running, the record command will poll for the app process in a loop with
an interval of 1ms. So to profile from launch of an application, we can first start the record
command with --app, then start the app. Below is an example.

```sh
$ python run_simpleperf_on_device.py record
    --app com.example.simpleperf.simpleperfexamplewithnative \
    -g --duration 1 -o /data/local/tmp/perf.data
# Start the app manually or using the `am` command.
```

To make it convenient to use, app_profiler.py supports using the -a option to start an Activity
after recording has started.

```sh
$ python app_profiler.py -p com.example.simpleperf.simpleperfexamplewithnative -a .MainActivity
```

### run_simpleperf_without_usb_connection.py

run_simpleperf_without_usb_connection.py records profiling data while the USB cable isn't
connected. Below is an example.

```sh
$ python run_simpleperf_without_usb_connection.py start \
    -p com.example.simpleperf.simpleperfexamplewithnative
# After the command finishes successfully, unplug the USB cable, run the
# SimpleperfExampleWithNative app. After a few seconds, plug in the USB cable.
$ python run_simpleperf_without_usb_connection.py stop
# It may take a while to stop recording. After that, the profiling data is collected in perf.data
# on host.
```

## binary_cache_builder.py

The binary_cache directory is a directory holding binaries needed by a profiling data file. The
binaries are expected to be unstripped, having debug information and symbol tables. The
binary_cache directory is used by report scripts to read symbols of binaries. It is also used by
report_html.py to generate annotated source code and disassembly.

By default, app_profiler.py builds the binary_cache directory after recording. But we can also
build binary_cache for existing profiling data files using binary_cache_builder.py. It is useful
when you record profiling data using `simpleperf record` directly, to do system wide profiling or
record without the USB cable connected.

binary_cache_builder.py can either pull binaries from an Android device, or find binaries in
directories on the host (via -lib).

```sh
# Generate binary_cache for perf.data, by pulling binaries from the device.
$ python binary_cache_builder.py

# Generate binary_cache, by pulling binaries from the device and finding binaries in
# SimpleperfExampleWithNative.
$ python binary_cache_builder.py -lib path_of_SimpleperfExampleWithNative
```

## run_simpleperf_on_device.py

This script pushes the simpleperf executable on the device, and run a simpleperf command on the
device. It is more convenient than running adb commands manually.

## report.py

report.py is a wrapper of the report command on the host. It accepts all options of the report
command.

```sh
# Report call graph
$ python report.py -g

# Report call graph in a GUI window implemented by Python Tk.
$ python report.py -g --gui
```

## report_html.py

report_html.py generates report.html based on the profiling data. Then the report.html can show
the profiling result without depending on other files. So it can be shown in local browsers or
passed to other machines. Depending on which command-line options are used, the content of the
report.html can include: chart statistics, sample table, flamegraphs, annotated source code for
each function, annotated disassembly for each function.

```sh
# Generate chart statistics, sample table and flamegraphs, based on perf.data.
$ python report_html.py

# Add source code.
$ python report_html.py --add_source_code --source_dirs path_of_SimpleperfExampleWithNative

# Add disassembly.
$ python report_html.py --add_disassembly

# Adding disassembly for all binaries can cost a lot of time. So we can choose to only add
# disassembly for selected binaries.
$ python report_html.py --add_disassembly --binary_filter libgame.so

# report_html.py accepts more than one recording data file.
$ python report_html.py -i perf1.data perf2.data
```

Below is an example of generating html profiling results for SimpleperfExampleWithNative.

```sh
$ python app_profiler.py -p com.example.simpleperf.simpleperfexamplewithnative
$ python report_html.py --add_source_code --source_dirs path_of_SimpleperfExampleWithNative \
    --add_disassembly
```

After opening the generated [report.html](./report_html.html) in a browser, there are several tabs:

The first tab is "Chart Statistics". You can click the pie chart to show the time consumed by each
process, thread, library and function.

The second tab is "Sample Table". It shows the time taken by each function. By clicking one row in
the table, we can jump to a new tab called "Function".

The third tab is "Flamegraph". It shows the graphs generated by [inferno](./inferno.md).

The fourth tab is "Function". It only appears when users click a row in the "Sample Table" tab.
It shows information of a function, including:

1. A flamegraph showing functions called by that function.
2. A flamegraph showing functions calling that function.
3. Annotated source code of that function. It only appears when there are source code files for
   that function.
4. Annotated disassembly of that function. It only appears when there are binaries containing that
   function.

## inferno

[inferno](./inferno.md) is a tool used to generate flamegraph in a html file.

```sh
# Generate flamegraph based on perf.data.
# On Windows, use inferno.bat instead of ./inferno.sh.
$ ./inferno.sh -sc --record_file perf.data

# Record a native program and generate flamegraph.
$ ./inferno.sh -np surfaceflinger
```

## pprof_proto_generator.py

It converts a profiling data file into pprof.proto, a format used by [pprof](https://github.com/google/pprof).

```sh
# Convert perf.data in the current directory to pprof.proto format.
$ python pprof_proto_generator.py
$ pprof -pdf pprof.profile
```

## report_sample.py

It converts a profiling data file into a format used by [FlameGraph](https://github.com/brendangregg/FlameGraph).

```sh
# Convert perf.data in the current directory to a format used by FlameGraph.
$ python report_sample.py --symfs binary_cache >out.perf
$ git clone https://github.com/brendangregg/FlameGraph.git
$ FlameGraph/stackcollapse-perf.pl out.perf >out.folded
$ FlameGraph/flamegraph.pl out.folded >a.svg
```

## simpleperf_report_lib.py

simpleperf_report_lib.py is a Python library used to parse profiling data files generated by the
record command. Internally, it uses libsimpleperf_report.so to do the work. Generally, for each
profiling data file, we create an instance of ReportLib, pass it the file path (via SetRecordFile).
Then we can read all samples through GetNextSample(). For each sample, we can read its event info
(via GetEventOfCurrentSample), symbol info (via GetSymbolOfCurrentSample) and call chain info
(via GetCallChainOfCurrentSample). We can also get some global information, like record options
(via GetRecordCmd), the arch of the device (via GetArch) and meta strings (via MetaInfo).

Examples of using simpleperf_report_lib.py are in report_sample.py, report_html.py,
pprof_proto_generator.py and inferno/inferno.py.

@echo off
rem Unset PYTHONPATH and PYTHONHOME to prevent the user's environment from
rem affecting the Python that we invoke.
rem See https://github.com/googlesamples/vulkan-basic-samples/issues/25
set PYTHONHOME=
set PYTHONPATH=
set NDK_ROOT=%~dp0\..
set PREBUILT_PATH=%NDK_ROOT%\prebuilt\windows-x86_64
"%PREBUILT_PATH%\bin\make.exe" -O -f "%NDK_ROOT%\build\core\build-local.mk" SHELL=cmd %*

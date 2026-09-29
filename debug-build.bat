@echo off

cls
cmake --preset windows-debug
cmake --build --preset windows-debug
echo(
pause
cls

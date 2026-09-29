@echo off

cls
cmake --preset windows-release
cmake --build --preset windows-release
echo(
pause
cls

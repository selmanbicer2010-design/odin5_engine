@echo off

cls
if not "%CD%"=="C:\ZedProjects\odin5\shaders" (
    cd /d "C:\ZedProjects\odin5\shaders"
)
@echo on
slangc main.slang -target spirv -profile spirv_1_4 -emit-spirv-directly -fvk-use-entrypoint-name -entry vert_main -entry frag_main -o main.spv
@echo off
echo(
pause
cls

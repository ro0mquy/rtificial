@echo off
IF %1.==. GOTO NOPARAM

@mkdir .\demo

echo === Debug
copy /y ..\..\vinyl\Builds\vs2015\bin\%1\Debug\rt.exe .\demo\rt.debug.exe
.\kkrunchy\kkrunchy_021.exe         --refsize 64 --out .\demo\rt.debug.min.021.exe         .\demo\rt.debug.exe
.\kkrunchy\kkrunchy_023a.exe        --refsize 64 --out .\demo\rt.debug.min.023a.exe        .\demo\rt.debug.exe
.\kkrunchy\kkrunchy_023a2_k7.exe    --refsize 64 --out .\demo\rt.debug.min.023a2_k7.exe    .\demo\rt.debug.exe
.\kkrunchy\kkrunchy_023a4_asm07.exe --refsize 64 --out .\demo\rt.debug.min.023a4_asm07.exe .\demo\rt.debug.exe
.\kkrunchy\conspiracy\Tools\kkrunchy_023_2014_importhack.exe --refsize 64 --out .\demo\rt.debug.min.023_2014_importhack.exe .\demo\rt.debug.exe
.\kkrunchy\conspiracy\Tools\rekkrunchy.exe                   --refsize 64 --out .\demo\rt.debug.min.rekkrunchy.exe          .\demo\rt.debug.exe
.\kkrunchy\bep.exe .\demo\rt.debug.exe .\demo\rt.debug.bep.exe +S

echo === Release
copy /y ..\..\vinyl\Builds\vs2015\bin\%1\Release\rt.exe .\demo\rt.release.exe
.\kkrunchy\kkrunchy_021.exe         --refsize 64 --out .\demo\rt.release.min.021.exe         .\demo\rt.release.exe
.\kkrunchy\kkrunchy_023a.exe        --refsize 64 --out .\demo\rt.release.min.023a.exe        .\demo\rt.release.exe
.\kkrunchy\kkrunchy_023a2_k7.exe    --refsize 64 --out .\demo\rt.release.min.023a2_k7.exe    .\demo\rt.release.exe
.\kkrunchy\kkrunchy_023a4_asm07.exe --refsize 64 --out .\demo\rt.release.min.023a4_asm07.exe .\demo\rt.release.exe
.\kkrunchy\conspiracy\Tools\kkrunchy_023_2014_importhack.exe --refsize 64 --out .\demo\rt.release.min.023_2014_importhack.exe .\demo\rt.release.exe
.\kkrunchy\conspiracy\Tools\rekkrunchy.exe                   --refsize 64 --out .\demo\rt.release.min.rekkrunchy.exe          .\demo\rt.release.exe
.\kkrunchy\bep.exe .\demo\rt.release.exe .\demo\rt.release.bep.exe +S

GOTO END

:NOPARAM
echo usage: .\kkrunchy.bat [config]
echo     config: a build configuration
echo             4klang, V2, dual_V2_4klang, vorbis, ...
echo             for possible options see rtificial/vinyl/README.md

:END
@pause

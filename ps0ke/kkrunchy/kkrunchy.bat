@mkdir .\demo

copy /y ..\..\strahlenwerk_runtime\VisualStudio2013\Debug\strahlenwerk_runtime.exe .\demo\demo.debug.exe
.\kkrunchy\kkrunchy_021.exe         --refsize 64 --out .\demo\demo.debug.min.021.exe         .\demo\demo.debug.exe
.\kkrunchy\kkrunchy_023a.exe        --refsize 64 --out .\demo\demo.debug.min.023a.exe        .\demo\demo.debug.exe
.\kkrunchy\kkrunchy_023a2_k7.exe    --refsize 64 --out .\demo\demo.debug.min.023a2_k7.exe    .\demo\demo.debug.exe
.\kkrunchy\kkrunchy_023a4_asm07.exe --refsize 64 --out .\demo\demo.debug.min.023a4_asm07.exe .\demo\demo.debug.exe

copy /y ..\..\strahlenwerk_runtime\VisualStudio2013\Release\strahlenwerk_runtime.exe .\demo\demo.release.exe
.\kkrunchy\kkrunchy_021.exe         --refsize 64 --out .\demo\demo.release.min.021.exe         .\demo\demo.release.exe
.\kkrunchy\kkrunchy_023a.exe        --refsize 64 --out .\demo\demo.release.min.023a.exe        .\demo\demo.release.exe
.\kkrunchy\kkrunchy_023a2_k7.exe    --refsize 64 --out .\demo\demo.release.min.023a2_k7.exe    .\demo\demo.release.exe
.\kkrunchy\kkrunchy_023a4_asm07.exe --refsize 64 --out .\demo\demo.release.min.023a4_asm07.exe .\demo\demo.release.exe

@pause
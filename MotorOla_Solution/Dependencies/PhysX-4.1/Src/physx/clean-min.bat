@REM Elimina todos los ficheros y directorios generados
@REM que quedan excluidos del repositorio.

del /q include\PxConfig.h

@REM \bin
rmdir /s /q bin\win.x86_64.vc142.md

@REM \Build
rmdir /s /q ..\..\Build\vc16win64-min\.vs
rmdir /s /q ..\..\Build\vc16win64-min\x64
rmdir /s /q ..\..\Build

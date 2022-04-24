@REM Elimina todos los ficheros y directorios generados
@REM que quedan excluidos del repositorio.

del /q include\PxConfig.h

@REM \bin
rmdir /s /q bin\win.x86_64.vc142.md

cd bin\win.x86_64.vc142.mt\
mkdir temporal

cd release
move glut32.dll ..\temporal
move PhysXDevice64.dll ..\temporal
del /s /q *.*
cd ..\temporal
move glut32.dll ..\release
move PhysXDevice64.dll ..\release
cd ..

cd debug
move glut32.dll ..\temporal
move PhysXDevice64.dll ..\temporal
del /s /q *.*
cd ..\temporal
move glut32.dll ..\debug
move PhysXDevice64.dll ..\debug
cd ..

cd profile
move glut32.dll ..\temporal
move PhysXDevice64.dll ..\temporal
del /s /q *.*
cd ..\temporal
move glut32.dll ..\profile
move PhysXDevice64.dll ..\profile
cd ..

cd checked
move glut32.dll ..\temporal
move PhysXDevice64.dll ..\temporal
del /s /q *.*
cd ..\temporal
move glut32.dll ..\checked
move PhysXDevice64.dll ..\checked
cd ..

rmdir /s /q temporal
cd ..\..

@REM \Build
rmdir /s /q ..\..\Build\vc16win64\.vs
rmdir /s /q ..\..\Build\vc16win64\x64
rmdir /s /q ..\..\Build\vc16win64-min\.vs
rmdir /s /q ..\..\Build\vc16win64-min\x64
rmdir /s /q ..\..\Build

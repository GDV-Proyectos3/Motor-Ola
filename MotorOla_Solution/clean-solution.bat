:: ROOT
rmdir /s /q .vs\
rmdir /s /q Bin\
rmdir /s /q Temp\
del Exes\*.dll
del Exes\*.lib
del Exes\*.pdb
del Exes\*.idb
del Exes\*.exe
@REM PAUSE >nul

:: Ogre
rmdir /s /q Dependencies\Ogre\Build\
@REM PAUSE >nul

:: PhysX
cd Dependencies\PhysX-4.1\Src\physx\
call clean-all.bat
cd ..\..\..\..\
@REM PAUSE >nul

:: Mensaje de verificación
Echo LIMPIEZA TERMINADA
PAUSE >nul

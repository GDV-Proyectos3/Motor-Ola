:: ROOT\
@REM rmdir /s /q .vs\
rmdir /s /q Bin\
rmdir /s /q Temp\
@REM PAUSE >nul

:: ROOT\Exes\
cd Exes\
del *.dll
del *.lib
del *.pdb
del *.exe
del *.ipdb
del *.iobj
del Ogre.log
cd ..
@REM PAUSE >nul

:: Ogre
rmdir /s /q Dependencies\Ogre\Build\
@REM PAUSE >nul

:: Lua
rmdir /s /q Dependencies\Lua\x64\
@REM PAUSE >nul

:: PhysX
cd Dependencies\PhysX-4.1\Src\physx\
call clean-min.bat
@REM call clean-all.bat
cd ..\..\..\..\
@REM PAUSE >nul

:: Mensaje de verificación
Echo LIMPIEZA TERMINADA
PAUSE >nul

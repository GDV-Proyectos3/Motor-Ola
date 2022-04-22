mkdir ..\..\Build\public
copy compiler\public\CMakeLists.txt ..\..\Build\public\CMakeLists.txt
call generate_projects.bat

if exist ..\..\Build\vc16win64\ (
    xcopy /E /I /y samples\media ..\..\Build\samples\media
    xcopy /E /I /y samples\sampleframework\media ..\..\Build\sampleframework\media
    cd ..\..\Build\vc16win64\
) else (
    cd ..\..\Build\vc16win64-min\
)

:: CMake hace la Build de PhysX-4.1
..\..\..\CMake\Src\bin\cmake.exe --build . --config debug
..\..\..\CMake\Src\bin\cmake.exe --build . --config release 

:: Debug -> copia de archivos .dll (falta añadir '_d' en debug para diferenciar los archivos)
cd ..\..\Src\physx\bin\win.x86_64.vc142.md\debug\
copy /y PhysX_64.dll "..\..\..\..\..\..\..\Exes\PhysX_64.dll"
copy /y PhysXCommon_64.dll "..\..\..\..\..\..\..\Exes\PhysXCommon_64.dll"
copy /y PhysXCooking_64.dll "..\..\..\..\..\..\..\Exes\PhysXCooking_64.dll"
copy /y PhysXFoundation_64.dll "..\..\..\..\..\..\..\Exes\PhysXFoundation_64.dll"
copy /y PhysXGpu_64.dll "..\..\..\..\..\..\..\Exes\PhysXGpu_64.dll"
@REM copy /y glut32.dll "..\..\..\..\..\..\..\Exes\glut32.dll"
@REM copy /y PhysXDevice64.dll "..\..\..\..\..\..\..\Exes\PhysXDevice64.dll"
cd ..\

:: Release -> copia de archivos .dll
cd release\
@REM copy /y PhysX_64.dll "..\..\..\..\..\..\..\Exes\PhysX_64.dll"
@REM copy /y PhysXCommon_64.dll "..\..\..\..\..\..\..\Exes\PhysXCommon_64.dll"
@REM copy /y PhysXCooking_64.dll "..\..\..\..\..\..\..\Exes\PhysXCooking_64.dll"
@REM copy /y PhysXFoundation_64.dll "..\..\..\..\..\..\..\Exes\PhysXFoundation_64.dll"
@REM copy /y PhysXGpu_64.dll "..\..\..\..\..\..\..\Exes\PhysXGpu_64.dll"
@REM copy /y glut32.dll "..\..\..\..\..\..\..\Exes\glut32.dll"
@REM copy /y PhysXDevice64.dll "..\..\..\..\..\..\..\Exes\PhysXDevice64.dll"
cd ..\..\..\

@REM set /p DUMMY=--Hecho--
::exit

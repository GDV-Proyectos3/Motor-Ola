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

..\..\..\CMake\Src\bin\cmake.exe --build . --config debug
:: ..\..\..\CMake\Src\bin\cmake.exe --build . --config release 

:: Copia de archivos .dll (falta añadir '_d' en debug y la parte del release)
cd ..\..\Src\physx\bin\win.x86_64.vc142.mt\debug\
copy /y PhysX_64.dll "..\..\..\..\..\..\..\Bin\PhysX_64.dll"
copy /y PhysXCommon_64.dll "..\..\..\..\..\..\..\Bin\PhysXCommon_64.dll"
copy /y PhysXFoundation_64.dll "..\..\..\..\..\..\..\Bin\PhysXFoundation_64.dll"
copy /y PhysXCooking_64.dll "..\..\..\..\..\..\..\Bin\PhysXCooking_64.dll"
copy /y PhysXGpu_64.dll "..\..\..\..\..\..\..\Bin\PhysXGpu_64.dll"

set /p DUMMY=--Hecho--
::exit

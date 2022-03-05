:: Reset errorlevel status so we are not inheriting this state from the calling process:
@call :CLEAN_EXIT
@echo off

pushd %~dp0
set PHYSX_ROOT_DIR=%CD%
popd
SET PHYSX_ROOT_DIR=%PHYSX_ROOT_DIR:\=/%
SET PM_VSWHERE_PATH=%PHYSX_ROOT_DIR%/../externals/VsWhere
SET PM_CMAKEMODULES_PATH=%PHYSX_ROOT_DIR%/../externals/CMakeModules
SET PM_PXSHARED_PATH=%PHYSX_ROOT_DIR%/../pxshared
SET PM_TARGA_PATH=%PHYSX_ROOT_DIR%/../externals/targa
SET PM_PATHS=%PM_CMAKEMODULES_PATH%;%PM_TARGA_PATH%

SET OLD_PTH=%PATH%
::::ECHO %OLD_PTH%
PATH=%OLD_PTH%;..\..\..\python-3.9.10-embed-amd64\src;..\..\..\CMake\Src\bin

where /q cmake
IF ERRORLEVEL 1 (    
	ECHO Cmake is missing, please install cmake version 3.12 and up.
    set /p DUMMY=Hit ENTER to continue...
	exit /b 1
)

where /q python
IF ERRORLEVEL 1 (
    if "%PM_python_PATH%" == "" (        
        ECHO Python is missing, please install python version 2.7.6 and up. Or set env variable PM_python_PATH pointing to python root directory.
        set /p DUMMY=Hit ENTER to continue...
        exit /b 1
    )
)

if "%PM_python_PATH%" == "" (    
    set PM_PYTHON=python.exe
) else (
    set PM_PYTHON="%PM_python_PATH%\python.exe"
)
:: vvv
set PM_PYTHON="..\..\..\python-3.9.10-embed-amd64\src\python.exe"

IF %1.==. GOTO ADDITIONAL_PARAMS_MISSING

for /f "usebackq tokens=*" %%i in (`%PM_vswhere_PATH%\VsWhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
	set InstallDir=%%i
	set VS150PATH="%%i"		
)	

:ADDITIONAL_PARAMS_MISSING
%PM_PYTHON% ./buildtools/cmake_generate_projects.py vc16win64
::%PM_PYTHON% ./buildtools/cmake_generate_projects.py %1
if %ERRORLEVEL% neq 0 (
    set /p DUMMY=Hit ENTER to continue...
    exit /b %errorlevel%
) else (
    goto CLEAN_EXIT
)

:CLEAN_EXIT
exit /b 0
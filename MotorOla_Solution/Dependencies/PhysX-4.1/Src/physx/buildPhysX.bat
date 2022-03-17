mkdir ..\..\build\public
copy compiler\public\CMakeLists.txt ..\..\build\public\CMakeLists.txt
xcopy /E /I samples\media ..\..\build\samples\media
xcopy /E /I samples\sampleframework\media ..\..\build\sampleframework\media
call generate_projects.bat
set /p DUMMY=--Hecho--
exit
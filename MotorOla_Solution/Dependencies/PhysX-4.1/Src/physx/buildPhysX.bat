mkdir ..\..\Build\public
copy compiler\public\CMakeLists.txt ..\..\Build\public\CMakeLists.txt
xcopy /E /I samples\media ..\..\Build\samples\media
xcopy /E /I samples\sampleframework\media ..\..\Build\sampleframework\media
call generate_projects.bat
set /p DUMMY=--Hecho--
exit
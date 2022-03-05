rmdir /q /s "..\..\build\vc16win64"
::https://stackoverflow.com/questions/1103994/how-to-run-multiple-bat-files-within-a-bat-file
call generate_projects.bat
cd compiler
move vc16win64 ..\..\..\build\vc16win64
set /p DUMMY=--Hecho--
exit
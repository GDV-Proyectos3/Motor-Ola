mkdir ..\..\build\public
copy compiler\public\CMakeLists.txt ..\..\build\public\CMakeLists.txt
call generate_projects.bat
set /p DUMMY=--Hecho--
exit
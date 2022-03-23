::Este .bat hay que ejecutarlo desde el Developer Command Prompt de Visual Studio que se puede encontrar en la pestaña tools de visual
::en Command Line

:: Se crea la carpeta Bin para meter las dlls
mkdir Bin

:: Entra en la carpeta de dependencias
cd Dependencies

:: OGRE
cd Ogre

:: Crea el directorio Ogre/Build y entramos
mkdir Build
cd Build

:: CMake hace la Build de Ogre
..\..\CMake\Src\bin\cmake.exe -DOGRE_BUILD_COMPONENT_OVERLAY=TRUE -DOGRE_BUILD_RENDERSYSTEM_D3D9=FALSE -DOGRE_BUILD_RENDERSYSTEM_D3D11=FALSE -DOGRE_BUILD_RENDERSYSTEM_GL3PLUS=FALSE -DOGRE_BUILD_RENDERSYSTEM_GL=TRUE -DOGRE_BUILD_RENDERSYSTEM_GLES2=FALSE -DOGRE_BUILD_PLUGIN_ASSIMP=FALSE -DOGRE_BUILD_PLUGIN_BSP=FALSE -DOGRE_BUILD_PLUGIN_OCTREE=FALSE -DOGRE_BUILD_PLUGIN_DOT_SCENE=FALSE -DOGRE_BUILD_PLUGIN_PCZ=FALSE -DOGRE_BUILD_COMPONENT_TERRAIN=FALSE -DOGRE_BUILD_COMPONENT_VOLUME=FALSE -DOGRE_BUILD_COMPONENT_BITES=FALSE -DOGRE_BUILD_COMPONENT_PYTHON=FALSE -DOGRE_BUILD_COMPONENT_JAVA=FALSE -DOGRE_BUILD_COMPONENT_CSHARP=FALSE -DOGRE_INSTALL_CMAKE=FALSE -DOGRE_INSTALL_SAMPLES=FALSE -DOGRE_INSTALL_DOCS=FALSE -DOGRE_INSTALL_PDB=FALSE -DOGRE_INSTALL_VSPROPS=TRUE -DOGRE_BUILD_TOOLS=FALSE ../Src
..\..\CMake\Src\bin\cmake.exe --build . --config release 
..\..\CMake\Src\bin\cmake.exe --build . --config debug

:: Se copian las dlls de Ogre a Bin
cd bin/debug
copy /Y Codec_STBI_d.dll "../../../../../Bin/Codec_STBI_d.dll"
copy /Y OgreMain_d.dll "../../../../../Bin/OgreMain_d.dll"
copy /Y OgreMain_d.dll "../../../../../Exes/OgreMain_d.dll"
copy /Y OgreMeshLodGenerator_d.dll "../../../../../Bin/OgreMeshLodGenerator_d.dll"
copy /Y OgreOverlay_d.dll "../../../../../Bin/OgreOverlay_d.dll"
copy /Y OgrePaging_d.dll "../../../../../Bin/OgrePaging_d.dll"
copy /Y OgreProperty_d.dll "../../../../../Bin/OgreProperty_d.dll"
copy /Y OgreRTShaderSystem_d.dll "../../../../../Bin/OgreRTShaderSystem_d.dll"
copy /Y Plugin_ParticleFX_d.dll "../../../../../Bin/Plugin_ParticleFX_d.dll"
copy /Y RenderSystem_GL_d.dll "../../../../../Bin/RenderSystem_GL_d.dll"
copy /Y SDL2.dll "../../../../../Bin/SDL2.dll"
copy /Y SDL2.dll "../../../../../Exes/SDL2.dll"
copy /Y zlib.dll "../../../../../Bin/zlib.dll"

:: Ojo que hay alguna dll más en release
cd ../release
copy /Y Codec_STBI.dll "../../../../../Bin/Codec_STBI.dll"
copy /Y OgreMain.dll "../../../../../Bin/OgreMain.dll"
copy /Y OgreMain.dll "../../../../../Exes/OgreMain.dll"
copy /Y OgreMeshLodGenerator.dll "../../../../../Bin/OgreMeshLodGenerator.dll"
copy /Y OgreOverlay.dll "../../../../../Bin/OgreOverlay.dll"
copy /Y OgrePaging.dll "../../../../../Bin/OgrePaging.dll"
copy /Y OgreProperty.dll "../../../../../Bin/OgreProperty.dll"
copy /Y OgreRTShaderSystem.dll "../../../../../Bin/OgreRTShaderSystem.dll"
copy /Y Plugin_ParticleFX.dll "../../../../../Bin/Plugin_ParticleFX.dll"
copy /Y RenderSystem_GL.dll "../../../../../Bin/RenderSystem_GL.dll"
:: Estas están repetidas pero por si acaso
copy /Y SDL2.dll "../../../../../Bin/SDL2.dll"
copy /Y SDL2.dll "../../../../../Exes/SDL2.dll"
copy /Y zlib.dll "../../../../../Bin/zlib.dll"
:: Copia SDL2d.dll en Bin y en Exes
cd ..\..\SDL2-build 
..\..\..\CMake\Src\bin\cmake.exe --build . --config release 
..\..\..\CMake\Src\bin\cmake.exe --build . --config debug

cd Debug
copy /Y SDL2d.dll "../../../../../Bin/SDL2d.dll"
copy /Y SDL2d.dll "../../../../../Exes/SDL2d.dll"

:: Copia fmod.dll y fmodL.dll en Bin y en Exes
cd ..\..\..\..\FMOD\libreria\x64
copy /Y fmod.dll "../../../../Bin/fmod.dll"
copy /Y fmod.dll "../../../../Exes/fmod.dll"
copy /Y fmodL.dll "../../../../Bin/fmodL.dll"
copy /Y fmodL.dll "../../../../Exes/fmodL.dll"

:: Mensaje de verificación
Echo BUILD DE OGRE COMPLETADA

:: Se va al directorio de Lua que esta en Dependencies
cd ..\..\..\Lua
:: Uso msbuild para que se haga la build de Lua y especifico que se haga en debug y release
msbuild -p:Configuration=Debug
msbuild -p:Configuration=Release

:: Me meto en la carpeta de Debug que se ha generado al hacer la build y copio Lua_d.dll en Bin y Exes
cd x64/Debug
copy /Y Lua_d.dll "../../../../Bin/Lua_d.dll"
copy /Y Lua_d.dll "../../../../Exes/Lua_d.dll"
:: Me muevo a la carpeta Release y copio Lua.dll en Bin y Exes
cd ../Release
copy /Y Lua.dll "../../../../Bin/Lua.dll"
copy /Y Lua.dll "../../../../Exes/Lua.dll"

:: Mensaje de verificacion
Echo BUILD DE LUA COMPLETA

:: Vuelta a la carpeta principal del proyecto (En la que se encuentra MotorOla.bat)
cd ..\..\..\..

:: Pausa para ver que ha ocurrido
PAUSE >nul
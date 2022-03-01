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
..\..\CMake\Src\bin\cmake.exe -DOGRE_BUILD_COMPONENT_OVERLAY=TRUE -DOGRE_BUILD_RENDERSYSTEM_D3D9=FALSE -DOGRE_BUILD_RENDERSYSTEM_D3D11=TRUE -DOGRE_BUILD_RENDERSYSTEM_GL3PLUS=TRUE -DOGRE_BUILD_RENDERSYSTEM_GL=TRUE -DOGRE_BUILD_RENDERSYSTEM_GLES2=TRUE -DOGRE_BUILD_PLUGIN_ASSIMP=FALSE -DOGRE_BUILD_PLUGIN_BSP=FALSE -DOGRE_BUILD_PLUGIN_OCTREE=FALSE -DOGRE_BUILD_PLUGIN_DOT_SCENE=FALSE -DOGRE_BUILD_PLUGIN_PCZ=FALSE -DOGRE_BUILD_COMPONENT_TERRAIN=TRUE -DOGRE_BUILD_COMPONENT_VOLUME=TRUE -DOGRE_BUILD_COMPONENT_BITES=FALSE -DOGRE_BUILD_COMPONENT_PYTHON=FALSE -DOGRE_BUILD_COMPONENT_JAVA=FALSE -DOGRE_BUILD_COMPONENT_CSHARP=FALSE -DOGRE_INSTALL_CMAKE=FALSE -DOGRE_INSTALL_SAMPLES=FALSE -DOGRE_INSTALL_DOCS=FALSE -DOGRE_INSTALL_PDB=FALSE -DOGRE_INSTALL_VSPROPS=TRUE -DOGRE_BUILD_TOOLS=FALSE ../Src
..\..\CMake\Src\bin\cmake.exe --build . --config release 
..\..\CMake\Src\bin\cmake.exe --build . --config debug

:: Se copian las dlls de Ogre a Bin
cd bin/debug
copy /Y Codec_STBI_d.dll "../../../../../Bin/Codec_STBI_d.dll"
copy /Y OgreMain_d.dll "../../../../../Exes/OgreMain_d.dll"
copy /Y OgreMeshLodGenerator_d.dll "../../../../../Bin/OgreMeshLodGenerator_d.dll"
copy /Y OgreOverlay_d.dll "../../../../../Bin/OgreOverlay_d.dll"
copy /Y OgrePaging_d.dll "../../../../../Bin/OgrePaging_d.dll"
copy /Y OgreProperty_d.dll "../../../../../Bin/OgreProperty_d.dll"
copy /Y OgreRTShaderSystem_d.dll "../../../../../Bin/OgreRTShaderSystem_d.dll"
copy /Y OgreTerrain_d.dll "../../../../../Bin/OgreTerrain_d.dll"
copy /Y OgreVolume_d.dll "../../../../../Bin/OgreVolume_d.dll"
copy /Y Plugin_OctreeSceneManager_d.dll "../../../../../Bin/Plugin_OctreeSceneManager_d.dll"
copy /Y Plugin_OctreeZone_d.dll "../../../../../Bin/Plugin_OctreeZone_d.dll"
copy /Y Plugin_ParticleFX_d.dll "../../../../../Bin/Plugin_ParticleFX_d.dll"
::copy /Y Plugin_PCZSceneManager_d.dll "../../../../../Bin/Plugin_PCZSceneManager_d.dll"
copy /Y RenderSystem_Direct3D11_d.dll "../../../../../Bin/RenderSystem_Direct3D11_d.dll"
copy /Y RenderSystem_GL_d.dll "../../../../../Bin/RenderSystem_GL_d.dll"
copy /Y RenderSystem_GL3Plus_d.dll "../../../../../Bin/RenderSystem_GL3Plus_d.dll"
copy /Y RenderSystem_GLES2_d.dll "../../../../../Bin/RenderSystem_GLES2_d.dll"
copy /Y SDL2.dll "../../../../../Bin/SDL2.dll"
copy /Y zlib.dll "../../../../../Bin/zlib.dll"
:: Ojo que hay alguna dll más en release
cd ../release
::copy /Y Codec_Assimp.dll "../../../../../Bin/Codec_Assimp.dll"
copy /Y Codec_STBI.dll "../../../../../Bin/Codec_STBI.dll"
copy /Y OgreMain.dll "../../../../../Exes/OgreMain.dll"
copy /Y OgreMeshLodGenerator.dll "../../../../../Bin/OgreMeshLodGenerator.dll"
copy /Y OgreOverlay.dll "../../../../../Bin/OgreOverlay.dll"
copy /Y OgrePaging.dll "../../../../../Bin/OgrePaging.dll"
copy /Y OgreProperty.dll "../../../../../Bin/OgreProperty.dll"
copy /Y OgreRTShaderSystem.dll "../../../../../Bin/OgreRTShaderSystem.dll"
copy /Y OgreTerrain.dll "../../../../../Bin/OgreTerrain.dll"
copy /Y OgreVolume.dll "../../../../../Bin/OgreVolume.dll"
::copy /Y Plugin_DotScene.dll "../../../../../Bin/Plugin_DotScene.dll"
copy /Y Plugin_OctreeSceneManager.dll "../../../../../Bin/Plugin_OctreeSceneManager.dll"
copy /Y Plugin_OctreeZone.dll "../../../../../Bin/Plugin_OctreeZone.dll"
copy /Y Plugin_ParticleFX.dll "../../../../../Bin/Plugin_ParticleFX.dll"
copy /Y Plugin_PCZSceneManager.dll "../../../../../Bin/Plugin_PCZSceneManager.dll"
copy /Y RenderSystem_Direct3D11.dll "../../../../../Bin/RenderSystem_Direct3D11.dll"
copy /Y RenderSystem_GL.dll "../../../../../Bin/RenderSystem_GL.dll"
copy /Y RenderSystem_GL3Plus.dll "../../../../../Bin/RenderSystem_GL3Plus.dll"
copy /Y RenderSystem_GLES2.dll "../../../../../Bin/RenderSystem_GLES2.dll"
:: Estas están repetidas pero por si acaso
copy /Y SDL2.dll "../../../../../Bin/SDL2.dll"
copy /Y zlib.dll "../../../../../Bin/zlib.dll"

:: Mensaje de verificación
Echo BUILD DE OGRE COMPLETADA

:: Pausa para ver que ha ocurrido
PAUSE >nul
@ECHO OFF
@ECHO --------------------------------------------------------------------------------
@ECHO 		  		DOOM BFA EDITION
@ECHO			   VS 2019 PROJECT GENERATION FOR x86 - x64
@ECHO --------------------------------------------------------------------------------
@ECHO WARNING: Make sure you have downloaded nuget.exe and set it on System's PATH
pause
cd ..
mkdir buildlog > NUL 2>&1
cd buildlog
mkdir x64 > NUL 2>&1
cd..
del /s /q buildx64 > NUL 2>&1
mkdir buildx64 > NUL 2>&1
cd buildx64
@ECHO Generating x64 files
cmake -G "Visual Studio 16" -A "x64" -DCMAKE_INSTALL_PREFIX=../bin/windows10-64 -DOPENAL=ON -DWINRT=ON -DUSE_XAUDIO2_PACKAGE=ON ../neo > ../buildlog/x64/log.txt
if ERRORLEVEL == 1 goto ERRORX64
::cd ..
::mkdir buildlog > NUL 2>&1
::cd buildlog
::mkdir ALx64 > NUL 2>&1
::cd ..
::del /s /q buildALx64 >NUL 2>&1
::mkdir buildALx64 >NUL 2>&1
::cd buildALx64
::@ECHO Generating x64 with openAL files
::cmake -G "Visual Studio 16" -A "x64" -DCMAKE_INSTALL_PREFIX=../bin/windows10-64 -DOPENAL=ON -DWINRT=ON -DUSE_XAUDIO2_PACKAGE=ON ../neo > ../buildlog/ALx64/log.txt 
::if ERRORLEVEL == 1 goto ERRORX64AL
cd ..
mkdir buildlog > NUL 2>&1
cd buildlog
mkdir Vkx64 > NUL 2>&1
cd ..
del /s /q buildVkx64 >NUL 2>&1
mkdir buildVkx64 >NUL 2>&1
cd buildVkx64
@ECHO Generating x64 with Vulkan files
cmake -G "Visual Studio 16" -A "x64" -DCMAKE_INSTALL_PREFIX=../bin/windows10-64 -DOPENAL=ON -DUSE_VULKAN=ON -DUSE_XAUDIO2_PACKAGE=ON -DWINRT=ON ../neo > ../buildlog/Vkx64/log.txt 
if ERRORLEVEL == 1 goto ERRORX64VK
:x86
cd ..
mkdir buildlog > NUL 2>&1
cd buildlog
mkdir x86 > NUL 2>&1
cd ..
del /s /q buildx86 > NUL 2>&1
mkdir buildx86 > NUL 2>&1
cd buildx86
@ECHO Generating x86 files
cmake -G "Visual Studio 16" -A "Win32" -DCMAKE_INSTALL_PREFIX=../bin/windows10-32 -DOPENAL=ON -DWINRT=ON -DUSE_XAUDIO2_PACKAGE=ON ../neo > ../buildlog/x86/log.txt
if ERRORLEVEL == 1 goto ERRORX86
::cd ..
::mkdir buildlog > NUL 2>&1
::cd buildlog
::mkdir ALx86 > NUL 2>&1
::cd ..
::del /s /q buildALx86 >NUL 2>&1
::mkdir buildALx86 >NUL 2>&1
::cd buildALx86
::@ECHO Generating x86 with openAL files
::cmake -G "Visual Studio 16" -A "Win32" -DCMAKE_INSTALL_PREFIX=../bin/windows10-32 -DOPENAL=ON -DWINRT=ON ../neo > ../buildlog/ALx86/log.txt 
::if ERRORLEVEL == 1 goto ERRORX86AL
pause
exit

:ERRORX64
@ECHO ERROR Generating x64 files
cd ..
rmdir /Q /S buildx64
goto x86
:ERRORX64AL
@ECHO ERROR Generating x64 with openAL files
cd ..
rmdir /Q /S buildALx64
goto x86
:ERRORX64VK
@ECHO ERROR Generating x64 with Vulkan files
cd ..
rmdir /Q /S buildVkx64
goto x86
:ERRORX86
@ECHO ERROR Generating x86 files
cd ..
rmdir /Q /S buildx86
pause
exit
:ERRORX86AL
@ECHO ERROR Generating x86 with openAL files
cd ..
rmdir /Q /S buildALx86
pause
exit
@ECHO OFF
@ECHO --------------------------------------------------------------------------------
@ECHO 		  	CLASSIC RBDOOM 3 BFG EDITION
@ECHO			   VS 2019 PROJECT GENERATION FOR x86 - x64
@ECHO --------------------------------------------------------------------------------
cd ..
del /s /q buildx64 > NUL 2>&1
mkdir buildx64 > NUL 2>&1
cd buildx64
@ECHO Generating x64 files
cmake -G "Visual Studio 16" -A "x64" -DCMAKE_INSTALL_PREFIX=../bin/windows10-64 ../neo > log.txt
if ERRORLEVEL == 1 goto ERRORX64
cd ..
del /s /q buildALx64 >NUL 2>&1
mkdir buildALx64 >NUL 2>&1
cd buildALx64
@ECHO Generating x64 with openAL files
cmake -G "Visual Studio 16" -A "x64" -DCMAKE_INSTALL_PREFIX=../bin/windows10-64 -DOPENAL=ON ../neo >log.txt 
if ERRORLEVEL == 1 goto ERRORX64AL
:x86
cd ..
del /s /q buildx86 > NUL 2>&1
mkdir buildx86 > NUL 2>&1
cd buildx86
@ECHO Generating x86 files
cmake -G "Visual Studio 16" -A "Win32" -DCMAKE_INSTALL_PREFIX=../bin/windows10-32 ../neo > log.txt
if ERRORLEVEL == 1 goto ERRORX86
cd ..
del /s /q buildALx86 >NUL 2>&1
mkdir buildALx86 >NUL 2>&1
cd buildALx86
@ECHO Generating x86 with openAL files
cmake -G "Visual Studio 16" -A "Win32" -DCMAKE_INSTALL_PREFIX=../bin/windows10-32 -DOPENAL=ON ../neo >log.txt 
if ERRORLEVEL == 1 goto ERRORX86AL
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
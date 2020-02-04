@ECHO OFF
@ECHO --------------------------------------------------------------------------------
@ECHO 		  		DOOM BFA EDITION
@ECHO			   VS 2017 PROJECT GENERATION FOR x86 - x64
@ECHO				WINDOWS 10 WITH WINXP TOOLS
@ECHO --------------------------------------------------------------------------------
cd ..
del /s /q buildx64 > NUL 2>&1
mkdir buildx64 > NUL 2>&1
cd buildx64
@ECHO Generating x64 files
cmake -G "Visual Studio 15 Win64" -T v141_xp -DCMAKE_INSTALL_PREFIX=../bin/windows10-64 -DWINDOWS10=ON ../neo > log.txt 
if ERRORLEVEL == 1 goto ERRORX64
cd ..
del /s /q buildALx64 >NUL 2>&1
mkdir buildALx64 >NUL 2>&1
cd buildALx64
@ECHO Generating x64 with openAL files
cmake -G "Visual Studio 15 Win64" -T v141_xp -DCMAKE_INSTALL_PREFIX=../bin/win10-64 -DOPENAL=ON -DWINDOWS10=ON ../neo >log.txt 
if ERRORLEVEL == 1 goto ERRORX64AL
:x86
cd ..
del /s /q buildx86 > NUL 2>&1
mkdir buildx86 > NUL 2>&1
cd buildx86
@ECHO Generating x86 files
cmake -G "Visual Studio 15" -T v141_xp -DCMAKE_INSTALL_PREFIX=../bin/windows10-32 -DWINDOWS10=ON ../neo > log.txt 
if ERRORLEVEL == 1 goto ERRORX86
cd ..
del /s /q buildALx86 >NUL 2>&1
mkdir buildALx86 >NUL 2>&1
cd buildALx86
@ECHO Generating x86 with openAL files
cmake -G "Visual Studio 15" -T v141_xp -DCMAKE_INSTALL_PREFIX=../bin/win10-32 -DOPENAL=ON -DWINDOWS10=ON ../neo >log.txt 
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
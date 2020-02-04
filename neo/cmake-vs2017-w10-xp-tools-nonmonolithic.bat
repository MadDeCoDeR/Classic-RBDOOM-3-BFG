@ECHO OFF
@ECHO --------------------------------------------------------------------------------
@ECHO 		  		DOOM BFA EDITION
@ECHO			   VS 2017 PROJECT GENERATION FOR x86 - x64
@ECHO				WINDOWS 10 WITH WINXP TOOLS
@ECHO					NON-MONOLITH
@ECHO --------------------------------------------------------------------------------
cd ..
del /s /q buildx64NM > NUL 2>&1
mkdir buildx64NM > NUL 2>&1
cd buildx64NM
@ECHO Generating x64 files
cmake -G "Visual Studio 15 Win64" -T v141_xp -DCMAKE_INSTALL_PREFIX=../bin/windows10-64 -DWINDOWS10=ON ../neo -DMONOLITH=OFF > log.txt 
if ERRORLEVEL == 1 goto ERRORX64
cd ..
del /s /q buildALx64NM >NUL 2>&1
mkdir buildALx64NM >NUL 2>&1
cd buildALx64NM
@ECHO Generating x64 with openAL files
cmake -G "Visual Studio 15 Win64" -T v141_xp -DCMAKE_INSTALL_PREFIX=../bin/win10-64 -DOPENAL=ON -DWINDOWS10=ON ../neo -DMONOLITH=OFF >log.txt 
if ERRORLEVEL == 1 goto ERRORX64AL
:x86
cd ..
del /s /q buildx86NM > NUL 2>&1
mkdir buildx86NM > NUL 2>&1
cd buildx86NM
@ECHO Generating x86 files
cmake -G "Visual Studio 15" -T v141_xp -DCMAKE_INSTALL_PREFIX=../bin/windows10-32 -DWINDOWS10=ON ../neo -DMONOLITH=OFF > log.txt 
if ERRORLEVEL == 1 goto ERRORX86
cd ..
del /s /q buildALx86NM >NUL 2>&1
mkdir buildALx86NM >NUL 2>&1
cd buildALx86NM
@ECHO Generating x86 with openAL files
cmake -G "Visual Studio 15" -T v141_xp -DCMAKE_INSTALL_PREFIX=../bin/win10-32 -DOPENAL=ON -DWINDOWS10=ON ../neo -DMONOLITH=OFF >log.txt 
if ERRORLEVEL == 1 goto ERRORX86AL
pause
exit

:ERRORX64
@ECHO ERROR Generating x64 files
cd ..
rmdir /Q /S buildx64NM
goto x86
:ERRORX64AL
@ECHO ERROR Generating x64 with openAL files
cd ..
rmdir /Q /S buildALx64NM
goto x86
:ERRORX86
@ECHO ERROR Generating x86 files
cd ..
rmdir /Q /S buildx86NM
pause
exit
:ERRORX86AL
@ECHO ERROR Generating x86 with openAL files
cd ..
rmdir /Q /S buildALx86NM
pause
exit
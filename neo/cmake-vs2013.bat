@ECHO OFF
@ECHO --------------------------------------------------------------------------------
@ECHO 		  		DOOM BFA EDITION
@ECHO			   VS 2013 PROJECT GENERATION FOR x86 - x64
@ECHO --------------------------------------------------------------------------------
cd ..
del /s /q buildx64 > NUL 2>&1
mkdir buildx64 > NUL 2>&1
cd buildx64
@ECHO Generating x64 files
cmake -G "Visual Studio 12 Win64" -DCMAKE_INSTALL_PREFIX=../bin/win8-64 ../neo > NUL 2>&1
if ERRORLEVEL == 1 goto ERRORX64
:x86
cd ..
del /s /q buildx86 > NUL 2>&1
mkdir buildx86 > NUL 2>&1
cd buildx86
@ECHO Generating x86 files
cmake -G "Visual Studio 12" -DCMAKE_INSTALL_PREFIX=../bin/win8-32 ../neo > NUL 2>&1
if ERRORLEVEL == 1 goto ERRORX86
pause
exit

:ERRORX64
@ECHO ERROR Generating x64 files
cd ..
rmdir /Q /S buildx64
goto x86
:ERRORX86
@ECHO ERROR Generating x86 files
cd ..
rmdir /Q /S buildx86
pause
exit
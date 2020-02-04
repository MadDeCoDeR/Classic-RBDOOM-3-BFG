@ECHO OFF
@ECHO --------------------------------------------------------------------------------
@ECHO 		  		DOOM BFA EDITION
@ECHO			   VS 2012 PROJECT GENERATION FOR x86
@ECHO				        OPEN AL
@ECHO --------------------------------------------------------------------------------
cd ..
del /s /q buildAL >NUL 2>&1
mkdir buildAL >NUL 2>&1
cd buildAL
@ECHO Generating files
cmake -G "Visual Studio 11" -DCMAKE_INSTALL_PREFIX=../bin/win8-32 -DOPENAL=ON ../neo >NUL 2>&1
if ERRORLEVEL == 1 goto ERROR
pause
exit

:ERROR
@ECHO ERROR Generating files
cd ..
rmdir /Q /S buildAL
pause
exit

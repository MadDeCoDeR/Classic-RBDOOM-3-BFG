@ECHO OFF
@ECHO --------------------------------------------------------------------------------
@ECHO 		  		DOOM BFA EDITION
@ECHO			   VS 2019 PROJECT GENERATION FOR x64 UWP
@ECHO --------------------------------------------------------------------------------
@ECHO WARNING: Make sure you have downloaded nuget.exe and set it on System's PATH
pause
set /p INPUT=Set UWP Game Folder(Add " in the begin and the end and replace \ with \\):
cd ..
mkdir buildlog > NUL 2>&1
cd buildlog
mkdir UWPx64 > NUL 2>&1
cd ..
del /s /q buildUWPx64 >NUL 2>&1
mkdir buildUWPx64 >NUL 2>&1
cd buildUWPx64
@ECHO Generating x64 for UWP files
cmake -G "Visual Studio 16" -A "x64" -DCMAKE_INSTALL_PREFIX=../bin/windows10-64 -DOPENAL=ON -DUSE_XAUDIO2_PACKAGE=ON -DWINRT=ON -DWINDOWS10=ON -DUWP_PATH=%INPUT% -DPACKAGED=ON ../neo > ../buildlog/UWPx64/log.txt 
if ERRORLEVEL == 1 goto ERRORX64
::cd ..
::mkdir buildlog > NUL 2>&1
::cd buildlog
::mkdir ALUWPx64 > NUL 2>&1
::cd ..
::del /s /q buildALUWPx64 >NUL 2>&1
::mkdir buildALUWPx64 >NUL 2>&1
::cd buildALUWPx64
::@ECHO Generating x64 with openAL for UWP files
::cmake -G "Visual Studio 16" -A "x64" -DCMAKE_INSTALL_PREFIX=../bin/windows10-64 -DOPENAL=ON -DWINRT=ON -DWINDOWS10=ON -DUWP_PATH=%INPUT% ../neo > ../buildlog/ALUWPx64/log.txt 
::if ERRORLEVEL == 1 goto ERRORX64AL
:exit
pause
exit

:ERRORX64
@ECHO ERROR Generating x64 files
cd ..
rmdir /Q /S buildx64
goto exit
:ERRORX64AL
@ECHO ERROR Generating x64 with openAL files
cd ..
rmdir /Q /S buildALx64
goto exit
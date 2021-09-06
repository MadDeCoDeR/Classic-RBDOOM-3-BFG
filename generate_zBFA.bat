@ECHO OFF
setlocal EnableDelayedExpansion 
set SAVE_FOLDER=%userprofile%\Saved Games\id Software\DOOM BFA\zBFA
set /p INPUT=Set Game Folder:
set /p SAVE_FOLDER=Set Game Save Folder(press Enter for default value (%SAVE_FOLDER%)):
del /s /q %INPUT%\base\zBFA.resources > NUL 2>&1
del /s /q %INPUT%\base\zBFA.manifest > NUL 2>&1
del /s /q %INPUT%\base\zBFA.crc > NUL 2>&1
mkdir %INPUT%\zBFA > NUL 2>&1
mkdir %INPUT%\zBFA\renderprogs > NUL 2>&1
xcopy zBFA %INPUT%\zBFA /s /q /y
xcopy base\renderprogs %INPUT%\zBFA\renderprogs /s /q /y
copy base\generate_zBFA_s1.cfg %INPUT%\base
copy base\generate_zBFA_s2.cfg %INPUT%\base
cd %INPUT%
@ECHO Press the ~ key and type exec generate_zBFA_s1.cfg wait for the level to load and exit the game (press the ~ key and type exit)
DoomBFA +set com_skipIntroVideos 1 +set com_game_mode 3 +set r_fullscreen 0 +set r_customWindowWidth 800 +set r_customWindowHeight 600 +set fs_game zBFA +set com_allowConsole 1
del /s /q %INPUT%\zBFA\guis\assets > NUL 2>&1
rmdir /Q /S %INPUT%\zBFA\guis\assets
rmdir /Q /S %INPUT%\zBFA\guis
del /s /q %INPUT%\zBFA\maps > NUL 2>&1
rmdir /Q /S %INPUT%\zBFA\maps
@ECHO Press the ~ key and type exec generate_zBFA_s2.cfg
DoomBFA +set com_skipIntroVideos 1 +set com_game_mode 3 +set r_fullscreen 0 +set r_customWindowWidth 800 +set r_customWindowHeight 600 +set fs_game zBFA +set com_allowConsole 1
copy "%SAVE_FOLDER%\zBFA.resources" "%INPUT%\base\maps"
copy "%SAVE_FOLDER%\zBFA.manifest" "%INPUT%\base\maps"
copy "%INPUT%\zBFA\zBFA.crc" "%INPUT%\base\maps"
del /s /q %INPUT%\zBFA > NUL 2>&1
rmdir /Q /S %INPUT%\zBFA
del /s /q %INPUT%\base\generate_zBFA_s1.cfg > NUL 2>&1
del /s /q %INPUT%\base\generate_zBFA_s2.cfg > NUL 2>&1
del /s /q "%SAVE_FOLDER%\zBFA.resources" > NUL 2>&1
del /s /q "%SAVE_FOLDER%\zBFA.manifest" > NUL 2>&1
pause

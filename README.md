```
______ _____  ________  ___     ____________ ___   
|  _  \  _  ||  _  |  \/  |  _  | ___ \  ___/ _ \  
| | | | | | || | | | .  . | (_) | |_/ / |_ / /_\ \ 
| | | | | | || | | | |\/| |     | ___ \  _||  _  | 
| |/ /\ \_/ /\ \_/ / |  | |  _  | |_/ / |  | | | | 
|___/  \___/  \___/\_|  |_/ (_) \____/\_|  \_| |_/ 
                                                   
 ___________ _____ _____ _____ _____ _   _         
|  ___|  _  \_   _|_   _|_   _|  _  | \ | |        
| |__ | | | | | |   | |   | | | | | |  \| |        
|  __|| | | | | |   | |   | | | | | | . ` |        
| |___| |/ / _| |_  | |  _| |_\ \_/ / |\  |        
\____/|___/  \___/  \_/  \___/ \___/\_| \_/      
```

DOOM-BFA Readme - https://github.com/MadDeCoDeR/Classic-RBDOOM-3-BFG

Thank you for downloading DOOM-BFA.

DOOM: BFA (Big Freaking Anniversary) Edition is a source port based on RBDOOM-3-BFG and enchance the experience of
Ultimate DOOM, DOOM 2 and DOOM 3.



_______________________________________

CONTENTS
_______________________________



This file contains the following sections:

	1) SYSTEM REQUIREMENT

	2) GENERAL NOTES
	
	3) LICENSE
	
	4) GETTING THE SOURCE CODE

	5) COMPILING ON WIN32 WITH VISUAL C++ 2013 EXPRESS EDITION OR NEWER VERSIONS

	6) COMPILING ON GNU/LINUX
	
	7) INSTALLATION, GETTING THE GAMEDATA, RUNNING THE GAME
	
	8) OVERALL CHANGES
	
	9) CONSOLE VARIABLES
	
	10) KNOWN ISSUES
	
	11) BUG REPORTS
	
	12) GAME MODIFICATIONS
	
	13) CODE LICENSE EXCEPTIONS



___________________________________

1) SYSTEM REQUIREMENTS
__________________________



Minimum system requirements:

	CPU: 2 GHz dual core
	System Memory: 3GB
	Graphics card: (Windows) Any GLSL 3.3 Compatible GPU. (Linux) NVIDIA GeForce 9800 GT / ATI Radeon HD 5750 / Intel HD graphics 530

Recommended system requirements:

	CPU: 3 GHz dual core
	System Memory: 3GB
	Graphics card: (Linux) NVIDIA GeForce GTX 260 / ATI Radeon HD 5850 or higher


Additional Requirements:
	Windows: DirectX 2010 support




_______________________________

2) GENERAL NOTES
______________________

This release does not contain any game data, the game data is still
covered by the original EULA and must be obeyed as usual.

You must patch the game to the latest version.

Note that Doom 3 BFG Edition is available on:
Steam store: http://store.steampowered.com/app/208200/

GOG store: https://www.gog.com/game/doom_3_bfg_edition

Bethesda.net store: https://bethesda.net/en/store/product/DO3CBFPCBG01

Bethesda.net store (2019): https://bethesda.net/en/store/product/DO3GNGPCBG01


Steam API:
------
The Doom BFA Edition GPL Source Code release does include functionality for integrating with 
Steam & Discord through Open Platform Api.  This includes only achievements and the overlay.


Bink:
-----
The RBDoom3BFG Edition GPL Source Code release includes functionality for rendering Bink Videos through FFmpeg.


Back End Rendering of Stencil Shadows:
--------------------------------------

The Doom BFA Edition GPL Source Code release does include functionality enabling rendering
of stencil shadows via the "depth fail" method, a functionality commonly known as "Carmack's Reverse".

This method was patented by Creative Labs and has finally expired on 2019-10-13.
(see https://patents.google.com/patent/US6384822B1/en for expiration status)





_______________________________

3) LICENSE
______________________


See COPYING.txt for the GNU GENERAL PUBLIC LICENSE

ADDITIONAL TERMS:  The Doom 3 BFG Edition GPL Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU GPL which accompanied the Doom 3 BFG Edition GPL Source Code.  If not, please request a copy in writing from id Software at id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.


____________________________________________

4) GETTING THE SOURCE CODE
___________________________________

This project's GitHub.net Git repository can be checked out through Git with the following instruction set: 

	> git clone https://github.com/MadDeCoDeR/Classic-RBDOOM-3-BFG.git

If you don't want to use git, you can download the source as a zip file at
	https://github.com/MadDeCoDeR/Classic-RBDOOM-3-BFG/archive/master.zip



___________________________________________________________________

5) COMPILING ON WINDOWS WITH VISUAL STUDIO
__________________________________________________________

___________________________________________________________________

	A) Preperation
__________________________________________________________

1. Download and install Visual Studio (For new Version make sure the "Desktop Development with C++" component is checked on the installer)

2. Download and install DirectX SDK:

	a. For June 2010 Release (Windows XP compatible/Legacy): Download and Install it from Microsoft's website https://www.microsoft.com/en-us/download/details.aspx?id=8109

	b. For Modern DirectX: 
	
		I) XInput 9.1.0 and Direct Input comes pre-installed on Windows 7 and later (.dll that the game will load). 
	
		II) XAudio2. It has two versions 2.8 (only for Windows 8 and later) and XAudio 2.9 (backwards compatilble with Windows 7).
			2.8 like with XInput comes pre-installed with the OS. For 2.9 you must download it using NuGet's package manager

3. (Optional if you want to use cmake presets with Visual Studio) Download and install the latest CMake (recommended 3.8 for compile and run out of the box).

4. (For Xaudio 2.9) Download, install and set to your PATH the NuGet CLI.

5. Download the latest stable ffmpeg (5.0) shared from https://github.com/BtbN/FFmpeg-Builds/releases (only 64-bit available) and put them on the game's folder (where the origianal .exe is)

___________________________________________________________________

	B) Project Configuration and Genenration
__________________________________________________________

For old Versions of Visual Studio.
	1) Generate the VC projects using CMake by doubleclicking a matching configuration .bat file in the neo/ folder.

For Visual Studio 2019 and newer. 
	1) Open the repository folder on Visual Studio and set the CMake source the neo folder (it will prompt you to set it)
	2) Select the desired preset.
		Available Presets:
			64-bit Windows: Actively used, uses OpenGL, OpenAL and XAudio2.9
			64-bit Windows with Vulkan: Expirimental FOR DEVS ONLY. uses Vulkan, OpenAL and XAudio2.9
			32-bit Windows: Deprecated, many of the currently used libs are 64-bit

___________________________________________________________________

	C) Compiling Project
__________________________________________________________

1. Use the VC solution to compile  what you need:
	Classic-RBDOOM-3-BFG/build(Lib&cpuType)/DoomBFA.sln
	
(NOTE: The pre-built binaries are made wth buildx64)

2. Select Desired Profile and compile (either by right clicking the DoomBFA project and selecting 'Build' or by running the debugger (F5))
	Avalable Profiles:
		Debug: No Optimizations and with Debug symbols, used ONLY for debuging
		Release: With Optimizationd and with some Debug elements
		Retail: With Optimizations and without any Debug element, used for distributing pre-build binaries

NOTE: The Working Directory has been set the default install directory of DOOM 3 BFG Edition on C: drive
NOTE 2: By default the launch argument "+set r_fullscreen 0" is used. If you are in a single monitor system is HEAVILY recommented to be left on

__________________________________

6) COMPILING ON GNU/LINUX
_________________________

___________________________________________________________________

	A) Preperation
__________________________________________________________

1. Make sure you have installed GNU GCC on your system.
	In order to check it, open a terminal and type 'gcc -v', if it returns an output then it is intstalled.
	If it doesn't then try to install gcc using the package manager of your system (Some distros might support also the 'build-essentials' which might include some addons)
	NOTE: You need GCC 11 or newer in order to compile the code

1. Install required packages in order to compile and run the game:
 
	On Debian or Ubuntu:

		> apt-get install cmake libsdl2-dev libopenal-dev libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libglu1-mesa-dev freeglut3-dev mesa-common-dev
	
	On Fedora

		// ffmpeg-devel is found in the rpm-fusion repo. Can be enabled from Gnome Software if not already enabled
		
		> dnf install cmake SDL2-devel openal-devel ffmpeg-devel gcc-c++
	
	On ArchLinux 
	
		> sudo pacman -S sdl2 ffmpeg openal cmake
	
	On openSUSE (tested in 13.1)
	
		> zypper in openal-soft-devel cmake libSDL2-devel libffmpeg1-devel
	
		NOTE: SDL 1 is not so well supported and it's missing various features and optimizations
		For SDL 2 replace "libSDL-devel" with "libSDL2-devel".
		"libffmpeg1-devel" requires the PackMan repository. If you don't have that repo, and don't want to add it, remove the "libffmpeg1-devel" option and compile without ffmpeg support.
		If you have the repo and compiles with ffmpeg support, make sure you download "libffmpeg1-devel", and not "libffmpeg-devel".
	
	Instead of SDL2 development files you can also use SDL1.2. Install SDL 1.2 and add to the cmake parameters -DSDL2=OFF
	
	SDL2 has better input support (especially in the console) and better 
	support for multiple displays (especially in fullscreen mode).
	
___________________________________________________________________

	B) Project Configuration and Genenration
__________________________________________________________

Without cmake presets:

1. To Generate the Makefiles using CMake:

	> cd neo/
	> ./cmake-linux-<profile>.sh(recommended retail profile)

With CMake presets:

CMake GUI (also applies to Windows):
	- Open GUI
	- Select the neo folder as source
	- The dropdown below the source will become active
	- Select profile
	- Press Config and then Generate

Visual Studio Code IDE (might apply to other IDE's with similar CMake support):
	- Make sure you have installed Microsoft's C/C++ - Extension Pack
	- Open the repository folder
	- Select desired Profile

Available Profiles: See 5.C.2

___________________________________________________________________

	C) Compiling Project
__________________________________________________________

	
1.To Compile DOOM BFA

	> cd ../build
	> make (add DoomBFA in order to skip game.so's compilation)
	
	or
	
	> make -j <number of CPU cores> (for faster compiles)


___________________________________________________

7) INSTALLATION, GETTING THE GAMEDATA, RUNNING THE GAME
__________________________________________


If you use the prebuilt Win32 binaries then simply extract them to your
C:\Program Files (x86)\Steam\SteamApps\common\Doom 3 BFG Edition\ directory and run DoomBFA.exe.

The following instructions are primarily intented for Linux users and all hackers on other operating systems.

To play the game, you need the game data from a legal copy of the game.

It requires Steam or GOG Windows Installer.

Even the DVD version of Doom 3 BFG only contains encrytped data that is decoded
by Steam on install.

Steam:

On Steam for Linux you right click on the game in your library and select Properties, from there just check the last check box you will see and then right click it again and select install.
To get the game files just right click on the game select Properties -> local files -> Browse local files.

Alternatively on Linux and OSX the easiest way to install is with SteamCMD: https://developer.valvesoftware.com/wiki/SteamCMD
See the description on https://developer.valvesoftware.com/wiki/SteamCMD#Linux (OS X is directly below that) on how to install SteamCMD on your system. You won't have to create a new user.

Then you can download Doom 3 BFG with

> ./steamcmd.sh +@sSteamCmdForcePlatformType windows +login <YOUR_STEAM_LOGIN_NAME> +force_install_dir ./doom3bfg/ +app_update 208200 validate +quit

(replace <YOUR_STEAM_LOGIN_NAME> with your steam login name)
When it's done you should have the normal windows installation of Doom 3 BFG in ./doom3bfg/ and the needed files in ./doom3bfg/base/
That number is the "AppID" of Doom 3 BFG; if you wanna use this to get the data of other games you own, you can look up the AppID at https://steamdb.info/

NOTE that we've previously recommended using download_depot in the Steam console to install the game data. That turned out to be unreliable and result in broken, unusable game data. So use SteamCMD instead, as described above.
GOG:

Alternatively with the GOG installer, you can use Wine to install the game. See https://winehq.org/download for details on how to install wine for Linux and Mac.

Once Wine is installed and configured on your system install Doom 3 BFG edition using the downloaded installers from gog.com:

> wine setup_doom_3_bfg_1.14_\(13452\)_\(g\).exe

(there will be several .exe files from GOG, make sure all of them are in the same directory)

Once this is complete, by default you can find your Doom 3 BFG "base/" directory at ".wine/drive_c/GOG\ Games/DOOM\ 3\ BFG/base".

Extra:

Additionally in Ubuntu you can open the RBDoom3BFG file with the text editor and add the missing paths on it and save it as .desktop.
This will give you a desktop shortcut for Ubuntu. (NOTE: I don't know if other operating systems support this file)

Also you can install www.mono-project.com. Which allow you to use CDL.exe natively on linux.
After you have install mono, download the latest release of this project and extract the CDL.exe in the same directory DoomBFA is.

Anyway:

1. Install Doom 3 BFG in Steam (Windows version) or SteamCMD, make sure it's getting
   updated/patched.

2. Create your own Doom 3 BFG directory, e.g. /path/to/Doom3BFG/

3. Copy the game-data's base dir from Steam to that directory 
   (e.g. /path/to/Doom3BFG/), it's in
	/your/path/to/Steam/steamapps/common/DOOM 3 BFG Edition/base/
	or, if you used SteamCMD, in the path you used above.

4. Copy your DoomBFA executable that you created in 5) or 6) and the FFmpeg DLLs to your own 
   Doom 3 BFG directory (/path/to/Doom3BFG).
   
   Your own Doom 3 BFG directory now should look like:
	/path/to/Doom3BFG/
	 ->	DoomBFA (or DoomBFA.exe on Windows)
	 -> avcodec-58.dll
	 -> avformat-58.dll
	 -> avutil-56.dll
	 -> swresample-3.dll
	 -> swscale-5.dll
	 
5. Copy paste the base directory from the source code directory to the base directory on your own 
   Doom 3 BFG directory (/path/to/Doom3BFG).
   
(Optional if you want EAX on Doom 3) Download this addon https://www.moddb.com/mods/classic-rbdoom-3-bfg-edition/downloads/roe-addon
and extract it to the directory on your own 
   Doom 3 BFG directory (/path/to/Doom3BFG)

6. Run the game by executing the DoomBFA executable.

7. Enjoy

8. If you run into bugs, please report them, see 11)

Additionally for the Final DOOM wads you must legally own them (either aquired from retail,steam or GOG)
and then copy the two WADs on the /path/to/Doom3BFG/base/wads.

And for the Master Levels you must legally own them (either aquired from retail,steam or GOG),
make a folder named "master" on /path/to/Doom3BFG/base/wads and copy all the 20 wad files on this folder.

Otherwise the game wont let you play this expansions (because there are no files dah)

For the zBFA.resources:

1. Copy paste zBFA folder to your own 
   Doom 3 BFG directory (/path/to/Doom3BFG)
   (Optional) Copy paste the base/renderprogs folder in the zBFA folder 

2. Open DoomBFA.exe with the launch argument +set com_allowConsole 1

3. Open Doom 3 and activate the console (press ~ key)

4. Type writeresourcefilewithdir zBFA

5. Copy paste from C:\Users\<username>\Saved Games\Id Software\DOOM BFA\base\zBFA.resources to your own 
   Doom 3 BFG directory (/path/to/Doom3BFG) \base\maps

___________________________________________________

8) OVERALL CHANGES
__________________________________________

- Flexible build system using CMake

- Linux support (32 and 64 bit)

- Win64 support

- OS X support

- OpenAL Soft sound backend primarily developed for Linux but works on Windows as well

- Bink video support through FFmpeg

- PNG image support

- Soft shadows using PCF hardware shadow mapping

	The implementation uses sampler2DArrayShadow and PCF which usually
	requires Direct3D 10.1 however it is in the OpenGL 3.2 core so it should
	be widely supported.
	All 3 light types are supported which means parallel lights (sun) use
	scene independent cascaded shadow mapping.
	The implementation is very fast with single taps (400 fps average per
	scene on a GTX 660 ti OC) however I defaulted it to 12 taps using a Poisson disc algorithm so the shadows look
	really good which should give you stable 100 fps on todays hardware (2014).

- Changed light interaction shaders to use Half-Lambert lighting like in Half-Life 2 to 
	make the game less dark. https://developer.valvesoftware.com/wiki/Half_Lambert

- True 64 bit HDR lighting with adaptive tone mapping and gamma-correct rendering in linear RGB space

- Enhanced Subpixel Morphological Antialiasing
	For more information see "Anti-Aliasing Methods in CryENGINE 3" and the docs at http://www.iryoku.com/smaa/

- Filmic post process effects like Technicolor color grading and film grain

- Additional ambient render pass to make the game less dark similar to the Quake 4 r_forceAmbient technique

___________________________________________________

9) CONSOLE VARIABLES
__________________________________________

Check here: https://github.com/MadDeCoDeR/Classic-RBDOOM-3-BFG/wiki/3.-New-Parameters


___________________________________________________

10) KNOWN ISSUES
__________________________________________

None.

___________________________________________________

11) BUG REPORTS
__________________________________________

DOOM-BFA is not perfect, it is not bug free as every other software.
For fixing as much problems as possible we need as much bug reports as possible.
We cannot fix anything if we do not know about the problems.

The best way for telling us about a bug is by submitting a bug report at our GitHub bug tracker page:

	https://github.com/MadDeCoDeR/Classic-RBDOOM-3-BFG/issues?state=open

The most important fact about this tracker is that we cannot simply forget to fix the bugs which are posted there. 
It is also a great way to keep track of fixed stuff.

If you want to report an issue with the game, you should make sure that your report includes all information useful to characterize and reproduce the bug.

    * Search on Google
    * Include the computer's hardware and software description ( CPU, RAM, 3D Card, distribution, kernel etc. )
    * If appropriate, send a console log, a screenshot, an strace ..
    * If you are sending a console log, make sure to enable developer output:

              DoomBFA.exe +set developer 1 +set logfile 2
			  
		You can find your qconsole.log on Windows in C:\Users\<your user name>\Saved Games\id Software\DOOM BFA\base\

NOTE: We cannot help you with OS-specific issues like configuring OpenGL correctly, configuring ALSA or configuring the network.
	

___________________________________________________

12) GAME MODIFCATIONS
__________________________________________
	
The Doom 3 BFG Edition GPL Source Code release allows mod editing, in order for it to accept any change in your
mod directory, you should first specify your mod directory adding the following command to the launcher:

"+set fs_game modDirectoryName"

so it would end up looking like: DoomBFA +set fs_game modDirectoryName

Also now you can package your mod in .resource files for more info check: https://github.com/MadDeCoDeR/Classic-RBDOOM-3-BFG/wiki

Classic doom mods are enabled and can be used with the old "-file" parameter like this: RBDoom3BFG -file "classicmod.wad"

IMPORTANT: DOOM-BFA does not support old Doom 3 (2004) modiciations that include sourcecode modifications in binary form (.dll)
You can fork DOOM-BFA and create a new binary (.dll) that includes all required C++ game code modifications.
	
____________________________________________________________________________________

13) CODE LICENSE EXCEPTIONS - The parts that are not covered by the GPL:
_______________________________________________________________________


EXCLUDED CODE:  The code described below and contained in the Doom 3 BFG Edition GPL Source Code release
is not part of the Program covered by the GPL and is expressly excluded from its terms. 
You are solely responsible for obtaining from the copyright holder a license for such code and complying with the applicable license terms.


JPEG library
-----------------------------------------------------------------------------
neo/libs/jpeg-6/*

Copyright (C) 1991-1995, Thomas G. Lane

Permission is hereby granted to use, copy, modify, and distribute this
software (or portions thereof) for any purpose, without fee, subject to these
conditions:
(1) If any part of the source code for this software is distributed, then this
README file must be included, with this copyright and no-warranty notice
unaltered; and any additions, deletions, or changes to the original files
must be clearly indicated in accompanying documentation.
(2) If only executable code is distributed, then the accompanying
documentation must state that "this software is based in part on the work of
the Independent JPEG Group".
(3) Permission for use of this software is granted only if the user accepts
full responsibility for any undesirable consequences; the authors accept
NO LIABILITY for damages of any kind.

These conditions apply to any software derived from or based on the IJG code,
not just to the unmodified library.  If you use our work, you ought to
acknowledge us.

NOTE: unfortunately the README that came with our copy of the library has
been lost, so the one from release 6b is included instead. There are a few
'glue type' modifications to the library to make it easier to use from
the engine, but otherwise the dependency can be easily cleaned up to a
better release of the library.

zlib library
---------------------------------------------------------------------------
neo/libs/zlib/*

Copyright (C) 1995-2012 Jean-loup Gailly and Mark Adler

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Base64 implementation
---------------------------------------------------------------------------
neo/idlib/Base64.cpp

Copyright (c) 1996 Lars Wirzenius.  All rights reserved.

June 14 2003: TTimo <ttimo@idsoftware.com>
	modified + endian bug fixes
	http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=197039

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

IO for (un)compress .zip files using zlib
---------------------------------------------------------------------------
neo/libs/zlib/minizip/*

Copyright (C) 1998-2010 Gilles Vollant (minizip) ( http://www.winimage.com/zLibDll/minizip.html )

Modifications of Unzip for Zip64
Copyright (C) 2007-2008 Even Rouault

Modifications for Zip64 support
Copyright (C) 2009-2010 Mathias Svensson ( http://result42.com )

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

MD4 Message-Digest Algorithm
-----------------------------------------------------------------------------
neo/idlib/hashing/MD4.cpp
Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD4 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD4 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

MD5 Message-Digest Algorithm
-----------------------------------------------------------------------------
neo/idlib/hashing/MD5.cpp
This code implements the MD5 message-digest algorithm.
The algorithm is due to Ron Rivest.  This code was
written by Colin Plumb in 1993, no copyright is claimed.
This code is in the public domain; do with it what you wish.

CRC32 Checksum
-----------------------------------------------------------------------------
neo/idlib/hashing/CRC32.cpp
Copyright (C) 1995-1998 Mark Adler

OpenGL headers
---------------------------------------------------------------------------
neo/renderer/OpenGL/glext.h
neo/renderer/OpenGL/wglext.h

Copyright (c) 2007-2012 The Khronos Group Inc.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and/or associated documentation files (the
"Materials"), to deal in the Materials without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Materials, and to
permit persons to whom the Materials are furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Materials.

THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.

Timidity
---------------------------------------------------------------------------
neo/libs/timidity/*

Copyright (c) 1995 Tuukka Toivonen 

From http://www.cgs.fi/~tt/discontinued.html :

If you'd like to continue hacking on TiMidity, feel free. I'm
hereby extending the TiMidity license agreement: you can now 
select the most convenient license for your needs from (1) the
GNU GPL, (2) the GNU LGPL, or (3) the Perl Artistic License.  



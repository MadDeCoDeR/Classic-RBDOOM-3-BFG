cmake_minimum_required(VERSION 3.15)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
project(DoomBFA)

option(USE_MFC_TOOLS
		"Compile the built-in MFC based tools" OFF)
		
option(MONOLITH
		"Embed game logic into main executable" ON)

option(USE_PRECOMPILED_HEADERS
		"Use precompiled headers during build" ON)
		
option(SDL2
		"Use SDL2 instead of SDL1.2" ON)
		
option(USE_XAUDIO2
		"Use XAudio2 alongside OpenAL Soft" OFF)

option(FFMPEG
		"Use FMPEG to render Bink videos" ON)

option(USE_PACKAGED_FFMPEG
		"Use FMPEG From Depedency Manager" OFF)

option(BINKDEC
		"Use included libbinkdec to render Bink videos" OFF)
		
option(USE_VULKAN
		"Use Vulkan instead of OpenGL" OFF)

option(ONATIVE
		"Optimize for the host CPU" OFF)
		
option(USE_SYSTEM_DX
		"Build using the System's DirectX and NOT the DirectX of June 2010" OFF)
		
option(WINDOWS10
		"Build for Windows 10" OFF)
option(PACKAGED
		"Build for packaged environment" OFF)
		
option(USE_SYSTEM_ZLIB
                "Use the system zlib instead of the bundled one" OFF)

option(USE_SYSTEM_LIBPNG
                "Use the system libpng instead of the bundled one" OFF)

option(USE_SYSTEM_LIBJPEG
                "Use the system libjpeg instead of the bundled one" OFF)

option(USE_SYSTEM_LIBGLEW
                "Use the system libglew instead of the bundled one" OFF)

option(USE_SYSTEM_RAPIDJSON
                "Use the system rapidjson instead of the bundled one" OFF)
				
option (USE_XAUDIO2_PACKAGE
				"Use XAudio 2.9 from nuget (requires nuget executable set on PATH)" OFF)

option (ALL_STATIC
				"Most vcpkg depedencies are static" OFF)

option (USE_DXGI
				"Use Direct3D to render OpenGL on Windows" OFF)

if (CMAKE_CL_64)
	set(CPU_ARCH x64)
else()
	set(CPU_ARCH x86)
endif()

set(CPU_TYPE "" CACHE STRING "When set, passes this string as CPU-ID which will be embedded into the binary.")

set(UWP_PATH "" CACHE STRING "When set, passes this string as the game's base search folder.")

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
	set(CPU_OPTIMIZATION "-mmmx -msse -msse2" CACHE STRING "Which CPU specific optimitations should be used beside the compiler's default?")
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "(powerpc|ppc)64le")
	option(USE_INTRINSICS "Compile using intrinsics (e.g mmx, sse, msse2)" OFF)
else()
	option(USE_INTRINSICS "Compile using intrinsics (e.g mmx, sse, msse2)" ON)
endif()

option (MAKE_DLL "Make game.dll solution" ON)

if(FFMPEG AND BINKDEC)
	message(FATAL_ERROR "Only one of FFMPEG and BINKDEC (or neither) can be enabled at a time")
endif()

if(UNIX)
	set(USE_XAUDIO2 FALSE)
	set(USE_XAUDIO2_PACKAGE FALSE)
endif()

if (CMAKE_CL_64)
	set(OS_CPU Win64)
else()
	set(OS_CPU Win32)
endif()
		

if(MSVC)
	if(USE_XAUDIO2_PACKAGE)
		set(USE_XAUDIO2 TRUE)
		set(USE_SYSTEM_DX TRUE)
	endif()
else()
	message(STATUS CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
	add_definitions(-pipe)
	#add_definitions(-Wall)
	add_definitions(-Werror=format-security)
	add_definitions(-Werror=format)
	set (CMAKE_CXX_STANDARD 17)
	if(CPU_TYPE)
		add_definitions(-DCPUSTRING="${CPU_TYPE}")
	endif()
	if (CPU_OPTIMIZATION)
		add_definitions(${CPU_OPTIMIZATION})
	endif()
	if (USE_INTRINSICS)
		add_definitions(-DUSE_INTRINSICS)
	endif()
	if(WIN32)
		# require msvcr70.dll or newer for _aligned_malloc etc
		# I think it is from Visual C++ .NET 2002, so it should be available on any remotely modern system.
		add_definitions(-D__MSVCRT_VERSION__=0x0700)
		
	#	find_package(DirectX REQUIRED)
		include_directories(libs/mingw-hacks)
	#	include_directories(${DirectX_INCLUDE_DIR})
	#	add_definitions(-DUSE_XINPUT)
	endif()	
	
	#if(NOT ANDROID)
		add_definitions(-DUSE_EXCEPTIONS)
	#endif()
	
	add_compile_options(-Wno-pragmas -Wno-unused-variable -Wno-switch -Wno-unused-value -Winvalid-pch -Wno-multichar)
	if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
		# add clang-specific settings for warnings (the second one make sure clang doesn't complain
		# about unknown -W flags, like -Wno-unused-but-set-variable)
		add_compile_options(-Wno-local-type-template-args -Wno-unknown-warning-option -Wno-inline-new-delete -Wno-switch-enum -Wno-varargs -Wno-delete-abstract-non-virtual-dtor -Wno-shift-negative-value -Wno-comment)
	endif()
	
	if(NOT CMAKE_CROSSCOMPILING AND ONATIVE)
		add_definitions(-march=native)
	endif()

	add_compile_options(-fno-strict-aliasing)
	
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG -Og -ggdb -fno-strict-aliasing ${my_warn_flags}")
	#set(CMAKE_C_FLAGS_DEBUGALL "${CMAKE_C_FLAGS_DEBUGALL} -g -ggdb -D_DEBUG -fno-strict-aliasing ${my_warn_flags}")
	#set(CMAKE_C_FLAGS_PROFILE "${CMAKE_C_FLAGS_PROFILE} -g -ggdb -D_DEBUG -O1 -fno-omit-frame-pointer -fno-strict-aliasing ${my_warn_flags}")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG -ffast-math -fno-unsafe-math-optimizations -fomit-frame-pointer -fno-strict-aliasing ${my_warn_flags}")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -g -O3 -DNDEBUG -ffast-math -fno-unsafe-math-optimizations -fomit-frame-pointer -fno-strict-aliasing ${my_warn_flags}")
	#set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -Os -ffast-math -fno-unsafe-math-optimizations -fomit-frame-pointer -fno-strict-aliasing ${my_warn_flags}")
	set(CMAKE_C_FLAGS_RETAIL "${CMAKE_C_FLAGS_RETAIL} -s -O3 -DID_RETAIL -DNDEBUG -ffast-math -fno-strict-aliasing -Werror -Wno-error=invalid-pch -Wno-error=invalid-offsetof ${my_warn_flags}" CACHE STRING "" FORCE)
	
	

	
	set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
	#set(CMAKE_CXX_FLAGS_DEBUGALL ${CMAKE_C_FLAGS_DEBUGALL})
	#set(CMAKE_CXX_FLAGS_PROFILE ${CMAKE_C_FLAGS_PROFILE})
	set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_C_FLAGS_RELWITHDEBINFO})
	#set(CMAKE_CXX_FLAGS_MINSIZEREL ${CMAKE_C_FLAGS_MINSIZEREL})
	set(CMAKE_CXX_FLAGS_RETAIL ${CMAKE_C_FLAGS_RETAIL} CACHE STRING "" FORCE)
	
	SET( CMAKE_EXE_LINKER_FLAGS_RETAIL "" CACHE STRING "" FORCE)
	
	SET( CMAKE_SHARED_LINKER_FLAGS_RETAIL "" CACHE STRING "" FORCE)
	#GK: Add the Retail build for linux and mingw
	MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_RETAIL
    CMAKE_C_FLAGS_RETAIL
    CMAKE_EXE_LINKER_FLAGS_RETAIL
    CMAKE_SHARED_LINKER_FLAGS_RETAIL )
	
	SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo Retail."
    FORCE )

	if(PACKAGED)
		add_definitions(-DPACKAGED)
	endif()
#endif()
elseif(MSVC)
	
	# Multiprocessor support for compiling
	# /MP
	
	# /MT  use multithreaded, static runtime library
	#      (=> binary won't need MSVCP100D.dll or similar)
	# /MTd the same with debug symbols
	# /MD and /MDd - the same with dynamic runtime lib
	#     (=> MSVCP100D.dll or similar is needed to run binary)

	# Inline Function Expansion: Any Suitable (/Ob2)
	# "/Ob2",
			
	# enable Intrinsic Functions
	# "/Oi",
			
	# Omit Frame Pointers
	# "/Oy",
	
	# if(MSVC_VERSION EQUAL 1700)
		# #message(STATUS "$ENV{LIBPATH}")
		
		# find_path(Windows_winmd_DIR NAMES Windows.winmd
			# HINTS
			# C:/Program Files (x86)/Windows Kits/8.0/References/CommonConfiguration/Neutral
			# PATH_SUFFIXES "Neutral")
			
		# message(STATUS "${Windows_winmd_DIR}")
		
		# #set(WINRT_OPTIONS "/ZW /D USE_WINRT /AI \"C:\Program Files (x86)\Windows Kits\8.0\References\CommonConfiguration\Neutral\"")
		# #set(WINRT_OPTIONS "/ZW /D USE_WINRT /AI \"$ENV{LIBPATH}\" ")
		# set(WINRT_OPTIONS "/ZW /D USE_WINRT")
	
		# set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D _DEBUG /MP ${WINRT_OPTIONS}")
		# set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MP /Oi /Oy ${WINRT_OPTIONS}")
		# set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MP /Oi ${WINRT_OPTIONS}")
		# set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /MP /Oi /Oy ${WINRT_OPTIONS}")
		
		# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP ${WINRT_OPTIONS}")
		# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MP /Oi /Oy ${WINRT_OPTIONS}")
		# set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MP ${WINRT_OPTIONS}")
		# set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MP /Oi /Oy ${WINRT_OPTIONS}")
	# else()
		if(NOT SET_UP_CONFIGURATIONS_DONE)
    set(SET_UP_CONFIGURATIONS_DONE 1)

    #GK: Use 3 configurations Debug,Release and Retail
    if(CMAKE_CONFIGURATION_TYPES) 
        set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Retail" CACHE STRING "" FORCE) 
    endif()
	#GK: Fix performance isse on classic doom for the debug build
		set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D _DEBUG /MP /Ob2 /Oi /Oy /MDd /std:c17")
		set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MP /Oi /Oy /MD /std:c17")
		#set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MP /Oi /MTd")
		#set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /MP /Oi /Oy /MT")
		set(CMAKE_C_FLAGS_RETAIL "${CMAKE_C_FLAGS_RELEASE} /D ID_RETAIL /MP /Oi /Oy /MD /std:c17")
		
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP /Ob2 /Oi /Oy /MDd /std:c++17 /std:c17 /W4")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MP /Oi /Oy /MD /std:c++17 /std:c17 /WX /W4 /sdl")
		#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MP /MTd")
		#set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MP /Oi /Oy /MT")
		set(CMAKE_CXX_FLAGS_RETAIL "${CMAKE_CXX_FLAGS_RELEASE} /D ID_RETAIL")
		# RB: without /SAFESEH:NO we can't link against ffmpeg libs and VS2013 or we have to build our own
		# libs for newer compilers
		set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG:FULL /STACK:16777216")
		#set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} /SAFESEH:NO")
		set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /STACK:16777216 /OPT:REF /OPT:ICF")
		#set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /SAFESEH:NO")
		set(CMAKE_EXE_LINKER_FLAGS_RETAIL "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
		set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /DEF:../neo/d3xp/Game.def")
		set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEF:../neo/d3xp/Game.def")
		set(CMAKE_SHARED_LINKER_FLAGS_RETAIL "${CMAKE_SHARED_LINKER_FLAGS_RETAIL} /DEF:../neo/d3xp/Game.def")
		endif()
	# endif()
	

	add_definitions(-DWIN32 
					-D_WINDOWS
					-D_CRT_SECURE_NO_DEPRECATE
					-D_CRT_NONSTDC_NO_DEPRECATE
					-D_CRT_SECURE_NO_WARNINGS
                                        -D_MBCS
                                       #-DUSE_OPENAL
					-DUSE_EXCEPTIONS)
        ## Check for Version ##
        if( USE_SYSTEM_DX ) # System DirectX
			add_definitions(-DUSE_SYS_DX)
        endif()
		
		if (WINDOWS10)
			add_definitions(-D_UWP)
			add_definitions(-DPACKAGED)
			add_definitions(-DUWP_GAME_PATH="${UWP_PATH}")
		endif()
		
		if(USE_XAUDIO2)
			add_definitions(-DUSE_XAUDIO2)
		endif()
		
		if( USE_XAUDIO2_PACKAGE ) # Windows RT
			add_definitions(-DUSE_XAUDIO2_PACKAGE)
        endif()

        if (USE_INTRINSICS)
			add_definitions(-DUSE_INTRINSICS)
        endif()
					
	if(NOT CMAKE_CL_64)
		add_definitions(-D_USE_32BIT_TIME_T)
	endif()
					
	if(USE_MFC_TOOLS)
		add_definitions(-D_AFXDLL)
		
		# 0: Use Standard Windows Libraries
		# 1: Use MFC in a Static Library
		# 2: Use MFC in a Shared DLL 
		set(CMAKE_MFC_FLAG 2)
	endif()
	
	
	
	if (MSVC_VERSION GREATER 1919 AND NOT USE_VULKAN AND NOT ALL_STATIC)
		add_definitions(-D_ITERATOR_DEBUG_LEVEL=1)
	endif()
	
endif()

#if(STANDALONE)
#	add_definitions(-DSTANDALONE)
#endif()

if (USE_SYSTEM_ZLIB)
  find_package(ZLIB REQUIRED)
endif(USE_SYSTEM_ZLIB)

if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
	if (MSVC AND ALL_STATIC)
		set(ZLIB_LIBRARY
		optimized "vcpkg_installed/${CPU_ARCH}-windows-static-md/lib/zlib"
		debug "vcpkg_installed/${CPU_ARCH}-windows-static-md/debug/lib/zlibd")
	else()
		set(ZLIB_LIBRARY ${ZLIB_LIBRARIES})
	endif()
else (ZLIB_FOUND)
    include_directories("libs/zlib")
    set(ZLIB_LIBRARY "" )
endif (ZLIB_FOUND)

if(USE_SYSTEM_LIBPNG)
find_package(PNG REQUIRED)
endif (USE_SYSTEM_LIBPNG)

if (PNG_FOUND)
    include_directories(${PNG_INCLUDE_DIRS})
	if (MSVC AND ALL_STATIC)
		set(PNG_LIBRARY
		optimized "vcpkg_installed/${CPU_ARCH}-windows-static-md/lib/libpng16"
		debug "vcpkg_installed/${CPU_ARCH}-windows-static-md/debug/lib/libpng16d")
	else()
		set(PNG_LIBRARY ${PNG_LIBRARIES})
	endif()
else (PNG_FOUND)
    include_directories("libs/png")
    set(PNG_LIBRARY "" )
endif (PNG_FOUND)


if(USE_SYSTEM_LIBJPEG)
find_package(JPEG REQUIRED)
endif(USE_SYSTEM_LIBJPEG)

if (JPEG_FOUND)
    include_directories(${JPEG_INCLUDE_DIRS})
	if (MSVC AND ALL_STATIC)
		set(JPEG_LIBRARY
		optimized "vcpkg_installed/${CPU_ARCH}-windows-static-md/lib/jpeg"
		debug "vcpkg_installed/${CPU_ARCH}-windows-static-md/debug/lib/jpeg")
	else()
		set(JPEG_LIBRARY ${JPEG_LIBRARIES})
	endif()
    add_definitions(-DUSE_NEWER_JPEG)
else (JPEG_FOUND)
    include_directories("libs/jpeg-6")
    set(JPEG_LIBRARY "" )
endif (JPEG_FOUND)


macro(SET_OPTION option value)
  set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()

if(USE_VULKAN)

	# override Glslang build options
	SET_OPTION(ENABLE_SPVREMAPPER OFF)
	SET_OPTION(ENABLE_GLSLANG_BINARIES OFF)
	SET_OPTION(ENABLE_HLSL OFF)
	#SET_OPTION(ENABLE_OPT OFF)
	SET_OPTION(ENABLE_OPT ON)
	SET_OPTION(SPIRV_SKIP_EXECUTABLES ON)
		
	# options required by July 2020 release
	SET_OPTION(ENABLE_PCH OFF)
	SET_OPTION(ENABLE_CTEST OFF)
	SET_OPTION(BUILD_EXTERNAL OFF)
	SET_OPTION(SKIP_GLSLANG_INSTALL ON)
	
	# Use FindVulkan module added with CMAKE 3.7
	if(NOT CMAKE_VERSION VERSION_LESS 3.7.0)
		message( STATUS "Using module to find Vulkan" )
		find_package(Vulkan)
	endif()
	
	
	if(NOT Vulkan_FOUND)
		message(FATAL_ERROR "Could not find Vulkan library!")
	else()
		message(STATUS ${Vulkan_LIBRARY})
	endif()
	
	add_definitions(-DUSE_VULKAN)
	include_directories($ENV{VULKAN_SDK}/Include)

else()

	if(USE_SYSTEM_LIBGLEW)
		find_package(GLEW)
	endif()

	if(GLEW_FOUND)
		include_directories(${GLEW_INCLUDE_DIRS})
		set(GLEW_LIBRARY ${GLEW_LIBRARIES})
		if (ALL_STATIC)
			add_definitions(-DGLEW_STATIC)
		endif()
	else()
		include_directories("libs/glew/include")
		set(GLEW_LIBRARY "" )
		add_definitions(-DGLEW_STATIC)
		
		set(GLEW_INCLUDES libs/glew/include/GL/glew.h)
		set(GLEW_SOURCES libs/glew/src/glew.c)

		if(WIN32)
			set(GLEW_INCLUDES ${GLEW_INCLUDES} libs/glew/include/GL/wglew.h)
		else()
			set(GLEW_INCLUDES ${GLEW_INCLUDES} libs/glew/include/GL/glxew.h)
		endif()
		
	endif()
endif()

if(USE_SYSTEM_RAPIDJSON)
find_package(rapidjson REQUIRED)
endif(USE_SYSTEM_RAPIDJSON)

if (RAPIDJSON_FOUND)
	include_directories("${RAPIDJSON_INCLUDE_DIRS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RAPIDJSON_CXX_FLAGS}")
else (RAPIDJSON_FOUND)
	include_directories("libs/rapidjson/include")
endif (RAPIDJSON_FOUND)


	
add_subdirectory(idlib)

if (MAKE_DLL)
	add_subdirectory(d3xp)
endif (MAKE_DLL)

file(GLOB AAS_INCLUDES aas/*.h)
file(GLOB AAS_SOURCES aas/*.cpp)

file(GLOB_RECURSE CM_INCLUDES cm/*.h)
file(GLOB_RECURSE CM_SOURCES cm/*.cpp)

file(GLOB FRAMEWORK_INCLUDES framework/*.h)
file(GLOB FRAMEWORK_SOURCES framework/*.cpp)

file(GLOB FRAMEWORK_ASYNC_INCLUDES framework/async/*.h)
file(GLOB FRAMEWORK_ASYNC_SOURCES framework/async/*.cpp)

file(GLOB RENDERER_INCLUDES renderer/*.h)
file(GLOB RENDERER_SOURCES renderer/*.cpp)
file(GLOB RENDERER_COLOR_INCLUDES renderer/Color/*.h)
file(GLOB RENDERER_COLOR_SOURCES renderer/Color/*.cpp)
file(GLOB RENDERER_DXT_INCLUDES renderer/DXT/*.h)
file(GLOB RENDERER_DXT_SOURCES renderer/DXT/*.cpp)
file(GLOB RENDERER_JOBS_INCLUDES renderer/jobs/*.h)
file(GLOB RENDERER_JOBS_SOURCES renderer/jobs/*.cpp)
file(GLOB RENDERER_JOBS_DYNAMICSHADOWVOLUME_INCLUDES renderer/jobs/dynamicshadowvolume/*.h)
file(GLOB RENDERER_JOBS_DYNAMICSHADOWVOLUME_SOURCES renderer/jobs/dynamicshadowvolume/*.cpp)
file(GLOB RENDERER_JOBS_PRELIGHTSHADOWVOLUME_INCLUDES renderer/jobs/prelightshadowvolume/*.h)
file(GLOB RENDERER_JOBS_PRELIGHTSHADOWVOLUME_SOURCES renderer/jobs/prelightshadowvolume/*.cpp)
file(GLOB RENDERER_JOBS_STATICSHADOWVOLUME_INCLUDES renderer/jobs/staticshadowvolume/*.h)
file(GLOB RENDERER_JOBS_STATICSHADOWVOLUME_SOURCES renderer/jobs/staticshadowvolume/*.cpp)

file(GLOB RENDERER_SMAA_INCLUDES renderer/SMAA/*.h)

file(GLOB RENDERER_OPENGL_INCLUDES renderer/OpenGL/*.h)
file(GLOB RENDERER_OPENGL_SOURCES renderer/OpenGL/*.cpp)

file(GLOB RENDERER_VULKAN_INCLUDES renderer/Vulkan/*.h)
file(GLOB RENDERER_VULKAN_SOURCES renderer/Vulkan/*.cpp)

file(GLOB SHADERS ../base/renderprogs/*.hlsl ../base/renderprogs/*.inc )

file(GLOB IRRXML_INCLUDES libs/irrxml/src/*.h)
file(GLOB IRRXML_SOURCES libs/irrxml/src/*.cpp)

if (NOT JPEG_FOUND)
    set(JPEG_INCLUDES
        libs/jpeg-6/jchuff.h
        libs/jpeg-6/jconfig.h
        libs/jpeg-6/jdct.h
        libs/jpeg-6/jdhuff.h
        libs/jpeg-6/jerror.h
        libs/jpeg-6/jinclude.h
        libs/jpeg-6/jmemsys.h
        libs/jpeg-6/jmorecfg.h
        libs/jpeg-6/jpegint.h
        libs/jpeg-6/jpeglib.h
        libs/jpeg-6/jversion.h)

    set(JPEG_SOURCES
        libs/jpeg-6/jcapimin.cpp
        libs/jpeg-6/jcapistd.cpp
        libs/jpeg-6/jccoefct.cpp
        libs/jpeg-6/jccolor.cpp
        libs/jpeg-6/jcdctmgr.cpp
        libs/jpeg-6/jchuff.cpp
        libs/jpeg-6/jcinit.cpp
        libs/jpeg-6/jcmainct.cpp
        libs/jpeg-6/jcmarker.cpp
        libs/jpeg-6/jcmaster.cpp
        libs/jpeg-6/jcomapi.cpp
        libs/jpeg-6/jcparam.cpp
        libs/jpeg-6/jcphuff.cpp
        libs/jpeg-6/jcprepct.cpp
        libs/jpeg-6/jcsample.cpp
        libs/jpeg-6/jctrans.cpp
        libs/jpeg-6/jdapimin.cpp
        libs/jpeg-6/jdapistd.cpp
        libs/jpeg-6/jdatadst.cpp
        libs/jpeg-6/jdatasrc.cpp
        libs/jpeg-6/jdcoefct.cpp
        libs/jpeg-6/jdcolor.cpp
        libs/jpeg-6/jddctmgr.cpp
        libs/jpeg-6/jdhuff.cpp
        libs/jpeg-6/jdinput.cpp
        libs/jpeg-6/jdmainct.cpp
        libs/jpeg-6/jdmarker.cpp
        libs/jpeg-6/jdmaster.cpp
        libs/jpeg-6/jdmerge.cpp
        libs/jpeg-6/jdphuff.cpp
        libs/jpeg-6/jdpostct.cpp
        libs/jpeg-6/jdsample.cpp
        libs/jpeg-6/jdtrans.cpp
        libs/jpeg-6/jerror.cpp
        libs/jpeg-6/jfdctflt.cpp
        libs/jpeg-6/jfdctfst.cpp
        libs/jpeg-6/jfdctint.cpp
        libs/jpeg-6/jidctflt.cpp
        libs/jpeg-6/jidctfst.cpp
        libs/jpeg-6/jidctint.cpp
        libs/jpeg-6/jidctred.cpp
        #libs/jpeg-6/jload.cpp
        libs/jpeg-6/jmemmgr.cpp
        libs/jpeg-6/jmemnobs.cpp
        #libs/jpeg-6/jpegtran.cpp
        libs/jpeg-6/jquant1.cpp
        libs/jpeg-6/jquant2.cpp
        libs/jpeg-6/jutils.cpp)
else (NOT JPEG_FOUND)
   set(JPEG_INCLUDES "")
   set(JPEG_SOURCES "")
endif (NOT JPEG_FOUND)

if (NOT PNG_FOUND)
    file(GLOB PNG_INCLUDES libs/png/*.h)
    file(GLOB PNG_SOURCES libs/png/*.c)
else (NOT PNG_FOUND)
    set(PNG_INCLUDES "")
    set(PNG_SOURCES "")
endif (NOT PNG_FOUND)

if (NOT ZLIB_FOUND)
file(GLOB ZLIB_INCLUDES libs/zlib/*.h)
file(GLOB ZLIB_SOURCES libs/zlib/*.c)
else (NOT ZLIB_FOUND)
  set(ZLIB_INCLUDES "")
  set(ZLIB_SOURCES "")
endif (NOT ZLIB_FOUND)

if(BINKDEC)
	file(GLOB BINKDEC_INCLUDES libs/libbinkdec/include/*.h)
	file(GLOB BINKDEC_SOURCES libs/libbinkdec/src/*.c libs/libbinkdec/src/*.cpp)
	add_definitions(-DUSE_BINKDEC)
	include_directories("libs/libbinkdec/include")
endif()

file(GLOB MINIZIP_INCLUDES libs/zlib/minizip/*.h)
file(GLOB MINIZIP_SOURCES libs/zlib/minizip/*.c libs/zlib/minizip/*.cpp)
	

	
set(FREETYPE_SOURCES
	libs/freetype/src/autofit/autofit.c
	libs/freetype/src/bdf/bdf.c
	libs/freetype/src/cff/cff.c
	libs/freetype/src/base/ftbase.c
	libs/freetype/src/base/ftbitmap.c
	libs/freetype/src/cache/ftcache.c
	libs/freetype/src/base/ftdebug.c
	libs/freetype/src/base/ftgasp.c
	libs/freetype/src/base/ftglyph.c
	libs/freetype/src/gzip/ftgzip.c
	libs/freetype/src/base/ftinit.c
	libs/freetype/src/lzw/ftlzw.c
	libs/freetype/src/base/ftstroke.c
	libs/freetype/src/base/ftsystem.c
	libs/freetype/src/smooth/smooth.c
	libs/freetype/src/base/ftbbox.c
	libs/freetype/src/base/ftmm.c
	libs/freetype/src/base/ftpfr.c
	libs/freetype/src/base/ftsynth.c
	libs/freetype/src/base/fttype1.c
	libs/freetype/src/base/ftwinfnt.c
	libs/freetype/src/pcf/pcf.c
	libs/freetype/src/pfr/pfr.c
	libs/freetype/src/psaux/psaux.c
	libs/freetype/src/pshinter/pshinter.c
	libs/freetype/src/psnames/psmodule.c
	libs/freetype/src/raster/raster.c
	libs/freetype/src/sfnt/sfnt.c
	libs/freetype/src/truetype/truetype.c
	libs/freetype/src/type1/type1.c
	libs/freetype/src/cid/type1cid.c
	libs/freetype/src/type42/type42.c
	libs/freetype/src/winfonts/winfnt.c)
	
	
set(SOUND_INCLUDES
	sound/snd_efxfile.h
	sound/snd_local.h
	sound/sound.h
	sound/SoundVoice.h
	sound/WaveFile.h
	sound/AVD.h
	sound/CCScriptDecl.h)
	
set(SOUND_SOURCES
	#sound/snd_cache.cpp
	#sound/snd_decoder.cpp
	sound/snd_efxfile.cpp
	sound/snd_emitter.cpp
	sound/snd_shader.cpp
	sound/snd_system.cpp
	sound/snd_world.cpp
	sound/SoundVoice.cpp
	sound/WaveFile.cpp
	sound/AVD.cpp
	sound/CCScriptDecl.cpp
	)
	
set(SOUND_COMMON_INCLUDES
	sound/common/SoundHardware.h
	sound/common/SoundSample.h
	sound/common/CommonSoundVoice.h
	sound/common/CinematicAudio.h)
	
set(SOUND_COMMON_SOURCES
	sound/common/SoundCVars.cpp)
	
set(XAUDIO2_INCLUDES
	sound/XAudio2/XA2_SoundHardware.h
	sound/XAudio2/XA2_SoundSample.h
	sound/XAudio2/XA2_SoundVoice.h
	sound/XAudio2/XA2_CinematicAudio.h)
	
set(XAUDIO2_SOURCES
	sound/XAudio2/XA2_SoundHardware.cpp
	sound/XAudio2/XA2_SoundSample.cpp
	sound/XAudio2/XA2_SoundVoice.cpp
	sound/XAudio2/XA2_CinematicAudio.cpp)
	#GK: Add EFX related files when using the openAL
set(OPENAL_INCLUDES
	sound/OpenAL/AL_EAX.h
	sound/OpenAL/AL_SoundHardware.h
	sound/OpenAL/AL_SoundSample.h
	sound/OpenAL/AL_SoundVoice.h
	sound/OpenAL/AL_CinematicAudio.h)
	
set(OPENAL_SOURCES
	sound/OpenAL/AL_EAX.cpp
	sound/OpenAL/AL_SoundHardware.cpp
	sound/OpenAL/AL_SoundSample.cpp
	sound/OpenAL/AL_SoundVoice.cpp
	sound/OpenAL/AL_CinematicAudio.cpp)
	
set(OGGVORBIS_INCLUDES
	libs/oggvorbis/ogg/ogg.h
	libs/oggvorbis/ogg/os_types.h)
	
set(OGGVORBIS_SOURCES
	libs/oggvorbis/oggsrc/bitwise.c
	libs/oggvorbis/oggsrc/framing.c
	
	libs/oggvorbis/vorbissrc/mdct.c
	libs/oggvorbis/vorbissrc/smallft.c
	libs/oggvorbis/vorbissrc/block.c
	libs/oggvorbis/vorbissrc/envelope.c
	libs/oggvorbis/vorbissrc/windowvb.c
	libs/oggvorbis/vorbissrc/lsp.c
	libs/oggvorbis/vorbissrc/lpc.c
	libs/oggvorbis/vorbissrc/analysis.c
	libs/oggvorbis/vorbissrc/synthesis.c
	libs/oggvorbis/vorbissrc/psy.c
	libs/oggvorbis/vorbissrc/info.c
	libs/oggvorbis/vorbissrc/floor1.c
	libs/oggvorbis/vorbissrc/floor0.c
	libs/oggvorbis/vorbissrc/res0.c
	libs/oggvorbis/vorbissrc/mapping0.c
	libs/oggvorbis/vorbissrc/registry.c
	libs/oggvorbis/vorbissrc/codebook.c
	libs/oggvorbis/vorbissrc/sharedbook.c
	libs/oggvorbis/vorbissrc/lookup.c
	libs/oggvorbis/vorbissrc/bitrate.c
	libs/oggvorbis/vorbissrc/vorbisfile.c)

	if (NOT RAPIDJSON_FOUND) 
		file(GLOB RAPIDJSON_INCLUDES libs/rapidjson/include/rapidjson/*.h)
	endif()

file(GLOB OPEN_PLATFORM_INCLUDES libs/open-platform/include)
include_directories(${OPEN_PLATFORM_INCLUDES})
#file(GLOB OPEN_PLATFORM_SOURCES libs/platform/*.cpp)
	
file(GLOB SYS_INCLUDES sys/*.h)
file(GLOB SYS_SOURCES sys/*.cpp)

file(GLOB UI_INCLUDES ui/*.h)
file(GLOB UI_SOURCES ui/*.cpp)

file(GLOB SWF_INCLUDES swf/*.h)
file(GLOB SWF_SOURCES swf/*.cpp)

file(GLOB CREDIT_INCLUDES swf/credits/*.h)
file(GLOB CREDIT_SOURCES swf/credits/*.cpp)


set(GAMED3XP_INCLUDES
	d3xp/Achievements.h
	d3xp/Actor.h
	d3xp/AF.h
	d3xp/AFEntity.h
	d3xp/AimAssist.h
	d3xp/BrittleFracture.h
	d3xp/Camera.h
	d3xp/Classic.h
	#d3xp/EndLevel.h
	d3xp/Entity.h
	d3xp/Fx.h
	d3xp/Game.h
	d3xp/GameEdit.h
	d3xp/Game_local.h
	d3xp/Grabber.h
	d3xp/IK.h
	d3xp/Item.h
	d3xp/Leaderboards.h
	d3xp/Light.h
	d3xp/Misc.h
	d3xp/Moveable.h
	d3xp/Mover.h
	d3xp/MultiplayerGame.h
	d3xp/Player.h
	d3xp/PlayerIcon.h
	d3xp/PlayerView.h
	d3xp/PredictedValue.h
	d3xp/Projectile.h
	d3xp/Pvs.h
	d3xp/SecurityCamera.h
	d3xp/SmokeParticles.h
	d3xp/Sound.h
	d3xp/Target.h
	d3xp/Trigger.h
	d3xp/Weapon.h
	d3xp/WorldSpawn.h)
	
set(GAMED3XP_SOURCES
	d3xp/Achievements.cpp
	d3xp/Actor.cpp
	d3xp/AF.cpp
	d3xp/AFEntity.cpp
	d3xp/AimAssist.cpp
	d3xp/BrittleFracture.cpp
	d3xp/Camera.cpp
	d3xp/Classic.cpp
	#d3xp/EndLevel.cpp
	d3xp/Entity.cpp
	d3xp/Fx.cpp
	d3xp/GameEdit.cpp
	d3xp/Game_local.cpp
	d3xp/Game_network.cpp
	d3xp/Grabber.cpp
	d3xp/IK.cpp
	d3xp/Item.cpp
	d3xp/Leaderboards.cpp
	d3xp/Light.cpp
	d3xp/Misc.cpp
	d3xp/Moveable.cpp
	d3xp/Mover.cpp
	d3xp/MultiplayerGame.cpp
	d3xp/Player.cpp
	d3xp/PlayerIcon.cpp
	d3xp/PlayerView.cpp
	d3xp/precompiled.cpp
	d3xp/Projectile.cpp
	d3xp/Pvs.cpp
	d3xp/SecurityCamera.cpp
	d3xp/SmokeParticles.cpp
	d3xp/Sound.cpp
	d3xp/Target.cpp
	d3xp/Trigger.cpp
	d3xp/Weapon.cpp
	d3xp/WorldSpawn.cpp)

file(GLOB GAMED3XP_AI_INCLUDES d3xp/ai/*.h)
file(GLOB GAMED3XP_AI_SOURCES d3xp/ai/*.cpp)

file(GLOB GAMED3XP_ANIM_INCLUDES d3xp/anim/*.h)
file(GLOB GAMED3XP_ANIM_SOURCES d3xp/anim/*.cpp)

file(GLOB GAMED3XP_GAMESYS_INCLUDES d3xp/gamesys/*.h)
set(GAMED3XP_GAMESYS_SOURCES
	#d3xp/gamesys/Callbacks.cpp
	d3xp/gamesys/Class.cpp
	#d3xp/gamesys/DebugGraph.cpp
	d3xp/gamesys/Event.cpp
	d3xp/gamesys/SaveGame.cpp
	d3xp/gamesys/SysCmds.cpp
	d3xp/gamesys/SysCvar.cpp
	#d3xp/gamesys/TypeInfo.cpp
	)

file(GLOB GAMED3XP_MENUS_INCLUDES d3xp/menus/*.h)
file(GLOB GAMED3XP_MENUS_SOURCES d3xp/menus/*.cpp)
	
file(GLOB GAMED3XP_PHYSICS_INCLUDES d3xp/physics/*.h)
file(GLOB GAMED3XP_PHYSICS_SOURCES d3xp/physics/*.cpp)

file(GLOB GAMED3XP_SCRIPT_INCLUDES d3xp/script/*.h)
file(GLOB GAMED3XP_SCRIPT_SOURCES d3xp/script/*.cpp)


file(GLOB COMPILER_INCLUDES tools/compilers/*.h)

file(GLOB COMPILER_AAS_INCLUDES tools/compilers/aas/*.h)
file(GLOB COMPILER_AAS_SOURCES tools/compilers/aas/*.cpp)

file(GLOB COMPILER_DMAP_INCLUDES tools/compilers/dmap/*.h)
file(GLOB COMPILER_DMAP_SOURCES tools/compilers/dmap/*.cpp)

file(GLOB COMPILER_RENDERBUMP_INCLUDES tools/compilers/renderbump/*.h)
file(GLOB COMPILER_RENDERBUMP_SOURCES tools/compilers/renderbump/*.cpp)

file(GLOB COMPILER_ROQVQ_INCLUDES tools/compilers/roqvq/*.h)
file(GLOB COMPILER_ROQVQ_SOURCES tools/compilers/roqvq/*.cpp)

set(TOOLS_INCLUDES tools/edit_public.h)

file(GLOB TOOLS_COMAFX_INCLUDES tools/comafx/*.h)
file(GLOB TOOLS_COMAFX_SOURCES tools/comafx/*.cpp)

set(TOOLS_COMMON_INCLUDES
	tools/common/ColorButton.h
	tools/common/DialogHelpers.h
	tools/common/MaskEdit.h
	tools/common/OpenFileDialog.h
	tools/common/PropertyGrid.h
	tools/common/RegistryOptions.h
	#tools/common/RenderBumpFlatDialog.h
	tools/common/RollupPanel.h
	tools/common/SpinButton.h)

set(TOOLS_COMMON_SOURCES
	tools/common/AlphaPopup.cpp
	tools/common/ColorButton.cpp
	tools/common/MaskEdit.cpp
	tools/common/OpenFileDialog.cpp
	tools/common/PropertyGrid.cpp
	tools/common/RegistryOptions.cpp
	#tools/common/RenderBumpFlatDialog.cpp
	tools/common/RollupPanel.cpp
	tools/common/SpinButton.cpp)

file(GLOB TOOLS_COMMON_PROPTREE_INCLUDES tools/common/PropTree/*.h)
file(GLOB TOOLS_COMMON_PROPTREE_SOURCES tools/common/PropTree/*.cpp)

file(GLOB EDITOR_AF_INCLUDES tools/af/*.h)
file(GLOB EDITOR_AF_SOURCES tools/af/*.cpp)

set(EDITOR_DECL_INCLUDES
	tools/decl/DialogDeclBrowser.h
	tools/decl/DialogDeclEditor.h
	tools/decl/DialogDeclNew.h)
	
set(EDITOR_DECL_SOURCES
	tools/decl/DialogDeclBrowser.cpp
	tools/decl/DialogDeclEditor.cpp
	tools/decl/DialogDeclNew.cpp)
	
file(GLOB EDITOR_DEBUGGER_INCLUDES tools/debugger/*.h)
file(GLOB EDITOR_DEBUGGER_SOURCES tools/debugger/*.cpp)

file(GLOB EDITOR_GUIED_INCLUDES tools/guied/*.h)
set(EDITOR_GUIED_SOURCES
	tools/guied/GEApp.cpp
	tools/guied/GECheckInDlg.cpp
	tools/guied/GEDeleteModifier.cpp
	tools/guied/GEHideModifier.cpp
	tools/guied/GEInsertModifier.cpp
	tools/guied/GEItemPropsDlg.cpp
	tools/guied/GEItemScriptsDlg.cpp
	tools/guied/GEKeyValueModifier.cpp
	tools/guied/GEModifier.cpp
	tools/guied/GEModifierGroup.cpp
	tools/guied/GEModifierStack.cpp
	tools/guied/GEMoveModifier.cpp
	tools/guied/GENavigator.cpp
	tools/guied/GEOptions.cpp
	tools/guied/GEOptionsDlg.cpp
	tools/guied/GEProperties.cpp
	tools/guied/GEPropertyPage.cpp
	tools/guied/GESelectionMgr.cpp
	tools/guied/GESizeModifier.cpp
	tools/guied/GEStateModifier.cpp
	tools/guied/GEStatusBar.cpp
	tools/guied/GETransformer.cpp
	tools/guied/GEViewer.cpp
	tools/guied/GEWindowWrapper.cpp
	#tools/guied/GEWindowWrapper_stub.cpp
	tools/guied/GEWorkspace.cpp
	tools/guied/GEWorkspaceFile.cpp
	tools/guied/GEZOrderModifier.cpp
	tools/guied/guied.cpp)

file(GLOB EDITOR_MATERIALEDITOR_INCLUDES tools/materialeditor/*.h)
file(GLOB EDITOR_MATERIALEDITOR_SOURCES tools/materialeditor/*.cpp)

file(GLOB EDITOR_PARTICLE_INCLUDES tools/particle/*.h)
file(GLOB EDITOR_PARTICLE_SOURCES tools/particle/*.cpp)

file(GLOB EDITOR_PDA_INCLUDES tools/pda/*.h)
file(GLOB EDITOR_PDA_SOURCES tools/pda/*.cpp)

file(GLOB EDITOR_RADIANT_INCLUDES tools/radiant/*.h)
file(GLOB EDITOR_RADIANT_SOURCES tools/radiant/*.cpp)

file(GLOB EDITOR_SCRIPT_INCLUDES tools/script/*.h)
file(GLOB EDITOR_SCRIPT_SOURCES tools/script/*.cpp)

file(GLOB EDITOR_SOUND_INCLUDES tools/sound/*.h)
file(GLOB EDITOR_SOUND_SOURCES tools/sound/*.cpp)

file(GLOB DOOMCLASSIC_INCLUDES ../doomclassic/doom/*.h)
file(GLOB DOOMCLASSIC_SOURCES ../doomclassic/doom/*.cpp)

set(DOOMCLASSIC_OPENAL_INCLUDES 
	../doomclassic/doom/s_efx.h
	../doomclassic/doom/i_sound_openal.h)
set(DOOMCLASSIC_OPENAL_SOURCES 
	../doomclassic/doom/i_sound_openal.cpp
	../doomclassic/doom/s_efx.cpp)
set(DOOMCLASSIC_XAUDIO2_SOURCES 
	../doomclassic/doom/i_sound_win32.cpp)
set(DOOMCLASSIC_XAUDIO2_INCLUDES 
	../doomclassic/doom/i_sound_win32.h)

list(REMOVE_ITEM DOOMCLASSIC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../doomclassic/doom/i_sound_stub.cpp)
list(REMOVE_ITEM DOOMCLASSIC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../doomclassic/doom/i_sound_openal.cpp)
list(REMOVE_ITEM DOOMCLASSIC_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../doomclassic/doom/i_sound_openal.h)
list(REMOVE_ITEM DOOMCLASSIC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../doomclassic/doom/s_efx.cpp)
list(REMOVE_ITEM DOOMCLASSIC_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../doomclassic/doom/s_efx.h)
list(REMOVE_ITEM DOOMCLASSIC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../doomclassic/doom/i_sound_win32.cpp)
list(REMOVE_ITEM DOOMCLASSIC_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../doomclassic/doom/i_sound_win32.h)
if(NOT MSVC)
list(REMOVE_ITEM DOOMCLASSIC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../doomclassic/doom/i_net_win32.cpp)
elseif(MSVC)
list(REMOVE_ITEM DOOMCLASSIC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../doomclassic/doom/i_net_unix.cpp)
endif()

file(GLOB TIMIDITY_INCLUDES libs/timidity/*.h)
file(GLOB TIMIDITY_SOURCES libs/timidity/*.cpp)

file(GLOB_RECURSE WIN32_INCLUDES sys/win32/*.h)
#file(GLOB_RECURSE WIN32_SOURCES sys/win32/*.cpp)

set(WIN32_SOURCES
	sys/win32/win_input.cpp
	sys/win32/win_main.cpp
	sys/win32/win_shared.cpp
	sys/win32/win_snd.cpp
	sys/win32/win_syscon.cpp
	sys/win32/win_taskkeyhook.cpp
	sys/win32/win_wndproc.cpp)

if (USE_DXGI)
	list(APPEND WIN32_SOURCES sys/win32/win_glDXimp.cpp)
else()
	list(APPEND WIN32_SOURCES sys/win32/win_glimp.cpp)
endif()
	
if(MSVC)
	list(APPEND WIN32_SOURCES sys/win32/win_cpu.cpp)
endif()

set(WIN32_RESOURCES
	sys/win32/rc/res/doom.ico
	)



#if(USE_MFC_TOOLS)
	list(APPEND WIN32_RESOURCES sys/win32/rc/doom.rc)
#else()
#	list(APPEND WIN32_RESOURCES sys/win32/rc/doom_nomfc.rc)
#endif()


file(GLOB POSIX_INCLUDES sys/posix/*.h)
file(GLOB POSIX_SOURCES sys/posix/*.cpp)

file(GLOB COMMON_INCLUDES sys/common/*.h)
file(GLOB COMMON_SOURCES sys/common/*.cpp)

file(GLOB SDL_INCLUDES sys/sdl/*.h)
file(GLOB SDL_SOURCES sys/sdl/*.cpp)
file(GLOB SDL_RESOURCES sys/sdl/res/*.*)


source_group("AAS" FILES ${AAS_INCLUDES})
source_group("AAS" FILES ${AAS_SOURCES})

source_group("CM" FILES ${CM_INCLUDES})
source_group("CM" FILES ${CM_SOURCES})

source_group("Framework" FILES ${FRAMEWORK_INCLUDES})
source_group("Framework" FILES ${FRAMEWORK_SOURCES})

source_group("Framework\\async" FILES ${FRAMEWORK_ASYNC_INCLUDES})
source_group("Framework\\async" FILES ${FRAMEWORK_ASYNC_SOURCES})

source_group("Renderer" FILES ${RENDERER_INCLUDES})
source_group("Renderer" FILES ${RENDERER_SOURCES})

source_group("Renderer\\Color" FILES ${RENDERER_COLOR_INCLUDES})
source_group("Renderer\\Color" FILES ${RENDERER_COLOR_SOURCES})

source_group("Renderer\\DXT" FILES ${RENDERER_DXT_INCLUDES})
source_group("Renderer\\DXT" FILES ${RENDERER_DXT_SOURCES})

source_group("Renderer\\Jobs" FILES ${RENDERER_JOBS_INCLUDES})
source_group("Renderer\\Jobs" FILES ${RENDERER_JOBS_SOURCES})

source_group("Renderer\\Jobs\\DynamicShadowVolume" FILES ${RENDERER_JOBS_DYNAMICSHADOWVOLUME_INCLUDES})
source_group("Renderer\\Jobs\\DynamicShadowVolume" FILES ${RENDERER_JOBS_DYNAMICSHADOWVOLUME_SOURCES})

source_group("Renderer\\Jobs\\PrelightShadowVolume" FILES ${RENDERER_JOBS_PRELIGHTSHADOWVOLUME_INCLUDES})
source_group("Renderer\\Jobs\\PrelightShadowVolume" FILES ${RENDERER_JOBS_PRELIGHTSHADOWVOLUME_SOURCES})

source_group("Renderer\\Jobs\\StaticShadowVolume" FILES ${RENDERER_JOBS_STATICSHADOWVOLUME_INCLUDES})
source_group("Renderer\\Jobs\\StaticShadowVolume" FILES ${RENDERER_JOBS_STATICSHADOWVOLUME_SOURCES})

source_group("Renderer\\OpenGL" FILES ${RENDERER_OPENGL_INCLUDES})
source_group("Renderer\\OpenGL" FILES ${RENDERER_OPENGL_SOURCES})

source_group("Renderer\\Vulkan" FILES ${RENDERER_VULKAN_INCLUDES})
source_group("Renderer\\Vulkan" FILES ${RENDERER_VULKAN_SOURCES})

source_group("Renderer\\SMAA" FILES ${RENDERER_SMAA_INCLUDES})

source_group("irrxml" FILES ${IRRXML_INCLUDES})
source_group("irrxml" FILES ${IRRXML_SOURCES})

source_group("jpeg-6" FILES ${JPEG_INCLUDES})
source_group("jpeg-6" FILES ${JPEG_SOURCES})

source_group("libbinkdec" FILES ${BINKDEC_INCLUDES})
source_group("libbinkdec" FILES ${BINKDEC_SOURCES})

source_group("png" FILES ${PNG_INCLUDES})
source_group("png" FILES ${PNG_SOURCES})

source_group("zlib" FILES ${ZLIB_INCLUDES})
source_group("zlib" FILES ${ZLIB_SOURCES})

source_group("zlib\\minizip" FILES ${MINIZIP_INCLUDES})
source_group("zlib\\minizip" FILES ${MINIZIP_SOURCES})

source_group("glew" FILES ${GLEW_INCLUDES})
source_group("glew" FILES ${GLEW_SOURCES})

source_group("rapidjson" FILES ${RAPIDJSON_INCLUDES})

source_group("freetype" FILES ${FREETYPE_SOURCES})

#source_group("platform" FILES ${OPEN_PLATFORM_INCLUDES})
#source_group("platform" FILES ${OPEN_PLATFORM_SOURCES})

source_group("Sound" FILES ${SOUND_INCLUDES})
source_group("Sound" FILES ${SOUND_SOURCES})

source_group("Sound\\XAudio2" FILES ${XAUDIO2_INCLUDES})
source_group("Sound\\XAudio2" FILES ${XAUDIO2_SOURCES})

source_group("Sound\\OpenAL" FILES ${OPENAL_INCLUDES})
source_group("Sound\\OpenAL" FILES ${OPENAL_SOURCES})

source_group("Sound\\Common" FILES ${SOUND_COMMON_INCLUDES})
source_group("Sound\\Common" FILES ${SOUND_COMMON_SOURCES})

source_group("oggvorbis" FILES ${OGGVORBIS_INCLUDES})
source_group("oggvorbis" FILES ${OGGVORBIS_SOURCES})

source_group("Ui" FILES ${UI_INCLUDES})
source_group("Ui" FILES ${UI_SOURCES})

source_group("SWF" FILES ${SWF_INCLUDES})
source_group("SWF" FILES ${SWF_SOURCES})

source_group("SWF\\Credits" FILES ${CREDIT_INCLUDES})
source_group("SWF\\Credits" FILES ${CREDIT_SOURCES})

source_group("Sys" FILES ${SYS_INCLUDES})
source_group("Sys" FILES ${SYS_SOURCES})

source_group("DOOM Classic" FILES ${DOOMCLASSIC_INCLUDES})
source_group("DOOM Classic" FILES ${DOOMCLASSIC_SOURCES})

source_group("DOOM Classic" FILES ${DOOMCLASSIC_STUBAUDIO_SOURCES})
source_group("DOOM Classic" FILES ${DOOMCLASSIC_OPENAL_INCLUDES})
source_group("DOOM Classic" FILES ${DOOMCLASSIC_OPENAL_SOURCES})
source_group("DOOM Classic" FILES ${DOOMCLASSIC_XAUDIO2_SOURCES})
source_group("DOOM Classic" FILES ${DOOMCLASSIC_XAUDIO2_INCLUDES})

source_group("timidity" FILES ${TIMIDITY_INCLUDES})
source_group("timidity" FILES ${TIMIDITY_SOURCES})

source_group("Game-D3XP" FILES ${GAMED3XP_INCLUDES})
source_group("Game-D3XP" FILES ${GAMED3XP_SOURCES})

source_group("Game-D3XP\\AI" FILES ${GAMED3XP_AI_INCLUDES})
source_group("Game-D3XP\\AI" FILES ${GAMED3XP_AI_SOURCES})

source_group("Game-D3XP\\Anim" FILES ${GAMED3XP_ANIM_INCLUDES})
source_group("Game-D3XP\\Anim" FILES ${GAMED3XP_ANIM_SOURCES})

source_group("Game-D3XP\\GameSys" FILES ${GAMED3XP_GAMESYS_INCLUDES})
source_group("Game-D3XP\\GameSys" FILES ${GAMED3XP_GAMESYS_SOURCES})

source_group("Game-D3XP\\Menus" FILES ${GAMED3XP_MENUS_INCLUDES})
source_group("Game-D3XP\\Menus" FILES ${GAMED3XP_MENUS_SOURCES})

source_group("Game-D3XP\\Physics" FILES ${GAMED3XP_PHYSICS_INCLUDES})
source_group("Game-D3XP\\Physics" FILES ${GAMED3XP_PHYSICS_SOURCES})

source_group("Game-D3XP\\Script" FILES ${GAMED3XP_SCRIPT_INCLUDES})
source_group("Game-D3XP\\Script" FILES ${GAMED3XP_SCRIPT_SOURCES})

source_group("Sys\\Win32" FILES ${WIN32_INCLUDES})
source_group("Sys\\Win32" FILES ${WIN32_SOURCES})
source_group("Sys\\Win32\\Resources" FILES ${WIN32_RESOURCES})

source_group("Sys\\Posix" FILES ${POSIX_INCLUDES})
source_group("Sys\\Posix" FILES ${POSIX_SOURCES})

source_group("Sys\\Common" FILES ${COMMON_INCLUDES})
source_group("Sys\\Common" FILES ${COMMON_SOURCES})

source_group("Sys\\SDL" FILES ${SDL_INCLUDES})
source_group("Sys\\SDL" FILES ${SDL_SOURCES})
source_group("Sys\\SDL\\Resources" FILES ${SDL_RESOURCES})


source_group("Tools\\Compilers" FILES ${COMPILER_INCLUDES})

source_group("Tools\\Compilers\\AAS" FILES ${COMPILER_AAS_INCLUDES})
source_group("Tools\\Compilers\\AAS" FILES ${COMPILER_AAS_SOURCES})

source_group("Tools\\Compilers\\Dmap" FILES ${COMPILER_DMAP_INCLUDES})
source_group("Tools\\Compilers\\Dmap" FILES ${COMPILER_DMAP_SOURCES})

source_group("Tools\\Compilers\\RenderBump" FILES ${COMPILER_RENDERBUMP_INCLUDES})
source_group("Tools\\Compilers\\RenderBump" FILES ${COMPILER_RENDERBUMP_SOURCES})

source_group("Tools\\Compilers\\RoqVq" FILES ${COMPILER_ROQVQ_INCLUDES})
source_group("Tools\\Compilers\\RoqVq" FILES ${COMPILER_ROQVQ_SOURCES})

source_group("Tools" FILES ${TOOLS_INCLUDES})

source_group("Tools\\ComaFX" FILES ${TOOLS_COMAFX_INCLUDES})
source_group("Tools\\ComaFX" FILES ${TOOLS_COMAFX_SOURCES})

source_group("Tools\\Common" FILES ${TOOLS_COMMON_INCLUDES})
source_group("Tools\\Common" FILES ${TOOLS_COMMON_SOURCES})

source_group("Tools\\Common\\PropTree" FILES ${TOOLS_COMMON_PROPTREE_INCLUDES})
source_group("Tools\\Common\\PropTree" FILES ${TOOLS_COMMON_PROPTREE_SOURCES})

source_group("Tools\\AF" FILES ${EDITOR_AF_INCLUDES})
source_group("Tools\\AF" FILES ${EDITOR_AF_SOURCES})

source_group("Tools\\Debugger" FILES ${EDITOR_DEBUGGER_INCLUDES})
source_group("Tools\\Debugger" FILES ${EDITOR_DEBUGGER_SOURCES})

source_group("Tools\\Decl" FILES ${EDITOR_DECL_INCLUDES})
source_group("Tools\\Decl" FILES ${EDITOR_DECL_SOURCES})

source_group("Tools\\GuiEd" FILES ${EDITOR_GUIED_INCLUDES})
source_group("Tools\\GuiEd" FILES ${EDITOR_GUIED_SOURCES})

source_group("Tools\\MaterialEditor" FILES ${EDITOR_MATERIALEDITOR_INCLUDES})
source_group("Tools\\MaterialEditor" FILES ${EDITOR_MATERIALEDITOR_SOURCES})

source_group("Tools\\Particle" FILES ${EDITOR_PARTICLE_INCLUDES})
source_group("Tools\\Particle" FILES ${EDITOR_PARTICLE_SOURCES})

source_group("Tools\\PDA" FILES ${EDITOR_PDA_INCLUDES})
source_group("Tools\\PDA" FILES ${EDITOR_PDA_SOURCES})

source_group("Tools\\Radiant" FILES ${EDITOR_RADIANT_INCLUDES})
source_group("Tools\\Radiant" FILES ${EDITOR_RADIANT_SOURCES})

source_group("Tools\\Script" FILES ${EDITOR_SCRIPT_INCLUDES})
source_group("Tools\\Script" FILES ${EDITOR_SCRIPT_SOURCES})

source_group("Tools\\Sound" FILES ${EDITOR_SOUND_INCLUDES})
source_group("Tools\\Sound" FILES ${EDITOR_SOUND_SOURCES})

set(LIB_INCLUDES
			${IRRXML_INCLUDES}
			${JPEG_INCLUDES}
			${PNG_INCLUDES}
			${ZLIB_INCLUDES}
			${MINIZIP_INCLUDES}
			${BINKDEC_INCLUDES}
			${GLEW_INCLUDES}
			#${OPEN_PLATFORM_INCLUDES}
			${RAPIDJSON_INCLUDES}
			#${FREETYPE_SOURCES}
			${TIMIDITY_INCLUDES}
)

set(LIB_SOURCES
			${IRRXML_SOURCES}
			${JPEG_SOURCES}
			${PNG_SOURCES}
			${ZLIB_SOURCES}
			${MINIZIP_SOURCES}
			${BINKDEC_SOURCES}
			${GLEW_SOURCES}
			#${OPEN_PLATFORM_SOURCES}
			#${FREETYPE_SOURCES}
			${TIMIDITY_SOURCES}
)

set(RBDOOM3_INCLUDES
			${AAS_INCLUDES}
			${CM_INCLUDES}
			${FRAMEWORK_INCLUDES}
			${FRAMEWORK_ASYNC_INCLUDES}
			${RENDERER_INCLUDES}
			${RENDERER_COLOR_INCLUDES}
			${RENDERER_DXT_INCLUDES}
			${RENDERER_JOBS_INCLUDES}
			${RENDERER_JOBS_DYNAMICSHADOWVOLUME_INCLUDES}
			${RENDERER_JOBS_PRELIGHTSHADOWVOLUME_INCLUDES}
			${RENDERER_JOBS_STATICSHADOWVOLUME_INCLUDES}
			
			${RENDERER_SMAA_INCLUDES}
			
			${SOUND_INCLUDES}
			#${OGGVORBIS_INCLUDES}
			${UI_INCLUDES}
			${SWF_INCLUDES}
			${CREDIT_INCLUDES}
			${COMMON_INCLUDES}
			${DOOMCLASSIC_INCLUDES}
			
			
			${COMPILER_INCLUDES}
			${COMPILER_AAS_INCLUDES} ${COMPILER_AAS_SOURCES}
			${COMPILER_DMAP_INCLUDES} ${COMPILER_DMAP_SOURCES}
			#${COMPILER_RENDERBUMP_INCLUDES} ${COMPILER_RENDERBUMP_SOURCES}
			#${COMPILER_ROQVQ_INCLUDES} ${COMPILER_ROQVQ_SOURCES}
			)
			
set(RBDOOM3_SOURCES
			${AAS_SOURCES}
			${CM_SOURCES}
			${FRAMEWORK_SOURCES}
			${FRAMEWORK_ASYNC_SOURCES} 
			${RENDERER_SOURCES}
			${RENDERER_COLOR_SOURCES}
			${RENDERER_DXT_SOURCES}
			${RENDERER_JOBS_SOURCES}
			${RENDERER_JOBS_DYNAMICSHADOWVOLUME_SOURCES}
			${RENDERER_JOBS_PRELIGHTSHADOWVOLUME_SOURCES}
			${RENDERER_JOBS_STATICSHADOWVOLUME_SOURCES}
			
			${SOUND_SOURCES}
			#${OGGVORBIS_INCLUDES} ${OGGVORBIS_SOURCES}
			${UI_SOURCES}
			${SWF_SOURCES}
			${CREDIT_SOURCES}
			${COMMON_SOURCES}
			${DOOMCLASSIC_SOURCES}
			
			${COMPILER_AAS_SOURCES}
			${COMPILER_DMAP_SOURCES}
			#${COMPILER_RENDERBUMP_SOURCES}
			#${COMPILER_ROQVQ_SOURCES}
			)

add_definitions(-DUSE_DOOMCLASSIC)
#GK:Enable Classic Doom multi player
add_definitions(-DID_ENABLE_DOOM_CLASSIC_NETWORKING)
add_definitions(-D__DOOM__
				#-DBUILD_FREETYPE
				#-DFT2_BUILD_LIBRARY
				)
add_definitions (-D__DOOM_DLL__)
				
if(MONOLITH)
	list(APPEND RBDOOM3_INCLUDES
		${GAMED3XP_INCLUDES}
		${GAMED3XP_AI_INCLUDES} 
		${GAMED3XP_ANIM_INCLUDES} 
		${GAMED3XP_GAMESYS_INCLUDES} 
		${GAMED3XP_MENUS_INCLUDES}
		${GAMED3XP_PHYSICS_INCLUDES}
		${GAMED3XP_SCRIPT_INCLUDES})
		
	list(APPEND RBDOOM3_SOURCES
		${GAMED3XP_SOURCES}
		${GAMED3XP_AI_SOURCES}
		${GAMED3XP_ANIM_SOURCES}
		${GAMED3XP_GAMESYS_SOURCES}
		${GAMED3XP_MENUS_SOURCES}
		${GAMED3XP_PHYSICS_SOURCES}
		${GAMED3XP_SCRIPT_SOURCES})
		add_definitions(-D__MONOLITH__)
else()
list(APPEND RBDOOM3_INCLUDES
		${GAMED3XP_MENUS_INCLUDES})
		
	list(APPEND RBDOOM3_SOURCES
		${GAMED3XP_MENUS_SOURCES})
	add_definitions(-D__DOOM_DLL__)
endif()

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			idlib
			#libs/curl/include
			#libs/openal/include
			#libs/freetype/include
            )
			
#link_directories(
#			${CMAKE_CURRENT_SOURCE_DIR}/libs/curl/lib
#			${CMAKE_CURRENT_SOURCE_DIR}/libs/openal/lib)

if(MSVC)

	# if(MSVC_VERSION EQUAL 1700)
		# set(DirectX_LIBRARIES
			# dinput8
			# dsound
			# dxguid
			# #dxerr
			# Xinput
			# xaudio2.lib
			# )
		
		# if(NOT OPENAL)
			# list(APPEND DirectX_LIBRARIES xaudio2.lib)
		# endif()
		
	# else()
	#GK: DXSDK 2010 can STILL BEEN INSTALLED on Windows 10 and been used even by the LATEST VERSION OF VS
	#if(WINRT AND OPENAL OR USE_XAUDIO2_PACKAGE)
	#	set(DirectX_LIBRARIES
	#		dinput8
			#dsound
	#		dxguid
			#dxerr
	#		Xinput
	#		)
	#elseif(WINRT AND NOT OPENAL)
	if (USE_SYSTEM_DX)
		set(DirectX_LIBRARIES
			dinput8
			#dsound
			dxguid
			#dxerr
			Xinput9_1_0 #GK: Win7 backwards compatibility
			)
		if(USE_XAUDIO2)
			list(APPEND DirectX_LIBRARIES xaudio2.lib)
		endif()
		if (USE_DXGI)
			list(APPEND DirectX_LIBRARIES dxgi d3d11)
		endif()
	else()
		find_package(DirectX REQUIRED)
		include_directories(${DirectX_INCLUDE_DIR})
	endif()
	
	if(USE_VULKAN)
		list(APPEND RBDOOM3_INCLUDES ${RENDERER_VULKAN_INCLUDES})
		list(APPEND RBDOOM3_SOURCES ${RENDERER_VULKAN_SOURCES})
		
		link_directories($ENV{VULKAN_SDK}/Lib)
		set(Vulkan_RELEASE_LIBRARIES
			${Vulkan_LIBRARY}
			optimized glslang
			optimized SPIRV
			optimized MachineIndependent
			optimized GenericCodeGen
			optimized OGLCompiler
			optimized OSDependent
			optimized SPIRV-Tools-opt
			optimized SPIRV-Tools
			optimized SPVRemapper
			)
		set(Vulkan_DEBUG_LIBRARIES
			${Vulkan_LIBRARY}
			debug glslangd
			debug SPIRVd
			debug MachineIndependentd
			debug GenericCodeGend
			debug OGLCompilerd
			debug OSDependentd
			debug SPIRV-Tools-optd
			debug SPIRV-Toolsd
			debug SPVRemapperd
			)
			
		if(ENABLE_GLSLANG_BINARIES)
			list(APPEND Vulkan_RELEASE_LIBRARIES optimized glslang-default-resource-limits)
			list(APPEND Vulkan_DEBUG_LIBRARIES debug glslang-default-resource-limitsd)
		endif()
		
	else()
		list(APPEND RBDOOM3_INCLUDES ${RENDERER_OPENGL_INCLUDES})
		list(APPEND RBDOOM3_SOURCES ${RENDERER_OPENGL_SOURCES})
		
		set(OpenGL_LIBRARIES
			opengl32
			glu32)
	endif()
	
	list(APPEND RBDOOM3_INCLUDES 
			${SOUND_COMMON_INCLUDES})
	list(APPEND RBDOOM3_SOURCES
			${SOUND_COMMON_SOURCES})
	
	#if(OPENAL)
		add_definitions(-DUSE_OPENAL)
	
		find_package(OpenAL)
		if (OPENAL_FOUND)
			include_directories(${OPENAL_INCLUDE_DIR})
			if (MSVC AND ALL_STATIC)
				set(OpenAL_LIBRARY
				optimized "vcpkg_installed/${CPU_ARCH}-windows-static-md/lib/OpenAL32"
				debug "vcpkg_installed/${CPU_ARCH}-windows-static-md/debug/lib/OpenAL32")
			else()
				set(OpenAL_LIBRARY ${OPENAL_LIBRARY})
			endif()
		else (OPENAL_FOUND)
			include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/openal-soft/include)
		
			link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/openal-soft/libs/${OS_CPU})

			set(OpenAL_LIBRARY
				OpenAL32)
			install(FILES libs/openal-soft/libs/${OS_CPU}/OpenAL32.dll DESTINATION .)
		endif (OPENAL_FOUND)
		
		
		list(APPEND RBDOOM3_INCLUDES 
			${OPENAL_INCLUDES}
			${DOOMCLASSIC_OPENAL_INCLUDES})
		list(APPEND RBDOOM3_SOURCES
			${OPENAL_SOURCES}
			${DOOMCLASSIC_OPENAL_SOURCES})
			
		if (USE_XAUDIO2_PACKAGE)
			find_package(xaudio2redist)
			if (ALL_STATIC)
				include_directories("${CMAKE_BINARY_DIR}/vcpkg_installed/${CPU_ARCH}-windows-static-md/include/xaudio2redist")
			else()
				include_directories("${CMAKE_BINARY_DIR}/vcpkg_installed/${CPU_ARCH}-windows/include/xaudio2redist")
			endif()
		#set(XAUDIO2_REDIST Microsoft.XAudio2.Redist)
		#find_program(NUGET_EXE NAMES nuget)
		#exec_program(${NUGET_EXE}
		#ARGS install ${XAUDIO2_REDIST} -ExcludeVersion -OutputDirectory ${CMAKE_BINARY_DIR}/packages)
		#include_directories(${CMAKE_BINARY_DIR}/packages/${XAUDIO2_REDIST}/build/native/include)
		#target_link_libraries(DoomBFA ${CMAKE_BINARY_DIR}/packages/${XAUDIO2_REDIST}/build/native/Microsoft.XAudio2.Redist.targets)
		#if (CMAKE_CL_64)
		#	set(CPU_ARCH x64)
		#else()
		#	set(CPU_ARCH x86)
		#endif()
		#set (XAUDIO2_REDIST_DEBUG_LIBRARIES 
		#		debug ${CMAKE_BINARY_DIR}/packages/${XAUDIO2_REDIST}/build/native/debug/lib/${CPU_ARCH}/xaudio2_9redist.lib
		#		debug ${CMAKE_BINARY_DIR}/packages/${XAUDIO2_REDIST}/build/native/debug/lib/${CPU_ARCH}/xapobaseredist.lib
		#		debug ${CMAKE_BINARY_DIR}/packages/${XAUDIO2_REDIST}/build/native/debug/lib/${CPU_ARCH}/xapobaseredist_md.lib
		#)
		#set (XAUDIO2_REDIST_RELEASE_LIBRARIES 
		#		optimized ${CMAKE_BINARY_DIR}/packages/${XAUDIO2_REDIST}/build/native/release/lib/${CPU_ARCH}/xaudio2_9redist.lib
		#		optimized ${CMAKE_BINARY_DIR}/packages/${XAUDIO2_REDIST}/build/native/release/lib/${CPU_ARCH}/xapobaseredist.lib
		#		optimized ${CMAKE_BINARY_DIR}/packages/${XAUDIO2_REDIST}/build/native/release/lib/${CPU_ARCH}/xapobaseredist_md.lib
		#)
		#target_link_libraries(DoomBFA ${XAUDIO2_REDIST_DEBUG_LIBRARIES} ${XAUDIO2_REDIST_RELEASE_LIBRARIES})
	endif()
		
	#else()
	if(USE_XAUDIO2)
		list(APPEND RBDOOM3_INCLUDES 
			${XAUDIO2_INCLUDES}
			${DOOMCLASSIC_XAUDIO2_INCLUDES})
		list(APPEND RBDOOM3_SOURCES
			${XAUDIO2_SOURCES}
			${DOOMCLASSIC_XAUDIO2_SOURCES})
	endif()
	
	if(FFMPEG)
		add_definitions(-DUSE_FFMPEG)
	
		if (USE_PACKAGED_FFMPEG)
			#find_package(ffmpeg REQUIRED)
			set(FFMPEG_FOUND TRUE)
		else(USE_PACKAGED_FFMPEG)
			set(FFMPEG_FOUND FALSE)
		endif(USE_PACKAGED_FFMPEG)

		if(FFMPEG_FOUND)
			#include_directories(${FFMPEG_INCLUDE_DIRS})
			#link_directories(${FFMPEG_LIBRARIES_DIRS})
			include_directories(
				"vcpkg_installed/${CPU_ARCH}-windows-static-md/include/libavcodec"
				"vcpkg_installed/${CPU_ARCH}-windows-static-md/include/libavdevice"
				"vcpkg_installed/${CPU_ARCH}-windows-static-md/include/libavfilter"
				"vcpkg_installed/${CPU_ARCH}-windows-static-md/include/libavformat"
				"vcpkg_installed/${CPU_ARCH}-windows-static-md/include/libavutil"
				"vcpkg_installed/${CPU_ARCH}-windows-static-md/include/libswresample"
				"vcpkg_installed/${CPU_ARCH}-windows-static-md/include/libswscale" 
			)
			if (MSVC AND ALL_STATIC)
				set(FFmpeg_LIBRARIES
				psapi.lib
				uuid.lib
				oleaut32.lib
				shlwapi.lib
				gdi32.lib
				vfw32.lib
				secur32.lib
				ws2_32.lib
				mfplat.lib
				mfuuid.lib
				strmiids.lib
				ole32.lib
				user32.lib
				bcrypt.lib
				optimized "vcpkg_installed/${CPU_ARCH}-windows-static-md/lib/avcodec"
				optimized "vcpkg_installed/${CPU_ARCH}-windows-static-md/lib/avdevice"
				optimized "vcpkg_installed/${CPU_ARCH}-windows-static-md/lib/avfilter"
				optimized "vcpkg_installed/${CPU_ARCH}-windows-static-md/lib/avformat"
				optimized "vcpkg_installed/${CPU_ARCH}-windows-static-md/lib/avutil"
				optimized "vcpkg_installed/${CPU_ARCH}-windows-static-md/lib/swresample"
				optimized "vcpkg_installed/${CPU_ARCH}-windows-static-md/lib/swscale" 
				debug "vcpkg_installed/${CPU_ARCH}-windows-static-md/debug/lib/avcodec"
				debug "vcpkg_installed/${CPU_ARCH}-windows-static-md/debug/lib/avdevice"
				debug "vcpkg_installed/${CPU_ARCH}-windows-static-md/debug/lib/avfilter"
				debug "vcpkg_installed/${CPU_ARCH}-windows-static-md/debug/lib/avformat"
				debug "vcpkg_installed/${CPU_ARCH}-windows-static-md/debug/lib/avutil"
				debug "vcpkg_installed/${CPU_ARCH}-windows-static-md/debug/lib/swresample"
				debug "vcpkg_installed/${CPU_ARCH}-windows-static-md/debug/lib/swscale")
			else()
				set(FFmpeg_LIBRARIES ${FFMPEG_LIBRARIES})
			endif()
		else(FFMPEG_FOUND)
			if(CMAKE_CL_64)
				set(FFMPEG_DIR ffmpeg-win64)
			else()
				set(FFMPEG_DIR ffmpeg-win32)
			endif()
		
			include_directories(libs/${FFMPEG_DIR}/include)
			include_directories(libs/${FFMPEG_DIR}/include/libswscale)
			include_directories(libs/${FFMPEG_DIR}/include/libavformat)
			include_directories(libs/${FFMPEG_DIR}/include/libavdevice)
			include_directories(libs/${FFMPEG_DIR}/include/libavcodec)
			include_directories(libs/${FFMPEG_DIR}/include/libswresample)
			
			link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/${FFMPEG_DIR}/lib)
		
			set(FFmpeg_LIBRARIES
					avcodec
					avformat
					avutil
					swscale
					swresample)
			
		endif(FFMPEG_FOUND)
		
	endif()
	
	if(USE_MFC_TOOLS)
		
		include_directories(libs/atlmfc/include)
		if(CMAKE_CL_64)
			link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/atlmfc/lib/amd64)
		else()
			link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/atlmfc/lib)
		endif()
		
		add_definitions(-DUSE_MFC_TOOLS)
		
		list(APPEND RBDOOM3_SOURCES
			${TOOLS_INCLUDES}
			${TOOLS_COMAFX_INCLUDES} ${TOOLS_COMAFX_SOURCES}
			${TOOLS_COMMON_INCLUDES} ${TOOLS_COMMON_SOURCES}
			${TOOLS_COMMON_PROPTREE_INCLUDES} ${TOOLS_COMMON_PROPTREE_SOURCES}
			
			${EDITOR_AF_INCLUDES} ${EDITOR_AF_SOURCES}
			${EDITOR_DEBUGGER_INCLUDES} ${EDITOR_DEBUGGER_SOURCES}
			${EDITOR_DECL_INCLUDES} ${EDITOR_DECL_SOURCES}
			${EDITOR_GUIED_INCLUDES} ${EDITOR_GUIED_SOURCES}
			${EDITOR_MATERIALEDITOR_INCLUDES} ${EDITOR_MATERIALEDITOR_SOURCES}
			${EDITOR_PARTICLE_INCLUDES} ${EDITOR_PARTICLE_SOURCES}
			${EDITOR_PDA_INCLUDES} ${EDITOR_PDA_SOURCES}
			${EDITOR_RADIANT_INCLUDES} ${EDITOR_RADIANT_SOURCES}
			${EDITOR_SCRIPT_INCLUDES} ${EDITOR_SCRIPT_SOURCES}
			${EDITOR_SOUND_INCLUDES} ${EDITOR_SOUND_SOURCES})
	endif()

	list(APPEND RBDOOM3_INCLUDES
		${SYS_INCLUDES} 
		${WIN32_INCLUDES})
		
	list(APPEND RBDOOM3_SOURCES
		${SYS_SOURCES}
		${WIN32_SOURCES})

	list(REMOVE_DUPLICATES RBDOOM3_SOURCES)
	
	if(USE_PRECOMPILED_HEADERS) 
		set(RBDOOM3_PRECOMPILED_SOURCES ${RBDOOM3_SOURCES})
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${TIMIDITY_SOURCES} ${JPEG_SOURCES} ${PNG_SOURCES} ${ZLIB_SOURCES} ${GLEW_SOURCES} ${BINKDEC_SOURCES})
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/libs/zlib/minizip/ioapi.c)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTDecoder.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder_SSE2.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/dynamicshadowvolume/DynamicShadowVolume.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/prelightshadowvolume/PreLightShadowVolume.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/staticshadowvolume/StaticShadowVolume.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/ShadowShared.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/RenderLog.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/framework/precompiled.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/Vulkan/vma.cpp)
	
		set_source_files_properties(
		${RBDOOM3_PRECOMPILED_SOURCES}
		PROPERTIES
		COMPILE_FLAGS "/Yuprecompiled.h"
		OBJECT_DEPENDS "precompiled.pch"

		)
		
		set_source_files_properties(framework/precompiled.cpp
		PROPERTIES
		COMPILE_FLAGS "/Ycprecompiled.h"
		OBJECT_OUTPUTS "precompiled.pch"
		)
	endif()
	
	set_source_files_properties( ${SHADERS} PROPERTIES VS_TOOL_OVERRIDE "None" )
	
	list(APPEND RBDOOM3_SOURCES ${WIN32_RESOURCES})
	add_library(external ${LIB_INCLUDES} ${LIB_SOURCES})
	target_compile_options(external PRIVATE /WX- /W3 /sdl-)
	add_custom_target(shaders SOURCES ${SHADERS})
	add_executable(DoomBFA WIN32 ${RBDOOM3_INCLUDES} ${RBDOOM3_SOURCES})
	#GK: Allow to compile and run the game from VS
	set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT DoomBFA)
	set_target_properties(DoomBFA PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "C:/Program Files (x86)/Steam/steamapps/common/DOOM 3 BFG Edition")
	#GK: add +set r_fullscreen "0" command line (VS DEBUGGER and Fullscreen not a good mix)
	set_target_properties(DoomBFA PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "+set r_fullscreen 0")
	
	if (NOT OPENAL_FOUND)
		add_custom_command(TARGET DoomBFA
					   POST_BUILD
					   COMMAND ${CMAKE_COMMAND} -E copy_if_different
					   ${CMAKE_SOURCE_DIR}/libs/openal-soft/bin/${OS_CPU}/soft_oal.dll 
					   $<TARGET_FILE_DIR:DoomBFA>/OpenAL32.dll
					   COMMENT "Copy OpenAL dll")
	endif(NOT OPENAL_FOUND)
		
	add_dependencies(DoomBFA idlib external)
	target_link_libraries(DoomBFA
		idlib
		external
		${DirectX_LIBRARIES}
		dbghelp
		#eaxguid
		iphlpapi
		winmm
		wsock32.lib
		Ws2_32.lib
		${Vulkan_RELEASE_LIBRARIES}
		${Vulkan_DEBUG_LIBRARIES}
		${OpenGL_LIBRARIES}
		${OpenAL_LIBRARY}
		${FFmpeg_LIBRARIES}
        ${ZLIB_LIBRARY}
        ${PNG_LIBRARY}
        ${JPEG_LIBRARY}
        ${GLEW_LIBRARY}
		${CMAKE_DL_LIBS}
		)
	

		
	#CMAKE_BINARY_DIR
	install(TARGETS DoomBFA
			RUNTIME DESTINATION .)
else()

	list(APPEND RBDOOM3_SOURCES
		${SYS_INCLUDES} ${SYS_SOURCES})
	
	find_package(OpenGL REQUIRED)
	include_directories(${OPENGL_INCLUDE_DIRS})

	if(UNIX)
		if(FFMPEG)
			find_package(FFMPEG REQUIRED)
			add_definitions(-DUSE_FFMPEG)
			
			include_directories(${FFMPEG_INCLUDE_DIR})
			link_directories(${FFMPEG_LIBRARIES_DIRS})
		endif()

		if(SDL2)
			find_package(SDL2 REQUIRED)
			include_directories(${SDL2_INCLUDE_DIR})
			set(SDLx_LIBRARY ${SDL2_LIBRARY})
		else()
			find_package(SDL REQUIRED)
			include_directories(${SDL_INCLUDE_DIR})
			set(SDLx_LIBRARY ${SDL_LIBRARY})
		endif()

		if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
			list(REMOVE_ITEM POSIX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sys/posix/platform_linux.cpp)
		else()
			list(REMOVE_ITEM POSIX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sys/posix/platform_osx.cpp)
		endif()

		list(APPEND RBDOOM3_SOURCES
			${POSIX_INCLUDES} ${POSIX_SOURCES}
			${SDL_INCLUDES} ${SDL_SOURCES} ${SDL_RESOURCES})
			
		list(APPEND RBDOOM3_INCLUDES 
			${SOUND_COMMON_INCLUDES})
		list(APPEND RBDOOM3_SOURCES
			${SOUND_COMMON_SOURCES})
			
		#if(OPENAL)
			find_package(OpenAL REQUIRED)
			add_definitions(-DUSE_OPENAL)

			include_directories(${OPENAL_INCLUDE_DIR})
			
			#if(CMAKE_CL_64)
			#	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/openal-soft/lib/win64)
			#else()
			#	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/openal-soft/lib/win32)
			#endif()
			
			list(APPEND RBDOOM3_INCLUDES 
				${OPENAL_INCLUDES}
				${DOOMCLASSIC_OPENAL_INCLUDES})
			list(APPEND RBDOOM3_SOURCES
				${OPENAL_SOURCES}
				${DOOMCLASSIC_OPENAL_SOURCES})
		#else()
		#	list(APPEND RBDOOM3_INCLUDES ${STUBAUDIO_INCLUDES})
		#	list(APPEND RBDOOM3_SOURCES
		#		${STUBAUDIO_SOURCES}
		#		${DOOMCLASSIC_STUBAUDIO_SOURCES})
		#endif()
		
		if(USE_VULKAN)
            list(APPEND RBDOOM3_INCLUDES ${RENDERER_VULKAN_INCLUDES})
            list(APPEND RBDOOM3_SOURCES ${RENDERER_VULKAN_SOURCES})
            
            set(Vulkan_RELEASE_LIBRARIES
				${Vulkan_LIBRARY}
				optimized glslang
				optimized SPIRV
				optimized MachineIndependent
				optimized GenericCodeGen
				optimized OGLCompiler
				optimized OSDependent
				optimized SPIRV-Tools-opt
				optimized SPIRV-Tools
				optimized SPVRemapper
			)
			set(Vulkan_DEBUG_LIBRARIES
				${Vulkan_LIBRARY}
				debug glslangd
				debug SPIRVd
				debug MachineIndependentd
				debug GenericCodeGend
				debug OGLCompilerd
				debug OSDependentd
				debug SPIRV-Tools-optd
				debug SPIRV-Toolsd
				debug SPVRemapperd
			)
			
			if(ENABLE_GLSLANG_BINARIES)
				list(APPEND Vulkan_RELEASE_LIBRARIES optimized glslang-default-resource-limits)
				list(APPEND Vulkan_DEBUG_LIBRARIES debug glslang-default-resource-limitsd)
			endif()
            set(Vulkan_LIBRARIES ${Vulkan_RELEASE_LIBRARIES} ${Vulkan_DEBUG_LIBRARIES})
        else()
            list(APPEND RBDOOM3_INCLUDES ${RENDERER_OPENGL_INCLUDES})
            list(APPEND RBDOOM3_SOURCES ${RENDERER_OPENGL_SOURCES})
        endif()
			
	endif()

	list(REMOVE_DUPLICATES RBDOOM3_SOURCES)
	
	if(USE_PRECOMPILED_HEADERS) 	
		set(RBDOOM3_PRECOMPILED_SOURCES ${RBDOOM3_SOURCES})
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${TIMIDITY_SOURCES} ${JPEG_SOURCES} ${PNG_SOURCES} ${ZLIB_SOURCES} ${GLEW_SOURCES} ${BINKDEC_SOURCES})
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/libs/zlib/minizip/ioapi.c)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTDecoder.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder_SSE2.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/dynamicshadowvolume/DynamicShadowVolume.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/prelightshadowvolume/PreLightShadowVolume.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/staticshadowvolume/StaticShadowVolume.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/ShadowShared.cpp)
		list(REMOVE_ITEM RBDOOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/RenderLog.cpp)

		foreach( src_file ${RBDOOM3_PRECOMPILED_SOURCES} )
			#message(STATUS "-include precompiled.h for ${src_file}")
			set_source_files_properties(
				${src_file}
				PROPERTIES
				COMPILE_FLAGS "-include ${CMAKE_CURRENT_SOURCE_DIR}/idlib/precompiled.h"
				)
		endforeach()

		# precompiled magic for GCC/clang, adapted from https://gist.github.com/573926
		STRING(TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name)
		SET(_compiler_FLAGS ${${_flags_var_name}})
		GET_DIRECTORY_PROPERTY(_directory_flags INCLUDE_DIRECTORIES)
		FOREACH(item ${_directory_flags})
			LIST(APPEND _compiler_FLAGS " -I${item}")
		ENDFOREACH(item)
	endif()
	#GK: fix for mingw and recent versions of cmake
		cmake_policy(SET CMP0059 NEW)
	GET_DIRECTORY_PROPERTY(_directory_flags DEFINITIONS)
	LIST(APPEND _compiler_FLAGS ${_directory_flags})
	SEPARATE_ARGUMENTS(_compiler_FLAGS)
	
	if(USE_PRECOMPILED_HEADERS)
		# we need to recreate the precompiled header for RBDoom3BFG 
		# (i.e. can't use the one created for idlib before)
		# because some definitions (e.g. -D__IDLIB__ -D__DOOM_DLL__) differ
		add_custom_target(precomp_header_doombfa ALL
				COMMAND ${CMAKE_CXX_COMPILER} ${_compiler_FLAGS} -x c++-header idlib/precompiled.h -o idlib/precompiled.h.gch
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				COMMENT "Creating idlib/precompiled.h.gch for DoomBFA"
				)
	endif()
	
	if(WIN32)
		set(remove_command "del")
	else()
		set(remove_command "rm")
	endif()
	add_library(external ${LIB_INCLUDES} ${LIB_SOURCES})
	target_compile_options(external PRIVATE "-Wno-error")
	add_custom_target(shaders SOURCES ${SHADERS})
	if (USE_PRECOMPILED_HEADERS)
		# it's ugly enough that the precompiled header binary needs to be in the 
		# source directory (instead of the build directory), so let's at least
		# delete it after build.
		add_custom_target(rm_precomp_header ALL
				COMMAND ${remove_command} "idlib/precompiled.h.gch"
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				COMMENT "remove idlib/precompiled.h.gch"
				)
				
		# make sure this is run after creating idlib
		add_dependencies(precomp_header_doombfa idlib external)
	endif()
	
	
	add_executable(DoomBFA WIN32 ${RBDOOM3_SOURCES} ${RBDOOM3_INCLUDES})

	if (USE_PRECOMPILED_HEADERS)
		# make sure precompiled header is created before executable is compiled
		add_dependencies(DoomBFA precomp_header_doombfa)
		
		# make sure precompiled header is deleted after executable is compiled
		add_dependencies(rm_precomp_header DoomBFA)
	endif()	

	if(NOT WIN32)
		if(NOT "${CMAKE_SYSTEM}" MATCHES "Darwin")
			set(RT_LIBRARY rt)
		endif()

		target_link_libraries(DoomBFA
			idlib
			external
			${OPENGL_LIBRARIES}
			pthread
			${RT_LIBRARY}
			${SDLx_LIBRARY}
			${OPENAL_LIBRARY}
			${FFMPEG_LIBRARIES}
            ${ZLIB_LIBRARY}
            ${PNG_LIBRARY}
            ${JPEG_LIBRARY}
            ${GLEW_LIBRARY}
	    ${CMAKE_DL_LIBS}
			)
	endif()
	
endif()

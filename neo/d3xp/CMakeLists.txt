
add_definitions( -DGAME_DLL -DGAME_VERSION="CustomDOOM-1")

if(OPENAL)
		add_definitions(-DUSE_OPENAL)
	
		include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libs/openal-soft/include)
		
		if(CMAKE_CL_64)
			link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libs/openal-soft/libs/win64)
		else()
			link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libs/openal-soft/libs/win32)
		endif()
		
		list(APPEND RBDOOM3_INCLUDES 
			${OPENAL_INCLUDES}
			${DOOMCLASSIC_OPENAL_INCLUDES})
		list(APPEND RBDOOM3_SOURCES
			${OPENAL_SOURCES}
			${DOOMCLASSIC_OPENAL_SOURCES})
		
		set(OpenAL_LIBRARIES
			OpenAL32)
			
		if(CMAKE_CL_64)
			install(FILES ../libs/openal-soft/libs/win64/OpenAL32.dll DESTINATION .)
		else()
			install(FILES ../libs/openal-soft/libs/win32/OpenAL32.dll DESTINATION .)
			install(FILES ../libs/openal-soft/libs/win32/OpenAL32.pdb DESTINATION .)
		endif()
endif()

set(ID_GAME_INCLUDES
	Achievements.h
	Actor.h
	AF.h
	AFEntity.h
	AimAssist.h
	BrittleFracture.h
	Camera.h
	Classic.h
	#d3xp/EndLevel.h
	Entity.h
	Fx.h
	Game.h
	GameEdit.h
	Game_Internal.h
	Game_local.h
	Grabber.h
	IK.h
	Item.h
	Leaderboards.h
	Light.h
	Misc.h
	Moveable.h
	Mover.h
	MultiplayerGame.h
	Player.h
	PlayerIcon.h
	PlayerView.h
	PredictedValue.h
	Projectile.h
	Pvs.h
	SecurityCamera.h
	SmokeParticles.h
	Sound.h
	Target.h
	Trigger.h
	Weapon.h
	WorldSpawn.h)
set(ID_GAME_SOURCES
	Achievements.cpp
	Actor.cpp
	AF.cpp
	AFEntity.cpp
	AimAssist.cpp
	BrittleFracture.cpp
	Camera.cpp
	Classic.cpp
	#d3xp/EndLevel.cpp
	Entity.cpp
	Fx.cpp
	GameEdit.cpp
	Game_local.cpp
	Game_network.cpp
	Grabber.cpp
	IK.cpp
	Item.cpp
	Leaderboards.cpp
	Light.cpp
	Misc.cpp
	Moveable.cpp
	Mover.cpp
	MultiplayerGame.cpp
	Player.cpp
	PlayerIcon.cpp
	PlayerView.cpp
	precompiled.cpp
	Projectile.cpp
	Pvs.cpp
	SecurityCamera.cpp
	SmokeParticles.cpp
	Sound.cpp
	Target.cpp
	Trigger.cpp
	Weapon.cpp
	WorldSpawn.cpp)
file(GLOB ID_AI_INCLUDES ai/*.h)
file(GLOB ID_AI_SOURCES ai/*.cpp)
file(GLOB ID_ANIM_INCLUDES anim/*.h)
file(GLOB ID_ANIM_SOURCES anim/*.cpp)
file(GLOB ID_GAMESYS_INCLUDES gamesys/*.h)
set(ID_GAMESYS_SOURCES
	#d3xp/gamesys/Callbacks.cpp
	gamesys/Class.cpp
	#d3xp/gamesys/DebugGraph.cpp
	gamesys/Event.cpp
	gamesys/SaveGame.cpp
	gamesys/SysCmds.cpp
	gamesys/SysCvar.cpp
	#d3xp/gamesys/TypeInfo.cpp
	)
#file(GLOB ID_MENUS_INCLUDES menus/*.h)
#file(GLOB ID_MENUS_SOURCES menus/*.cpp)
file(GLOB ID_PHYSICS_INCLUDES physics/*.h)
file(GLOB ID_PHYSICS_SOURCES physics/*.cpp)
file(GLOB ID_SCRIPT_INCLUDES script/*.h)
file(GLOB ID_SCRIPT_SOURCES script/*.cpp)


set(ID_INCLUDES_ALL
			${ID_GAME_INCLUDES}
			${ID_AI_INCLUDES}
			${ID_ANIM_INCLUDES}
			${ID_GAMESYS_INCLUDES}
		#	${ID_MENUS_INCLUDES}
			${ID_PHYSICS_INCLUDES}
			${ID_SCRIPT_INCLUDES}
			)
			
set(ID_SOURCES_ALL
			${ID_GAME_SOURCES}
			${ID_AI_SOURCES}
			${ID_ANIM_SOURCES}
			${ID_GAMESYS_SOURCES}
		#	${ID_MENUS_SOURCES}
			${ID_PHYSICS_SOURCES}
			${ID_SCRIPT_SOURCES}
			)

source_group("" FILES ${ID_GAME_INCLUDES})
source_group("" FILES ${ID_GAME_SOURCES})
source_group("AI" FILES ${ID_AI_INCLUDES})
source_group("AI" FILES ${ID_AI_SOURCES})
source_group("Anim" FILES ${ID_ANIM_INCLUDES})
source_group("Anim" FILES ${ID_ANIM_SOURCES})
source_group("GameSys" FILES ${ID_GAMESYS_INCLUDES})
source_group("GameSys" FILES ${ID_GAMESYS_SOURCES})
#source_group("menus" FILES ${ID_MENUS_INCLUDES})
#source_group("menus" FILES ${ID_MENUS_SOURCES})
source_group("Physics" FILES ${ID_PHYSICS_INCLUDES})
source_group("Physics" FILES ${ID_PHYSICS_SOURCES})
source_group("Script" FILES ${ID_SCRIPT_INCLUDES})
source_group("Script" FILES ${ID_SCRIPT_SOURCES})

if(MSVC)

#GK: Fixe performance isse on classic doom for the debug build
		set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D _DEBUG /MP /Ob2 /Oi /Oy /LDd")
		set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MP /Oi /Oy /LD")
		#set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MP /Oi /MTd")
		#set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /MP /Oi /Oy /MT")
		set(CMAKE_C_FLAGS_RETAIL "${CMAKE_C_FLAGS_RELEASE} /D ID_RETAIL /MP /Oi /Oy /LD")
		
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP /Ob2 /Oi /Oy /LDd")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MP /Oi /Oy /LD")
		#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MP /MTd")
		#set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MP /Oi /Oy /MT")
		set(CMAKE_CXX_FLAGS_RETAIL "${CMAKE_CXX_FLAGS_RELEASE} /D ID_RETAIL /MP /Oi /Oy /LD")
		set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /ALLOWBIND:NO /NOASSEMBLY /DEF:../../neo/d3xp/Game.def")
		set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /ALLOWBIND:NO /NOASSEMBLY /DEF:../../neo/d3xp/Game.def")
		set(CMAKE_SHARED_LINKER_FLAGS_RETAIL "${CMAKE_SHARED_LINKER_FLAGS_RETAIL} /ALLOWBIND:NO /NOASSEMBLY /DEF:../../neo/d3xp/Game.def")

		if (CMAKE_CL_64)
			set (CPU_TYPE "x64")
		else()
			set (CPU_TYPE "x86")
		endif()
    add_library(game${CPU_TYPE} SHARED ${ID_SOURCES_ALL} ${ID_INCLUDES_ALL})
	include_directories(
			../idlib)
	add_dependencies(game${CPU_TYPE} idlib)
	target_link_libraries(game${CPU_TYPE}
		idlib)
else()
	add_compile_options(-fPIC)
	if(USE_PRECOMPILED_HEADERS) 	
		set(GAME_PRECOMPILED_SOURCES ${ID_SOURCE_ALL})

		foreach( src_file ${GAME_PRECOMPILED_SOURCES} )
			#message(STATUS "-include precompiled.h for ${src_file}")
			set_source_files_properties(
				${src_file}
				PROPERTIES
				COMPILE_FLAGS "-include ${CMAKE_CURRENT_SOURCE_DIR}/../idlib/precompiled.h"
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
		add_custom_target(precomp_header_game ALL
				COMMAND ${CMAKE_CXX_COMPILER} ${_compiler_FLAGS} -x c++-header ../idlib/precompiled.h -o ../idlib/precompiled.h.gch
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				COMMENT "Creating ../idlib/precompiled.h.gch for game"
				)
	endif()
	include_directories(
			.
			../idlib
			#libs/curl/include
			#libs/openal/include
			#libs/freetype/include
            )
	add_library(game SHARED ${ID_SOURCES_ALL} ${ID_INCLUDES_ALL})
	if (USE_PRECOMPILED_HEADERS)
		# make sure precompiled header is created before executable is compiled
		add_dependencies(game precomp_header_game)
		
	endif()
	target_link_libraries(game
		idlib)
		#GK: Make sure the name is right
	set_target_properties(game PROPERTIES PREFIX "")
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set_target_properties(game PROPERTIES SUFFIX "x86_64.so")
	else()
	set_target_properties(game PROPERTIES SUFFIX "x86.so")
	endif()
	
endif()
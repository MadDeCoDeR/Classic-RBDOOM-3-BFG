/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 2015 Robert Beckebans

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#pragma hdrstop
#include "precompiled.h"


#define linit_c
//#define LUA_LIB

extern "C"
{

#include <lua.h>

#include <lualib.h>
#include <lauxlib.h>

//#include "Lua_local.h"

	/*
	** these libs are loaded by lua.c and are readily available to any Lua
	** program
	*/
	
	
	
	int			luaopen_sys( lua_State* L );
	
	static const luaL_Reg loadedlibs[] =
	{
		{"_G", luaopen_base},
		{LUA_LOADLIBNAME, luaopen_package},
		{LUA_COLIBNAME, luaopen_coroutine},
		{LUA_TABLIBNAME, luaopen_table},
		{LUA_IOLIBNAME, luaopen_io},
		{LUA_OSLIBNAME, luaopen_os},
		{LUA_STRLIBNAME, luaopen_string},
//	{LUA_BITLIBNAME, luaopen_bit32},
		{LUA_MATHLIBNAME, luaopen_math},
		{LUA_DBLIBNAME, luaopen_debug},
//		{"sys", luaopen_sys},
		{NULL, NULL}
	};
	
	
	/*
	** these libs are preloaded and must be required before used
	*/
	static const luaL_Reg preloadedlibs[] =
	{
		{NULL, NULL}
	};
	
	
	
	LUALIB_API void luaL_openlibs( lua_State* L )
	{
		const luaL_Reg* lib;
		
		/* call open functions from 'loadedlibs' and set results to global table */
		for( lib = loadedlibs; lib->func; lib++ )
		{
			luaL_requiref( L, lib->name, lib->func, 1 );
			lua_pop( L, 1 ); /* remove lib */
		}
		
		/* add open functions from 'preloadedlibs' into 'package.preload' table */
		luaL_getsubtable( L, LUA_REGISTRYINDEX, "_PRELOAD" );
		for( lib = preloadedlibs; lib->func; lib++ )
		{
			lua_pushcfunction( L, lib->func );
			lua_setfield( L, -2, lib->name );
		}
		lua_pop( L, 1 ); /* remove _PRELOAD table */
	}
	
}

void* idSWF::LuaAlloc( void* ud, void* ptr, size_t osize, size_t nsize )
{
	( void )ud;
	//( void )osize; /* not used */
	
#if 0
	if( nsize == 0 )
	{
		free( ptr );
		return NULL;
	}
	else
	{
		return realloc( ptr, nsize );
	}
#else
	if( nsize == 0 )
	{
		Mem_Free( ptr );
		return NULL;
	}
	else
	{
		void* mem = Mem_Alloc( nsize, TAG_LUA );
	
		if( ptr != NULL )
		{
			SIMDProcessor->Memcpy( mem, ptr, ( osize < nsize ) ? osize : nsize );
			Mem_Free( ptr );
		}
	
		return mem;
	}
	
#endif
}

int idSWF::LuaPanic( lua_State* L )
{
	//lua_printstack( L );
	
	//luai_writestringerror( "PANIC: unprotected error in call to Lua API (%s)\n", lua_tostring( L, -1 ) );
	idLib::Error( "PANIC: unprotected error in call to Lua API (%s)\n", lua_tostring( L, -1 ) );
	
	return 0;  /* return to Lua to abort */
}

int idSWF::LuaNativeScriptFunctionCall( lua_State* L )
{
	//lua_printstack( L );
	
	extern idCVar swf_debugInvoke;
	
	const char* function = lua_tostring( L, lua_upvalueindex( 1 ) );
	
	if( swf_debugInvoke.GetBool() )
	{
		idLib::Printf( "LuaNativeScriptFunctionCall( %s", function );
	}
	
	// convert Lua parms to Flash parms
	idSWFParmList parms;
	
	int top = lua_gettop( L );
	
	for( int i = 1; i <= top; i++ )
	{
		int t = lua_type( L, i );
		
		switch( t )
		{
			case LUA_TNUMBER:
				if( swf_debugInvoke.GetBool() )
				{
					idLib::Printf( ", %g", lua_tonumber( L, i ) );
				}
				parms.Append( ( float ) lua_tonumber( L, i ) );
				break;
				
			case LUA_TBOOLEAN:
				if( swf_debugInvoke.GetBool() )
				{
					idLib::Printf( ", %s", lua_toboolean( L, i ) ? "true" : "false" );
				}
				parms.Append( lua_toboolean( L, i ) != 0 );
				break;
				
			case LUA_TSTRING:
				if( swf_debugInvoke.GetBool() )
				{
					idLib::Printf( ", %s", lua_tostring( L, i ) );
				}
				parms.Append( lua_tostring( L, i ) );
				break;
				
			default:
				if( swf_debugInvoke.GetBool() )
				{
					idLib::Error( "LuaNativeScriptFunctionCall: unknown parameter = %s", lua_typename( L, t ) );
				}
				break;
		}
	}
	
	if( swf_debugInvoke.GetBool() )
	{
		idLib::Printf( " )\n" );
	}
	
	idSWFSprite* sprite = luaSpriteInstance->sprite;
	idSWFScriptVar globalFunc = sprite->GetSWF()->GetGlobal( function );
	if( globalFunc.IsFunction() )
	{
		idSWFScriptVar results = globalFunc.GetFunction()->Call( NULL, parms );
		
		// TODO if results > 1 push to stack
	}
	
	return 0;
}

void lua_printstack( lua_State* L )
{
	idLib::Printf( "Lua stack: " );
	
	int top = lua_gettop( L );
	
	for( int i = 1; i <= top; i++ )
	{
		int t = lua_type( L, i );
		
		switch( t )
		{
			case LUA_TNUMBER:
				idLib::Printf( "'%g'", lua_tonumber( L, i ) );
				break;
				
			case LUA_TBOOLEAN:
				idLib::Printf( "'%s'", lua_toboolean( L, i ) ? "true" : "false" );
				break;
				
			case LUA_TSTRING:
				idLib::Printf( "'%s'", lua_tostring( L, i ) );
				break;
				
			default:
				idLib::Printf( "%s", lua_typename( L, t ) );
				break;
		}
		
		idLib::Printf( " " );
	}
	
	idLib::Printf( "\n" );
}

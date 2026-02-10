/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2014 Robert Beckebans

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


#include "RenderCommon.h"

/*

This file only has a single entry point:

void R_LoadImage( const char *name, byte **pic, int *width, int *height, bool makePowerOf2 );

*/

/*
 * Include file for users of JPEG library.
 * You will need to have included system headers that define at least
 * the typedefs FILE and size_t before you can include jpeglib.h.
 * (stdio.h is sufficient on ANSI-conforming systems.)
 * You may also wish to include "jerror.h".
 */


#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// hooks from jpeg lib to our system

void jpg_Error( const char* fmt, ... )
{
	va_list		argptr;
	char		msg[2048];
	
	va_start( argptr, fmt );
	vsprintf( msg, fmt, argptr );
	va_end( argptr );
	
	common->FatalError( "%s", msg );
}

void jpg_Printf( const char* fmt, ... )
{
	va_list		argptr;
	char		msg[2048];
	
	va_start( argptr, fmt );
	vsprintf( msg, fmt, argptr );
	va_end( argptr );
	
	common->Printf( "%s", msg );
}



/*
================
R_WriteTGA
================
*/
void R_WriteTGA( const char* filename, const byte* data, int width, int height, bool flipVertical, const char* basePath )
{
	byte*	buffer;
	int		i;
	int		bufferSize = width * height * 4 + 18;
	int     imgStart = 18;
	
	idTempArray<byte> buf( bufferSize );
	buffer = ( byte* )buf.Ptr();
	memset( buffer, 0, 18 );
	buffer[2] = 2;		// uncompressed type
	buffer[12] = width & 255;
	buffer[13] = width >> 8;
	buffer[14] = height & 255;
	buffer[15] = height >> 8;
	buffer[16] = 32;	// pixel size
	if( !flipVertical )
	{
		buffer[17] = ( 1 << 5 );	// flip bit, for normal top to bottom raster order
	}
	
	// swap rgb to bgr
	for( i = imgStart ; i < bufferSize ; i += 4 )
	{
		buffer[i] = data[i - imgStart + 2];		// blue
		buffer[i + 1] = data[i - imgStart + 1];		// green
		buffer[i + 2] = data[i - imgStart + 0];		// red
		buffer[i + 3] = data[i - imgStart + 3];		// alpha
	}
	
	fileSystem->WriteFile( filename, buffer, bufferSize, basePath );
}

static void LoadTGA( const char* name, byte** pic, int* width, int* height, ID_TIME_T* timestamp );
static void LoadSTB_RGBA8( const char* name, byte** pic, int* width, int* height, ID_TIME_T* timestamp );

/*
========================================================================

TGA files are used for 24/32 bit images

========================================================================
*/

typedef struct _TargaHeader
{
	unsigned char 	id_length, colormap_type, image_type;
	unsigned short	colormap_index, colormap_length;
	unsigned char	colormap_size;
	unsigned short	x_origin, y_origin, width, height;
	unsigned char	pixel_size, attributes;
} TargaHeader;


/*
=========================================================

TARGA LOADING

=========================================================
*/

/*
=============
LoadTGA
=============
*/
static void LoadTGA( const char* name, byte** pic, int* width, int* height, ID_TIME_T* timestamp )
{
	int		columns, rows, numPixels, fileSize, numBytes;
	byte*	pixbuf;
	int		row, column;
	byte*	buf_p;
	byte*	buffer;
	TargaHeader	targa_header;
	byte*		targa_rgba;
	
	if( !pic )
	{
		fileSystem->ReadFile( name, NULL, timestamp );
		return;	// just getting timestamp
	}
	
	*pic = NULL;
	
	//
	// load the file
	//
	fileSize = fileSystem->ReadFile( name, ( void** )&buffer, timestamp );
	if( !buffer )
	{
		return;
	}
	
	buf_p = buffer;
	
	targa_header.id_length = *buf_p++;
	targa_header.colormap_type = *buf_p++;
	targa_header.image_type = *buf_p++;
	
	targa_header.colormap_index = LittleShort( *( short* )buf_p );
	buf_p += 2;
	targa_header.colormap_length = LittleShort( *( short* )buf_p );
	buf_p += 2;
	targa_header.colormap_size = *buf_p++;
	targa_header.x_origin = LittleShort( *( short* )buf_p );
	buf_p += 2;
	targa_header.y_origin = LittleShort( *( short* )buf_p );
	buf_p += 2;
	targa_header.width = LittleShort( *( short* )buf_p );
	buf_p += 2;
	targa_header.height = LittleShort( *( short* )buf_p );
	buf_p += 2;
	targa_header.pixel_size = *buf_p++;
	targa_header.attributes = *buf_p++;
	
	if( targa_header.image_type != 2 && targa_header.image_type != 10 && targa_header.image_type != 3 )
	{
		common->Error( "LoadTGA( %s ): Only type 2 (RGB), 3 (gray), and 10 (RGB) TGA images supported\n", name );
	}
	
	if( targa_header.colormap_type != 0 )
	{
		common->Error( "LoadTGA( %s ): colormaps not supported\n", name );
	}
	
	if( ( targa_header.pixel_size != 32 && targa_header.pixel_size != 24 ) && targa_header.image_type != 3 )
	{
		common->Error( "LoadTGA( %s ): Only 32 or 24 bit images supported (no colormaps)\n", name );
	}
	
	if( targa_header.image_type == 2 || targa_header.image_type == 3 )
	{
		numBytes = targa_header.width * targa_header.height * ( targa_header.pixel_size >> 3 );
		if( numBytes > fileSize - 18 - targa_header.id_length )
		{
			common->Error( "LoadTGA( %s ): incomplete file\n", name );
		}
	}
	
	columns = targa_header.width;
	rows = targa_header.height;
	numPixels = columns * rows;
	
	if( width )
	{
		*width = columns;
	}
	if( height )
	{
		*height = rows;
	}
	
	targa_rgba = ( byte* )R_StaticAlloc( numPixels * 4, TAG_IMAGE );
	*pic = targa_rgba;
	
	if( targa_header.id_length != 0 )
	{
		buf_p += targa_header.id_length;  // skip TARGA image comment
	}
	
	if( targa_header.image_type == 2 || targa_header.image_type == 3 )
	{
		// Uncompressed RGB or gray scale image
		for( row = rows - 1; row >= 0; row-- )
		{
			pixbuf = targa_rgba + row * columns * 4;
			for( column = 0; column < columns; column++ )
			{
				unsigned char red, green, blue, alphabyte;
				switch( targa_header.pixel_size )
				{
				
					case 8:
						blue = *buf_p++;
						green = blue;
						red = blue;
						*pixbuf++ = red;
						*pixbuf++ = green;
						*pixbuf++ = blue;
						*pixbuf++ = 255;
						break;
						
					case 24:
						blue = *buf_p++;
						green = *buf_p++;
						red = *buf_p++;
						*pixbuf++ = red;
						*pixbuf++ = green;
						*pixbuf++ = blue;
						*pixbuf++ = 255;
						break;
					case 32:
						blue = *buf_p++;
						green = *buf_p++;
						red = *buf_p++;
						alphabyte = *buf_p++;
						*pixbuf++ = red;
						*pixbuf++ = green;
						*pixbuf++ = blue;
						*pixbuf++ = alphabyte;
						break;
					default:
						common->Error( "LoadTGA( %s ): illegal pixel_size '%d'\n", name, targa_header.pixel_size );
						break;
				}
			}
		}
	}
	else if( targa_header.image_type == 10 )      // Runlength encoded RGB images
	{
		unsigned char red, green, blue, alphabyte, packetHeader, packetSize, j;
		
		red = 0;
		green = 0;
		blue = 0;
		alphabyte = 0xff;
		
		for( row = rows - 1; row >= 0; row-- )
		{
			pixbuf = targa_rgba + row * columns * 4;
			for( column = 0; column < columns; )
			{
				packetHeader = *buf_p++;
				packetSize = 1 + ( packetHeader & 0x7f );
				if( packetHeader & 0x80 )           // run-length packet
				{
					switch( targa_header.pixel_size )
					{
						case 24:
							blue = *buf_p++;
							green = *buf_p++;
							red = *buf_p++;
							alphabyte = 255;
							break;
						case 32:
							blue = *buf_p++;
							green = *buf_p++;
							red = *buf_p++;
							alphabyte = *buf_p++;
							break;
						default:
							common->Error( "LoadTGA( %s ): illegal pixel_size '%d'\n", name, targa_header.pixel_size );
							break;
					}
					
					for( j = 0; j < packetSize; j++ )
					{
						*pixbuf++ = red;
						*pixbuf++ = green;
						*pixbuf++ = blue;
						*pixbuf++ = alphabyte;
						column++;
						if( column == columns )    // run spans across rows
						{
							column = 0;
							if( row > 0 )
							{
								row--;
							}
							else
							{
								goto breakOut;
							}
							pixbuf = targa_rgba + row * columns * 4;
						}
					}
				}
				else                              // non run-length packet
				{
					for( j = 0; j < packetSize; j++ )
					{
						switch( targa_header.pixel_size )
						{
							case 24:
								blue = *buf_p++;
								green = *buf_p++;
								red = *buf_p++;
								*pixbuf++ = red;
								*pixbuf++ = green;
								*pixbuf++ = blue;
								*pixbuf++ = 255;
								break;
							case 32:
								blue = *buf_p++;
								green = *buf_p++;
								red = *buf_p++;
								alphabyte = *buf_p++;
								*pixbuf++ = red;
								*pixbuf++ = green;
								*pixbuf++ = blue;
								*pixbuf++ = alphabyte;
								break;
							default:
								common->Error( "LoadTGA( %s ): illegal pixel_size '%d'\n", name, targa_header.pixel_size );
								break;
						}
						column++;
						if( column == columns )    // pixel packet run spans across rows
						{
							column = 0;
							if( row > 0 )
							{
								row--;
							}
							else
							{
								goto breakOut;
							}
							pixbuf = targa_rgba + row * columns * 4;
						}
					}
				}
			}
breakOut:
			;
		}
	}
	
	if( ( targa_header.attributes & ( 1 << 5 ) ) )  			// image flp bit
	{
		if( width != NULL && height != NULL )
		{
			R_VerticalFlip( *pic, *width, *height );
		}
	}
	
	fileSystem->FreeFile( buffer );
}

/*
=========================================================

Generic Image LOADING

Interfaces with the huge Stb_image
=========================================================
*/

/*
=============
LoadSTB_RGBA8
=============
*/
static void LoadSTB_RGBA8( const char* filename, unsigned char** pic, int* width, int* height, ID_TIME_T* timestamp )
{

	byte*	fbuffer;
	byte*  bbuf;
	int     len;
	int w = 0, h = 0, comp = 0;

	if( pic )
	{
		*pic = NULL;		// until proven otherwise
	}
	idFile* f;
	
	f = fileSystem->OpenFileRead( filename );
	if( !f )
	{
		return;
	}
	len = f->Length();
	if( timestamp )
	{
		*timestamp = f->Timestamp();
	}
	if( !pic )
	{
		fileSystem->CloseFile( f );
		return;	// just getting timestamp
	}
	fbuffer = ( byte* )Mem_ClearedAlloc( len + 4096, TAG_JPG );
	f->Read( fbuffer, len );
	fileSystem->CloseFile( f );

	bbuf = stbi_load_from_memory(fbuffer, len, &w, &h, &comp, 4);
	Mem_Free(fbuffer);

	if (bbuf == NULL) {
		common->Warning("Failed to load jpg image %s with error: %s\n", filename, stbi_failure_reason());
		return;
	}

	int size = w * h * 4;
	*pic = (unsigned char*)R_StaticAlloc(size, TAG_IMAGE);
	memcpy(*pic, bbuf, size);
	*width = w;
	*height = h;
	stbi_image_free(bbuf);
}

/*
==================
WriteScreenshotForSTBIW

Callback to each stbi_write_* function
==================
*/
static void WriteScreenshotForSTBIW( void* context, void* data, int size )
{
	idFile* f = ( idFile* )context;
	f->Write( data, size );
}


/*
================
R_WritePNG
================
*/

void R_WritePNG( const char* filename, const byte* data, int bytesPerPixel, int width, int height, bool flipVertical, const char* basePath)
{
	if( bytesPerPixel != 4  && bytesPerPixel != 3 )
	{
		common->Error( "R_WritePNG( %s ): bytesPerPixel = %i not supported", filename, bytesPerPixel );
	}

	idFileLocal file( fileSystem->OpenFileWrite( filename, basePath ) );
	if( file == NULL )
	{
		common->Printf( "R_WritePNG: Failed to open %s\n", filename );
		return;
	}
	//GK: You can FLIP it like that and get some free fries /j
	stbi_flip_vertically_on_write(flipVertical);
	//stbi_write_png_compression_level = idMath::ClampInt( 0, 9, r_screenshotPngCompression.GetInteger() );
	stbi_write_png_to_func( WriteScreenshotForSTBIW, file, width, height, bytesPerPixel, data, bytesPerPixel * width );
	stbi_flip_vertically_on_write(!flipVertical);
}

//===================================================================


typedef struct
{
	const char*	ext;
	void	( *ImageLoader )( const char* filename, unsigned char** pic, int* width, int* height, ID_TIME_T* timestamp );
} imageExtToLoader_t;

static imageExtToLoader_t imageLoaders[] =
{
	{"png", LoadSTB_RGBA8},
	{"tga", LoadTGA},
	{"jpg", LoadSTB_RGBA8}
	
	
};

static const int numImageLoaders = sizeof( imageLoaders ) / sizeof( imageLoaders[0] );

/*
=================
R_LoadImage

Loads any of the supported image types into a cannonical
32 bit format.

Automatically attempts to load .jpg files if .tga files fail to load.

*pic will be NULL if the load failed.

Anything that is going to make this into a texture would use
makePowerOf2 = true, but something loading an image as a lookup
table of some sort would leave it in identity form.

It is important to do this at image load time instead of texture load
time for bump maps.

Timestamp may be NULL if the value is going to be ignored

If pic is NULL, the image won't actually be loaded, it will just find the
timestamp.
=================
*/
void R_LoadImage( const char* cname, byte** pic, int* width, int* height, ID_TIME_T* timestamp, bool makePowerOf2 )
{
	idStr name = cname;
	
	if( pic )
	{
		*pic = NULL;
	}
	if( timestamp )
	{
		*timestamp = FILE_NOT_FOUND_TIMESTAMP;
	}
	if( width )
	{
		*width = 0;
	}
	if( height )
	{
		*height = 0;
	}
	
	name.DefaultFileExtension( ".tga" );
	
	if( name.Length() < 5 )
	{
		return;
	}
	
	name.ToLower();
	idStr ext;
	name.ExtractFileExtension( ext );
	idStr origName = name;
	
// RB begin
	//if( !ext.IsEmpty() )
	{
		int i;
		/*for( i = 0; i < numImageLoaders; i++ )
		{
			if( !ext.Icmp( imageLoaders[i].ext ) )
			{
				imageLoaders[i].ImageLoader( name.c_str(), pic, width, height, timestamp );
				break;
			}
		}
		
		if( i < numImageLoaders )*/
		{
			//if( pic && *pic == NULL )
			{
				// image with the specified extension was not found so try all formats
				for( i = 0; i < numImageLoaders; i++ )
				{
					name.SetFileExtension( imageLoaders[i].ext );
					imageLoaders[i].ImageLoader( name.c_str(), pic, width, height, timestamp );
					
					if( pic && *pic != NULL )
					{
						//common->Warning("image %s failed to load, using %s instead", origName.c_str(), name.c_str());
						break;
					}
				}
			}
		}
	}
// RB end

	if( ( width && *width < 1 ) || ( height && *height < 1 ) )
	{
		if( pic && *pic )
		{
			R_StaticFree( *pic );
			*pic = 0;
		}
	}
	
	//
	// convert to exact power of 2 sizes
	//
	/*
	if ( pic && *pic && makePowerOf2 ) {
		int		w, h;
		int		scaled_width, scaled_height;
		byte	*resampledBuffer;
	
		w = *width;
		h = *height;
	
		for (scaled_width = 1 ; scaled_width < w ; scaled_width<<=1)
			;
		for (scaled_height = 1 ; scaled_height < h ; scaled_height<<=1)
			;
	
		if ( scaled_width != w || scaled_height != h ) {
			resampledBuffer = R_ResampleTexture( *pic, w, h, scaled_width, scaled_height );
			R_StaticFree( *pic );
			*pic = resampledBuffer;
			*width = scaled_width;
			*height = scaled_height;
		}
	}
	*/
}


/*
=======================
R_LoadCubeImages

Loads six files with proper extensions
=======================
*/
bool R_LoadCubeImages( const char* imgName, cubeFiles_t extensions, byte* pics[6], int* outSize, ID_TIME_T* timestamp, int cubeMapSize)
{
	int		i, j;
	const char*	cameraSides[6] =  { "_forward.tga", "_back.tga", "_left.tga", "_right.tga",
									"_up.tga", "_down.tga"
								  };
	const char*	axisSides[6] =  { "_px.tga", "_nx.tga", "_py.tga", "_ny.tga",
								  "_pz.tga", "_nz.tga"
								};
	const char**	sides;
	char	fullName[MAX_IMAGE_NAME];
	int		width, height, size = 0;
	
	if( extensions == CF_CAMERA )
	{
		sides = cameraSides;
	}
	else
	{
		sides = axisSides;
	}
	
	// FIXME: precompressed cube map files
	if( pics )
	{
		memset( pics, 0, 6 * sizeof( pics[0] ) );
	}
	if( timestamp )
	{
		*timestamp = 0;
	}

	//SP Begin
	if (extensions == CF_SINGLE && cubeMapSize != 0)
	{
		ID_TIME_T thisTime;
		byte* thisPic[1];
		thisPic[0] = nullptr;

		if (pics)
		{
			R_LoadImageProgram(imgName, thisPic, &width, &height, &thisTime);
		}
		else
		{
			// load just the timestamps
			R_LoadImageProgram(imgName, nullptr, &width, &height, &thisTime);
		}


		if (thisTime == FILE_NOT_FOUND_TIMESTAMP)
		{
			return false;
		}

		if (timestamp)
		{
			if (thisTime > *timestamp)
			{
				*timestamp = thisTime;
			}
		}

		if (pics)
		{
			*outSize = cubeMapSize;

			for (int i1 = 0; i1 < 6; i1++)
			{
				pics[i1] = R_GenerateCubeMapSideFromSingleImage(thisPic[0], width, height, cubeMapSize, i1);
				switch (i1)
				{
				case 0:	// forward
					R_RotatePic(pics[i1], cubeMapSize);
					break;
				case 1:	// back
					R_RotatePic(pics[i1], cubeMapSize);
					R_HorizontalFlip(pics[i1], cubeMapSize, cubeMapSize);
					R_VerticalFlip(pics[i1], cubeMapSize, cubeMapSize);
					break;
				case 2:	// left
					R_VerticalFlip(pics[i1], cubeMapSize, cubeMapSize);
					break;
				case 3:	// right
					R_HorizontalFlip(pics[i1], cubeMapSize, cubeMapSize);
					break;
				case 4:	// up
					R_RotatePic(pics[i1], cubeMapSize);
					break;
				case 5: // down
					R_RotatePic(pics[i1], cubeMapSize);
					break;
				}
			}

			R_StaticFree(thisPic[0]);
		}

		return true;
	}
	//SP End
	
	for( i = 0 ; i < 6 ; i++ )
	{
		idStr::snPrintf( fullName, sizeof( fullName ), "%s%s", imgName, sides[i] );
		
		ID_TIME_T thisTime;
		if( !pics )
		{
			// just checking timestamps
			R_LoadImageProgram( fullName, NULL, &width, &height, &thisTime );
		}
		else
		{
			R_LoadImageProgram( fullName, &pics[i], &width, &height, &thisTime );
		}
		if( thisTime == FILE_NOT_FOUND_TIMESTAMP )
		{
			break;
		}
		if( i == 0 )
		{
			size = width;
		}
		if( width != size || height != size )
		{
			common->Warning( "Mismatched sizes on cube map '%s'", imgName );
			break;
		}
		if( timestamp )
		{
			if( thisTime > *timestamp )
			{
				*timestamp = thisTime;
			}
		}
		if( pics && extensions == CF_CAMERA )
		{
			// convert from "camera" images to native cube map images
			switch( i )
			{
				case 0:	// forward
					R_RotatePic( pics[i], width );
					break;
				case 1:	// back
					R_RotatePic( pics[i], width );
					R_HorizontalFlip( pics[i], width, height );
					R_VerticalFlip( pics[i], width, height );
					break;
				case 2:	// left
					R_VerticalFlip( pics[i], width, height );
					break;
				case 3:	// right
					R_HorizontalFlip( pics[i], width, height );
					break;
				case 4:	// up
					R_RotatePic( pics[i], width );
					break;
				case 5: // down
					R_RotatePic( pics[i], width );
					break;
			}
		}
	}
	
	if( i != 6 )
	{
		// we had an error, so free everything
		if( pics )
		{
			for( j = 0 ; j < i ; j++ )
			{
				R_StaticFree( pics[j] );
			}
		}
		
		if( timestamp )
		{
			*timestamp = 0;
		}
		return false;
	}
	
	if( outSize )
	{
		*outSize = size;
	}
	return true;
}

/**
* Copyright (C) 2018 George Kalmpokis
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is furnished to
* do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software. As clarification, there
* is no requirement that the copyright notice and permission be included in
* binary distributions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "Precompiled.h"
#include "globaldata.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "doomdef.h"
#include "m_swap.h"
#include <png.h>
#include "i_video.h"
#include <jpeglib.h>
#include <jerror.h>
#ifdef _DEBUG
#include <iostream>
#include <fstream>
#endif


png_byte grAb[5] = { 67,72,41,62,(png_byte)'\0' };
int x, y, flmp;

void InitColorMap() {
	if (!::g->cpind) {
		byte* pallete = (byte*)W_CacheLumpName("PLAYPAL", PU_CACHE_SHARED);
		if (::g->cmap.size() > 0) {
			::g->cmap.clear();
		}
		int size = (255<<16) + (255<<8) + (255) + 1;
		::g->cmap.resize(size);
		int psize = W_LumpLength(W_GetNumForName("PLAYPAL"))/3;
		for (int i = 0; i < psize; i++) {
			unsigned char* dpixel = pallete + (i * 3);
			int index = (dpixel[0]<<16) + (dpixel[1]<<8) + (dpixel[2]) + 0;
			::g->cmap[index] = i;
		}
	}
}

patch_t* GetPreloaded() {
	if ((int)::g->cpatch.size() >= flmp) {
		if (::g->cpatch[flmp - 1] != NULL) {
			return ::g->cpatch[flmp - 1];
		}
	}
	return NULL;
}

bool checkpng(unsigned char* buff) {
	if (buff[0] == 137 && buff[1] == 80 && buff[2] == 78 && buff[3] == 71) {
		return true;
	}
	return false;//!png_sig_cmp((png_bytep)buff, 0, 8);
}

bool checkjpeg(unsigned char* buff) {
	if (buff[0] == 255 && buff[1] == 216) {
		return true;
	}
	return false;
}

void ReadDataFromInputStream(png_structp png_ptr, png_bytep outBytes,
	png_size_t byteCountToRead)
{
#if PNG_LIBPNG_VER_MAJOR == 1 && PNG_LIBPNG_VER_MINOR < 4
	memcpy(outBytes, (byte*)png_ptr->io_ptr, byteCountToRead);

	png_ptr->io_ptr = ((byte*)png_ptr->io_ptr) + byteCountToRead;
#else

	byte** ioptr = (byte**)png_get_io_ptr(png_ptr);
	memcpy(outBytes, *ioptr, byteCountToRead);
	*ioptr += byteCountToRead;
#endif
}

int Get_custom_chunk(png_structp png_ptr, png_unknown_chunkp chunk) {
	if (idStr::Icmp((char*)chunk->name,"grAb")) {
		return 0;
	}
	if (chunk->size < 8) {
		return -1;
	}
	x = (chunk->data[0] << 24) | (chunk->data[1] << 16) | (chunk->data[2] << 8) | (chunk->data[3]);
	y = (chunk->data[4] << 24) | (chunk->data[5] << 16) | (chunk->data[6] << 8) | (chunk->data[7]);
	return 1;
}

bool istrans(png_byte* pixel) {
	return ( pixel[3] == 0);
}

unsigned char GetColorMap(png_byte* pixel) {
	int index = (pixel[0]<<16) + (pixel[1]<<8) + (pixel[2]) + 0;
	return ::g->cmap[index];
}

void GetPNGInfo(png_structp &png_ptr, png_infop &info_ptr) {
	png_set_keep_unknown_chunks(png_ptr, 2, grAb, 1);

	png_set_read_user_chunk_fn(png_ptr, NULL, (png_user_chunk_ptr)Get_custom_chunk);

	png_set_sig_bytes(png_ptr, 0);

	png_read_info(png_ptr, info_ptr);

	int bitd = png_get_bit_depth(png_ptr, info_ptr);

	int ct = png_get_color_type(png_ptr, info_ptr);

	if (bitd > 8) {
		png_set_strip_16(png_ptr);
	}

	if (bitd < 8)
		png_set_packing(png_ptr);

	if (ct == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png_ptr);
	}

	if (ct == PNG_COLOR_TYPE_GRAY || ct == PNG_COLOR_TYPE_GRAY_ALPHA) {
		png_set_gray_to_rgb(png_ptr);
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
	}

	png_read_update_info(png_ptr, info_ptr);
}

patch_t* PNG2lmp(unsigned char* buffer) {
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep* rows;	
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		return NULL;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		return NULL;
	}

	/*if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr,
			NULL);
		return NULL;
	}*/
	
	png_set_read_fn(png_ptr, buffer, ReadDataFromInputStream);

	GetPNGInfo(png_ptr, info_ptr);
	int w = png_get_image_width(png_ptr, info_ptr);
	int h = png_get_image_height(png_ptr, info_ptr);
	int headoffs= (w * sizeof(int)) + (4 * sizeof(short));
	patch_t* patch = (patch_t*)malloc(headoffs);

	patch->width = w;
	patch->height = h;
	patch->leftoffset = x;
	patch->topoffset = y;
	patch->columnofs[0] = headoffs;

	rows = (png_bytep*)malloc(patch->height * sizeof(png_bytep));
	for (int i = 0; i < patch->height; i++) {
		rows[i] = (png_bytep)malloc(png_get_rowbytes(png_ptr, info_ptr));
	}
	png_read_image(png_ptr, rows);
	int oc = 0;
	bool sc = false;
	int pc = 0;
	int offset = 4;
	postColumn_t* tpat;
	int eh = 0;
	int posc = 0;
	int imagesize = headoffs;
	int* offsets = (int*)malloc(patch->width * sizeof(int));
	unsigned char** post = (unsigned char**)malloc(patch->height * sizeof(byte*));
	for (int i = 0; i < patch->height; i++) {
		post[i] = (unsigned char*)malloc(4);
	}
	for (int i = 0; i < patch->width; i++) {
		sc = false;
		pc = 0;
		offset = 3;
		eh = 0;
		posc = 0;
		for (int j = 0; j < patch->height; j++) {
			png_byte* pixel = &(rows[j][i*4]);
			if (!istrans(pixel)) {
				byte dpix = GetColorMap(pixel);
				if (!sc) {
					post[posc][0] = j;
					post[posc][1] = pc;
					post[posc][2] = dpix;
					sc = true;
					eh = 0;
				}
					pc++;
					post[posc][1] = pc;
					post[posc] = (unsigned char*)realloc(post[posc],4 + pc);
					post[posc][offset] = dpix;
					offset++;
				
			}
			else {
				if (sc) {
					pc++;
					post[posc] = (unsigned char*)realloc(post[posc], 4 + pc);
					post[posc][offset] = post[posc][offset-1];
					offset++;
					offsets[posc] = offset;
					posc++;
					free(post[posc]);
					post[posc] = (unsigned char*)malloc(4);
					sc = false;
					pc = 0;
					offset = 3;
					eh = 0;
				}
				else {
					eh++;
				}

			}
		}
		if (sc || eh != patch->height) {
			if (sc) {
				pc++;
				post[posc] = (unsigned char*)realloc(post[posc], 4 + pc);
				post[posc][offset] = post[posc][offset - 1];
				offset++;
				offsets[posc] = offset;
				posc++;
			}
			offset = 0;
			for (int o = 0; o < posc; o++) {
				imagesize += offsets[o];
				patch = (patch_t*)realloc(patch, imagesize);
				if (o == posc - 1) {
					imagesize++;
					patch = (patch_t*)realloc(patch, imagesize);
				}
			}
			tpat =(postColumn_t*) &((byte*)patch + LONG(patch->columnofs[oc]))[0];
			//int offs = 0;
			for (int o = 0; o < posc; o++) {
				if (o > 0) {
					tpat = (postColumn_t*)&((byte*)tpat + tpat->length + 4)[0];
				}
				memcpy(tpat, post[o], offsets[o]);
				offset += offsets[o];
			}
			tpat = (postColumn_t*)&((byte*)tpat + tpat->length + 4)[0];
			tpat->topdelta = 255;
			offset++;
			oc++;
			if (oc < patch->width) {
				patch->columnofs[oc] = patch->columnofs[oc - 1] + offset;
			}
		}
		else if (eh == patch->height){
				post[posc][0] = (unsigned char)255;
				offset = 1;
				imagesize++;
				patch = (patch_t*)realloc(patch, imagesize);
				tpat =(postColumn_t*) &((byte*)patch + LONG(patch->columnofs[oc]))[0];
				memcpy(tpat, post[posc], offset);
				oc++;
				if (oc < patch->width) {
					patch->columnofs[oc] = patch->columnofs[oc - 1] + offset;
				}
		}
	}
	for (int i = 0; i < patch->height; i++) {
		free(rows[i]);
	}
	free(rows);
	rows = NULL;
	for (int i = 0; i < patch->height; i++) {
		free(post[i]);
	}
	free(post);
	post = NULL;
	free(offsets);
	offsets = NULL;
	png_infop end_info;
	end_info = png_create_info_struct(png_ptr);
	png_read_end(png_ptr, end_info);
#if PNG_LIBPNG_VER_MAJOR == 1 && PNG_LIBPNG_VER_MINOR < 4
	png_read_destroy(png_ptr, info_ptr, end_info);
#endif
	png_ptr = NULL;
	info_ptr = NULL;
	end_info = NULL;

	::g->cpind = 1;
	if ((int)::g->cpatch.size() < flmp) {
		::g->cpatch.resize(flmp);
	}
	::g->cpatch[flmp - 1] = (patch_t*)malloc(imagesize);
	memcpy(::g->cpatch[flmp-1],patch,imagesize);
#ifdef _DEBUG
	char* ddir = "base//lmps//";
#ifdef _WIN32
	CreateDirectory(ddir, NULL);
#else
	mkdir(ddir, S_IRWXU);
#endif
	std::string name = W_GetNameForNum(flmp);
	std::string filename = "base//lmps//" + name + ".lmp";
	std::ofstream of(filename.c_str(), std::ios::binary);
	of.write((char*)patch,imagesize);
	of.flush();
	of.close();
#endif
	Z_Free(buffer);
	free(patch);
	patch = NULL;
	return GetPreloaded();
}

patch_t* JPEG2lmp(unsigned char* buffer, int size) {
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	/* More stuff */
	png_bytep *rows;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */
	cinfo.err = jpeg_std_error(&jerr);
	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(&cinfo);
	/* Step 2: specify data source (eg, a file) */

#ifdef USE_NEWER_JPEG
	jpeg_mem_src(&cinfo, buffer, size);
#else
	jpeg_stdio_src(&cinfo, buffer);
#endif

	jpeg_read_header(&cinfo, true);
	jpeg_start_decompress(&cinfo);
	row_stride = cinfo.output_width * cinfo.output_components;
	int w = cinfo.output_width;
	int h = cinfo.output_height;
	x = 0;
	y = 0;
	int headoffs = (w * sizeof(int)) + (4 * sizeof(short));
	patch_t* patch = (patch_t*)malloc(headoffs);
	patch->width = w;
	patch->height = h;
	patch->leftoffset = x;
	patch->topoffset = y;
	patch->columnofs[0] = headoffs;
	rows = (png_bytep*)malloc(patch->height * sizeof(png_bytep));
	for (int i = 0; i < patch->height; i++) {
		rows[i] = (png_bytep)malloc(row_stride);
	}
	int r = 0;
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo,&rows[r], 1);
		r++;
	}
		int oc = 0;
		bool sc = false;
		int pc = 0;
		int offset = 4;
		postColumn_t* tpat;
		int eh = 0;
		int posc = 0;
		int imagesize = headoffs;
		int* offsets = (int*)malloc(patch->width * sizeof(int));
		unsigned char** post = (unsigned char**)malloc(patch->height * sizeof(byte*));
		for (int i = 0; i < patch->height; i++) {
			post[i] = (unsigned char*)malloc(4);
		}
		for (int i = 0; i < patch->width; i++) {
			sc = false;
			pc = 0;
			offset = 3;
			eh = 0;
			posc = 0;
			for (int j = 0; j < patch->height; j++) {
				png_byte* pixel = &(rows[j][i * 4]);
				{
					byte dpix = GetColorMap(pixel);
					if (!sc) {
						post[posc][0] = j;
						post[posc][1] = pc;
						post[posc][2] = dpix;
						sc = true;
						eh = 0;
					}
					pc++;
					post[posc][1] = pc;
					post[posc] = (unsigned char*)realloc(post[posc], 4 + pc);
					post[posc][offset] = dpix;
					offset++;

				}
			}
			if (sc || eh != patch->height) {
				if (sc) {
					pc++;
					post[posc] = (unsigned char*)realloc(post[posc], 4 + pc);
					post[posc][offset] = post[posc][offset - 1];
					offset++;
					offsets[posc] = offset;
					posc++;
				}
				offset = 0;
				for (int o = 0; o < posc; o++) {
					imagesize += offsets[o];
					patch = (patch_t*)realloc(patch, imagesize);
					if (o == posc - 1) {
						imagesize++;
						patch = (patch_t*)realloc(patch, imagesize);
					}
				}
				tpat = (postColumn_t*) &((byte*)patch + LONG(patch->columnofs[oc]))[0];
				//int offs = 0;
				for (int o = 0; o < posc; o++) {
					if (o > 0) {
						tpat = (postColumn_t*)&((byte*)tpat + tpat->length + 4)[0];
					}
					memcpy(tpat, post[o], offsets[o]);
					offset += offsets[o];
				}
				tpat = (postColumn_t*)&((byte*)tpat + tpat->length + 4)[0];
				tpat->topdelta = 255;
				offset++;
				oc++;
				if (oc < patch->width) {
					patch->columnofs[oc] = patch->columnofs[oc - 1] + offset;
				}
			}
			else if (eh == patch->height) {
				post[posc][0] = (unsigned char)255;
				offset = 1;
				imagesize++;
				patch = (patch_t*)realloc(patch, imagesize);
				tpat = (postColumn_t*) &((byte*)patch + LONG(patch->columnofs[oc]))[0];
				memcpy(tpat, post[posc], offset);
				oc++;
				if (oc < patch->width) {
					patch->columnofs[oc] = patch->columnofs[oc - 1] + offset;
				}
			}
		}
		for (int i = 0; i < patch->height; i++) {
			free(rows[i]);
		}
		free(rows);
		rows = NULL;
		for (int i = 0; i < patch->height; i++) {
			free(post[i]);
		}
		free(post);
		post = NULL;
		free(offsets);
		offsets = NULL;
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);

		::g->cpind = 1;
		if ((int)::g->cpatch.size() < flmp) {
			::g->cpatch.resize(flmp);
		}
		::g->cpatch[flmp - 1] =(patch_t*) malloc(imagesize);
		memcpy(::g->cpatch[flmp - 1], patch, imagesize);
#ifdef _DEBUG
		char* ddir = "base//lmps//";
#ifdef _WIN32
		CreateDirectory(ddir, NULL);
#else
		mkdir(ddir, S_IRWXU);
#endif
		std::string name = W_GetNameForNum(flmp);
		std::string filename = "base//lmps//" + name + ".lmp";
		std::ofstream of(filename.c_str(), std::ios::binary);
		of.write((char*)patch, imagesize);
		of.flush();
		of.close();
#endif
		Z_Free(buffer);
		free(patch);
		patch = NULL;
		return GetPreloaded();
}

patch_t* img2lmp(void* buff,int lump) {
	bool is_png;
	bool is_jpeg;
	if (buff != NULL || buff != nullptr) {
		flmp = lump;
		unsigned char* imgbuf = reinterpret_cast<unsigned char*>(buff);
		is_png = checkpng(imgbuf);
		is_jpeg = checkjpeg(imgbuf);
		if (!is_png && !is_jpeg) {
			return (patch_t*)buff;
		}
		if (is_png) {
			patch_t* patch = NULL;
			if (::g->cpind) {
				patch = GetPreloaded();
			}
			if (patch != NULL) {
				return patch;
			}
			else {
				InitColorMap();
				return PNG2lmp(imgbuf);
			}
		}
		if (is_jpeg) {
			patch_t* patch = NULL;
			if (::g->cpind) {
				patch = GetPreloaded();
			}
			if (patch != NULL) {
				return patch;
			}
			else {
				InitColorMap();
				return JPEG2lmp(imgbuf, W_LumpLength(lump));
			}
		}
	}
	return NULL;
}


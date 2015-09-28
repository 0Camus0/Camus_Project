#ifndef __gl2ext_h_
#define __gl2ext_h_

#ifndef __gl2_h_
#   include <GLES2/gl2.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
** License Applicability. Except to the extent portions of this file are
** made subject to an alternative license as permitted in the SGI Free
** Software License B, Version 1.0 (the "License"), the contents of this
** file are subject only to the provisions of the License. You may not use
** this file except in compliance with the License. You may obtain a copy
** of the License at Silicon Graphics, Inc., attn: Legal Services, 1600
** Amphitheatre Parkway, Mountain View, CA 94043-1351, or at:
**
** http://oss.sgi.com/projects/FreeB
**
** Note that, as provided in the License, the Software is distributed on an
** "AS IS" basis, with ALL EXPRESS AND IMPLIED WARRANTIES AND CONDITIONS
** DISCLAIMED, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES AND
** CONDITIONS OF MERCHANTABILITY, SATISFACTORY QUALITY, FITNESS FOR A
** PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
**
** Original Code. The Original Code is: OpenGL Sample Implementation,
** Version 1.2.1, released January 26, 2000, developed by Silicon Graphics,
** Inc. The Original Code is Copyright (c) 1991-2000 Silicon Graphics, Inc.
** Copyright in any portions created by third parties is as indicated
** elsewhere herein. All Rights Reserved.
**
** Additional Notice Provisions: The application programming interfaces
** established by SGI in conjunction with the Original Code are The
** OpenGL(R) Graphics System: A Specification (Version 1.2.1), released
** April 1, 1999; The OpenGL(R) Graphics System Utility Library (Version
** 1.3), released November 4, 1998; and OpenGL(R) Graphics with the X
** Window System(R) (Version 1.3), released October 19, 1998. This software
** was created using the OpenGL(R) version 1.2.1 Sample Implementation
** published by SGI, but has not been independently verified as being
** compliant with the OpenGL(R) version 1.2.1 Specification.
*/

#ifndef GL_APIENTRYP
#   define GL_APIENTRYP GL_APIENTRY*
#endif

/*------------------------------------------------------------------------*
 * IMG extension tokens
 *------------------------------------------------------------------------*/

#define GL_SGX_BINARY_IMG 0x79CD

/*------------------------------------------------------------------------*
 * OES extension tokens
 *------------------------------------------------------------------------*/

/* GL_OES_texture3D */
#ifndef GL_OES_texture3D
#define GL_TEXTURE_WRAP_R_OES									0x8072
#define GL_TEXTURE_3D_OES										0x806F
#define GL_TEXTURE_BINDING_3D_OES								0x806A
#define GL_MAX_3D_TEXTURE_SIZE_OES								0x8073
#define GL_SAMPLER_3D_OES										0x8B5F
#endif

/* GL_OES_mapbuffer */
#ifndef GL_OES_mapbuffer
#define GL_BUFFER_MAPPED_OES									0x88BC
#define GL_BUFFER_MAP_POINTER_OES								0x88BD
#endif

/* GL_OES_rgb8_rgba8 */
#ifndef GL_OES_rgb8_rgba8
#define GL_RGB8_OES												0x8051
#define GL_RGBA8_OES											0x8058
#endif

/* GL_OES_depth24 */
#ifndef GL_OES_depth24
#define GL_DEPTH_COMPONENT24_OES								0x81A6
#endif

/* GL_OES_depth32 */
#ifndef GL_OES_depth32
#define GL_DEPTH_COMPONENT32_OES								0x81A7
#endif

/* GL_OES_stencil1 */
#ifndef GL_OES_stencil1
#define GL_STENCIL_INDEX1_OES									0x8D46
#endif

/* GL_OES_stencil4 */
#ifndef GL_OES_stencil4
#define GL_STENCIL_INDEX4_OES									0x8D47
#endif

/* GL_OES_stencil8 */
#ifndef GL_OES_stencil8
#define GL_STENCIL_INDEX8_OES									0x8D48
#endif

/* GL_OES_vertex_half_float */
#ifndef GL_OES_vertex_half_float
#define GL_HALF_FLOAT_OES										0x8D61
#endif

/* GL_OES_texture_half_float */
/* GL_OES_texture_float */
/* GL_HALF_FLOAT_OES defined in GL_OES_vertex_half_float already. */

/* GL_OES_compressed_ETC1_RGB8_texture */
#ifndef GL_OES_compressed_ETC1_RGB8_texture
#define GL_ETC1_RGB8_OES										0x8D64
#endif

/* GL_OES_depth_texture */
#ifndef GL_OES_depth_texture
/* #define GL_UNSIGNED_INT_24_OES								0x???? */
/* #define GL_UNSIGNED_INT_32_OES								0x???? */
#define GL_DEPTH_TEXTURE_MODE_OES								0x884B
#endif

/* GL_OES_compressed_paletted_texture */
#ifndef GL_OES_compressed_paletted_texture
#define GL_PALETTE4_RGB8_OES									0x8B90
#define GL_PALETTE4_RGBA8_OES									0x8B91
#define GL_PALETTE4_R5_G6_B5_OES								0x8B92
#define GL_PALETTE4_RGBA4_OES									0x8B93
#define GL_PALETTE4_RGB5_A1_OES									0x8B94
#define GL_PALETTE8_RGB8_OES									0x8B95
#define GL_PALETTE8_RGBA8_OES									0x8B96
#define GL_PALETTE8_R5_G6_B5_OES								0x8B97
#define GL_PALETTE8_RGBA4_OES									0x8B98
#define GL_PALETTE8_RGB5_A1_OES									0x8B99
#endif

/*------------------------------------------------------------------------*
 * OES extension functions
 *------------------------------------------------------------------------*/

/* GL_OES_texture3D */
#ifndef GL_OES_texture3D
#define GL_OES_texture3D 1
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glTexImage3DOES (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
GL_APICALL void GL_APIENTRY glTexSubImage3DOES (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
GL_APICALL void GL_APIENTRY glCopyTexSubImage3DOES (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GL_APICALL void GL_APIENTRY glCompressedTexImage3DOES (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
GL_APICALL void GL_APIENTRY glCompressedTexSubImage3DOES (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
#endif
typedef void (GL_APIENTRYP PFNGLTEXIMAGE3DOESPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
typedef void (GL_APIENTRYP PFNGLTEXSUBIMAGE3DOESPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
typedef void (GL_APIENTRYP PFNGLCOPYTEXSUBIMAGE3DOESPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GL_APIENTRYP PFNGLCOMPRESSEDTEXIMAGE3DOESPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
typedef void (GL_APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE3DOESPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
#endif

/* GL_OES_mapbuffer */
#ifndef GL_OES_mapbuffer
#define GL_OES_mapbuffer 1
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void* GL_APIENTRY glMapBufferOES (GLenum target, GLenum access);
GL_APICALL GLboolean GL_APIENTRY glUnmapBufferOES (GLenum target);
#endif
typedef void* (GL_APIENTRYP PFNGLMAPBUFFEROESPROC) (GLenum target, GLenum access);
typedef GLboolean (GL_APIENTRYP PFNGLUNMAPBUFFEROESPROC) (GLenum target);
#endif

/* GL_OES_rgb8_rgba8 */
#ifndef GL_OES_rgb8_rgba8
#define GL_OES_rgb8_rgba8 1
#endif

/* GL_OES_depth24 */
#ifndef GL_OES_depth24
#define GL_OES_depth24 1
#endif

/* GL_OES_depth32 */
#ifndef GL_OES_depth32
#define GL_OES_depth32 1
#endif

/* GL_OES_stencil1 */
#ifndef GL_OES_stencil1
#define GL_OES_stencil1 1
#endif

/* GL_OES_stencil4 */
#ifndef GL_OES_stencil4
#define GL_OES_stencil4 1
#endif

/* GL_OES_stencil8 */
#ifndef GL_OES_stencil8
#define GL_OES_stencil8 1
#endif

/* GL_OES_vertex_half_float */
#ifndef GL_OES_vertex_half_float
#define GL_OES_vertex_half_float 1
#endif

/* GL_OES_texture_half_float */
#ifndef GL_OES_texture_half_float
#define GL_OES_texture_half_float 1
#endif

/* GL_OES_texture_float */
#ifndef GL_OES_texture_float
#define GL_OES_texture_float 1
#endif

/* GL_OES_compressed_ETC1_RGB8_texture */
#ifndef GL_OES_compressed_ETC1_RGB8_texture
#define GL_OES_compressed_ETC1_RGB8_texture 1
#endif

/* GL_OES_depth_texture */
#ifndef GL_OES_depth_texture
#define GL_OES_depth_texture 1
#endif

/* GL_OES_compressed_paletted_texture */
#ifndef GL_OES_compressed_paletted_texture
#define GL_OES_compressed_paletted_texture 1
#endif

#ifdef __cplusplus
}
#endif

#endif /* __gl2ext_h_ */

//=======================================================================
//
// This confidential and proprietary software may be used only as
// authorized by a licensing agreement from Siliconarts Inc.
//
//   (C) Copyright 2010 ~ 2013  Siliconarts Inc. ALL RIGHTS RESERVED
//
// The entire notice above must be reproduced on all authorized
// copies and copies may only be made to the extent permitted
// by a licensing agreement from Siliconarts.
//
// Project : API
// DATE    : 1/26/2014 Sun
// Author  : Sang Gon, Kim (sgkim@siliconarts.co.kr)
//
//=======================================================================
#include <GLES/gl.h>

/* Available only in Common profile */
GL_API void GL_APIENTRY glAlphaFunc (GLenum func, GLclampf ref) {};
GL_API void GL_APIENTRY glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {};
GL_API void GL_APIENTRY glClearDepthf (GLclampf depth) {};
GL_API void GL_APIENTRY glClipPlanef (GLenum plane, const GLfloat *equation) {};
GL_API void GL_APIENTRY glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {};
GL_API void GL_APIENTRY glDepthRangef (GLclampf zNear, GLclampf zFar) {};
GL_API void GL_APIENTRY glFogf (GLenum pname, GLfloat param) {};
GL_API void GL_APIENTRY glFogfv (GLenum pname, const GLfloat *params) {};
GL_API void GL_APIENTRY glFrustumf (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar) {};
GL_API void GL_APIENTRY glGetClipPlanef (GLenum pname, GLfloat eqn[4]) {};
GL_API void GL_APIENTRY glGetFloatv (GLenum pname, GLfloat *params) {};
GL_API void GL_APIENTRY glGetLightfv (GLenum light, GLenum pname, GLfloat *params) {};
GL_API void GL_APIENTRY glGetMaterialfv (GLenum face, GLenum pname, GLfloat *params) {};
GL_API void GL_APIENTRY glGetTexEnvfv (GLenum env, GLenum pname, GLfloat *params) {};
GL_API void GL_APIENTRY glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params) {};
GL_API void GL_APIENTRY glLightModelf (GLenum pname, GLfloat param) {};
GL_API void GL_APIENTRY glLightModelfv (GLenum pname, const GLfloat *params) {};
GL_API void GL_APIENTRY glLightf (GLenum light, GLenum pname, GLfloat param) {};
GL_API void GL_APIENTRY glLightfv (GLenum light, GLenum pname, const GLfloat *params) {};
GL_API void GL_APIENTRY glLineWidth (GLfloat width) {};
GL_API void GL_APIENTRY glLoadMatrixf (const GLfloat *m) {};
GL_API void GL_APIENTRY glMaterialf (GLenum face, GLenum pname, GLfloat param) {};
GL_API void GL_APIENTRY glMaterialfv (GLenum face, GLenum pname, const GLfloat *params) {};
GL_API void GL_APIENTRY glMultMatrixf (const GLfloat *m) {};
GL_API void GL_APIENTRY glMultiTexCoord4f (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) {};
GL_API void GL_APIENTRY glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz) {};
GL_API void GL_APIENTRY glOrthof (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar) {};
GL_API void GL_APIENTRY glPointParameterf (GLenum pname, GLfloat param) {};
GL_API void GL_APIENTRY glPointParameterfv (GLenum pname, const GLfloat *params) {};
GL_API void GL_APIENTRY glPointSize (GLfloat size) {};
GL_API void GL_APIENTRY glPolygonOffset (GLfloat factor, GLfloat units) {};
GL_API void GL_APIENTRY glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {};
GL_API void GL_APIENTRY glScalef (GLfloat x, GLfloat y, GLfloat z) {};
GL_API void GL_APIENTRY glTexEnvf (GLenum target, GLenum pname, GLfloat param) {};
GL_API void GL_APIENTRY glTexEnvfv (GLenum target, GLenum pname, const GLfloat *params) {};
GL_API void GL_APIENTRY glTexParameterf (GLenum target, GLenum pname, GLfloat param) {};
GL_API void GL_APIENTRY glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params) {};
GL_API void GL_APIENTRY glTranslatef (GLfloat x, GLfloat y, GLfloat z) {};

/* Available in both Common and Common-Lite profiles */
GL_API void GL_APIENTRY glActiveTexture (GLenum texture) {};
GL_API void GL_APIENTRY glAlphaFuncx (GLenum func, GLclampx ref) {};
GL_API void GL_APIENTRY glBindBuffer (GLenum target, GLuint buffer) {};
GL_API void GL_APIENTRY glBindTexture (GLenum target, GLuint texture) {};
GL_API void GL_APIENTRY glBlendFunc (GLenum sfactor, GLenum dfactor) {};
GL_API void GL_APIENTRY glBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) {};
GL_API void GL_APIENTRY glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data) {};
GL_API void GL_APIENTRY glClear (GLbitfield mask) {};
GL_API void GL_APIENTRY glClearColorx (GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha) {};
GL_API void GL_APIENTRY glClearDepthx (GLclampx depth) {};
GL_API void GL_APIENTRY glClearStencil (GLint s) {};
GL_API void GL_APIENTRY glClientActiveTexture (GLenum texture) {};
GL_API void GL_APIENTRY glClipPlanex (GLenum plane, const GLfixed *equation) {};
GL_API void GL_APIENTRY glColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) {};
GL_API void GL_APIENTRY glColor4x (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha) {};
GL_API void GL_APIENTRY glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) {};
GL_API void GL_APIENTRY glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {};
GL_API void GL_APIENTRY glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data) {};
GL_API void GL_APIENTRY glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data) {};
GL_API void GL_APIENTRY glCopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {};
GL_API void GL_APIENTRY glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {};
GL_API void GL_APIENTRY glCullFace (GLenum mode) {};
GL_API void GL_APIENTRY glDeleteBuffers (GLsizei n, const GLuint *buffers) {};
GL_API void GL_APIENTRY glDeleteTextures (GLsizei n, const GLuint *textures) {};
GL_API void GL_APIENTRY glDepthFunc (GLenum func) {};
GL_API void GL_APIENTRY glDepthMask (GLboolean flag) {};
GL_API void GL_APIENTRY glDepthRangex (GLclampx zNear, GLclampx zFar) {};
GL_API void GL_APIENTRY glDisable (GLenum cap) {};
GL_API void GL_APIENTRY glDisableClientState (GLenum array) {};
GL_API void GL_APIENTRY glDrawArrays (GLenum mode, GLint first, GLsizei count) {};
GL_API void GL_APIENTRY glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) {};
GL_API void GL_APIENTRY glEnable (GLenum cap) {};
GL_API void GL_APIENTRY glEnableClientState (GLenum array) {};
GL_API void GL_APIENTRY glFinish (void) {};
GL_API void GL_APIENTRY glFlush (void) {};
GL_API void GL_APIENTRY glFogx (GLenum pname, GLfixed param) {};
GL_API void GL_APIENTRY glFogxv (GLenum pname, const GLfixed *params) {};
GL_API void GL_APIENTRY glFrontFace (GLenum mode) {};
GL_API void GL_APIENTRY glFrustumx (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar) {};
GL_API void GL_APIENTRY glGetBooleanv (GLenum pname, GLboolean *params) {};
GL_API void GL_APIENTRY glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params) {};
GL_API void GL_APIENTRY glGetClipPlanex (GLenum pname, GLfixed eqn[4]) {};
GL_API void GL_APIENTRY glGenBuffers (GLsizei n, GLuint *buffers) {};
GL_API void GL_APIENTRY glGenTextures (GLsizei n, GLuint *textures) {};
GL_API GLenum GL_APIENTRY glGetError (void) { return GL_NO_ERROR; };
GL_API void GL_APIENTRY glGetFixedv (GLenum pname, GLfixed *params) {};
GL_API void GL_APIENTRY glGetIntegerv (GLenum pname, GLint *params) {};
GL_API void GL_APIENTRY glGetLightxv (GLenum light, GLenum pname, GLfixed *params) {};
GL_API void GL_APIENTRY glGetMaterialxv (GLenum face, GLenum pname, GLfixed *params) {};
GL_API void GL_APIENTRY glGetPointerv (GLenum pname, GLvoid **params) {};
GL_API const GLubyte * GL_APIENTRY glGetString (GLenum name) { return 0; };
GL_API void GL_APIENTRY glGetTexEnviv (GLenum env, GLenum pname, GLint *params) {};
GL_API void GL_APIENTRY glGetTexEnvxv (GLenum env, GLenum pname, GLfixed *params) {};
GL_API void GL_APIENTRY glGetTexParameteriv (GLenum target, GLenum pname, GLint *params) {};
GL_API void GL_APIENTRY glGetTexParameterxv (GLenum target, GLenum pname, GLfixed *params) {};
GL_API void GL_APIENTRY glHint (GLenum target, GLenum mode) {};
GL_API GLboolean GL_APIENTRY glIsBuffer (GLuint buffer) { return GL_FALSE; };
GL_API GLboolean GL_APIENTRY glIsEnabled (GLenum cap) { return GL_FALSE; };
GL_API GLboolean GL_APIENTRY glIsTexture (GLuint texture) { return GL_FALSE; };
GL_API void GL_APIENTRY glLightModelx (GLenum pname, GLfixed param) {};
GL_API void GL_APIENTRY glLightModelxv (GLenum pname, const GLfixed *params) {};
GL_API void GL_APIENTRY glLightx (GLenum light, GLenum pname, GLfixed param) {};
GL_API void GL_APIENTRY glLightxv (GLenum light, GLenum pname, const GLfixed *params) {};
GL_API void GL_APIENTRY glLineWidthx (GLfixed width) {};
GL_API void GL_APIENTRY glLoadIdentity (void) {};
GL_API void GL_APIENTRY glLoadMatrixx (const GLfixed *m) {};
GL_API void GL_APIENTRY glLogicOp (GLenum opcode) {};
GL_API void GL_APIENTRY glMaterialx (GLenum face, GLenum pname, GLfixed param) {};
GL_API void GL_APIENTRY glMaterialxv (GLenum face, GLenum pname, const GLfixed *params) {};
GL_API void GL_APIENTRY glMatrixMode (GLenum mode) {};
GL_API void GL_APIENTRY glMultMatrixx (const GLfixed *m) {};
GL_API void GL_APIENTRY glMultiTexCoord4x (GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q) {};
GL_API void GL_APIENTRY glNormal3x (GLfixed nx, GLfixed ny, GLfixed nz) {};
GL_API void GL_APIENTRY glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer) {};
GL_API void GL_APIENTRY glOrthox (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar) {};
GL_API void GL_APIENTRY glPixelStorei (GLenum pname, GLint param) {};
GL_API void GL_APIENTRY glPointParameterx (GLenum pname, GLfixed param) {};
GL_API void GL_APIENTRY glPointParameterxv (GLenum pname, const GLfixed *params) {};
GL_API void GL_APIENTRY glPointSizex (GLfixed size) {};
GL_API void GL_APIENTRY glPolygonOffsetx (GLfixed factor, GLfixed units) {};
GL_API void GL_APIENTRY glPopMatrix (void) {};
GL_API void GL_APIENTRY glPushMatrix (void) {};
GL_API void GL_APIENTRY glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels) {};
GL_API void GL_APIENTRY glRotatex (GLfixed angle, GLfixed x, GLfixed y, GLfixed z) {};
GL_API void GL_APIENTRY glSampleCoverage (GLclampf value, GLboolean invert) {};
GL_API void GL_APIENTRY glSampleCoveragex (GLclampx value, GLboolean invert) {};
GL_API void GL_APIENTRY glScalex (GLfixed x, GLfixed y, GLfixed z) {};
GL_API void GL_APIENTRY glScissor (GLint x, GLint y, GLsizei width, GLsizei height) {};
GL_API void GL_APIENTRY glShadeModel (GLenum mode) {};
GL_API void GL_APIENTRY glStencilFunc (GLenum func, GLint ref, GLuint mask) {};
GL_API void GL_APIENTRY glStencilMask (GLuint mask) {};
GL_API void GL_APIENTRY glStencilOp (GLenum fail, GLenum zfail, GLenum zpass) {};
GL_API void GL_APIENTRY glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {};
GL_API void GL_APIENTRY glTexEnvi (GLenum target, GLenum pname, GLint param) {};
GL_API void GL_APIENTRY glTexEnvx (GLenum target, GLenum pname, GLfixed param) {};
GL_API void GL_APIENTRY glTexEnviv (GLenum target, GLenum pname, const GLint *params) {};
GL_API void GL_APIENTRY glTexEnvxv (GLenum target, GLenum pname, const GLfixed *params) {};
GL_API void GL_APIENTRY glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels) {};
GL_API void GL_APIENTRY glTexParameteri (GLenum target, GLenum pname, GLint param) {};
GL_API void GL_APIENTRY glTexParameterx (GLenum target, GLenum pname, GLfixed param) {};
GL_API void GL_APIENTRY glTexParameteriv (GLenum target, GLenum pname, const GLint *params) {};
GL_API void GL_APIENTRY glTexParameterxv (GLenum target, GLenum pname, const GLfixed *params) {};
GL_API void GL_APIENTRY glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels) {};
GL_API void GL_APIENTRY glTranslatex (GLfixed x, GLfixed y, GLfixed z) {};
GL_API void GL_APIENTRY glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {};
GL_API void GL_APIENTRY glViewport (GLint x, GLint y, GLsizei width, GLsizei height) {};

/*------------------------------------------------------------------------*
 * Required OES extension functions
 *------------------------------------------------------------------------*/
/* GL_OES_point_size_array */
GL_API void GL_APIENTRY glPointSizePointerOES (GLenum type, GLsizei stride, const GLvoid *pointer) {};


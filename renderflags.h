
// renderflags.h

#ifndef RENDERFLAGS_H
#define RENDERFLAGS_H

#include <windows.h>
#include <gl\gl.h>

extern const int render_blend;
extern const int render_texture;
extern const int render_depthtest;
extern const int render_lighting;
extern const int render_wireframe;

void renderenable(const int&);
void renderdisable(const int&);
void renderset(const int&, const BOOL&);
void renderinit(void);

/* void twBindTexture(const GLenum&, const GLuint&); */

#endif

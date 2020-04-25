
// renderflags.cpp

#include "renderflags.h"

//--------------------------------------------------------
// TCW
//
// Rendering feature flags
//
// This code was added after noticing the screen flashing
// white after making the debris untextured.  I remembered
//	reading that glEnabling GL_TEXTURE_2D unnecessarily
// causes problems with GeForce cards, and also that 
// calling glEnable and glDisable is a big waste of time,
// so I came up with this system of internal flags to 
// control the various rendering options.  It could be
// expanded to include a lot more, but this is all that
// I need for this game.
//--------------------------------------------------------
int renderflags = 0;

const int render_blend		= 1;
const int render_texture	= 2;
const int render_depthtest	= 4;
const int render_lighting	= 8;
const int render_wireframe = 16;

void renderenable(const int& flags)
{
	if (((flags & render_blend) == render_blend) && ((renderflags & render_blend) != render_blend))
		glEnable(GL_BLEND);
 
	if (((flags & render_texture) == render_texture) && ((renderflags & render_texture) != render_texture))
		glEnable(GL_TEXTURE_2D);

	if (((flags & render_depthtest) == render_depthtest) && ((renderflags & render_depthtest) != render_depthtest))
		glEnable(GL_DEPTH_TEST);

	if (((flags & render_lighting) == render_lighting) && ((renderflags & render_lighting) != render_lighting))
		glEnable(GL_LIGHTING);

	if (((flags & render_wireframe) == render_wireframe) && ((renderflags & render_wireframe) != render_wireframe))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	renderflags = renderflags | flags;
}
 
void renderdisable(const int& flags)
{
	if (((flags & render_blend) == render_blend) && ((renderflags & render_blend) == render_blend))
		glDisable(GL_BLEND);

	if (((flags & render_texture) == render_texture) && ((renderflags & render_texture) == render_texture))
		glDisable(GL_TEXTURE_2D);

	if (((flags & render_depthtest) == render_depthtest) && ((renderflags & render_depthtest) == render_depthtest))
		glDisable(GL_DEPTH_TEST);

	if (((flags & render_lighting) == render_lighting) && ((renderflags & render_lighting) == render_lighting))
		glDisable(GL_LIGHTING);

	if (((flags & render_wireframe) == render_wireframe) && ((renderflags & render_wireframe) == render_wireframe))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	renderflags = renderflags - (renderflags & flags);
}

void renderset(const int& flags, const BOOL& value)
{
	if (value)
		renderenable(flags);
	else
		renderdisable(flags);
}

// This routine is used to reset the rendering
// options after reinitializing the window during
// fullscreen/window toggle
void renderinit(void)
{
	if ((renderflags & render_blend) == render_blend)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
 
	if ((renderflags & render_texture) == render_texture) 
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
	
	if ((renderflags & render_depthtest) == render_depthtest) 
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	if ((renderflags & render_lighting) == render_lighting) 
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	if ((renderflags & render_wireframe) == render_wireframe) 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}
 
/*
GLenum PrevTarget = NULL;
GLuint PrevTexture = NULL;

void twBindTexture(const GLenum& target, const GLuint& texture)
{
	if ((target != PrevTarget) || (PrevTexture != texture))
	{
		glBindTexture(target, texture);
		
		PrevTarget = target;
		PrevTexture = texture;
	}
}
*/
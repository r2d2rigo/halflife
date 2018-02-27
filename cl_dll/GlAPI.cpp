#ifdef _WIN32
#define HSPRITE DeletedWinapi_HSPRITE// prevent windows.h from defining it
#include <windows.h>
#undef HSPRITE
#endif

#include "GL/glew.h"
#include "GlAPI.h"

CGlAPI::CGlAPI(void)
{
}

void CGlAPI::Begin(const GlBeginMode &mode)
{
	switch (mode)
	{
	case GlBeginMode::Lines:
	{
		glBegin(GL_LINES);
	}
	break;
	}
}

void CGlAPI::End()
{
	glEnd();
}

void CGlAPI::Color4f(const float &r, const float &g, const float &b, const float &a)
{
	glColor4f(r, g, b, a);
}

void CGlAPI::Color4ub(const byte &r, const byte &g, const byte &b, const byte &a)
{
	glColor4ub(r, g, b, a);
}

void CGlAPI::Vertex3f(const float &x, const float &y, const float &z)
{
	glVertex3f(x, y, z);
}

void CGlAPI::Vertex3fv(const float *v)
{
	glVertex3fv(v);
}

void CGlAPI::LineWidth(const float &width)
{
	glLineWidth(width);
}

void CGlAPI::Enable(const GlEnableMode &enableMode)
{
	switch (enableMode)
	{
	case GlEnableMode::Texture2D:
	{
		glEnable(GL_TEXTURE_2D);
	}
	break;
	case GlEnableMode::DepthTest:
	{
		glEnable(GL_DEPTH_TEST);
	}
	break;
	case GlEnableMode::DepthWrite:
	{
		glDepthMask(true);
	}
	break;
	}
}

void CGlAPI::Disable(const GlEnableMode &enableMode)
{
	switch (enableMode)
	{
	case GlEnableMode::Texture2D:
	{
		glDisable(GL_TEXTURE_2D);
	}
	break;
	case GlEnableMode::DepthTest:
	{
		glDisable(GL_DEPTH_TEST);
	}
	break;
	case GlEnableMode::DepthWrite:
	{
		glDepthMask(false);
	}
	break;
	}
}
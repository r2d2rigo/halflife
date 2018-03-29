#if !defined( GLAPI_H )
#define GLAPI_H
#if defined( _WIN32 )
#pragma once
#endif

enum GlBeginMode
{
	Lines = 0,
	Triangles = 1,
	Quads = 2,
};

enum GlEnableMode
{
	Texture2D = 0,
	DepthTest = 1,
	DepthWrite = 2,
	CullFace = 3,
};

class CGlAPI
{
public:
	CGlAPI(void);

	void Begin(const GlBeginMode &beginMode);
	void End();

	void Color4f(const float &r, const float &g, const float &b, const float &a);
	void Color4ub(const byte &r, const byte &g, const byte &b, const byte &a);

	void Vertex3f(const float &x, const float &y, const float &z);
	void Vertex3fv(const float *v);

	void Normal3f(const float &x, const float &y, const float &z);

	void LineWidth(const float &width);
	
	void Enable(const GlEnableMode &enableMode);
	void Disable(const GlEnableMode &enableMode);
};

#endif
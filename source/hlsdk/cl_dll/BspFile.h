#if !defined( BSPFILE_H )
#define BSPFILE_H
#if defined( _WIN32 )
#pragma once
#endif

#include <string>
#include <vector>
#include "hud.h"

struct BspLump
{
	int Offset;
	int Length;
};

struct BspHeader
{
	int Version;
	BspLump Lumps[17];
};

struct BspCubemap
{
	int Position[3];
	int Size;
	int Offset;
	byte *Data;
};

struct BspLeafAmbientLight
{
	int Position[3];
	byte AmbientColor[6][3];
};

class CBspFile
{
public:
	BspHeader Header;
	std::vector<BspCubemap> Cubemaps;
	std::vector<BspLeafAmbientLight> AmbientLights;

public:
	CBspFile(void);
	void LoadBsp(const std::string &filename);
};

#endif
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
	BspLump Lumps[16];
};

struct BspCubemap
{
	int Position[3];
	int Size;
	int Offset;
	byte *Data;
};

class CBspFile
{
public:
	BspHeader Header;
	std::vector<BspCubemap> Cubemaps;

public:
	CBspFile(void);
	void LoadBsp(const std::string &filename);
};

#endif
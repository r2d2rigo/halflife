#include <fstream>
#include "BspFile.h"

CBspFile::CBspFile(void)
{
}

void CBspFile::LoadBsp(const std::string &filename)
{
	std::ifstream fileStream;
	fileStream.open(filename, std::ios::in | std::ios::binary);

	if (fileStream.is_open())
	{
		fileStream.read((char*)&Header.Version, sizeof(int));

		for (int i = 0; i < 16; i++)
		{
			fileStream.read((char*)&Header.Lumps[i], sizeof(BspLump));
		}

		fileStream.seekg(Header.Lumps[15].Offset);
		int cubemapCount;
		fileStream.read((char*)&cubemapCount, sizeof(int));

		for (int i = 0; i < cubemapCount; i++)
		{
			BspCubemap cubemap;

			fileStream.read((char*)&cubemap.Position, sizeof(int) * 3);
			fileStream.read((char*)&cubemap.Size, sizeof(int));

			int dataSize = 6 * (cubemap.Size *cubemap.Size) * 3;
			cubemap.Data = new byte[dataSize];

			fileStream.read((char*)cubemap.Data, dataSize);

			Cubemaps.insert(Cubemaps.end(), cubemap);
		}

		fileStream.close();
	}
}
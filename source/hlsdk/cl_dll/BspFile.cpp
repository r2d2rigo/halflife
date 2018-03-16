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

		for (int i = 0; i < 8; i++)
		{
			BspCubemap cubemap;

			fileStream.read((char*)&cubemap.Position, sizeof(int) * 3);
			fileStream.read((char*)&cubemap.Size, sizeof(int));
			fileStream.read((char*)&cubemap.Offset, sizeof(int));

			int cubemapSize = 32;

			if (cubemap.Size > 0)
			{
				cubemapSize = (int)pow(2, cubemap.Size - 1);
			}

			int dataSize = 6 * (cubemapSize * cubemapSize ) * 3;
			cubemap.Data = new byte[dataSize];

			int oldFileOffset = fileStream.tellg();
			fileStream.seekg(Header.Lumps[15].Offset + cubemap.Offset);
			fileStream.read((char*)cubemap.Data, dataSize);
			fileStream.seekg(oldFileOffset);

			Cubemaps.insert(Cubemaps.end(), cubemap);
		}

		fileStream.close();
	}
}
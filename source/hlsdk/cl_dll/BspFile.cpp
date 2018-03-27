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

		for (int i = 0; i < 17; i++)
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
			fileStream.read((char*)&cubemap.Offset, sizeof(int));

			int cubemapSize = 32;

			if (cubemap.Size > 0)
			{
				cubemapSize = (int)pow(2, cubemap.Size - 1);
			}

			int dataSize = 6 * (cubemapSize * cubemapSize ) * 3;
			cubemap.Data = new byte[dataSize];
			cubemap.Size = cubemapSize;

			int oldFileOffset = fileStream.tellg();
			fileStream.seekg(Header.Lumps[15].Offset + cubemap.Offset);
			fileStream.read((char*)cubemap.Data, dataSize);
			fileStream.seekg(oldFileOffset);

			Cubemaps.insert(Cubemaps.end(), cubemap);
		}

		fileStream.seekg(Header.Lumps[16].Offset);

		for (int i = 0; i < 44; i++)
		{ 
			BspLeafAmbientLight ambientLight;
			int padding;

			fileStream.read((char*)&ambientLight.Position, sizeof(int) * 3);
			fileStream.read((char*)&ambientLight.AmbientColor, sizeof(byte) * 6 * 3);
			fileStream.read((char*)&padding, sizeof(byte) * 2);

			AmbientLights.insert(AmbientLights.end(), ambientLight);
		}

		fileStream.close();
	}
}
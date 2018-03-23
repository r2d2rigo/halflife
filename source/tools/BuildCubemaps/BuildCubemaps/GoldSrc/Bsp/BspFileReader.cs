using OpenTK;
using System;
using System.Collections.Generic;
using System.IO;

namespace GoldSrc.Bsp
{
    public class BspFileReader
    {
        private Stream _stream;

        public BspFileReader(Stream stream)
        {
            _stream = stream;
        }

        public BspFile ReadBspFile()
        {
            using (var reader = new BinaryReader(_stream))
            {
                var lumpParseActions = new Dictionary<LumpTypes, Action<BspLump, BspFile>>();
                lumpParseActions.Add(LumpTypes.Entities, (l, f) => ReadEntitiesLump(l, f));
                lumpParseActions.Add(LumpTypes.Planes, (l, f) => ReadPlanesLump(l, f));
                lumpParseActions.Add(LumpTypes.Textures, (l, f) => ReadTexturesLump(l, f));
                lumpParseActions.Add(LumpTypes.Vertices, (l, f) => ReadVerticesLump(l, f));
                lumpParseActions.Add(LumpTypes.Visibility, (l, f) => ReadVisibilityLump(l, f));
                lumpParseActions.Add(LumpTypes.Nodes, (l, f) => ReadNodesLump(l, f));
                lumpParseActions.Add(LumpTypes.TexInfo, (l, f) => ReadTexInfoLump(l, f));
                lumpParseActions.Add(LumpTypes.Faces, (l, f) => ReadFacesLump(l, f));
                lumpParseActions.Add(LumpTypes.Lighting, (l, f) => ReadLightingData(l, f));
                lumpParseActions.Add(LumpTypes.ClipNodes, (l, f) => ReadClipNodesData(l, f));
                lumpParseActions.Add(LumpTypes.Leaves, (l, f) => ReadLeavesLump(l, f));
                lumpParseActions.Add(LumpTypes.MarkSurfaces, (l, f) => ReadMarkSurfacesLump(l, f));
                lumpParseActions.Add(LumpTypes.Edges, (l, f) => ReadEdgesLump(l, f));
                lumpParseActions.Add(LumpTypes.SurfEdges, (l, f) => ReadSurfEdgesLump(l, f));
                lumpParseActions.Add(LumpTypes.Models, (l, f) => ReadModelsData(l, f));
                lumpParseActions.Add(LumpTypes.Cubemaps, (l, f) => ReadCubemapsLump(l, f));

                var bspFile = new BspFile();
                var bspHeader = ReadHeader(reader);

                for (int i = 0; i < (int)LumpTypes.Maximum; i++)
                {
                    var currentLump = bspHeader.Lumps[i];
                    reader.BaseStream.Seek(currentLump.Offset, SeekOrigin.Begin);
                    currentLump.Data = reader.ReadBytes(currentLump.Length);

                    if (lumpParseActions.ContainsKey((LumpTypes)i))
                    {
                        lumpParseActions[(LumpTypes)i].Invoke(currentLump, bspFile);
                    }
                }

                return bspFile;
            }
        }

        private BspHeader ReadHeader(BinaryReader reader)
        {
            var maxLumps = (int)LumpTypes.Maximum;
            var header = new BspHeader();

            header.Version = reader.ReadInt32();
            header.Lumps = new BspLump[maxLumps];

            for (int i = 0; i < maxLumps; i++)
            {
                BspLump newLump = new BspLump();
                newLump.Offset = reader.ReadInt32();
                newLump.Length = reader.ReadInt32();

                header.Lumps[i] = newLump;
            }

            return header;
        }

        private void ReadVerticesLump(BspLump lumpData, BspFile bspFile)
        {
            List<Vector3> lumpVertices = new List<Vector3>();

            using (var reader = new BinaryReader(new MemoryStream(lumpData.Data)))
            {
                while (reader.BaseStream.Position < reader.BaseStream.Length)
                {
                    var newVertex = new Vector3();
                    newVertex.X = reader.ReadSingle();
                    newVertex.Z = reader.ReadSingle();
                    newVertex.Y = reader.ReadSingle();

                    lumpVertices.Add(newVertex);
                }
            }

            bspFile.Vertices = lumpVertices.ToArray();
        }

        private void ReadEdgesLump(BspLump lumpData, BspFile bspFile)
        {
            List<BspEdge> lumpEdges = new List<BspEdge>();

            using (var reader = new BinaryReader(new MemoryStream(lumpData.Data)))
            {
                while (reader.BaseStream.Position < reader.BaseStream.Length)
                {
                    var newEdge = new BspEdge();
                    newEdge.Vertices = new UInt16[2];
                    newEdge.Vertices[0] = reader.ReadUInt16();
                    newEdge.Vertices[1] = reader.ReadUInt16();

                    lumpEdges.Add(newEdge);
                }
            }

            bspFile.Edges = lumpEdges.ToArray();
        }

        private void ReadFacesLump(BspLump lumpData, BspFile bspFile)
        {
            List<BspFace> lumpFaces = new List<BspFace>();

            using (var reader = new BinaryReader(new MemoryStream(lumpData.Data)))
            {
                while (reader.BaseStream.Position < reader.BaseStream.Length)
                {
                    var newFace = new BspFace();
                    newFace.Plane = reader.ReadUInt16();
                    newFace.PlaneSide = reader.ReadUInt16();
                    newFace.FirstEdge = reader.ReadUInt32();
                    newFace.EdgeCount = reader.ReadUInt16();
                    newFace.TextureInfo = reader.ReadUInt16();
                    newFace.Styles = reader.ReadBytes(4);
                    newFace.LightmapOffset = reader.ReadUInt32();

                    lumpFaces.Add(newFace);
                }
            }

            bspFile.Faces = lumpFaces.ToArray();
        }

        private void ReadSurfEdgesLump(BspLump lumpData, BspFile bspFile)
        {
            List<Int32> lumpSurfEdges = new List<Int32>();

            using (var reader = new BinaryReader(new MemoryStream(lumpData.Data)))
            {
                while (reader.BaseStream.Position < reader.BaseStream.Length)
                {
                    lumpSurfEdges.Add(reader.ReadInt32());
                }
            }

            bspFile.SurfEdges = lumpSurfEdges.ToArray();
        }

        private void ReadLeavesLump(BspLump lumpData, BspFile bspFile)
        {
            List<BspLeaf> lumpLeaves = new List<BspLeaf>();

            using (var reader = new BinaryReader(new MemoryStream(lumpData.Data)))
            {
                while (reader.BaseStream.Position < reader.BaseStream.Length)
                {
                    var newLeaf = new BspLeaf();
                    newLeaf.Contents = reader.ReadInt32();
                    newLeaf.VisOffset = reader.ReadInt32();
                    newLeaf.Mins = new Int16[3];
                    newLeaf.Mins[0] = reader.ReadInt16();
                    newLeaf.Mins[2] = reader.ReadInt16();
                    newLeaf.Mins[1] = reader.ReadInt16();
                    newLeaf.Maxs = new Int16[3];
                    newLeaf.Maxs[0] = reader.ReadInt16();
                    newLeaf.Maxs[2] = reader.ReadInt16();
                    newLeaf.Maxs[1] = reader.ReadInt16();
                    newLeaf.FirstMarkSurface = reader.ReadUInt16();
                    newLeaf.MarkSurfaceCount = reader.ReadUInt16();
                    newLeaf.AmbientLevels = reader.ReadBytes(4);

                    lumpLeaves.Add(newLeaf);
                }
            }

            bspFile.Leaves = lumpLeaves.ToArray();
        }

        private void ReadMarkSurfacesLump(BspLump lumpData, BspFile bspFile)
        {
            List<UInt16> lumpMarkSurfaces = new List<UInt16>();

            using (var reader = new BinaryReader(new MemoryStream(lumpData.Data)))
            {
                while (reader.BaseStream.Position < reader.BaseStream.Length)
                {
                    lumpMarkSurfaces.Add(reader.ReadUInt16());
                }
            }

            bspFile.MarkSurfaces = lumpMarkSurfaces.ToArray();
        }


        private void ReadTexInfoLump(BspLump lumpData, BspFile bspFile)
        {
            List<BspTextureInfo> lumpTextureInfos = new List<BspTextureInfo>();

            using (var reader = new BinaryReader(new MemoryStream(lumpData.Data)))
            {
                while (reader.BaseStream.Position < reader.BaseStream.Length)
                {
                    var newTexInfo = new BspTextureInfo();
                    newTexInfo.S = new Vector3();
                    newTexInfo.S.X = reader.ReadSingle();
                    newTexInfo.S.Z = reader.ReadSingle();
                    newTexInfo.S.Y = reader.ReadSingle();
                    newTexInfo.SShift = reader.ReadSingle();
                    newTexInfo.T = new Vector3();
                    newTexInfo.T.X = reader.ReadSingle();
                    newTexInfo.T.Z = reader.ReadSingle();
                    newTexInfo.T.Y = reader.ReadSingle();
                    newTexInfo.TShift = reader.ReadSingle();
                    newTexInfo.MipTex = reader.ReadUInt32();
                    newTexInfo.Flags = reader.ReadUInt32();

                    lumpTextureInfos.Add(newTexInfo);
                }
            }

            bspFile.TexInfos = lumpTextureInfos.ToArray();
        }

        private void ReadTexturesLump(BspLump lumpData, BspFile bspFile)
        {
            List<BspMipTexture> lumpTextures = new List<BspMipTexture>();

            using (var reader = new BinaryReader(new MemoryStream(lumpData.Data)))
            {
                while (reader.BaseStream.Position < reader.BaseStream.Length)
                {
                    var textureHeader = new BspTextureHeader();
                    textureHeader.MipTextureCount = reader.ReadUInt32();
                    textureHeader.MipTextureOffsets = new Int32[textureHeader.MipTextureCount];

                    for (int i = 0; i < textureHeader.MipTextureCount; i++)
                    {
                        textureHeader.MipTextureOffsets[i] = reader.ReadInt32();
                    }

                    for (int i = 0; i < textureHeader.MipTextureCount; i++)
                    {
                        reader.BaseStream.Seek(textureHeader.MipTextureOffsets[i], SeekOrigin.Begin);

                        var newMipTexture = new BspMipTexture();
                        newMipTexture.Name = reader.ReadChars(16);
                        newMipTexture.Width = reader.ReadUInt32();
                        newMipTexture.Height = reader.ReadUInt32();
                        newMipTexture.Offsets = new UInt32[4];
                        newMipTexture.Offsets[0] = reader.ReadUInt32();
                        newMipTexture.Offsets[1] = reader.ReadUInt32();
                        newMipTexture.Offsets[2] = reader.ReadUInt32();
                        newMipTexture.Offsets[3] = reader.ReadUInt32();

                        lumpTextures.Add(newMipTexture);
                    }
                }
            }

            bspFile.MipTextures = lumpTextures.ToArray();
        }

        private void ReadNodesLump(BspLump lumpData, BspFile bspFile)
        {
            List<BspNode> lumpNodes = new List<BspNode>();

            using (var reader = new BinaryReader(new MemoryStream(lumpData.Data)))
            {
                while (reader.BaseStream.Position < reader.BaseStream.Length)
                {
                    var newNode = new BspNode();
                    newNode.Plane = reader.ReadUInt32();
                    newNode.Children = new Int16[2];
                    newNode.Children[0] = reader.ReadInt16();
                    newNode.Children[1] = reader.ReadInt16();
                    newNode.Mins = new Int16[3];
                    newNode.Mins[0] = reader.ReadInt16();
                    newNode.Mins[2] = reader.ReadInt16();
                    newNode.Mins[1] = reader.ReadInt16();
                    newNode.Maxs = new Int16[3];
                    newNode.Maxs[0] = reader.ReadInt16();
                    newNode.Maxs[2] = reader.ReadInt16();
                    newNode.Maxs[1] = reader.ReadInt16();
                    newNode.FirstFace = reader.ReadUInt16();
                    newNode.FaceCount = reader.ReadUInt16();

                    lumpNodes.Add(newNode);
                }
            }

            bspFile.Nodes = lumpNodes.ToArray();
        }

        private void ReadVisibilityLump(BspLump lumpData, BspFile bspFile)
        {
            bspFile.VisData = new BspVisData();
            bspFile.VisData.CompressedData = lumpData.Data;
        }

        private void ReadPlanesLump(BspLump lumpData, BspFile bspFile)
        {
            List<BspPlane> lumpPlanes = new List<BspPlane>();

            using (var reader = new BinaryReader(new MemoryStream(lumpData.Data)))
            {
                while (reader.BaseStream.Position < reader.BaseStream.Length)
                {
                    var newPlane = new BspPlane();
                    newPlane.Normal = new Vector3();
                    newPlane.Normal.X = reader.ReadSingle();
                    newPlane.Normal.Z = reader.ReadSingle();
                    newPlane.Normal.Y = reader.ReadSingle();
                    newPlane.Distance = reader.ReadSingle();
                    newPlane.Type = reader.ReadInt32();

                    lumpPlanes.Add(newPlane);
                }
            }

            bspFile.Planes = lumpPlanes.ToArray();
        }

        private void ReadEntitiesLump(BspLump lumpData, BspFile bspFile)
        {
            using (var reader = new BinaryReader(new MemoryStream(lumpData.Data)))
            {
                var lumpEntity = reader.ReadChars(lumpData.Length);

                bspFile.EntityData = new string(lumpEntity);
            }
        }

        private void ReadLightingData(BspLump lumpData, BspFile bspFile)
        {
            bspFile.LightmapData = new BspLightmapData();
            bspFile.LightmapData.Data = lumpData.Data;
        }

        private void ReadClipNodesData(BspLump lumpData, BspFile bspFile)
        {
            bspFile.ClipNodesData = lumpData.Data;
        }

        private void ReadModelsData(BspLump lumpData, BspFile bspFile)
        {
            bspFile.ModelsData = lumpData.Data;
        }

        private void ReadCubemapsLump(BspLump lumpData, BspFile bspFile)
        {
            List<BspCubemap> lumpCubemaps = new List<BspCubemap>();

            using (var reader = new BinaryReader(new MemoryStream(lumpData.Data)))
            {
                while (reader.BaseStream.Position < reader.BaseStream.Length)
                {
                    var newCubemap = new BspCubemap();
                    newCubemap.Position = new int[3];
                    newCubemap.Position[0] = reader.ReadInt32();
                    newCubemap.Position[2] = reader.ReadInt32();
                    newCubemap.Position[1] = reader.ReadInt32();
                    newCubemap.Size = reader.ReadInt32();
                    newCubemap.Offset = reader.ReadInt32();

                    if (newCubemap.Offset > 0)
                    {
                        throw new NotImplementedException("Not implemented yet");
                        // newCubemap.Data = reader.ReadBytes(6 * (newCubemap.Size * newCubemap.Size) * 3);
                    }

                    lumpCubemaps.Add(newCubemap);
                }
            }

            bspFile.Cubemaps = lumpCubemaps.ToArray();
        }
    }
}
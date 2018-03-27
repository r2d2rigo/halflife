using OpenTK;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace GoldSrc.Bsp
{
    public class BspFileWriter
    {
        private Stream _stream;

        public BspFileWriter(Stream stream)
        {
            _stream = stream;
        }

        public void WriteBspFile(BspFile bsp)
        {
            using (var writer = new BinaryWriter(_stream))
            {
                var bspHeader = new BspHeader();
                bspHeader.Version = 30;
                bspHeader.Lumps = new BspLump[(int)LumpTypes.Maximum];

                writer.Write(bspHeader.Version);
                foreach (var lump in bspHeader.Lumps)
                {
                    writer.Write(lump.Offset);
                    writer.Write(lump.Length);
                }

                bspHeader.Lumps[(int)LumpTypes.Planes] = WriteLump(WritePlanesLump(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.Leaves] = WriteLump(WriteLeavesLump(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.Vertices] = WriteLump(WriteVerticesLump(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.Nodes] = WriteLump(WriteNodesLump(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.TexInfo] = WriteLump(WriteTexInfoLump(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.Faces] = WriteLump(WriteFacesLump(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.ClipNodes] = WriteLump(WriteClipNodesLump(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.MarkSurfaces] = WriteLump(WriteMarkSurfacesLump(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.SurfEdges] = WriteLump(WriteSurfEdgesLump(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.Edges] = WriteLump(WriterEdgesLump(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.Models] = WriteLump(WriteModelsLump(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.Lighting] = WriteLump(WriteLightingData(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.Visibility] = WriteLump(WriteVisibilityLump(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.Entities] = WriteLump(WriteEntitiesLump(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.Textures] = WriteLump(WriteTexturesLump(bsp), writer);
                bspHeader.Lumps[(int)LumpTypes.Cubemaps] = WriteLump(WriteCubemapsLump(bsp), writer);

                writer.BaseStream.Seek(0, SeekOrigin.Begin);

                writer.Write(bspHeader.Version);
                foreach (var lump in bspHeader.Lumps)
                {
                    writer.Write(lump.Offset);
                    writer.Write(lump.Length);
                }
            }
        }

        private byte[] WriteVerticesLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    foreach (var vertex in bspFile.Vertices)
                    {
                        writer.Write(vertex.X);
                        writer.Write(vertex.Z);
                        writer.Write(vertex.Y);
                    }

                    return stream.ToArray();
                }
            }
        }

        private byte[] WriterEdgesLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    foreach (var edge in bspFile.Edges)
                    {
                        writer.Write(edge.Vertices[0]);
                        writer.Write(edge.Vertices[1]);
                    }

                    return stream.ToArray();
                }
            }
        }

        private byte[] WriteFacesLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    foreach (var face in bspFile.Faces)
                    {
                        writer.Write(face.Plane);
                        writer.Write(face.PlaneSide);
                        writer.Write(face.FirstEdge);
                        writer.Write(face.EdgeCount);
                        writer.Write(face.TextureInfo);
                        writer.Write(face.Styles);
                        writer.Write(face.LightmapOffset);
                    }

                    return stream.ToArray();
                }
            }
        }

        private byte[] WriteSurfEdgesLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    foreach (var surfEdge in bspFile.SurfEdges)
                    {
                        writer.Write(surfEdge);
                    }

                    return stream.ToArray();
                }
            }
        }

        private byte[] WriteLeavesLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    foreach (var leaf in bspFile.Leaves)
                    {
                        writer.Write(leaf.Contents);
                        writer.Write(leaf.VisOffset);
                        writer.Write(leaf.Mins[0]);
                        writer.Write(leaf.Mins[2]);
                        writer.Write(leaf.Mins[1]);
                        writer.Write(leaf.Maxs[0]);
                        writer.Write(leaf.Maxs[2]);
                        writer.Write(leaf.Maxs[1]);
                        writer.Write(leaf.FirstMarkSurface);
                        writer.Write(leaf.MarkSurfaceCount);
                        writer.Write(leaf.AmbientLevels);
                    }

                    return stream.ToArray();
                }
            }
        }

        private byte[] WriteMarkSurfacesLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    foreach (var markSurface in bspFile.MarkSurfaces)
                    {
                        writer.Write(markSurface);
                    }

                    return stream.ToArray();
                }
            }
        }

        private byte[] WriteTexInfoLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    foreach (var texInfo in bspFile.TexInfos)
                    {
                        writer.Write(texInfo.S.X);
                        writer.Write(texInfo.S.Z);
                        writer.Write(texInfo.S.Y);
                        writer.Write(texInfo.SShift);
                        writer.Write(texInfo.T.X);
                        writer.Write(texInfo.T.Z);
                        writer.Write(texInfo.T.Y);
                        writer.Write(texInfo.TShift);
                        writer.Write(texInfo.MipTex);
                        writer.Write(texInfo.Flags);
                    }

                    return stream.ToArray();
                }
            }
        }

        private byte[] WriteTexturesLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    writer.Write(bspFile.MipTextures.Length);
                    var textureOffset = sizeof(Int32) + (sizeof(Int32) * bspFile.MipTextures.Length);

                    foreach (var mipTexture in bspFile.MipTextures)
                    {
                        writer.Write(textureOffset);
                        textureOffset += 40;
                    }

                    foreach (var mipTexture in bspFile.MipTextures)
                    {
                        writer.Write(mipTexture.Name);
                        writer.Write(mipTexture.Width);
                        writer.Write(mipTexture.Height);
                        writer.Write(mipTexture.Offsets[0]);
                        writer.Write(mipTexture.Offsets[1]);
                        writer.Write(mipTexture.Offsets[2]);
                        writer.Write(mipTexture.Offsets[3]);
                    }

                    return stream.ToArray();
                }
            }
        }

        private byte[] WriteNodesLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    foreach (var node in bspFile.Nodes)
                    {
                        writer.Write(node.Plane);
                        writer.Write(node.Children[0]);
                        writer.Write(node.Children[1]);
                        writer.Write(node.Mins[0]);
                        writer.Write(node.Mins[2]);
                        writer.Write(node.Mins[1]);
                        writer.Write(node.Maxs[0]);
                        writer.Write(node.Maxs[2]);
                        writer.Write(node.Maxs[1]);
                        writer.Write(node.FirstFace);
                        writer.Write(node.FaceCount);
                    }

                    return stream.ToArray();
                }
            }
        }

        private byte[] WriteVisibilityLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    writer.Write(bspFile.VisData.CompressedData);

                    var finalSize = (bspFile.VisData.CompressedData.Length + 3) & ~3;
                    if (finalSize > stream.Length)
                    {
                        var emptyBytes = finalSize - stream.Length;
                        for (int i = 0; i < emptyBytes; i++)
                        {
                            writer.Write((byte)0x00);
                        }
                    }

                    return stream.ToArray();
                }
            }
        }

        private byte[] WritePlanesLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    foreach (var plane in bspFile.Planes)
                    {
                        writer.Write(plane.Normal.X);
                        writer.Write(plane.Normal.Z);
                        writer.Write(plane.Normal.Y);
                        writer.Write(plane.Distance);
                        writer.Write(plane.Type);
                    }

                    return stream.ToArray();
                }
            }
        }

        private byte[] WriteEntitiesLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    writer.Write(bspFile.EntityData.ToCharArray());

                    return stream.ToArray();
                }
            }
        }

        private byte[] WriteLightingData(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    writer.Write(bspFile.LightmapData.Data);

                    var finalSize = (bspFile.LightmapData.Data.Length + 3) & ~3;
                    if (finalSize > stream.Length)
                    {
                        var emptyBytes = finalSize - stream.Length;
                        for (int i = 0; i < emptyBytes; i++)
                        {
                            writer.Write((byte)0x00);
                        }
                    }

                    return stream.ToArray();
                }
            }
        }

        private byte[] WriteClipNodesLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    writer.Write(bspFile.ClipNodesData);

                    return stream.ToArray();
                }
            }
        }

        private byte[] WriteModelsLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    writer.Write(bspFile.ModelsData);

                    return stream.ToArray();
                }
            }
        }

        private byte[] WriteCubemapsLump(BspFile bspFile)
        {
            using (var stream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(stream))
                {
                    var dataOffset = sizeof(int) +                  // Cubemap count
                                        ((sizeof(int) * 3) +        // Cubemap sample position
                                        sizeof(int) +               // Cubemap sample size
                                        sizeof(int)) *              // Cubemap sample data offset
                                    bspFile.Cubemaps.Length;

                    writer.Write(bspFile.Cubemaps.Length);

                    foreach (var cubemap in bspFile.Cubemaps)
                    {
                        writer.Write(cubemap.Position[0]);
                        writer.Write(cubemap.Position[2]);
                        writer.Write(cubemap.Position[1]);
                        writer.Write(cubemap.Size);

                        if (cubemap.Data != null && cubemap.Data.Length > 0)
                        {
                            writer.Write(dataOffset);

                            dataOffset += cubemap.Data.Length;
                        }
                        else
                        {
                            writer.Write(0);
                        }
                    }

                    foreach (var cubemap in bspFile.Cubemaps)
                    {
                        if (cubemap.Data != null && cubemap.Data.Length > 0)
                        {
                            writer.Write(cubemap.Data);
                        }
                    }

                    return stream.ToArray();
                }
            }
        }

        private BspLump WriteLump(byte[] lumpData, BinaryWriter writer)
        {
            var lumpOffset = (int)writer.BaseStream.Position;

            writer.Write(lumpData);
            var finalSize = (lumpData.Length + 3) & ~3;

            if (finalSize > lumpData.Length)
            {
                var emptyBytes = finalSize - lumpData.Length;
                for (int i = 0; i < emptyBytes; i++)
                {
                    writer.Write((byte)0x00);
                }
            }

            return new BspLump()
            {
                Offset = lumpOffset,
                Data = lumpData,
                Length = lumpData.Length,
            };
        }
    }
}
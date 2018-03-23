using OpenTK;
using System;
using System.Collections.Generic;
using System.IO;

namespace GoldSrc.Bsp
{
    public class BspFile
    {
        public Vector3[] Vertices;
        public BspEdge[] Edges;
        public BspFace[] Faces;
        public Int32[] SurfEdges;
        public BspLeaf[] Leaves;
        public UInt16[] MarkSurfaces;
        public BspTextureInfo[] TexInfos;
        public BspMipTexture[] MipTextures;
        public BspNode[] Nodes;
        public BspVisData VisData;
        public BspPlane[] Planes;
        public string EntityData;
        public BspLightmapData LightmapData;
        public byte[] ClipNodesData;
        public byte[] ModelsData;
        public BspCubemap[] Cubemaps;

        public BspFile()
        {
            Vertices = new Vector3[0];
            Edges = new BspEdge[0];
            Faces = new BspFace[0];
            SurfEdges = new int[0];
            Leaves = new BspLeaf[0];
            MarkSurfaces = new ushort[0];
            TexInfos = new BspTextureInfo[0];
            MipTextures = new BspMipTexture[0];
            Nodes = new BspNode[0];
            VisData = new BspVisData();
            Planes = new BspPlane[0];
            EntityData = string.Empty;
            LightmapData = new BspLightmapData();
            ClipNodesData = new byte[0];
            ModelsData = new byte[0];
            Cubemaps = new BspCubemap[0];
        }

        public static BspFile Load(Stream stream)
        {
            var bspFileReader = new BspFileReader(stream);

            return bspFileReader.ReadBspFile();
        }

        public void Save(Stream stream)
        {
            var bspFileWriter = new BspFileWriter(stream);

            bspFileWriter.WriteBspFile(this);
        }

        public UInt16 GetSurfEdgeVertex(Int32 surfEdge)
        {
            if (surfEdge < 0)
            {
                return Edges[-surfEdge].Vertices[1];
            }

            return Edges[surfEdge].Vertices[0];
        }
    }
}
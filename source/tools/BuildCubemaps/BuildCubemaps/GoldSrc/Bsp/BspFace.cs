using System;

namespace GoldSrc.Bsp
{
    public struct BspFace
    {
        public UInt16 Plane;
        public UInt16 PlaneSide;
        public UInt32 FirstEdge;
        public UInt16 EdgeCount;
        public UInt16 TextureInfo;
        public Byte[] Styles;
        public UInt32 LightmapOffset;
    }
}

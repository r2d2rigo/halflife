using System;

namespace GoldSrc.Bsp
{
    public struct BspLeaf
    {
        public Int32 Contents;
        public Int32 VisOffset;
        public Int16[] Mins;
        public Int16[] Maxs;
        public UInt16 FirstMarkSurface;
        public UInt16 MarkSurfaceCount;
        public Byte[] AmbientLevels;
    }
}

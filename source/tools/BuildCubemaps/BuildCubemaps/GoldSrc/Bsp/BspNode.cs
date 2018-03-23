using System;

namespace GoldSrc.Bsp
{
    public struct BspNode
    {
        public UInt32 Plane;
        public Int16[] Children;
        public Int16[] Mins;
        public Int16[] Maxs;
        public UInt16 FirstFace;
        public UInt16 FaceCount;
    }
}

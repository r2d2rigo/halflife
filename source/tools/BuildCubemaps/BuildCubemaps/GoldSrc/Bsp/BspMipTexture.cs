using System;

namespace GoldSrc.Bsp
{
    public struct BspMipTexture
    {
        public char[] Name;
        public UInt32 Width;
        public UInt32 Height;
        public UInt32[] Offsets;
    }
}

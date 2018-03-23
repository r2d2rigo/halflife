using System;

namespace GoldSrc.Bsp
{
    public struct BspHeader
    {
        public Int32 Version;
        public BspLump[] Lumps;
    }
}

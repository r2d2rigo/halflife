using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GoldSrc.Bsp
{
    public struct BspTextureHeader
    {
        public UInt32 MipTextureCount;
        public Int32[] MipTextureOffsets;
    }
}

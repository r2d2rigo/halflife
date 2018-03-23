using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GoldSrc.Wad
{
    public struct WadTexture
    {
        public char[] Name;
        public UInt32 Width;
        public UInt32 Height;
        public UInt32[] Offsets;
    }
}

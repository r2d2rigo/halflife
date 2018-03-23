using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GoldSrc.Wad
{
    public struct WadHeader
    {
        public byte[] MagicNumber;
        public Int32 DirEntries;
        public Int32 DirEntriesOffset;
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GoldSrc.Wad
{
    public struct WadDirEntry
    {
        public Int32 FilePos;
        public Int32 DiskSize;
        public Int32 Size;
        public byte Type;
        public bool Compression;
        public Int16 Dummy;
        public char[] Name;
    }
}

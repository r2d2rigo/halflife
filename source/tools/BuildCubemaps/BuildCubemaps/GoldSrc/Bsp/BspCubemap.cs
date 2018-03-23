using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoldSrc.Bsp
{
    public class BspCubemap
    {
        public int[] Position;
        public int Size;
        public int Offset;
        public byte[] Data;

        public BspCubemap()
        {
            Position = new Int32[3];
            Size = 0;
            Offset = 0;
            Data = null;
        }
    }
}

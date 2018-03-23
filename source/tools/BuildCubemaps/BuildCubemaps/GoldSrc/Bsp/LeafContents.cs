using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GoldSrc.Bsp
{
    public enum LeafContents : int
    {
        Empty = -1,
        Solid = -2,
        Water = -3,
        Sky = -6,
        /*
        SLIME        -4,
        LAVA         -5,
        SKY          -6,
        ORIGIN       -7,
        CLIP         -8,
        CURRENT_0    -9,
        CURRENT_90   -10,
        CURRENT_180  -11,
        CURRENT_270  -12,
        CURRENT_UP   -13,
        CURRENT_DOWN -14,
        TRANSLUCENT  -15,
        */
    }
}

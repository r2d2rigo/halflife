using OpenTK;
using System;

namespace GoldSrc.Bsp
{
    public struct BspTextureInfo
    {
        public Vector3 S;
        public float SShift;
        public Vector3 T;
        public float TShift;
        public UInt32 MipTex;
        public UInt32 Flags;
    }
}

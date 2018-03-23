using OpenTK;
using System;

namespace GoldSrc.Bsp
{
    public struct BspPlane
    {
        public Vector3 Normal;
        public float Distance;
        public Int32 Type;
    }
}

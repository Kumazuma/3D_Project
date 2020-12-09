using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapToolCore
{
    [TypeConverter(typeof(ValueTypeTypeConverter<Scale>))]
    public struct Scale
    {
        private float x;
        private float y;
        private float z;
        public Scale(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
        public float X
        {
            get => x;
            set { x = value; }
        }
        public float Y
        {
            get => y;
            set { y = value; }
        }
        public float Z
        {
            get => z;
            set { z = value; }
        }
        public override string ToString()
        {
            return $"({x}, {y}, {z})";
        }
    }
}

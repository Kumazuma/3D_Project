using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapToolCore
{
    [TypeConverter(typeof(ValueTypeTypeConverter<Rotation>))]
    public struct Rotation: IJSONSerializable
    {
        private float x;
        private float y;
        private float z;
        public Rotation(float x, float y, float z)
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

        public JToken Serialize() => new JObject
        {
            {"x", X },
            {"y", Y },
            {"z", Z }
        };

        public override string ToString()
        {
            return $"({x}, {y}, {z})";
        }
    }
}

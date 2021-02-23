using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using Newtonsoft.Json.Linq;

namespace MapToolCore
{
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class Transform : INotifyPropertyChanged, IJSONSerializable
    {
        Position position;
        Rotation rotation;
        float scale = 1.0f;
        public event PropertyChangedEventHandler PropertyChanged;
        public Position Position
        {
            get => position;
            set
            {
                position = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Position"));
            }
        }
        public float Scale
        {
            get => scale;
            set
            {
                scale = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Scale"));
            }
        }
        public Rotation Rotation
        {
            get => rotation;
            set
            {
                rotation = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Rotation"));
            }
        }
        public Transform Clone() => new Transform
        {
            Position = this.position,
            Scale = this.scale,
            Rotation = this.rotation
        };

        public JToken Serialize() => new JObject
        {
            {"position", position.Serialize() },
            {"scale", scale },
            {"rotation", rotation.Serialize() }
        };
        public static Transform Parse(JObject jObj)
        {
            Transform res = new Transform();
            res.position = Position.Parse(jObj["position"] as JObject);
            res.rotation = Rotation.Parse(jObj["rotation"] as JObject);
            var scale = jObj["scale"];
            switch (scale.Type)
            {
                case JTokenType.Object:
                    res.scale = scale.Value<float>("x");
                    break;
                case JTokenType.Float:
                    res.scale = scale.Value<float>();
                    break;
                default:
                    res.scale = 1.0f;
                    break;
            }
            return res;
        }
        public override string ToString()
        {
            return $"pos{{x:{position.X}, y:{position.Y}, z:{position.Z}}}, rotation{{x:{rotation.X}, y:{rotation.Y}, z:{rotation.Z}}}, scale{{{scale}}}";
        }
    }
}

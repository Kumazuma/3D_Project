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
        Scale scale = new Scale(1f, 1f, 1f);
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
        public Scale Scale
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
            {"scale", scale.Serialize() },
            {"rotation", rotation.Serialize() }
        };

    }
}

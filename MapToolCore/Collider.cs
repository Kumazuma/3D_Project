using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapToolCore
{
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class Collider: INotifyPropertyChanged
    {
        protected Offset offset;
        PropertyChangedEventHandler m_offsetPropertyChangedEventHandler;
        public event PropertyChangedEventHandler PropertyChanged;
        public Collider()
        {
            m_offsetPropertyChangedEventHandler = new PropertyChangedEventHandler(OnOffsetPropertyChanged);
            offset = new Offset();
            offset.PropertyChanged += m_offsetPropertyChangedEventHandler;
        }
        private void OnOffsetPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Offset"));
        }
        [CategoryAttribute("Common")]
        public Offset Offset
        {
            get => offset;
            set
            {
                if(value == null) return;
                this.offset.PropertyChanged -= m_offsetPropertyChangedEventHandler;
                this.offset = value;
                this.offset.PropertyChanged += m_offsetPropertyChangedEventHandler;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Offset"));
            }
        }
        protected void BroadcastPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

    }
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class Offset: INotifyPropertyChanged
    {
        float x;
        float y;
        float z;
        public Offset():
            this(0.0f, 0.0f, 0.0f)
        {
        }
        public Offset(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
        public float X
        {
            get => x;
            set
            {
                x = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("X"));
            }
        }
        public float Y
        {
            get => y;
            set
            {
                y = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Y"));
            }
        }
        public float Z
        {
            get => z;
            set
            {
                z = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Z"));
            }
        }
        
        public event PropertyChangedEventHandler PropertyChanged;
    }
    public class BoxCollider : Collider
    {
        private float width;
        private float height;
        private float depth;
        public BoxCollider()
        {
            width = 1.0f;
            height = 1.0f;
            depth = 1.0f;
        }
        [CategoryAttribute("Box")]
        public float Width
        {
            get => width;
            set
            {
                width = value;
                BroadcastPropertyChanged("Width");
            }
        }
        [CategoryAttribute("Box")]
        public float Height
        {
            get => height;
            set
            {
                height = value;
                BroadcastPropertyChanged("Height");
            }
        }
        [CategoryAttribute("Box")]
        public float Depth
        {
            get => depth;
            set
            {
                depth = value;
                BroadcastPropertyChanged("Depth");
            }
        }
    }
    public class SphareCollider : Collider
    {
        private float radius;
        public SphareCollider()
        {
            radius = 1.0f;
        }
        [CategoryAttribute("Sphare")]
        public float Radius
        {
            get => radius;
            set
            {
                radius = value;
                BroadcastPropertyChanged("Radius");
            }
        }
    }
}

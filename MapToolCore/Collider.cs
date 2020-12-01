using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapToolCore
{
    public class Collider: INotifyPropertyChanged
    {
        protected Offset offset;
        PropertyChangedEventHandler m_offsetPropertyChangedEventHandler;
        public event PropertyChangedEventHandler PropertyChanged;
        Collider()
        {
            m_offsetPropertyChangedEventHandler = new PropertyChangedEventHandler(OnOffsetPropertyChanged);
            offset = new Offset();
            offset.PropertyChanged += m_offsetPropertyChangedEventHandler;
        }
        private void OnOffsetPropertyChanged(object sender, PropertyChangedEventArgs e)
        {

        }
    }
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
}

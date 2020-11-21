using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
namespace MapToolCore
{
    public class Environment: INotifyPropertyChanged
    {
        private static Environment s_instance = null;
        private string m_projectBasePath;
        
        public event PropertyChangedEventHandler PropertyChanged;
        public string ProjectDirectory
        {
            get => m_projectBasePath;
            set
            {
                m_projectBasePath = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("ProjectDirectory"));
            }
        }
        public static Environment Instance
        {
            get
            {
                if(s_instance == null)
                {
                    lock(typeof(Environment))
                    {
                        if(s_instance == null)
                        {
                            s_instance = new Environment();
                        }
                    }
                }
                return s_instance;
            }
        }
    }
}

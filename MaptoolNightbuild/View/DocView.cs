using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MaptoolNightbuild.View
{
    public class DockView<T> : WeifenLuo.WinFormsUI.Docking.DockContent where T : Control, new()
    {
        private T m_panel;
        public T Content { get => m_panel; }
        public DockView()
        {
            m_panel = new T();
            m_panel.Dock = DockStyle.Fill;
            m_panel.Parent = this;
            m_panel.Visible = true;
        }
    }
}

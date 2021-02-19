using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MaptoolNightbuild
{
    static class Program
    {
        /// <summary>
        /// 해당 애플리케이션의 주 진입점입니다.
        /// </summary>
        [STAThread]
        static void Main()
        {
            String projectDir = Properties.Settings.Default.ProjectDir;
            if (projectDir == null || projectDir == "")
            {
                projectDir = System.IO.Path.GetFullPath("./");
                Properties.Settings.Default.ProjectDir = projectDir;
            }
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
            Properties.Settings.Default.Save();
        }
    }
}

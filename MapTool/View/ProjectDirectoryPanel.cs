using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;

namespace MapTool.View
{
    public partial class ProjectDirectoryPanel : UserControl
    {
        string projectPath;
        string currentPath;
        
        public ProjectDirectoryPanel()
        {
            InitializeComponent();
            projectPath = MapToolCore.Environment.Instance.ProjectDirectory;
            projectPath = System.IO.Path.GetFullPath(projectPath);
            currentPath = projectPath;
            ShowCurrentDirectory();
        }
        private void ShowCurrentDirectory()
        {
            listView1.Items.Clear();
            string[] children = System.IO.Directory.GetFileSystemEntries(currentPath);
            ImageList imageList = new ImageList();
            listView1.BeginUpdate();
            Icon icon = DefaultIcons.FolderLarge;

            imageList.ImageSize = new Size(32, 32);
            imageList.Images.Add("folder", icon);

            foreach (var child in children)
            {
                string name = System.IO.Path.GetFileName(child);
                if (System.IO.File.Exists(child))
                {
                    icon = Icon.ExtractAssociatedIcon(child);
                    if (icon == null)
                    {
                        icon = SystemIcons.WinLogo;
                    }
                    imageList.Images.Add(child, icon);
                }
                else
                {

                }
            }
            //listView1.View = System.Windows.Forms.View.SmallIcon;
            //listView1.SmallImageList = imageList;
            listView1.LargeImageList = imageList;
            listView1.Items.Add("..", "..", "folder").Tag = "..";
            foreach (var child in children)
            {
                string name = System.IO.Path.GetFileName(child);
                string imgKey = child;
                string tag = child;
                if (!System.IO.File.Exists(child))
                {
                    imgKey = "folder";
                    tag = "folder";
                }
                listView1.Items.Add(child, name, imgKey).Tag = tag;
            }
            //listView1.LargeImageList = imageList;
            listView1.EndUpdate();
        }

        private void listView1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if(listView1.SelectedItems.Count == 1)
            {
                String tag = listView1.SelectedItems[0].Tag as String;
                if (tag == "folder")
                {
                    currentPath = Path.Combine(currentPath, listView1.SelectedItems[0].Name);
                    ShowCurrentDirectory();
                }
                else if(tag == ".." && currentPath != projectPath)
                {
                    currentPath = Path.GetFullPath(Path.Combine(currentPath, ".."));
                    ShowCurrentDirectory();

                }
            }
        }

        private void listView1_ItemDrag(object sender, ItemDragEventArgs e)
        {
            if (listView1.SelectedItems.Count == 1)
            {
                String tag = listView1.SelectedItems[0].Tag as String;
                if (tag == "folder")
                {
                    return;
                }
                else if (tag == "..")
                {
                    return;
                }
                DoDragDrop(tag, DragDropEffects.Link);
            }
        }
    }

    public static class DefaultIcons
    {
        private static readonly Lazy<Icon> _lazyFolderIcon = new Lazy<Icon>(FetchIcon, true);

        public static Icon FolderLarge
        {
            get { return _lazyFolderIcon.Value; }
        }

        private static Icon FetchIcon()
        {
            var tmpDir = Directory.CreateDirectory(Path.Combine(Path.GetTempPath(), Guid.NewGuid().ToString())).FullName;
            var icon = ExtractFromPath(tmpDir);
            Directory.Delete(tmpDir);
            return icon;
        }

        private static Icon ExtractFromPath(string path)
        {
            SHFILEINFO shinfo = new SHFILEINFO();
            SHGetFileInfo(
                path,
                0, ref shinfo, (uint)Marshal.SizeOf(shinfo),
                SHGFI_ICON | SHGFI_LARGEICON);
            return System.Drawing.Icon.FromHandle(shinfo.hIcon);
        }

        //Struct used by SHGetFileInfo function
        [StructLayout(LayoutKind.Sequential)]
        private struct SHFILEINFO
        {
            public IntPtr hIcon;
            public int iIcon;
            public uint dwAttributes;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
            public string szDisplayName;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 80)]
            public string szTypeName;
        };

        [DllImport("shell32.dll")]
        private static extern IntPtr SHGetFileInfo(string pszPath, uint dwFileAttributes, ref SHFILEINFO psfi, uint cbSizeFileInfo, uint uFlags);

        private const uint SHGFI_ICON = 0x100;
        private const uint SHGFI_LARGEICON = 0x0;
        private const uint SHGFI_SMALLICON = 0x000000001;
    }
}

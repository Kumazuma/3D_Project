using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MapToolRender;
namespace MapTool.Doc
{
    public partial class MapFile
    {
        List<MapObject> mapObjects = new List<MapObject>();
        Collection collection;
        string filePath;
        string projectPath;
        string skyboxTexturePath;
        public MapFile()
        {
            collection = new Collection(this);
        }
        public Collection Nodes
        {
            get => collection;
            set => collection = value;
        }
        public string ProjectPath
        {
            get => projectPath;
            set => projectPath = value;
        }
        public string FilePath
        {
            get => filePath;
            set => filePath = value;

        }
        public string SkyBoxTexturePath
        {
            get => skyboxTexturePath;
            set => skyboxTexturePath = value;
        }
        string FormatString(string path)
        {
            if (path == null) return null;
            string currentProjectDirectory = MapToolCore.Environment.Instance.ProjectDirectory;
            path = System.IO.Path.GetFullPath(path);
            path = MapToolCore.Utility.GetRelativePath(currentProjectDirectory, path);
            
            if(path.StartsWith(".."))
            {
                path = System.IO.Path.PathSeparator + path;
            }
            else
            {
                path = path.Substring(1);
            }
            return path.Replace('\\', '/');
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MaptoolRenderer;
namespace MaptoolNightbuild.MapEditor
{
    class Controller
    {
        static Controller s_instance = new Controller();
        public static Controller Instance { get => s_instance; }
        public async Task AddObjMesh(string[] paths)
        {
            var tasks = new List<Task<OBJMesh> >();
            var objMeshs = new List<OBJMesh>();
            var meshObjects = new List<IRenderable>();
            objMeshs.Capacity = paths.Length;
            tasks.Capacity = paths.Length;
            foreach(var path in paths)
            {
                tasks.Add(Doc.ResourceManager.Instance.GetObjMesh(path));
            }
            foreach (var task in tasks)
            {
                var mesh = await task;
                objMeshs.Add(mesh);
            }
            foreach (var mesh in objMeshs)
            {
                meshObjects.Add(MeshObject.Create(mesh));
            }
            Document.Instance.RenderObjects.AddRange(meshObjects);
        }
        public async Task OpenMap(string path)
        {
            var file = await MapFile.Load(path, Properties.Settings.Default.ProjectDir);
            Document.Instance.RenderObjects.Clear();
            var s = from node in file.Nodes where node is IRenderable select node as IRenderable;
            Document.Instance.RenderObjects.Add(Document.Instance.SkyBox);
            Document.Instance.RenderObjects.AddRange(s);
            Document.Instance.SkyBox.Texture = new CubeTexture(GraphicsDevice.Instance, file.SkyBoxTexturePath);

        }
        public async Task SaveMap(string path)
        {
            var file = new MapFile();
            file.ProjectPath = Properties.Settings.Default.ProjectDir;
            file.SkyBoxTexturePath = null;
            file.SkyBoxTexturePath = Document.Instance.SkyBox.Texture?.FilePath;
            foreach(var obj in Document.Instance.RenderObjects)
            {
                if (obj is SkyBox) continue;
                if (obj is Camera) continue;
                file.Nodes.Add(obj);
            }
            await file.Save(path);
        }
        public async Task NewMap()
        {
            await Task.Run(() =>
            {
                Document.Instance.RenderObjects.Clear();
                Document.Instance.RenderObjects.Add(Document.Instance.SkyBox);
            });
        }
    }
}

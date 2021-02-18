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
        public async Task NewMap()
        {
            await Task.Run(() =>
            {
                Document.Instance.RenderObjects.Clear();
            });
        }
    }
}

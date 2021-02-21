using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MaptoolNightbuild.CharacterMetaEditor
{
    class Controller
    {
        private static Controller s_instance = new Controller();
        public static Controller Instance{ get => s_instance; }
        public void Save()
        {
            

        }
        public async Task LoadXMesh(String path)
        {
            var xmesh = await Doc.ResourceManager.Instance.GetSkinnedMesh(path);
            var xSkinnedMeshObject = new MaptoolRenderer.SkinnedXMeshObject(xmesh);
            var prevXMesh = Document.Instance.Mesh;
            Document.Instance.Mesh = xSkinnedMeshObject;
            Document.Instance.RenderObjects.Remove(prevXMesh);
            Document.Instance.RenderObjects.Add(xSkinnedMeshObject);
        }
        public async Task Load(String path)
        {

        }
    }
}

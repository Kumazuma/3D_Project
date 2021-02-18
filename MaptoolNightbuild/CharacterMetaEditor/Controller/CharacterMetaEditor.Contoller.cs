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
            var document = Doc.CharacterMetaDoc.Instance;

        }
        public async Task LoadXMesh(String path)
        {
            var xmesh = await Doc.ResourceManager.Instance.GetSkinnedMesh(path);
            var xSkinnedMeshObject = new MaptoolRenderer.SkinnedXMeshObject(xmesh);
            var prevXMesh = Doc.CharacterMetaDoc.Instance.Mesh;
            Doc.CharacterMetaDoc.Instance.Mesh = xSkinnedMeshObject;
            Doc.CharacterMetaDoc.Instance.RenderObjects.Remove(prevXMesh);
            Doc.CharacterMetaDoc.Instance.RenderObjects.Add(xSkinnedMeshObject);
            
        }
    }
}

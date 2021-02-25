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
        public void Save(String path)
        {
            String projectDir = Properties.Settings.Default.ProjectDir;
            MetaFile metaFile = new MetaFile();
            metaFile.MeshFilePath = Document.Instance.Mesh.FilePath;
            int unnamed = 1;
            foreach (var collider in Document.Instance.ColliderObject)
            {
                string name = collider.Name;
                if(name == null)
                {
                    name = unnamed.ToString();
                    ++unnamed;
                }
                metaFile.ColliderList.Add(name, collider);
            }
            foreach (var animation in Document.Instance.AnimationTable)
            {
                metaFile.AnimationTable.Add(animation.Key, animation.Value);
            }
            metaFile.Save(path, projectDir);
        }
        public async Task LoadXMesh(String path)
        {
            var xmesh = await Doc.ResourceManager.Instance.GetSkinnedMesh(path);
            var xSkinnedMeshObject = new MaptoolRenderer.SkinnedXMeshObject(xmesh);
            var prevXMesh = Document.Instance.Mesh;
            Document.Instance.Mesh = xSkinnedMeshObject;
            Document.Instance.RenderObjects.Remove(prevXMesh);
            Document.Instance.RenderObjects.Add(xSkinnedMeshObject);
            Document.Instance.AnimationTable = new AnimationTable((int)xSkinnedMeshObject.AnimationCount);
        }
        public async Task<bool> Load(String path)
        {
            String projectDir = Properties.Settings.Default.ProjectDir;
            MetaFile metaFile = MetaFile.Load(path, projectDir);
            var xmesh = await Doc.ResourceManager.Instance.GetSkinnedMesh(metaFile.MeshFilePath);
            if(xmesh == null)
            {
                return false;
            }
            var xSkinnedMeshObject = new MaptoolRenderer.SkinnedXMeshObject(xmesh);
            var prevXMesh = Document.Instance.Mesh;
            Document.Instance.Mesh = xSkinnedMeshObject;
            Document.Instance.RenderObjects.Remove(prevXMesh);
            Document.Instance.RenderObjects.Add(xSkinnedMeshObject);
            Document.Instance.ColliderObject.Clear();
            Document.Instance.ColliderObject.AddRange(metaFile.ColliderList.Values);
            var animationTable = new AnimationTable((int)xSkinnedMeshObject.AnimationCount);
            foreach (var animation in metaFile.AnimationTable)
            {
                if(animationTable.Count <= animation.Key)
                {
                    continue;
                }
                animationTable[animation.Key] = animation.Value;
            }
            Document.Instance.AnimationTable = animationTable;
            foreach(var collider in Document.Instance.ColliderObject)
            {
                collider.ParentObject = xSkinnedMeshObject;
            }
            return true;
        }
        public void AddCollider()
        {
            var document = Document.Instance;
            var newColliderObject = new MaptoolRenderer.ColliderObject();
            newColliderObject.ParentObject = document.Mesh;
            document.ColliderObject.Add(newColliderObject);
        }
        public void RemoveCollider(IEnumerable<MaptoolRenderer.ColliderObject> colliders)
        {
            var document = Document.Instance;
            foreach (var item in colliders)
            {
                document.ColliderObject.Remove(item);
            }
        }
    }
}

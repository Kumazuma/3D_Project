using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MaptoolRenderer;
namespace MapTool.Doc
{
    class ResourceManager
    {
        static ResourceManager s_instance = new ResourceManager();
        //TODO:
        //Dictionary<string, StaticXMeshObj> m_staticMeshs = new Dictionary<string, StaticXMeshObj>();
        //Dictionary<string, SkinnedXMeshObj> m_skinnedMeshs = new Dictionary<string, SkinnedXMeshObj>();
        //Dictionary<string, WowMapMesh> m_objMeshs = new Dictionary<string, WowMapMesh>();
        //TargetObject targetObject;
        Dictionary<string, OBJMesh> m_objMeshs = new Dictionary<string, OBJMesh>();
        Dictionary<string, Task<OBJMesh> > m_loadingMeshsPath = new Dictionary<string, Task<OBJMesh>>();
        public static ResourceManager Instance { get => s_instance; }

        public SkinnedXMeshObj GetSkinnedMesh(string path)
        {
            if(path == null)
            {
                throw new ArgumentNullException();
            }
            path = System.IO.Path.GetFullPath(path);
            if(m_skinnedMeshs.ContainsKey(path))
            {
                return m_skinnedMeshs[path] as SkinnedXMeshObj;
            }
            var newMesh = new SkinnedXMeshObj(GraphicsDevice.Instance, path);
            m_skinnedMeshs.Add(path, newMesh);
            return newMesh.Clone() as SkinnedXMeshObj;
        }
        public StaticXMeshObj GetStaticMesh(string path)
        {
            if (path == null)
            {
                throw new ArgumentNullException();
            }
            path = System.IO.Path.GetFullPath(path);
            if (m_staticMeshs.ContainsKey(path))
            {
                return m_staticMeshs[path] as StaticXMeshObj;
            }
            var newMesh = new StaticXMeshObj(GraphicsDevice.Instance, path);
            m_staticMeshs.Add(path, newMesh);
            return newMesh.Clone() as StaticXMeshObj;
        }
        public TargetObject GetTargetObject()
        {
            if (targetObject != null)
            {
                return targetObject.Clone() as TargetObject;
            }
            lock (this)
            {
                if (targetObject == null)
                {
                    targetObject = new TargetObject(GraphicsDevice.Instance);
                }
            }
            return targetObject.Clone() as TargetObject;
        }
        public Task<WowMapMesh> GetObjMesh(string path)
        {
            if (path == null)
            {
                throw new ArgumentNullException();
            }

            path = System.IO.Path.GetFullPath(path);

            lock(this)
            {
                if(m_loadingMeshsPath.ContainsKey(path))
                {
                    return Task<WowMapMesh>.Factory.StartNew(() => m_loadingMeshsPath[path].Result.Clone() as WowMapMesh);
                }
                var task = System.Threading.Tasks.Task<WowMapMesh>.Factory.StartNew(() =>
                {
                    lock(this)
                    {
                        if (m_objMeshs.ContainsKey(path))
                        {
                            return m_objMeshs[path].Clone() as WowMapMesh;
                        }
                    }
                
                    var newMesh = new WowMapMesh(GraphicsDevice.Instance, path);
                    lock(this)
                    {
                        m_objMeshs.Add(path, newMesh);
                     
                    }
                    return newMesh.Clone() as WowMapMesh;
                });
                m_loadingMeshsPath.Add(path, task);
                return task;
            }
        }
    }
}

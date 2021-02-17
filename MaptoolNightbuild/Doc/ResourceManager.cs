using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MaptoolRenderer;
namespace MaptoolNightbuild.Doc
{
    class ResourceManager
    {
        static ResourceManager s_instance = new ResourceManager();

        Dictionary<string, OBJMesh> m_objMeshs = new Dictionary<string, OBJMesh>();
        Dictionary<string, SkinnedXMesh> m_skinnedXMeshs = new Dictionary<string, SkinnedXMesh>();
        Dictionary<string, Task<OBJMesh>> m_loadingMeshs = new Dictionary<string, Task<OBJMesh>>();
        Dictionary<string, Task<SkinnedXMesh>> m_loadingXMeshs = new Dictionary<string, Task<SkinnedXMesh>>();
        public static ResourceManager Instance { get => s_instance; }

        public async Task<SkinnedXMesh> GetSkinnedMesh(string path)
        {
            if (path == null)
            {
                throw new ArgumentNullException();
            }
            path = System.IO.Path.GetFullPath(path);
            SkinnedXMesh mesh;
            if (m_skinnedXMeshs.TryGetValue(path, out mesh))
            {
                return mesh;
            }
            Task<SkinnedXMesh> loadingTask;
            lock (this)
            {
                m_loadingXMeshs.TryGetValue(path, out loadingTask);
            }
            if (loadingTask != null)
            {
                mesh = await loadingTask;
                lock (this)
                {
                    if (m_loadingXMeshs.Remove(path))
                    {
                        m_skinnedXMeshs.Add(path, mesh);
                    }
                }
                return mesh;
            }

            lock (this)
            {
                loadingTask = Task<SkinnedXMesh>.Factory.StartNew(() =>
                {
                    return new SkinnedXMesh(path);
                });
                m_loadingXMeshs.Add(path, loadingTask);
            }
            mesh = await loadingTask;
            lock (this)
            {
                if (m_loadingXMeshs.Remove(path))
                {
                    m_skinnedXMeshs.Add(path, mesh);
                }
            }
            return mesh;
        }
        public async Task<OBJMesh> GetObjMesh(string path)
        {
            if (path == null)
            {
                throw new ArgumentNullException();
            }
            path = System.IO.Path.GetFullPath(path);
            OBJMesh mesh;
            if(m_objMeshs.TryGetValue(path, out mesh))
            {
                return mesh;
            }
            Task<OBJMesh> loadingTask;
            lock (this)
            {
                m_loadingMeshs.TryGetValue(path, out loadingTask);
            }
            if(loadingTask != null)
            {
                mesh = await loadingTask;
                lock (this)
                {
                    if(m_loadingMeshs.Remove(path))
                    {
                        m_objMeshs.Add(path, mesh);
                    }
                }
                return mesh;
            }
            
            lock (this)
            {
                loadingTask = Task<OBJMesh>.Factory.StartNew(() =>
                {
                    return new OBJMesh(path);
                });
                m_loadingMeshs.Add(path, loadingTask);
            }
            mesh = await loadingTask;
            lock (this)
            {
                if (m_loadingMeshs.Remove(path))
                {
                    m_objMeshs.Add(path, mesh);
                }
            }
            return mesh;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MaptoolRenderer;
namespace MaptoolNightbuild.Doc
{
    class ResourceManager
    {
        static ResourceManager s_instance = new ResourceManager();

        Dictionary<string, WeakReference<OBJMesh>> m_objMeshs = new Dictionary<string, WeakReference<OBJMesh>>();
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
                        if (mesh != null)
                        {
                            m_skinnedXMeshs.Add(path, mesh);
                        }
                    }
                }
                return mesh;
            }

            lock (this)
            {
                loadingTask = Task<SkinnedXMesh>.Factory.StartNew(() =>
                {
                    SkinnedXMesh skinnedLoadedMesh = null;
                    try
                    {
                        skinnedLoadedMesh = new SkinnedXMesh(path);
                    }
                    catch(Exception e)
                    {
                        Debug.Write(e.StackTrace);
                        Debug.Write(e.Message);
                    }
                    return skinnedLoadedMesh;
                });
                m_loadingXMeshs.Add(path, loadingTask);
            }
            mesh = await loadingTask;
            lock (this)
            {
                if (m_loadingXMeshs.Remove(path))
                {
                    if(mesh != null)
                    {
                        m_skinnedXMeshs.Add(path, mesh);
                    }
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
            WeakReference<OBJMesh> weakRef;
            OBJMesh mesh;
            m_objMeshs.TryGetValue(path, out weakRef);
            if(weakRef != null && weakRef.TryGetTarget(out mesh))
            {
                return mesh;
            }
            m_objMeshs.Remove(path);

            Task<OBJMesh> loadingTask;
            lock (this)
            {
                m_loadingMeshs.TryGetValue(path, out loadingTask);
            }
            if(loadingTask != null)
            {
                mesh = await loadingTask;
                return mesh;
            }
            lock (this)
            {
                loadingTask = Task<OBJMesh>.Factory.StartNew(() =>
                {
                    OBJMesh loadedMesh = null;
                    try
                    {
                        loadedMesh = new OBJMesh(path);
                        lock (this)
                        {
                            m_objMeshs.Add(path, new WeakReference<OBJMesh>(loadedMesh));
                        }
                    }
                    catch (Exception e)
                    {
                        Debug.Write(e.StackTrace);
                        Debug.Write(e.Message);
                    }
                    return loadedMesh;
                });
                m_loadingMeshs.Add(path, loadingTask);
            }
            mesh = await loadingTask;
            lock (this)
            {
                m_loadingMeshs.Remove(path);
            }
            return mesh;
        }
    }
}

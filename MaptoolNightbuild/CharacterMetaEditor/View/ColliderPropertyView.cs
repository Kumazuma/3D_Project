using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections.Specialized;
namespace MaptoolNightbuild.CharacterMetaEditor.View
{
    public partial class ColliderPropertyView : UserControl
    {
        BindingList<MaptoolRenderer.ColliderObject> colliderObjects
            = new BindingList<MaptoolRenderer.ColliderObject>();

        public ColliderPropertyView()
        {
            #region
            InitializeComponent();
            listBox1.DataSource = colliderObjects;
            listBox1.SelectedIndexChanged += ListBox1_SelectedIndexChanged;
            listBox1.MouseUp += ListBox1_MouseUp;
            #endregion
            var document = Document.Instance;
            document.PropertyChanged += Document_PropertyChanged;
            document.ColliderObject.CollectionChanged
                += ColliderObject_CollectionChanged;
        }

        private void ColliderObject_CollectionChanged(
            object sender,
            NotifyCollectionChangedEventArgs e)
        {
            switch(e.Action )
            {
                case NotifyCollectionChangedAction.Add:
                    {
                        foreach(var item in e.NewItems)
                        {
                            var obj = item as MaptoolRenderer.ColliderObject;
                            if (obj == null) continue;
                            colliderObjects.Add(obj);
                        }
                        colliderObjects.ResetBindings();
                    }
                    break;
                case NotifyCollectionChangedAction.Remove:
                case NotifyCollectionChangedAction.Reset:
                    {
                        foreach (var item in e.OldItems)
                        {
                            var obj = item as MaptoolRenderer.ColliderObject;
                            if (obj == null) continue;
                            colliderObjects.Remove(obj);
                        }
                        colliderObjects.ResetBindings();
                    }
                    break;
            }
        }

        private void ListBox1_MouseUp(object sender, MouseEventArgs e)
        {
            if(e.Button != MouseButtons.Right)
            {
                return;
            }
            contextMenuStrip1.Show((sender as Control).PointToScreen(e.Location));
        }

        private void Document_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if(e.PropertyName == "Mesh")
            {
                var document = Document.Instance;
                foreach(var item in document.ColliderObject)
                {
                    item.ParentObject = document.Mesh;
                }
            }
        }

        private void ListBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.pgCollider.SelectedObject = listBox1.SelectedItem;
        }

        private void btnAddColiider_Click(object sender, EventArgs e)
        {
            Controller.Instance.AddCollider();
        }

        private void miDelete_Click(object sender, EventArgs e)
        {
            if (listBox1.SelectedItems.Count == 0)
            {
                return;
            }
            var selectedItems = new MaptoolRenderer.ColliderObject[listBox1.SelectedItems.Count];
            listBox1.SelectedItems.CopyTo(selectedItems, 0);
            Controller.Instance.RemoveCollider(selectedItems);
        }
    }
}

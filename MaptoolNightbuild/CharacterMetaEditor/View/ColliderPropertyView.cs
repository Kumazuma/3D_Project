using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MaptoolNightbuild.CharacterMetaEditor.View
{
    public partial class ColliderPropertyView : UserControl
    {
        BindingList<MaptoolRenderer.ColliderObject> colliderObjects = new BindingList<MaptoolRenderer.ColliderObject>();

        public ColliderPropertyView()
        {
            InitializeComponent();
            listBox1.DataSource = colliderObjects;
            listBox1.SelectedIndexChanged += ListBox1_SelectedIndexChanged;
            var document = Document.Instance;
            document.PropertyChanged += Document_PropertyChanged;
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
            var document = Document.Instance;
            var newColliderObject = new MaptoolRenderer.ColliderObject();
            newColliderObject.ParentObject = document.Mesh;

            document.ColliderObject.Add(newColliderObject);
            colliderObjects.Add(newColliderObject);
            colliderObjects.ResetBindings();
        }
    }
}

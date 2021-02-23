using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
namespace MaptoolNightbuild.CharacterMetaEditor.View
{
    public partial class AnimationTableView : UserControl
    {
        DataTable table;
        public AnimationTableView()
        {
            InitializeComponent();
            CharacterMetaEditor.Document.Instance.PropertyChanged += Document_PropertyChanged;
        }

        private void Document_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if(e.PropertyName != "AnimationTable")
            {
                return;
            }
            this.Invoke(new Action(() =>
            {
                propertyGrid1.PropertySort = PropertySort.NoSort;
                propertyGrid1.SelectedObject = Document.Instance.AnimationTable;
            }));
        }

    }
}

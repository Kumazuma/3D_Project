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
            if(e.PropertyName != "Mesh")
            {
                return;
            }
            table = new DataTable
            {
                Locale = CultureInfo.InvariantCulture
            };
            DataColumn dataColumn;
            dataColumn = new DataColumn
            { ColumnName = "anim idx", DataType = typeof(int) };
            table.Columns.Add(dataColumn);
            dataColumn = new DataColumn
            { ColumnName = "ID", DataType = typeof(String) };
            table.Columns.Add(dataColumn);
            table.PrimaryKey = new DataColumn[1] { table.Columns[0] };
            DataSet ds = new DataSet();
            ds.Tables.Add(table);
            dataGridView1.DataSource = table;
        }

    }
}

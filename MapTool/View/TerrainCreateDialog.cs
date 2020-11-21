using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MapTool.View
{
    public partial class TerrainCreateDialog : Form
    {
        public TerrainCreateDialog()
        {
            InitializeComponent();
        }
        public string HeightmapImgPath { get; set; }
        public float Interval { get; set; }
        public float MaxHeight { get; set; }
        private void btnBrowse_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog fileDialog = new OpenFileDialog())
            {
                fileDialog.Filter = "이미지 파일(*.bmp;*.png;*.tga;*.gif;)|*.bmp;*.png;";
                fileDialog.Multiselect = false;
                fileDialog.CheckFileExists = true;

                if (fileDialog.ShowDialog() == DialogResult.OK)
                {
                    //TODO: 터레인을 생성한다. 지금은 여기서 생성하지만 어디 클래스 한 곳에서 집중하여 관리하는 것이 좋아보인다
                    HeightmapImgPath = fileDialog.FileName;
                    textBox1.Text = HeightmapImgPath;
                }
            }
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (!System.IO.File.Exists(textBox1.Text))
            {
                MessageBox.Show("파일이 존재하지 않습니다.");
                return;
            }
            HeightmapImgPath = textBox1.Text;
            Interval = (float)numericUpDown1.Value;
            MaxHeight = (float)numericUpDown2.Value;
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}

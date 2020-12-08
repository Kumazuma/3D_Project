
namespace MapTool.View
{
    partial class AnimationView
    {
        /// <summary> 
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 구성 요소 디자이너에서 생성한 코드

        /// <summary> 
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.btnXFileOpen = new System.Windows.Forms.Button();
            this.btnJsonOpen = new System.Windows.Forms.Button();
            this.btnJSONSave = new System.Windows.Forms.Button();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.AnimIndex = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this.animStringID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.comboAnim = new System.Windows.Forms.ComboBox();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.pgCollider = new System.Windows.Forms.PropertyGrid();
            this.btnAddCollider = new System.Windows.Forms.Button();
            this.listColliders = new System.Windows.Forms.ListBox();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.tabPage2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.tableLayoutPanel1);
            this.splitContainer1.Panel1.Controls.Add(this.tabControl1);
            this.splitContainer1.Panel1.Controls.Add(this.comboAnim);
            this.splitContainer1.Size = new System.Drawing.Size(1246, 888);
            this.splitContainer1.SplitterDistance = 266;
            this.splitContainer1.TabIndex = 0;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tableLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.tableLayoutPanel1.ColumnCount = 3;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 43.84615F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 56.15385F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 74F));
            this.tableLayoutPanel1.Controls.Add(this.btnXFileOpen, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.btnJsonOpen, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.btnJSONSave, 2, 0);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(258, 44);
            this.tableLayoutPanel1.TabIndex = 8;
            // 
            // btnXFileOpen
            // 
            this.btnXFileOpen.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnXFileOpen.Location = new System.Drawing.Point(3, 3);
            this.btnXFileOpen.Name = "btnXFileOpen";
            this.btnXFileOpen.Size = new System.Drawing.Size(74, 38);
            this.btnXFileOpen.TabIndex = 4;
            this.btnXFileOpen.Text = "X파일 열기";
            this.btnXFileOpen.UseVisualStyleBackColor = true;
            this.btnXFileOpen.Click += new System.EventHandler(this.btnXFileOpen_Click_1);
            // 
            // btnJsonOpen
            // 
            this.btnJsonOpen.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnJsonOpen.Location = new System.Drawing.Point(83, 3);
            this.btnJsonOpen.Name = "btnJsonOpen";
            this.btnJsonOpen.Size = new System.Drawing.Size(97, 38);
            this.btnJsonOpen.TabIndex = 3;
            this.btnJsonOpen.Text = "JSON파일 열기";
            this.btnJsonOpen.UseVisualStyleBackColor = true;
            this.btnJsonOpen.Click += new System.EventHandler(this.btnJsonOpen_Click_1);
            // 
            // btnJSONSave
            // 
            this.btnJSONSave.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnJSONSave.Location = new System.Drawing.Point(186, 3);
            this.btnJSONSave.Name = "btnJSONSave";
            this.btnJSONSave.Size = new System.Drawing.Size(69, 38);
            this.btnJSONSave.TabIndex = 5;
            this.btnJSONSave.Text = "JSON 저장";
            this.btnJSONSave.UseVisualStyleBackColor = true;
            this.btnJSONSave.Click += new System.EventHandler(this.btnJSONSave_Click_1);
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(2, 79);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(259, 804);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.dataGridView1);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(251, 778);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "애니메이션";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.AnimIndex,
            this.animStringID});
            this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridView1.Location = new System.Drawing.Point(3, 3);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(245, 772);
            this.dataGridView1.TabIndex = 9;
            // 
            // AnimIndex
            // 
            this.AnimIndex.HeaderText = "anim Index";
            this.AnimIndex.Name = "AnimIndex";
            this.AnimIndex.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.AnimIndex.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
            // 
            // animStringID
            // 
            this.animStringID.HeaderText = "ID";
            this.animStringID.Name = "animStringID";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.splitContainer2);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(251, 778);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "콜라이더";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // comboAnim
            // 
            this.comboAnim.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.comboAnim.FormattingEnabled = true;
            this.comboAnim.Location = new System.Drawing.Point(3, 53);
            this.comboAnim.Name = "comboAnim";
            this.comboAnim.Size = new System.Drawing.Size(258, 20);
            this.comboAnim.TabIndex = 7;
            this.comboAnim.SelectedIndexChanged += new System.EventHandler(this.comboAnim_SelectedIndexChanged);
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(3, 3);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.pgCollider);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.listColliders);
            this.splitContainer2.Panel2.Controls.Add(this.btnAddCollider);
            this.splitContainer2.Size = new System.Drawing.Size(245, 772);
            this.splitContainer2.SplitterDistance = 307;
            this.splitContainer2.TabIndex = 0;
            // 
            // pgCollider
            // 
            this.pgCollider.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pgCollider.Location = new System.Drawing.Point(0, 0);
            this.pgCollider.Name = "pgCollider";
            this.pgCollider.Size = new System.Drawing.Size(245, 307);
            this.pgCollider.TabIndex = 0;
            // 
            // btnAddCollider
            // 
            this.btnAddCollider.Dock = System.Windows.Forms.DockStyle.Top;
            this.btnAddCollider.Location = new System.Drawing.Point(0, 0);
            this.btnAddCollider.Name = "btnAddCollider";
            this.btnAddCollider.Size = new System.Drawing.Size(245, 23);
            this.btnAddCollider.TabIndex = 0;
            this.btnAddCollider.Text = "콜라이더 추가";
            this.btnAddCollider.UseVisualStyleBackColor = true;
            this.btnAddCollider.Click += new System.EventHandler(this.btnAddCollider_Click);
            // 
            // listColliders
            // 
            this.listColliders.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listColliders.FormattingEnabled = true;
            this.listColliders.ItemHeight = 12;
            this.listColliders.Location = new System.Drawing.Point(0, 23);
            this.listColliders.Name = "listColliders";
            this.listColliders.Size = new System.Drawing.Size(245, 438);
            this.listColliders.TabIndex = 1;
            // 
            // AnimationView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Name = "AnimationView";
            this.Size = new System.Drawing.Size(1246, 888);
            this.Enter += new System.EventHandler(this.AnimationView_Enter);
            this.Leave += new System.EventHandler(this.AnimationView_Leave);
            this.splitContainer1.Panel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.tabPage2.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Button btnXFileOpen;
        private System.Windows.Forms.Button btnJsonOpen;
        private System.Windows.Forms.Button btnJSONSave;
        private System.Windows.Forms.ComboBox comboAnim;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.DataGridViewComboBoxColumn AnimIndex;
        private System.Windows.Forms.DataGridViewTextBoxColumn animStringID;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.PropertyGrid pgCollider;
        private System.Windows.Forms.ListBox listColliders;
        private System.Windows.Forms.Button btnAddCollider;
    }
}

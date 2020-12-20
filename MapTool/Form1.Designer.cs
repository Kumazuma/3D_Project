
namespace MapTool
{
    partial class Form1
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

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.dockPanel1 = new WeifenLuo.WinFormsUI.Docking.DockPanel();
            this.vS2015DarkTheme1 = new WeifenLuo.WinFormsUI.Docking.VS2015DarkTheme();
            this.visualStudioToolStripExtender1 = new WeifenLuo.WinFormsUI.Docking.VisualStudioToolStripExtender(this.components);
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.파일ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.OpenFIleToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.projectDirectoryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.종료ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.추가ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.TerrainToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.StaticXMeshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.wowMapMashToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.naviMeshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.targetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // dockPanel1
            // 
            this.dockPanel1.CausesValidation = false;
            this.dockPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dockPanel1.DockBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(45)))), ((int)(((byte)(48)))));
            this.dockPanel1.DocumentStyle = WeifenLuo.WinFormsUI.Docking.DocumentStyle.DockingWindow;
            this.dockPanel1.Location = new System.Drawing.Point(0, 24);
            this.dockPanel1.Name = "dockPanel1";
            this.dockPanel1.Padding = new System.Windows.Forms.Padding(6);
            this.dockPanel1.ShowAutoHideContentOnHover = false;
            this.dockPanel1.Size = new System.Drawing.Size(800, 426);
            this.dockPanel1.TabIndex = 0;
            this.dockPanel1.Theme = this.vS2015DarkTheme1;
            // 
            // visualStudioToolStripExtender1
            // 
            this.visualStudioToolStripExtender1.DefaultRenderer = null;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.파일ToolStripMenuItem,
            this.추가ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(800, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // 파일ToolStripMenuItem
            // 
            this.파일ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.OpenFIleToolStripMenuItem,
            this.projectDirectoryToolStripMenuItem,
            this.SaveToolStripMenuItem,
            this.종료ToolStripMenuItem});
            this.파일ToolStripMenuItem.Name = "파일ToolStripMenuItem";
            this.파일ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.파일ToolStripMenuItem.Text = "파일";
            // 
            // OpenFIleToolStripMenuItem
            // 
            this.OpenFIleToolStripMenuItem.Name = "OpenFIleToolStripMenuItem";
            this.OpenFIleToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.OpenFIleToolStripMenuItem.Text = "열기";
            this.OpenFIleToolStripMenuItem.Click += new System.EventHandler(this.OpenFIleToolStripMenuItem_Click);
            // 
            // projectDirectoryToolStripMenuItem
            // 
            this.projectDirectoryToolStripMenuItem.Name = "projectDirectoryToolStripMenuItem";
            this.projectDirectoryToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.projectDirectoryToolStripMenuItem.Text = "기준 폴더";
            this.projectDirectoryToolStripMenuItem.Click += new System.EventHandler(this.projectDirectoryToolStripMenuItem_Click);
            // 
            // SaveToolStripMenuItem
            // 
            this.SaveToolStripMenuItem.Name = "SaveToolStripMenuItem";
            this.SaveToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.SaveToolStripMenuItem.Text = "저장";
            this.SaveToolStripMenuItem.Click += new System.EventHandler(this.SaveToolStripMenuItem_Click);
            // 
            // 종료ToolStripMenuItem
            // 
            this.종료ToolStripMenuItem.Name = "종료ToolStripMenuItem";
            this.종료ToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.종료ToolStripMenuItem.Text = "종료";
            // 
            // 추가ToolStripMenuItem
            // 
            this.추가ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TerrainToolStripMenuItem,
            this.StaticXMeshToolStripMenuItem,
            this.wowMapMashToolStripMenuItem,
            this.naviMeshToolStripMenuItem,
            this.targetToolStripMenuItem});
            this.추가ToolStripMenuItem.Name = "추가ToolStripMenuItem";
            this.추가ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.추가ToolStripMenuItem.Text = "추가";
            // 
            // TerrainToolStripMenuItem
            // 
            this.TerrainToolStripMenuItem.Name = "TerrainToolStripMenuItem";
            this.TerrainToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
            this.TerrainToolStripMenuItem.Text = "터레인";
            this.TerrainToolStripMenuItem.Click += new System.EventHandler(this.TerrainToolStripMenuItem_Click);
            // 
            // StaticXMeshToolStripMenuItem
            // 
            this.StaticXMeshToolStripMenuItem.Name = "StaticXMeshToolStripMenuItem";
            this.StaticXMeshToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
            this.StaticXMeshToolStripMenuItem.Text = "스태틱 X 메시";
            this.StaticXMeshToolStripMenuItem.Click += new System.EventHandler(this.StaticXMeshToolStripMenuItem_Click);
            // 
            // wowMapMashToolStripMenuItem
            // 
            this.wowMapMashToolStripMenuItem.Name = "wowMapMashToolStripMenuItem";
            this.wowMapMashToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
            this.wowMapMashToolStripMenuItem.Text = "WoW Map 메시";
            this.wowMapMashToolStripMenuItem.Click += new System.EventHandler(this.wowMapMashToolStripMenuItem_Click);
            // 
            // naviMeshToolStripMenuItem
            // 
            this.naviMeshToolStripMenuItem.Name = "naviMeshToolStripMenuItem";
            this.naviMeshToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
            this.naviMeshToolStripMenuItem.Text = "Navi 메시";
            this.naviMeshToolStripMenuItem.Click += new System.EventHandler(this.naviMeshToolStripMenuItem_Click);
            // 
            // targetToolStripMenuItem
            // 
            this.targetToolStripMenuItem.Name = "targetToolStripMenuItem";
            this.targetToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.targetToolStripMenuItem.Text = "Target";
            this.targetToolStripMenuItem.Click += new System.EventHandler(this.targetToolStripMenuItem_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.dockPanel1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Form1";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private WeifenLuo.WinFormsUI.Docking.DockPanel dockPanel1;
        private WeifenLuo.WinFormsUI.Docking.VisualStudioToolStripExtender visualStudioToolStripExtender1;
        private WeifenLuo.WinFormsUI.Docking.VS2015DarkTheme vS2015DarkTheme1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 파일ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem OpenFIleToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem SaveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 종료ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 추가ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem TerrainToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem StaticXMeshToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem wowMapMashToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem naviMeshToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem projectDirectoryToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem targetToolStripMenuItem;
    }
}


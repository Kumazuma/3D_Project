
namespace MaptoolNightbuild.MapEditor
{
    partial class MainFrame
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
            this.components = new System.ComponentModel.Container();
            this.dockPanel1 = new WeifenLuo.WinFormsUI.Docking.DockPanel();
            this.vS2005Theme1 = new WeifenLuo.WinFormsUI.Docking.VS2005Theme();
            this.visualStudioToolStripExtender1 = new WeifenLuo.WinFormsUI.Docking.VisualStudioToolStripExtender(this.components);
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.파일ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.imNewMap = new System.Windows.Forms.ToolStripMenuItem();
            this.저장ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.열기ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.오브젝트추가ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.miAddOBJMesh = new System.Windows.Forms.ToolStripMenuItem();
            this.miAddTarget = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // dockPanel1
            // 
            this.dockPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dockPanel1.DocumentStyle = WeifenLuo.WinFormsUI.Docking.DocumentStyle.DockingWindow;
            this.dockPanel1.Location = new System.Drawing.Point(0, 24);
            this.dockPanel1.Name = "dockPanel1";
            this.dockPanel1.Size = new System.Drawing.Size(1667, 909);
            this.dockPanel1.TabIndex = 0;
            // 
            // visualStudioToolStripExtender1
            // 
            this.visualStudioToolStripExtender1.DefaultRenderer = null;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.파일ToolStripMenuItem,
            this.오브젝트추가ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1667, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // 파일ToolStripMenuItem
            // 
            this.파일ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.imNewMap,
            this.저장ToolStripMenuItem,
            this.열기ToolStripMenuItem});
            this.파일ToolStripMenuItem.Name = "파일ToolStripMenuItem";
            this.파일ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.파일ToolStripMenuItem.Text = "파일";
            // 
            // imNewMap
            // 
            this.imNewMap.Name = "imNewMap";
            this.imNewMap.Size = new System.Drawing.Size(126, 22);
            this.imNewMap.Text = "새 맵파일";
            this.imNewMap.Click += new System.EventHandler(this.imNewMap_Click);
            // 
            // 저장ToolStripMenuItem
            // 
            this.저장ToolStripMenuItem.Name = "저장ToolStripMenuItem";
            this.저장ToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
            this.저장ToolStripMenuItem.Text = "저장";
            // 
            // 열기ToolStripMenuItem
            // 
            this.열기ToolStripMenuItem.Name = "열기ToolStripMenuItem";
            this.열기ToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
            this.열기ToolStripMenuItem.Text = "열기";
            // 
            // 오브젝트추가ToolStripMenuItem
            // 
            this.오브젝트추가ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miAddOBJMesh,
            this.miAddTarget});
            this.오브젝트추가ToolStripMenuItem.Name = "오브젝트추가ToolStripMenuItem";
            this.오브젝트추가ToolStripMenuItem.Size = new System.Drawing.Size(95, 20);
            this.오브젝트추가ToolStripMenuItem.Text = "오브젝트 추가";
            // 
            // miAddOBJMesh
            // 
            this.miAddOBJMesh.Name = "miAddOBJMesh";
            this.miAddOBJMesh.Size = new System.Drawing.Size(180, 22);
            this.miAddOBJMesh.Text = "OBJ 메시";
            this.miAddOBJMesh.Click += new System.EventHandler(this.miAddOBJMesh_Click);
            // 
            // miAddTarget
            // 
            this.miAddTarget.Name = "miAddTarget";
            this.miAddTarget.Size = new System.Drawing.Size(180, 22);
            this.miAddTarget.Text = "타겟 추가";
            this.miAddTarget.Click += new System.EventHandler(this.miAddTarget_Click);
            // 
            // MainFrame
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.dockPanel1);
            this.Controls.Add(this.menuStrip1);
            this.Name = "MainFrame";
            this.Size = new System.Drawing.Size(1667, 933);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private WeifenLuo.WinFormsUI.Docking.DockPanel dockPanel1;
        private WeifenLuo.WinFormsUI.Docking.VS2005Theme vS2005Theme1;
        private WeifenLuo.WinFormsUI.Docking.VisualStudioToolStripExtender visualStudioToolStripExtender1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 파일ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem imNewMap;
        private System.Windows.Forms.ToolStripMenuItem 저장ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 열기ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 오브젝트추가ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem miAddOBJMesh;
        private System.Windows.Forms.ToolStripMenuItem miAddTarget;
    }
}

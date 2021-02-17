
namespace MaptoolNightbuild.CharacterMetaEditor.View
{
    partial class CharacterMetaRenderView
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnStop = new System.Windows.Forms.Button();
            this.btnPlay = new System.Windows.Forms.Button();
            this.btnSaveJSON = new System.Windows.Forms.Button();
            this.btnOpenJson = new System.Windows.Forms.Button();
            this.btnLoadXMesh = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.btnStop);
            this.panel1.Controls.Add(this.btnPlay);
            this.panel1.Controls.Add(this.btnSaveJSON);
            this.panel1.Controls.Add(this.btnOpenJson);
            this.panel1.Controls.Add(this.btnLoadXMesh);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1400, 29);
            this.panel1.TabIndex = 0;
            // 
            // btnStop
            // 
            this.btnStop.Location = new System.Drawing.Point(346, 3);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(75, 23);
            this.btnStop.TabIndex = 1;
            this.btnStop.Text = "정지";
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // btnPlay
            // 
            this.btnPlay.Location = new System.Drawing.Point(265, 3);
            this.btnPlay.Name = "btnPlay";
            this.btnPlay.Size = new System.Drawing.Size(75, 23);
            this.btnPlay.TabIndex = 1;
            this.btnPlay.Text = "재생";
            this.btnPlay.UseVisualStyleBackColor = true;
            this.btnPlay.Click += new System.EventHandler(this.btnPlay_Click);
            // 
            // btnSaveJSON
            // 
            this.btnSaveJSON.Location = new System.Drawing.Point(184, 3);
            this.btnSaveJSON.Name = "btnSaveJSON";
            this.btnSaveJSON.Size = new System.Drawing.Size(75, 23);
            this.btnSaveJSON.TabIndex = 1;
            this.btnSaveJSON.Text = "JSON 저장";
            this.btnSaveJSON.UseVisualStyleBackColor = true;
            // 
            // btnOpenJson
            // 
            this.btnOpenJson.Location = new System.Drawing.Point(103, 3);
            this.btnOpenJson.Name = "btnOpenJson";
            this.btnOpenJson.Size = new System.Drawing.Size(75, 23);
            this.btnOpenJson.TabIndex = 1;
            this.btnOpenJson.Text = "JSON 열기";
            this.btnOpenJson.UseVisualStyleBackColor = true;
            // 
            // btnLoadXMesh
            // 
            this.btnLoadXMesh.Location = new System.Drawing.Point(3, 3);
            this.btnLoadXMesh.Name = "btnLoadXMesh";
            this.btnLoadXMesh.Size = new System.Drawing.Size(94, 23);
            this.btnLoadXMesh.TabIndex = 0;
            this.btnLoadXMesh.Text = "X 메시 로드";
            this.btnLoadXMesh.UseVisualStyleBackColor = true;
            this.btnLoadXMesh.Click += new System.EventHandler(this.btnLoadXMesh_Click);
            // 
            // CharacterMetaRenderView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panel1);
            this.Name = "CharacterMetaRenderView";
            this.Size = new System.Drawing.Size(1400, 916);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.Button btnPlay;
        private System.Windows.Forms.Button btnSaveJSON;
        private System.Windows.Forms.Button btnOpenJson;
        private System.Windows.Forms.Button btnLoadXMesh;
    }
}

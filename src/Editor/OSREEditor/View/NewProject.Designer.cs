using OSREEditor.Model.Actions;

namespace OSREEditor.View
{
    partial class NewProject
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.ProjectName = new System.Windows.Forms.TextBox();
            this.NewProjectButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.CancelButton = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.ProjectType = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // ProjectName
            // 
            this.ProjectName.Location = new System.Drawing.Point(177, 16);
            this.ProjectName.Name = "ProjectName";
            this.ProjectName.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.ProjectName.Size = new System.Drawing.Size(120, 20);
            this.ProjectName.TabIndex = 0;
            this.ProjectName.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // NewProjectButton
            // 
            this.NewProjectButton.Location = new System.Drawing.Point(15, 86);
            this.NewProjectButton.Name = "NewProjectButton";
            this.NewProjectButton.Size = new System.Drawing.Size(93, 45);
            this.NewProjectButton.TabIndex = 1;
            this.NewProjectButton.Text = "New Project";
            this.NewProjectButton.UseVisualStyleBackColor = true;
            this.NewProjectButton.Click += new System.EventHandler(this.NewProjectButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Project Name";
            // 
            // CancelButton
            // 
            this.CancelButton.Location = new System.Drawing.Point(188, 86);
            this.CancelButton.Name = "CancelButton";
            this.CancelButton.Size = new System.Drawing.Size(109, 45);
            this.CancelButton.TabIndex = 3;
            this.CancelButton.Text = "Cancel";
            this.CancelButton.UseVisualStyleBackColor = true;
            this.CancelButton.Click += new System.EventHandler(this.CancelProjectButton_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 43);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(67, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Project Type";
            // 
            // ProjectType
            // 
            this.ProjectType.FormattingEnabled = true;
            this.ProjectType.Items.AddRange(new object[] {
            "3D-Scene",
            "2D-Scene"});
            this.ProjectType.Location = new System.Drawing.Point(177, 43);
            this.ProjectType.Name = "ProjectType";
            this.ProjectType.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.ProjectType.Size = new System.Drawing.Size(120, 21);
            this.ProjectType.TabIndex = 5;
            // 
            // NewProject
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(309, 143);
            this.Controls.Add(this.ProjectType);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.CancelButton);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.NewProjectButton);
            this.Controls.Add(this.ProjectName);
            this.Name = "NewProject";
            this.Text = "Generate new project";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox ProjectName;
        private System.Windows.Forms.Button NewProjectButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button CancelButton;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox ProjectType;
    }
}
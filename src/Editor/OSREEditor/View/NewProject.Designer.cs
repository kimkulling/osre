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
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.ProjectAuthor = new System.Windows.Forms.TextBox();
            this.ProjectMajorVersion = new System.Windows.Forms.TextBox();
            this.ProjectMinorVersion = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
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
            this.NewProjectButton.Location = new System.Drawing.Point(15, 135);
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
            this.CancelButton.Location = new System.Drawing.Point(188, 135);
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
            this.ProjectType.BackColor = System.Drawing.SystemColors.Menu;
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
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 75);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(38, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Author";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 102);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(42, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Version";
            // 
            // ProjectAuthor
            // 
            this.ProjectAuthor.Location = new System.Drawing.Point(177, 74);
            this.ProjectAuthor.Name = "ProjectAuthor";
            this.ProjectAuthor.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.ProjectAuthor.Size = new System.Drawing.Size(120, 20);
            this.ProjectAuthor.TabIndex = 8;
            this.ProjectAuthor.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // ProjectMajorVersion
            // 
            this.ProjectMajorVersion.Location = new System.Drawing.Point(177, 102);
            this.ProjectMajorVersion.Name = "ProjectMajorVersion";
            this.ProjectMajorVersion.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.ProjectMajorVersion.Size = new System.Drawing.Size(52, 20);
            this.ProjectMajorVersion.TabIndex = 9;
            this.ProjectMajorVersion.Text = "0";
            this.ProjectMajorVersion.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // ProjectMinorVersion
            // 
            this.ProjectMinorVersion.Location = new System.Drawing.Point(251, 102);
            this.ProjectMinorVersion.Name = "ProjectMinorVersion";
            this.ProjectMinorVersion.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.ProjectMinorVersion.Size = new System.Drawing.Size(46, 20);
            this.ProjectMinorVersion.TabIndex = 10;
            this.ProjectMinorVersion.Text = "1";
            this.ProjectMinorVersion.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(235, 105);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(11, 13);
            this.label5.TabIndex = 11;
            this.label5.Text = ".";
            // 
            // NewProject
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(367, 204);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.ProjectMinorVersion);
            this.Controls.Add(this.ProjectMajorVersion);
            this.Controls.Add(this.ProjectAuthor);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
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
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox ProjectAuthor;
        private System.Windows.Forms.TextBox ProjectMajorVersion;
        private System.Windows.Forms.TextBox ProjectMinorVersion;
        private System.Windows.Forms.Label label5;
    }
}
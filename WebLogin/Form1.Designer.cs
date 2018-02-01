namespace WebLogin
{
    partial class Form1
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
            this.webBrowser1 = new System.Windows.Forms.WebBrowser();
            this.label1 = new System.Windows.Forms.Label();
            this.lbLevel = new System.Windows.Forms.Label();
            this.lbSumFightCount = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.lbGold = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.lbNiceName = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.lbMsg = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.txtLevel = new System.Windows.Forms.TextBox();
            this.txtSumCount = new System.Windows.Forms.TextBox();
            this.txtGold = new System.Windows.Forms.TextBox();
            this.txtNiceName = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // webBrowser1
            // 
            this.webBrowser1.Location = new System.Drawing.Point(19, 137);
            this.webBrowser1.MinimumSize = new System.Drawing.Size(20, 20);
            this.webBrowser1.Name = "webBrowser1";
            this.webBrowser1.ScriptErrorsSuppressed = true;
            this.webBrowser1.Size = new System.Drawing.Size(743, 480);
            this.webBrowser1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "等级:";
            // 
            // lbLevel
            // 
            this.lbLevel.AutoSize = true;
            this.lbLevel.ForeColor = System.Drawing.Color.Red;
            this.lbLevel.Location = new System.Drawing.Point(54, 13);
            this.lbLevel.Name = "lbLevel";
            this.lbLevel.Size = new System.Drawing.Size(11, 12);
            this.lbLevel.TabIndex = 2;
            this.lbLevel.Text = "0";
            // 
            // lbSumFightCount
            // 
            this.lbSumFightCount.AutoSize = true;
            this.lbSumFightCount.ForeColor = System.Drawing.Color.Red;
            this.lbSumFightCount.Location = new System.Drawing.Point(54, 41);
            this.lbSumFightCount.Name = "lbSumFightCount";
            this.lbSumFightCount.Size = new System.Drawing.Size(11, 12);
            this.lbSumFightCount.TabIndex = 4;
            this.lbSumFightCount.Text = "0";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(17, 41);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 12);
            this.label4.TabIndex = 3;
            this.label4.Text = "总场:";
            // 
            // lbGold
            // 
            this.lbGold.AutoSize = true;
            this.lbGold.ForeColor = System.Drawing.Color.Red;
            this.lbGold.Location = new System.Drawing.Point(54, 70);
            this.lbGold.Name = "lbGold";
            this.lbGold.Size = new System.Drawing.Size(11, 12);
            this.lbGold.TabIndex = 6;
            this.lbGold.Text = "0";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(17, 70);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(29, 12);
            this.label6.TabIndex = 5;
            this.label6.Text = "金币";
            // 
            // lbNiceName
            // 
            this.lbNiceName.AutoSize = true;
            this.lbNiceName.ForeColor = System.Drawing.Color.Red;
            this.lbNiceName.Location = new System.Drawing.Point(54, 95);
            this.lbNiceName.Name = "lbNiceName";
            this.lbNiceName.Size = new System.Drawing.Size(11, 12);
            this.lbNiceName.TabIndex = 8;
            this.lbNiceName.Text = "0";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(17, 95);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(35, 12);
            this.label8.TabIndex = 7;
            this.label8.Text = "昵称:";
            // 
            // lbMsg
            // 
            this.lbMsg.AutoSize = true;
            this.lbMsg.ForeColor = System.Drawing.Color.Red;
            this.lbMsg.Location = new System.Drawing.Point(17, 122);
            this.lbMsg.Name = "lbMsg";
            this.lbMsg.Size = new System.Drawing.Size(53, 12);
            this.lbMsg.TabIndex = 9;
            this.lbMsg.Text = "Wait……";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(657, 90);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 10;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // txtLevel
            // 
            this.txtLevel.Location = new System.Drawing.Point(88, 10);
            this.txtLevel.Name = "txtLevel";
            this.txtLevel.Size = new System.Drawing.Size(100, 21);
            this.txtLevel.TabIndex = 11;
            // 
            // txtSumCount
            // 
            this.txtSumCount.Location = new System.Drawing.Point(88, 41);
            this.txtSumCount.Name = "txtSumCount";
            this.txtSumCount.Size = new System.Drawing.Size(100, 21);
            this.txtSumCount.TabIndex = 12;
            // 
            // txtGold
            // 
            this.txtGold.Location = new System.Drawing.Point(88, 68);
            this.txtGold.Name = "txtGold";
            this.txtGold.Size = new System.Drawing.Size(100, 21);
            this.txtGold.TabIndex = 13;
            // 
            // txtNiceName
            // 
            this.txtNiceName.Location = new System.Drawing.Point(88, 95);
            this.txtNiceName.Name = "txtNiceName";
            this.txtNiceName.Size = new System.Drawing.Size(100, 21);
            this.txtNiceName.TabIndex = 14;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(774, 620);
            this.Controls.Add(this.txtNiceName);
            this.Controls.Add(this.txtGold);
            this.Controls.Add(this.txtSumCount);
            this.Controls.Add(this.txtLevel);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.lbMsg);
            this.Controls.Add(this.lbNiceName);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.lbGold);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.lbSumFightCount);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.lbLevel);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.webBrowser1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.WebBrowser webBrowser1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lbLevel;
        private System.Windows.Forms.Label lbSumFightCount;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label lbGold;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label lbNiceName;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label lbMsg;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox txtLevel;
        private System.Windows.Forms.TextBox txtSumCount;
        private System.Windows.Forms.TextBox txtGold;
        private System.Windows.Forms.TextBox txtNiceName;
    }
}


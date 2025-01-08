<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing AndAlso components IsNot Nothing Then
            components.Dispose()
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Me.Button_Read_EE_Execute = New System.Windows.Forms.Button
        Me.GroupBox1 = New System.Windows.Forms.GroupBox
        Me.Label2 = New System.Windows.Forms.Label
        Me.TextBox_Read_EE_Byte_Count = New System.Windows.Forms.TextBox
        Me.Label1 = New System.Windows.Forms.Label
        Me.TextBox_Read_EE_Word_Addr = New System.Windows.Forms.TextBox
        Me.RichTextBox_Display = New System.Windows.Forms.RichTextBox
        Me.GroupBox2 = New System.Windows.Forms.GroupBox
        Me.Label5 = New System.Windows.Forms.Label
        Me.TextBox_Write_EE_Byte2 = New System.Windows.Forms.TextBox
        Me.Button_Write_EE_Execute = New System.Windows.Forms.Button
        Me.Label3 = New System.Windows.Forms.Label
        Me.TextBox_Write_EE_Byte1 = New System.Windows.Forms.TextBox
        Me.Label4 = New System.Windows.Forms.Label
        Me.TextBox_Write_EE_Word_Addr = New System.Windows.Forms.TextBox
        Me.GroupBox3 = New System.Windows.Forms.GroupBox
        Me.Label6 = New System.Windows.Forms.Label
        Me.TextBox_Receive_EE_Byte_Count = New System.Windows.Forms.TextBox
        Me.Button_Receive_EE_Execute = New System.Windows.Forms.Button
        Me.GroupBox4 = New System.Windows.Forms.GroupBox
        Me.CheckBox_RTC = New System.Windows.Forms.CheckBox
        Me.Label_RTC = New System.Windows.Forms.Label
        Me.Timer_RTC = New System.Windows.Forms.Timer(Me.components)
        Me.GroupBox5 = New System.Windows.Forms.GroupBox
        Me.ProgressBar_ADC = New System.Windows.Forms.ProgressBar
        Me.Label_ADC_Count = New System.Windows.Forms.Label
        Me.CheckBox_ADC = New System.Windows.Forms.CheckBox
        Me.Timer_ADC = New System.Windows.Forms.Timer(Me.components)
        Me.GroupBox6 = New System.Windows.Forms.GroupBox
        Me.TextBox_Bit_Rate_Set = New System.Windows.Forms.TextBox
        Me.Button_Get_Bit_Rate = New System.Windows.Forms.Button
        Me.Button_Set_Bit_Rate = New System.Windows.Forms.Button
        Me.GroupBox7 = New System.Windows.Forms.GroupBox
        Me.Button_Use_Ext_Volt = New System.Windows.Forms.Button
        Me.TextBox_Volt = New System.Windows.Forms.TextBox
        Me.Button_Set_Volt = New System.Windows.Forms.Button
        Me.Button_Get_Volt = New System.Windows.Forms.Button
        Me.GroupBox8 = New System.Windows.Forms.GroupBox
        Me.Button_Set_Aux_Status = New System.Windows.Forms.Button
        Me.CheckBox_Dir_2 = New System.Windows.Forms.CheckBox
        Me.CheckBox_State_2 = New System.Windows.Forms.CheckBox
        Me.CheckBox_Dir_1 = New System.Windows.Forms.CheckBox
        Me.CheckBox_State_1 = New System.Windows.Forms.CheckBox
        Me.Button_Get_Aux_Status = New System.Windows.Forms.Button
        Me.GroupBox1.SuspendLayout()
        Me.GroupBox2.SuspendLayout()
        Me.GroupBox3.SuspendLayout()
        Me.GroupBox4.SuspendLayout()
        Me.GroupBox5.SuspendLayout()
        Me.GroupBox6.SuspendLayout()
        Me.GroupBox7.SuspendLayout()
        Me.GroupBox8.SuspendLayout()
        Me.SuspendLayout()
        '
        'Button_Read_EE_Execute
        '
        Me.Button_Read_EE_Execute.Location = New System.Drawing.Point(133, 35)
        Me.Button_Read_EE_Execute.Name = "Button_Read_EE_Execute"
        Me.Button_Read_EE_Execute.Size = New System.Drawing.Size(75, 23)
        Me.Button_Read_EE_Execute.TabIndex = 0
        Me.Button_Read_EE_Execute.Text = "Execute"
        Me.Button_Read_EE_Execute.UseVisualStyleBackColor = True
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.Label2)
        Me.GroupBox1.Controls.Add(Me.TextBox_Read_EE_Byte_Count)
        Me.GroupBox1.Controls.Add(Me.Label1)
        Me.GroupBox1.Controls.Add(Me.Button_Read_EE_Execute)
        Me.GroupBox1.Controls.Add(Me.TextBox_Read_EE_Word_Addr)
        Me.GroupBox1.Location = New System.Drawing.Point(85, 4)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(222, 75)
        Me.GroupBox1.TabIndex = 1
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "EE Read"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(69, 21)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(59, 13)
        Me.Label2.TabIndex = 3
        Me.Label2.Text = "Byte Count"
        '
        'TextBox_Read_EE_Byte_Count
        '
        Me.TextBox_Read_EE_Byte_Count.Location = New System.Drawing.Point(81, 37)
        Me.TextBox_Read_EE_Byte_Count.Name = "TextBox_Read_EE_Byte_Count"
        Me.TextBox_Read_EE_Byte_Count.Size = New System.Drawing.Size(30, 20)
        Me.TextBox_Read_EE_Byte_Count.TabIndex = 3
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(4, 21)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(58, 13)
        Me.Label1.TabIndex = 2
        Me.Label1.Text = "Word Addr"
        '
        'TextBox_Read_EE_Word_Addr
        '
        Me.TextBox_Read_EE_Word_Addr.Location = New System.Drawing.Point(18, 37)
        Me.TextBox_Read_EE_Word_Addr.Name = "TextBox_Read_EE_Word_Addr"
        Me.TextBox_Read_EE_Word_Addr.Size = New System.Drawing.Size(30, 20)
        Me.TextBox_Read_EE_Word_Addr.TabIndex = 2
        '
        'RichTextBox_Display
        '
        Me.RichTextBox_Display.Location = New System.Drawing.Point(62, 249)
        Me.RichTextBox_Display.Name = "RichTextBox_Display"
        Me.RichTextBox_Display.Size = New System.Drawing.Size(266, 115)
        Me.RichTextBox_Display.TabIndex = 2
        Me.RichTextBox_Display.Text = ""
        '
        'GroupBox2
        '
        Me.GroupBox2.Controls.Add(Me.Label5)
        Me.GroupBox2.Controls.Add(Me.TextBox_Write_EE_Byte2)
        Me.GroupBox2.Controls.Add(Me.Button_Write_EE_Execute)
        Me.GroupBox2.Controls.Add(Me.Label3)
        Me.GroupBox2.Controls.Add(Me.TextBox_Write_EE_Byte1)
        Me.GroupBox2.Controls.Add(Me.Label4)
        Me.GroupBox2.Controls.Add(Me.TextBox_Write_EE_Word_Addr)
        Me.GroupBox2.Location = New System.Drawing.Point(85, 85)
        Me.GroupBox2.Name = "GroupBox2"
        Me.GroupBox2.Size = New System.Drawing.Size(222, 72)
        Me.GroupBox2.TabIndex = 6
        Me.GroupBox2.TabStop = False
        Me.GroupBox2.Text = "EE 2 Byte Write"
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Location = New System.Drawing.Point(101, 19)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(34, 13)
        Me.Label5.TabIndex = 6
        Me.Label5.Text = "Byte2"
        '
        'TextBox_Write_EE_Byte2
        '
        Me.TextBox_Write_EE_Byte2.Location = New System.Drawing.Point(100, 35)
        Me.TextBox_Write_EE_Byte2.Name = "TextBox_Write_EE_Byte2"
        Me.TextBox_Write_EE_Byte2.Size = New System.Drawing.Size(36, 20)
        Me.TextBox_Write_EE_Byte2.TabIndex = 5
        '
        'Button_Write_EE_Execute
        '
        Me.Button_Write_EE_Execute.Location = New System.Drawing.Point(141, 34)
        Me.Button_Write_EE_Execute.Name = "Button_Write_EE_Execute"
        Me.Button_Write_EE_Execute.Size = New System.Drawing.Size(75, 23)
        Me.Button_Write_EE_Execute.TabIndex = 4
        Me.Button_Write_EE_Execute.Text = "Execute"
        Me.Button_Write_EE_Execute.UseVisualStyleBackColor = True
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(60, 19)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(34, 13)
        Me.Label3.TabIndex = 3
        Me.Label3.Text = "Byte1"
        '
        'TextBox_Write_EE_Byte1
        '
        Me.TextBox_Write_EE_Byte1.Location = New System.Drawing.Point(59, 35)
        Me.TextBox_Write_EE_Byte1.Name = "TextBox_Write_EE_Byte1"
        Me.TextBox_Write_EE_Byte1.Size = New System.Drawing.Size(36, 20)
        Me.TextBox_Write_EE_Byte1.TabIndex = 2
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Location = New System.Drawing.Point(3, 19)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(58, 13)
        Me.Label4.TabIndex = 1
        Me.Label4.Text = "Word Addr"
        '
        'TextBox_Write_EE_Word_Addr
        '
        Me.TextBox_Write_EE_Word_Addr.Location = New System.Drawing.Point(11, 35)
        Me.TextBox_Write_EE_Word_Addr.Name = "TextBox_Write_EE_Word_Addr"
        Me.TextBox_Write_EE_Word_Addr.Size = New System.Drawing.Size(36, 20)
        Me.TextBox_Write_EE_Word_Addr.TabIndex = 0
        '
        'GroupBox3
        '
        Me.GroupBox3.Controls.Add(Me.Label6)
        Me.GroupBox3.Controls.Add(Me.TextBox_Receive_EE_Byte_Count)
        Me.GroupBox3.Controls.Add(Me.Button_Receive_EE_Execute)
        Me.GroupBox3.Location = New System.Drawing.Point(85, 163)
        Me.GroupBox3.Name = "GroupBox3"
        Me.GroupBox3.Size = New System.Drawing.Size(222, 75)
        Me.GroupBox3.TabIndex = 4
        Me.GroupBox3.TabStop = False
        Me.GroupBox3.Text = "EE Receive"
        '
        'Label6
        '
        Me.Label6.AutoSize = True
        Me.Label6.Location = New System.Drawing.Point(69, 19)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(59, 13)
        Me.Label6.TabIndex = 3
        Me.Label6.Text = "Byte Count"
        '
        'TextBox_Receive_EE_Byte_Count
        '
        Me.TextBox_Receive_EE_Byte_Count.Location = New System.Drawing.Point(81, 35)
        Me.TextBox_Receive_EE_Byte_Count.Name = "TextBox_Receive_EE_Byte_Count"
        Me.TextBox_Receive_EE_Byte_Count.Size = New System.Drawing.Size(30, 20)
        Me.TextBox_Receive_EE_Byte_Count.TabIndex = 3
        '
        'Button_Receive_EE_Execute
        '
        Me.Button_Receive_EE_Execute.Location = New System.Drawing.Point(141, 33)
        Me.Button_Receive_EE_Execute.Name = "Button_Receive_EE_Execute"
        Me.Button_Receive_EE_Execute.Size = New System.Drawing.Size(75, 23)
        Me.Button_Receive_EE_Execute.TabIndex = 0
        Me.Button_Receive_EE_Execute.Text = "Execute"
        Me.Button_Receive_EE_Execute.UseVisualStyleBackColor = True
        '
        'GroupBox4
        '
        Me.GroupBox4.Controls.Add(Me.CheckBox_RTC)
        Me.GroupBox4.Controls.Add(Me.Label_RTC)
        Me.GroupBox4.Location = New System.Drawing.Point(85, 370)
        Me.GroupBox4.Name = "GroupBox4"
        Me.GroupBox4.Size = New System.Drawing.Size(222, 47)
        Me.GroupBox4.TabIndex = 8
        Me.GroupBox4.TabStop = False
        Me.GroupBox4.Text = "RTC"
        '
        'CheckBox_RTC
        '
        Me.CheckBox_RTC.AutoSize = True
        Me.CheckBox_RTC.Location = New System.Drawing.Point(72, 18)
        Me.CheckBox_RTC.Name = "CheckBox_RTC"
        Me.CheckBox_RTC.Size = New System.Drawing.Size(140, 17)
        Me.CheckBox_RTC.TabIndex = 2
        Me.CheckBox_RTC.Text = "Enable Real Time Clock"
        Me.CheckBox_RTC.UseVisualStyleBackColor = True
        '
        'Label_RTC
        '
        Me.Label_RTC.AutoSize = True
        Me.Label_RTC.Location = New System.Drawing.Point(8, 19)
        Me.Label_RTC.Name = "Label_RTC"
        Me.Label_RTC.Size = New System.Drawing.Size(49, 13)
        Me.Label_RTC.TabIndex = 1
        Me.Label_RTC.Text = "00:00:00"
        '
        'Timer_RTC
        '
        Me.Timer_RTC.Interval = 500
        '
        'GroupBox5
        '
        Me.GroupBox5.Controls.Add(Me.ProgressBar_ADC)
        Me.GroupBox5.Controls.Add(Me.Label_ADC_Count)
        Me.GroupBox5.Controls.Add(Me.CheckBox_ADC)
        Me.GroupBox5.Location = New System.Drawing.Point(85, 430)
        Me.GroupBox5.Name = "GroupBox5"
        Me.GroupBox5.Size = New System.Drawing.Size(222, 66)
        Me.GroupBox5.TabIndex = 9
        Me.GroupBox5.TabStop = False
        Me.GroupBox5.Text = "ADC"
        '
        'ProgressBar_ADC
        '
        Me.ProgressBar_ADC.Location = New System.Drawing.Point(7, 44)
        Me.ProgressBar_ADC.Maximum = 255
        Me.ProgressBar_ADC.Name = "ProgressBar_ADC"
        Me.ProgressBar_ADC.Size = New System.Drawing.Size(205, 10)
        Me.ProgressBar_ADC.TabIndex = 9
        '
        'Label_ADC_Count
        '
        Me.Label_ADC_Count.AutoSize = True
        Me.Label_ADC_Count.Location = New System.Drawing.Point(7, 20)
        Me.Label_ADC_Count.Name = "Label_ADC_Count"
        Me.Label_ADC_Count.Size = New System.Drawing.Size(35, 13)
        Me.Label_ADC_Count.TabIndex = 9
        Me.Label_ADC_Count.Text = "Count"
        '
        'CheckBox_ADC
        '
        Me.CheckBox_ADC.AutoSize = True
        Me.CheckBox_ADC.Location = New System.Drawing.Point(72, 19)
        Me.CheckBox_ADC.Name = "CheckBox_ADC"
        Me.CheckBox_ADC.Size = New System.Drawing.Size(84, 17)
        Me.CheckBox_ADC.TabIndex = 2
        Me.CheckBox_ADC.Text = "Enable ADC"
        Me.CheckBox_ADC.UseVisualStyleBackColor = True
        '
        'Timer_ADC
        '
        '
        'GroupBox6
        '
        Me.GroupBox6.Controls.Add(Me.TextBox_Bit_Rate_Set)
        Me.GroupBox6.Controls.Add(Me.Button_Get_Bit_Rate)
        Me.GroupBox6.Controls.Add(Me.Button_Set_Bit_Rate)
        Me.GroupBox6.Location = New System.Drawing.Point(85, 505)
        Me.GroupBox6.Name = "GroupBox6"
        Me.GroupBox6.Size = New System.Drawing.Size(222, 47)
        Me.GroupBox6.TabIndex = 9
        Me.GroupBox6.TabStop = False
        Me.GroupBox6.Text = "Bit Rate"
        '
        'TextBox_Bit_Rate_Set
        '
        Me.TextBox_Bit_Rate_Set.Location = New System.Drawing.Point(100, 19)
        Me.TextBox_Bit_Rate_Set.Name = "TextBox_Bit_Rate_Set"
        Me.TextBox_Bit_Rate_Set.Size = New System.Drawing.Size(36, 20)
        Me.TextBox_Bit_Rate_Set.TabIndex = 8
        '
        'Button_Get_Bit_Rate
        '
        Me.Button_Get_Bit_Rate.Location = New System.Drawing.Point(6, 18)
        Me.Button_Get_Bit_Rate.Name = "Button_Get_Bit_Rate"
        Me.Button_Get_Bit_Rate.Size = New System.Drawing.Size(75, 23)
        Me.Button_Get_Bit_Rate.TabIndex = 4
        Me.Button_Get_Bit_Rate.Text = "Get"
        Me.Button_Get_Bit_Rate.UseVisualStyleBackColor = True
        '
        'Button_Set_Bit_Rate
        '
        Me.Button_Set_Bit_Rate.Location = New System.Drawing.Point(140, 18)
        Me.Button_Set_Bit_Rate.Name = "Button_Set_Bit_Rate"
        Me.Button_Set_Bit_Rate.Size = New System.Drawing.Size(75, 23)
        Me.Button_Set_Bit_Rate.TabIndex = 7
        Me.Button_Set_Bit_Rate.Text = "Set"
        Me.Button_Set_Bit_Rate.UseVisualStyleBackColor = True
        '
        'GroupBox7
        '
        Me.GroupBox7.Controls.Add(Me.Button_Use_Ext_Volt)
        Me.GroupBox7.Controls.Add(Me.TextBox_Volt)
        Me.GroupBox7.Controls.Add(Me.Button_Set_Volt)
        Me.GroupBox7.Controls.Add(Me.Button_Get_Volt)
        Me.GroupBox7.Location = New System.Drawing.Point(67, 560)
        Me.GroupBox7.Name = "GroupBox7"
        Me.GroupBox7.Size = New System.Drawing.Size(257, 57)
        Me.GroupBox7.TabIndex = 10
        Me.GroupBox7.TabStop = False
        Me.GroupBox7.Text = "Voltage"
        '
        'Button_Use_Ext_Volt
        '
        Me.Button_Use_Ext_Volt.Location = New System.Drawing.Point(197, 21)
        Me.Button_Use_Ext_Volt.Name = "Button_Use_Ext_Volt"
        Me.Button_Use_Ext_Volt.Size = New System.Drawing.Size(54, 23)
        Me.Button_Use_Ext_Volt.TabIndex = 8
        Me.Button_Use_Ext_Volt.Text = "Use Ext"
        Me.Button_Use_Ext_Volt.UseVisualStyleBackColor = True
        '
        'TextBox_Volt
        '
        Me.TextBox_Volt.Location = New System.Drawing.Point(86, 22)
        Me.TextBox_Volt.Name = "TextBox_Volt"
        Me.TextBox_Volt.Size = New System.Drawing.Size(42, 20)
        Me.TextBox_Volt.TabIndex = 3
        '
        'Button_Set_Volt
        '
        Me.Button_Set_Volt.Location = New System.Drawing.Point(131, 21)
        Me.Button_Set_Volt.Name = "Button_Set_Volt"
        Me.Button_Set_Volt.Size = New System.Drawing.Size(54, 23)
        Me.Button_Set_Volt.TabIndex = 7
        Me.Button_Set_Volt.Text = "Set"
        Me.Button_Set_Volt.UseVisualStyleBackColor = True
        '
        'Button_Get_Volt
        '
        Me.Button_Get_Volt.Location = New System.Drawing.Point(11, 21)
        Me.Button_Get_Volt.Name = "Button_Get_Volt"
        Me.Button_Get_Volt.Size = New System.Drawing.Size(54, 23)
        Me.Button_Get_Volt.TabIndex = 6
        Me.Button_Get_Volt.Text = "Get"
        Me.Button_Get_Volt.UseVisualStyleBackColor = True
        '
        'GroupBox8
        '
        Me.GroupBox8.Controls.Add(Me.Button_Set_Aux_Status)
        Me.GroupBox8.Controls.Add(Me.CheckBox_Dir_2)
        Me.GroupBox8.Controls.Add(Me.CheckBox_State_2)
        Me.GroupBox8.Controls.Add(Me.CheckBox_Dir_1)
        Me.GroupBox8.Controls.Add(Me.CheckBox_State_1)
        Me.GroupBox8.Controls.Add(Me.Button_Get_Aux_Status)
        Me.GroupBox8.Location = New System.Drawing.Point(67, 624)
        Me.GroupBox8.Name = "GroupBox8"
        Me.GroupBox8.Size = New System.Drawing.Size(257, 57)
        Me.GroupBox8.TabIndex = 11
        Me.GroupBox8.TabStop = False
        Me.GroupBox8.Text = "Aux Status"
        '
        'Button_Set_Aux_Status
        '
        Me.Button_Set_Aux_Status.Location = New System.Drawing.Point(197, 21)
        Me.Button_Set_Aux_Status.Name = "Button_Set_Aux_Status"
        Me.Button_Set_Aux_Status.Size = New System.Drawing.Size(54, 23)
        Me.Button_Set_Aux_Status.TabIndex = 13
        Me.Button_Set_Aux_Status.Text = "Set"
        Me.Button_Set_Aux_Status.UseVisualStyleBackColor = True
        '
        'CheckBox_Dir_2
        '
        Me.CheckBox_Dir_2.AutoSize = True
        Me.CheckBox_Dir_2.Location = New System.Drawing.Point(77, 34)
        Me.CheckBox_Dir_2.Name = "CheckBox_Dir_2"
        Me.CheckBox_Dir_2.Size = New System.Drawing.Size(48, 17)
        Me.CheckBox_Dir_2.TabIndex = 12
        Me.CheckBox_Dir_2.Text = "Dir 2"
        Me.CheckBox_Dir_2.UseVisualStyleBackColor = True
        '
        'CheckBox_State_2
        '
        Me.CheckBox_State_2.AutoSize = True
        Me.CheckBox_State_2.Location = New System.Drawing.Point(11, 34)
        Me.CheckBox_State_2.Name = "CheckBox_State_2"
        Me.CheckBox_State_2.Size = New System.Drawing.Size(60, 17)
        Me.CheckBox_State_2.TabIndex = 11
        Me.CheckBox_State_2.Text = "State 2"
        Me.CheckBox_State_2.UseVisualStyleBackColor = True
        '
        'CheckBox_Dir_1
        '
        Me.CheckBox_Dir_1.AutoSize = True
        Me.CheckBox_Dir_1.Location = New System.Drawing.Point(77, 17)
        Me.CheckBox_Dir_1.Name = "CheckBox_Dir_1"
        Me.CheckBox_Dir_1.Size = New System.Drawing.Size(48, 17)
        Me.CheckBox_Dir_1.TabIndex = 10
        Me.CheckBox_Dir_1.Text = "Dir 1"
        Me.CheckBox_Dir_1.UseVisualStyleBackColor = True
        '
        'CheckBox_State_1
        '
        Me.CheckBox_State_1.AutoSize = True
        Me.CheckBox_State_1.Location = New System.Drawing.Point(11, 17)
        Me.CheckBox_State_1.Name = "CheckBox_State_1"
        Me.CheckBox_State_1.Size = New System.Drawing.Size(60, 17)
        Me.CheckBox_State_1.TabIndex = 9
        Me.CheckBox_State_1.Text = "State 1"
        Me.CheckBox_State_1.UseVisualStyleBackColor = True
        '
        'Button_Get_Aux_Status
        '
        Me.Button_Get_Aux_Status.Location = New System.Drawing.Point(131, 21)
        Me.Button_Get_Aux_Status.Name = "Button_Get_Aux_Status"
        Me.Button_Get_Aux_Status.Size = New System.Drawing.Size(54, 23)
        Me.Button_Get_Aux_Status.TabIndex = 7
        Me.Button_Get_Aux_Status.Text = "Get"
        Me.Button_Get_Aux_Status.UseVisualStyleBackColor = True
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(391, 693)
        Me.Controls.Add(Me.GroupBox8)
        Me.Controls.Add(Me.GroupBox7)
        Me.Controls.Add(Me.GroupBox6)
        Me.Controls.Add(Me.GroupBox5)
        Me.Controls.Add(Me.GroupBox4)
        Me.Controls.Add(Me.GroupBox3)
        Me.Controls.Add(Me.GroupBox2)
        Me.Controls.Add(Me.RichTextBox_Display)
        Me.Controls.Add(Me.GroupBox1)
        Me.Name = "Form1"
        Me.Text = "Form1"
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.GroupBox2.ResumeLayout(False)
        Me.GroupBox2.PerformLayout()
        Me.GroupBox3.ResumeLayout(False)
        Me.GroupBox3.PerformLayout()
        Me.GroupBox4.ResumeLayout(False)
        Me.GroupBox4.PerformLayout()
        Me.GroupBox5.ResumeLayout(False)
        Me.GroupBox5.PerformLayout()
        Me.GroupBox6.ResumeLayout(False)
        Me.GroupBox6.PerformLayout()
        Me.GroupBox7.ResumeLayout(False)
        Me.GroupBox7.PerformLayout()
        Me.GroupBox8.ResumeLayout(False)
        Me.GroupBox8.PerformLayout()
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents Button_Read_EE_Execute As System.Windows.Forms.Button
    Friend WithEvents GroupBox1 As System.Windows.Forms.GroupBox
    Friend WithEvents TextBox_Read_EE_Word_Addr As System.Windows.Forms.TextBox
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents TextBox_Read_EE_Byte_Count As System.Windows.Forms.TextBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents RichTextBox_Display As System.Windows.Forms.RichTextBox
    Friend WithEvents GroupBox2 As System.Windows.Forms.GroupBox
    Friend WithEvents Label5 As System.Windows.Forms.Label
    Friend WithEvents TextBox_Write_EE_Byte2 As System.Windows.Forms.TextBox
    Friend WithEvents Button_Write_EE_Execute As System.Windows.Forms.Button
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents TextBox_Write_EE_Byte1 As System.Windows.Forms.TextBox
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents TextBox_Write_EE_Word_Addr As System.Windows.Forms.TextBox
    Friend WithEvents GroupBox3 As System.Windows.Forms.GroupBox
    Friend WithEvents Label6 As System.Windows.Forms.Label
    Friend WithEvents TextBox_Receive_EE_Byte_Count As System.Windows.Forms.TextBox
    Friend WithEvents Button_Receive_EE_Execute As System.Windows.Forms.Button
    Friend WithEvents GroupBox4 As System.Windows.Forms.GroupBox
    Friend WithEvents CheckBox_RTC As System.Windows.Forms.CheckBox
    Friend WithEvents Label_RTC As System.Windows.Forms.Label
    Friend WithEvents Timer_RTC As System.Windows.Forms.Timer
    Friend WithEvents GroupBox5 As System.Windows.Forms.GroupBox
    Friend WithEvents ProgressBar_ADC As System.Windows.Forms.ProgressBar
    Friend WithEvents Label_ADC_Count As System.Windows.Forms.Label
    Friend WithEvents CheckBox_ADC As System.Windows.Forms.CheckBox
    Friend WithEvents Timer_ADC As System.Windows.Forms.Timer
    Friend WithEvents GroupBox6 As System.Windows.Forms.GroupBox
    Friend WithEvents Button_Get_Bit_Rate As System.Windows.Forms.Button
    Friend WithEvents TextBox_Bit_Rate_Set As System.Windows.Forms.TextBox
    Friend WithEvents Button_Set_Bit_Rate As System.Windows.Forms.Button
    Friend WithEvents GroupBox7 As System.Windows.Forms.GroupBox
    Friend WithEvents Button_Use_Ext_Volt As System.Windows.Forms.Button
    Friend WithEvents TextBox_Volt As System.Windows.Forms.TextBox
    Friend WithEvents Button_Set_Volt As System.Windows.Forms.Button
    Friend WithEvents Button_Get_Volt As System.Windows.Forms.Button
    Friend WithEvents GroupBox8 As System.Windows.Forms.GroupBox
    Friend WithEvents Button_Set_Aux_Status As System.Windows.Forms.Button
    Friend WithEvents CheckBox_Dir_2 As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBox_State_2 As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBox_Dir_1 As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBox_State_1 As System.Windows.Forms.CheckBox
    Friend WithEvents Button_Get_Aux_Status As System.Windows.Forms.Button

End Class

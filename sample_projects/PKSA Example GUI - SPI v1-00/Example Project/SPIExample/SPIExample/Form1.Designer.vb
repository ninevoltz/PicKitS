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
        Me.RichTextBox_Display = New System.Windows.Forms.RichTextBox
        Me.GroupBox1 = New System.Windows.Forms.GroupBox
        Me.Label2 = New System.Windows.Forms.Label
        Me.Label1 = New System.Windows.Forms.Label
        Me.Button_Execute = New System.Windows.Forms.Button
        Me.TextBox_Receive_Byte_Count = New System.Windows.Forms.TextBox
        Me.TextBox_Send_Data_5 = New System.Windows.Forms.TextBox
        Me.TextBox_Send_Data_4 = New System.Windows.Forms.TextBox
        Me.TextBox_Send_Data_3 = New System.Windows.Forms.TextBox
        Me.TextBox_Send_Data_2 = New System.Windows.Forms.TextBox
        Me.TextBox_Send_Data_1 = New System.Windows.Forms.TextBox
        Me.TextBox_Send_Data_0 = New System.Windows.Forms.TextBox
        Me.GroupBox6 = New System.Windows.Forms.GroupBox
        Me.TextBox_Bit_Rate_Set = New System.Windows.Forms.TextBox
        Me.Button_Get_Bit_Rate = New System.Windows.Forms.Button
        Me.Button_Set_Bit_Rate = New System.Windows.Forms.Button
        Me.GroupBox2 = New System.Windows.Forms.GroupBox
        Me.CheckBox_Voltage = New System.Windows.Forms.CheckBox
        Me.GroupBox3 = New System.Windows.Forms.GroupBox
        Me.Button_Get_Status = New System.Windows.Forms.Button
        Me.CheckBox_Chip_Select_State = New System.Windows.Forms.CheckBox
        Me.CheckBox_Clock_State = New System.Windows.Forms.CheckBox
        Me.CheckBox_SDO_State = New System.Windows.Forms.CheckBox
        Me.CheckBox_SDI_State = New System.Windows.Forms.CheckBox
        Me.CheckBox_Auto_Output_Disable = New System.Windows.Forms.CheckBox
        Me.CheckBox_Clock_Polarity = New System.Windows.Forms.CheckBox
        Me.CheckBox_Clock_Edge_Select = New System.Windows.Forms.CheckBox
        Me.CheckBox_Sample_Phase = New System.Windows.Forms.CheckBox
        Me.GroupBox1.SuspendLayout()
        Me.GroupBox6.SuspendLayout()
        Me.GroupBox2.SuspendLayout()
        Me.GroupBox3.SuspendLayout()
        Me.SuspendLayout()
        '
        'RichTextBox_Display
        '
        Me.RichTextBox_Display.Location = New System.Drawing.Point(44, 352)
        Me.RichTextBox_Display.Name = "RichTextBox_Display"
        Me.RichTextBox_Display.Size = New System.Drawing.Size(276, 109)
        Me.RichTextBox_Display.TabIndex = 6
        Me.RichTextBox_Display.Text = ""
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.Label2)
        Me.GroupBox1.Controls.Add(Me.Label1)
        Me.GroupBox1.Controls.Add(Me.Button_Execute)
        Me.GroupBox1.Controls.Add(Me.TextBox_Receive_Byte_Count)
        Me.GroupBox1.Controls.Add(Me.TextBox_Send_Data_5)
        Me.GroupBox1.Controls.Add(Me.TextBox_Send_Data_4)
        Me.GroupBox1.Controls.Add(Me.TextBox_Send_Data_3)
        Me.GroupBox1.Controls.Add(Me.TextBox_Send_Data_2)
        Me.GroupBox1.Controls.Add(Me.TextBox_Send_Data_1)
        Me.GroupBox1.Controls.Add(Me.TextBox_Send_Data_0)
        Me.GroupBox1.Location = New System.Drawing.Point(64, 23)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(237, 90)
        Me.GroupBox1.TabIndex = 5
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Send and Receive Data"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(173, 27)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(58, 13)
        Me.Label2.TabIndex = 9
        Me.Label2.Text = "Send Data"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(33, 59)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(102, 13)
        Me.Label1.TabIndex = 7
        Me.Label1.Text = "Receive Byte Count"
        '
        'Button_Execute
        '
        Me.Button_Execute.Location = New System.Drawing.Point(164, 54)
        Me.Button_Execute.Name = "Button_Execute"
        Me.Button_Execute.Size = New System.Drawing.Size(58, 23)
        Me.Button_Execute.TabIndex = 0
        Me.Button_Execute.Text = "Execute"
        Me.Button_Execute.UseVisualStyleBackColor = True
        '
        'TextBox_Receive_Byte_Count
        '
        Me.TextBox_Receive_Byte_Count.Location = New System.Drawing.Point(6, 55)
        Me.TextBox_Receive_Byte_Count.Name = "TextBox_Receive_Byte_Count"
        Me.TextBox_Receive_Byte_Count.Size = New System.Drawing.Size(22, 20)
        Me.TextBox_Receive_Byte_Count.TabIndex = 8
        '
        'TextBox_Send_Data_5
        '
        Me.TextBox_Send_Data_5.Location = New System.Drawing.Point(146, 23)
        Me.TextBox_Send_Data_5.Name = "TextBox_Send_Data_5"
        Me.TextBox_Send_Data_5.Size = New System.Drawing.Size(22, 20)
        Me.TextBox_Send_Data_5.TabIndex = 7
        '
        'TextBox_Send_Data_4
        '
        Me.TextBox_Send_Data_4.Location = New System.Drawing.Point(118, 23)
        Me.TextBox_Send_Data_4.Name = "TextBox_Send_Data_4"
        Me.TextBox_Send_Data_4.Size = New System.Drawing.Size(22, 20)
        Me.TextBox_Send_Data_4.TabIndex = 6
        '
        'TextBox_Send_Data_3
        '
        Me.TextBox_Send_Data_3.Location = New System.Drawing.Point(90, 23)
        Me.TextBox_Send_Data_3.Name = "TextBox_Send_Data_3"
        Me.TextBox_Send_Data_3.Size = New System.Drawing.Size(22, 20)
        Me.TextBox_Send_Data_3.TabIndex = 5
        '
        'TextBox_Send_Data_2
        '
        Me.TextBox_Send_Data_2.Location = New System.Drawing.Point(62, 23)
        Me.TextBox_Send_Data_2.Name = "TextBox_Send_Data_2"
        Me.TextBox_Send_Data_2.Size = New System.Drawing.Size(22, 20)
        Me.TextBox_Send_Data_2.TabIndex = 4
        '
        'TextBox_Send_Data_1
        '
        Me.TextBox_Send_Data_1.Location = New System.Drawing.Point(34, 23)
        Me.TextBox_Send_Data_1.Name = "TextBox_Send_Data_1"
        Me.TextBox_Send_Data_1.Size = New System.Drawing.Size(22, 20)
        Me.TextBox_Send_Data_1.TabIndex = 3
        '
        'TextBox_Send_Data_0
        '
        Me.TextBox_Send_Data_0.Location = New System.Drawing.Point(6, 23)
        Me.TextBox_Send_Data_0.Name = "TextBox_Send_Data_0"
        Me.TextBox_Send_Data_0.Size = New System.Drawing.Size(22, 20)
        Me.TextBox_Send_Data_0.TabIndex = 2
        '
        'GroupBox6
        '
        Me.GroupBox6.Controls.Add(Me.TextBox_Bit_Rate_Set)
        Me.GroupBox6.Controls.Add(Me.Button_Get_Bit_Rate)
        Me.GroupBox6.Controls.Add(Me.Button_Set_Bit_Rate)
        Me.GroupBox6.Location = New System.Drawing.Point(64, 128)
        Me.GroupBox6.Name = "GroupBox6"
        Me.GroupBox6.Size = New System.Drawing.Size(237, 47)
        Me.GroupBox6.TabIndex = 10
        Me.GroupBox6.TabStop = False
        Me.GroupBox6.Text = "Bit Rate"
        '
        'TextBox_Bit_Rate_Set
        '
        Me.TextBox_Bit_Rate_Set.Location = New System.Drawing.Point(106, 18)
        Me.TextBox_Bit_Rate_Set.Name = "TextBox_Bit_Rate_Set"
        Me.TextBox_Bit_Rate_Set.Size = New System.Drawing.Size(44, 20)
        Me.TextBox_Bit_Rate_Set.TabIndex = 8
        '
        'Button_Get_Bit_Rate
        '
        Me.Button_Get_Bit_Rate.Location = New System.Drawing.Point(14, 17)
        Me.Button_Get_Bit_Rate.Name = "Button_Get_Bit_Rate"
        Me.Button_Get_Bit_Rate.Size = New System.Drawing.Size(75, 23)
        Me.Button_Get_Bit_Rate.TabIndex = 4
        Me.Button_Get_Bit_Rate.Text = "Get"
        Me.Button_Get_Bit_Rate.UseVisualStyleBackColor = True
        '
        'Button_Set_Bit_Rate
        '
        Me.Button_Set_Bit_Rate.Location = New System.Drawing.Point(152, 17)
        Me.Button_Set_Bit_Rate.Name = "Button_Set_Bit_Rate"
        Me.Button_Set_Bit_Rate.Size = New System.Drawing.Size(75, 23)
        Me.Button_Set_Bit_Rate.TabIndex = 7
        Me.Button_Set_Bit_Rate.Text = "Set"
        Me.Button_Set_Bit_Rate.UseVisualStyleBackColor = True
        '
        'GroupBox2
        '
        Me.GroupBox2.Controls.Add(Me.CheckBox_Voltage)
        Me.GroupBox2.Location = New System.Drawing.Point(64, 181)
        Me.GroupBox2.Name = "GroupBox2"
        Me.GroupBox2.Size = New System.Drawing.Size(237, 47)
        Me.GroupBox2.TabIndex = 11
        Me.GroupBox2.TabStop = False
        Me.GroupBox2.Text = "Voltage"
        '
        'CheckBox_Voltage
        '
        Me.CheckBox_Voltage.AutoSize = True
        Me.CheckBox_Voltage.Location = New System.Drawing.Point(48, 19)
        Me.CheckBox_Voltage.Name = "CheckBox_Voltage"
        Me.CheckBox_Voltage.Size = New System.Drawing.Size(143, 17)
        Me.CheckBox_Voltage.TabIndex = 0
        Me.CheckBox_Voltage.Text = "PKSA is supplying power"
        Me.CheckBox_Voltage.UseVisualStyleBackColor = True
        '
        'GroupBox3
        '
        Me.GroupBox3.Controls.Add(Me.Button_Get_Status)
        Me.GroupBox3.Controls.Add(Me.CheckBox_Chip_Select_State)
        Me.GroupBox3.Controls.Add(Me.CheckBox_Clock_State)
        Me.GroupBox3.Controls.Add(Me.CheckBox_SDO_State)
        Me.GroupBox3.Controls.Add(Me.CheckBox_SDI_State)
        Me.GroupBox3.Controls.Add(Me.CheckBox_Auto_Output_Disable)
        Me.GroupBox3.Controls.Add(Me.CheckBox_Clock_Polarity)
        Me.GroupBox3.Controls.Add(Me.CheckBox_Clock_Edge_Select)
        Me.GroupBox3.Controls.Add(Me.CheckBox_Sample_Phase)
        Me.GroupBox3.Location = New System.Drawing.Point(30, 234)
        Me.GroupBox3.Name = "GroupBox3"
        Me.GroupBox3.Size = New System.Drawing.Size(305, 97)
        Me.GroupBox3.TabIndex = 12
        Me.GroupBox3.TabStop = False
        Me.GroupBox3.Text = "Status"
        '
        'Button_Get_Status
        '
        Me.Button_Get_Status.Location = New System.Drawing.Point(218, 68)
        Me.Button_Get_Status.Name = "Button_Get_Status"
        Me.Button_Get_Status.Size = New System.Drawing.Size(75, 23)
        Me.Button_Get_Status.TabIndex = 9
        Me.Button_Get_Status.Text = "Get"
        Me.Button_Get_Status.UseVisualStyleBackColor = True
        '
        'CheckBox_Chip_Select_State
        '
        Me.CheckBox_Chip_Select_State.AutoSize = True
        Me.CheckBox_Chip_Select_State.Location = New System.Drawing.Point(119, 72)
        Me.CheckBox_Chip_Select_State.Name = "CheckBox_Chip_Select_State"
        Me.CheckBox_Chip_Select_State.Size = New System.Drawing.Size(93, 17)
        Me.CheckBox_Chip_Select_State.TabIndex = 7
        Me.CheckBox_Chip_Select_State.Text = "Chip Sel State"
        Me.CheckBox_Chip_Select_State.UseVisualStyleBackColor = True
        '
        'CheckBox_Clock_State
        '
        Me.CheckBox_Clock_State.AutoSize = True
        Me.CheckBox_Clock_State.Location = New System.Drawing.Point(119, 54)
        Me.CheckBox_Clock_State.Name = "CheckBox_Clock_State"
        Me.CheckBox_Clock_State.Size = New System.Drawing.Size(81, 17)
        Me.CheckBox_Clock_State.TabIndex = 6
        Me.CheckBox_Clock_State.Text = "Clock State"
        Me.CheckBox_Clock_State.UseVisualStyleBackColor = True
        '
        'CheckBox_SDO_State
        '
        Me.CheckBox_SDO_State.AutoSize = True
        Me.CheckBox_SDO_State.Location = New System.Drawing.Point(119, 36)
        Me.CheckBox_SDO_State.Name = "CheckBox_SDO_State"
        Me.CheckBox_SDO_State.Size = New System.Drawing.Size(77, 17)
        Me.CheckBox_SDO_State.TabIndex = 5
        Me.CheckBox_SDO_State.Text = "SDO State"
        Me.CheckBox_SDO_State.UseVisualStyleBackColor = True
        '
        'CheckBox_SDI_State
        '
        Me.CheckBox_SDI_State.AutoSize = True
        Me.CheckBox_SDI_State.Location = New System.Drawing.Point(119, 18)
        Me.CheckBox_SDI_State.Name = "CheckBox_SDI_State"
        Me.CheckBox_SDI_State.Size = New System.Drawing.Size(72, 17)
        Me.CheckBox_SDI_State.TabIndex = 4
        Me.CheckBox_SDI_State.Text = "SDI State"
        Me.CheckBox_SDI_State.UseVisualStyleBackColor = True
        '
        'CheckBox_Auto_Output_Disable
        '
        Me.CheckBox_Auto_Output_Disable.AutoSize = True
        Me.CheckBox_Auto_Output_Disable.Location = New System.Drawing.Point(14, 72)
        Me.CheckBox_Auto_Output_Disable.Name = "CheckBox_Auto_Output_Disable"
        Me.CheckBox_Auto_Output_Disable.Size = New System.Drawing.Size(101, 17)
        Me.CheckBox_Auto_Output_Disable.TabIndex = 3
        Me.CheckBox_Auto_Output_Disable.Text = "Auto Output Dis"
        Me.CheckBox_Auto_Output_Disable.UseVisualStyleBackColor = True
        '
        'CheckBox_Clock_Polarity
        '
        Me.CheckBox_Clock_Polarity.AutoSize = True
        Me.CheckBox_Clock_Polarity.Location = New System.Drawing.Point(14, 54)
        Me.CheckBox_Clock_Polarity.Name = "CheckBox_Clock_Polarity"
        Me.CheckBox_Clock_Polarity.Size = New System.Drawing.Size(90, 17)
        Me.CheckBox_Clock_Polarity.TabIndex = 2
        Me.CheckBox_Clock_Polarity.Text = "Clock Polarity"
        Me.CheckBox_Clock_Polarity.UseVisualStyleBackColor = True
        '
        'CheckBox_Clock_Edge_Select
        '
        Me.CheckBox_Clock_Edge_Select.AutoSize = True
        Me.CheckBox_Clock_Edge_Select.Location = New System.Drawing.Point(14, 36)
        Me.CheckBox_Clock_Edge_Select.Name = "CheckBox_Clock_Edge_Select"
        Me.CheckBox_Clock_Edge_Select.Size = New System.Drawing.Size(99, 17)
        Me.CheckBox_Clock_Edge_Select.TabIndex = 1
        Me.CheckBox_Clock_Edge_Select.Text = "Clock Edge Sel"
        Me.CheckBox_Clock_Edge_Select.UseVisualStyleBackColor = True
        '
        'CheckBox_Sample_Phase
        '
        Me.CheckBox_Sample_Phase.AutoSize = True
        Me.CheckBox_Sample_Phase.Location = New System.Drawing.Point(14, 18)
        Me.CheckBox_Sample_Phase.Name = "CheckBox_Sample_Phase"
        Me.CheckBox_Sample_Phase.Size = New System.Drawing.Size(94, 17)
        Me.CheckBox_Sample_Phase.TabIndex = 0
        Me.CheckBox_Sample_Phase.Text = "Sample Phase"
        Me.CheckBox_Sample_Phase.UseVisualStyleBackColor = True
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(364, 475)
        Me.Controls.Add(Me.GroupBox3)
        Me.Controls.Add(Me.GroupBox2)
        Me.Controls.Add(Me.GroupBox6)
        Me.Controls.Add(Me.RichTextBox_Display)
        Me.Controls.Add(Me.GroupBox1)
        Me.Name = "Form1"
        Me.Text = "Form1"
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.GroupBox6.ResumeLayout(False)
        Me.GroupBox6.PerformLayout()
        Me.GroupBox2.ResumeLayout(False)
        Me.GroupBox2.PerformLayout()
        Me.GroupBox3.ResumeLayout(False)
        Me.GroupBox3.PerformLayout()
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents RichTextBox_Display As System.Windows.Forms.RichTextBox
    Friend WithEvents GroupBox1 As System.Windows.Forms.GroupBox
    Friend WithEvents TextBox_Send_Data_0 As System.Windows.Forms.TextBox
    Friend WithEvents Button_Execute As System.Windows.Forms.Button
    Friend WithEvents TextBox_Send_Data_5 As System.Windows.Forms.TextBox
    Friend WithEvents TextBox_Send_Data_4 As System.Windows.Forms.TextBox
    Friend WithEvents TextBox_Send_Data_3 As System.Windows.Forms.TextBox
    Friend WithEvents TextBox_Send_Data_2 As System.Windows.Forms.TextBox
    Friend WithEvents TextBox_Send_Data_1 As System.Windows.Forms.TextBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents TextBox_Receive_Byte_Count As System.Windows.Forms.TextBox
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents GroupBox6 As System.Windows.Forms.GroupBox
    Friend WithEvents TextBox_Bit_Rate_Set As System.Windows.Forms.TextBox
    Friend WithEvents Button_Get_Bit_Rate As System.Windows.Forms.Button
    Friend WithEvents Button_Set_Bit_Rate As System.Windows.Forms.Button
    Friend WithEvents GroupBox2 As System.Windows.Forms.GroupBox
    Friend WithEvents CheckBox_Voltage As System.Windows.Forms.CheckBox
    Friend WithEvents GroupBox3 As System.Windows.Forms.GroupBox
    Friend WithEvents CheckBox_Clock_Edge_Select As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBox_Sample_Phase As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBox_Clock_Polarity As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBox_Auto_Output_Disable As System.Windows.Forms.CheckBox
    Friend WithEvents Button_Get_Status As System.Windows.Forms.Button
    Friend WithEvents CheckBox_Chip_Select_State As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBox_Clock_State As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBox_SDO_State As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBox_SDI_State As System.Windows.Forms.CheckBox

End Class

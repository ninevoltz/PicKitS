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
        Me.Button_TX = New System.Windows.Forms.Button
        Me.Button_RX = New System.Windows.Forms.Button
        Me.TextBox_TX = New System.Windows.Forms.TextBox
        Me.GroupBox1 = New System.Windows.Forms.GroupBox
        Me.RichTextBox_Display = New System.Windows.Forms.RichTextBox
        Me.GroupBox2 = New System.Windows.Forms.GroupBox
        Me.CheckBox_Timer = New System.Windows.Forms.CheckBox
        Me.Timer_Read = New System.Windows.Forms.Timer(Me.components)
        Me.GroupBox3 = New System.Windows.Forms.GroupBox
        Me.CheckBox_Event = New System.Windows.Forms.CheckBox
        Me.GroupBox4 = New System.Windows.Forms.GroupBox
        Me.TextBox_Baud = New System.Windows.Forms.TextBox
        Me.Button_Set_Baud = New System.Windows.Forms.Button
        Me.Button_Get_Baud = New System.Windows.Forms.Button
        Me.GroupBox5 = New System.Windows.Forms.GroupBox
        Me.Button_Use_Ext_Volt = New System.Windows.Forms.Button
        Me.TextBox_Volt = New System.Windows.Forms.TextBox
        Me.Button_Set_Volt = New System.Windows.Forms.Button
        Me.Button_Get_Volt = New System.Windows.Forms.Button
        Me.GroupBox6 = New System.Windows.Forms.GroupBox
        Me.Button_Get_Aux_Status = New System.Windows.Forms.Button
        Me.CheckBox_State_1 = New System.Windows.Forms.CheckBox
        Me.CheckBox_Dir_1 = New System.Windows.Forms.CheckBox
        Me.CheckBox_Dir_2 = New System.Windows.Forms.CheckBox
        Me.CheckBox_State_2 = New System.Windows.Forms.CheckBox
        Me.Button_Set_Aux_Status = New System.Windows.Forms.Button
        Me.GroupBox1.SuspendLayout()
        Me.GroupBox2.SuspendLayout()
        Me.GroupBox3.SuspendLayout()
        Me.GroupBox4.SuspendLayout()
        Me.GroupBox5.SuspendLayout()
        Me.GroupBox6.SuspendLayout()
        Me.SuspendLayout()
        '
        'Button_TX
        '
        Me.Button_TX.Location = New System.Drawing.Point(92, 19)
        Me.Button_TX.Name = "Button_TX"
        Me.Button_TX.Size = New System.Drawing.Size(36, 23)
        Me.Button_TX.TabIndex = 0
        Me.Button_TX.Text = "TX"
        Me.Button_TX.UseVisualStyleBackColor = True
        '
        'Button_RX
        '
        Me.Button_RX.Location = New System.Drawing.Point(149, 19)
        Me.Button_RX.Name = "Button_RX"
        Me.Button_RX.Size = New System.Drawing.Size(36, 23)
        Me.Button_RX.TabIndex = 1
        Me.Button_RX.Text = "RX"
        Me.Button_RX.UseVisualStyleBackColor = True
        '
        'TextBox_TX
        '
        Me.TextBox_TX.Location = New System.Drawing.Point(11, 21)
        Me.TextBox_TX.Name = "TextBox_TX"
        Me.TextBox_TX.Size = New System.Drawing.Size(75, 20)
        Me.TextBox_TX.TabIndex = 2
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.TextBox_TX)
        Me.GroupBox1.Controls.Add(Me.Button_RX)
        Me.GroupBox1.Controls.Add(Me.Button_TX)
        Me.GroupBox1.Location = New System.Drawing.Point(79, 37)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(200, 57)
        Me.GroupBox1.TabIndex = 3
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Manual Read"
        '
        'RichTextBox_Display
        '
        Me.RichTextBox_Display.Location = New System.Drawing.Point(41, 236)
        Me.RichTextBox_Display.Name = "RichTextBox_Display"
        Me.RichTextBox_Display.Size = New System.Drawing.Size(276, 109)
        Me.RichTextBox_Display.TabIndex = 4
        Me.RichTextBox_Display.Text = ""
        '
        'GroupBox2
        '
        Me.GroupBox2.Controls.Add(Me.CheckBox_Timer)
        Me.GroupBox2.Location = New System.Drawing.Point(79, 100)
        Me.GroupBox2.Name = "GroupBox2"
        Me.GroupBox2.Size = New System.Drawing.Size(200, 57)
        Me.GroupBox2.TabIndex = 4
        Me.GroupBox2.TabStop = False
        Me.GroupBox2.Text = "Timer Read"
        '
        'CheckBox_Timer
        '
        Me.CheckBox_Timer.AutoSize = True
        Me.CheckBox_Timer.Location = New System.Drawing.Point(59, 23)
        Me.CheckBox_Timer.Name = "CheckBox_Timer"
        Me.CheckBox_Timer.Size = New System.Drawing.Size(69, 17)
        Me.CheckBox_Timer.TabIndex = 0
        Me.CheckBox_Timer.Text = "Timer On"
        Me.CheckBox_Timer.UseVisualStyleBackColor = True
        '
        'Timer_Read
        '
        '
        'GroupBox3
        '
        Me.GroupBox3.Controls.Add(Me.CheckBox_Event)
        Me.GroupBox3.Location = New System.Drawing.Point(79, 163)
        Me.GroupBox3.Name = "GroupBox3"
        Me.GroupBox3.Size = New System.Drawing.Size(200, 57)
        Me.GroupBox3.TabIndex = 5
        Me.GroupBox3.TabStop = False
        Me.GroupBox3.Text = "Event Driven Read"
        '
        'CheckBox_Event
        '
        Me.CheckBox_Event.AutoSize = True
        Me.CheckBox_Event.Location = New System.Drawing.Point(20, 25)
        Me.CheckBox_Event.Name = "CheckBox_Event"
        Me.CheckBox_Event.Size = New System.Drawing.Size(153, 17)
        Me.CheckBox_Event.TabIndex = 0
        Me.CheckBox_Event.Text = "Enable Event Driven Read"
        Me.CheckBox_Event.UseVisualStyleBackColor = True
        '
        'GroupBox4
        '
        Me.GroupBox4.Controls.Add(Me.TextBox_Baud)
        Me.GroupBox4.Controls.Add(Me.Button_Set_Baud)
        Me.GroupBox4.Controls.Add(Me.Button_Get_Baud)
        Me.GroupBox4.Location = New System.Drawing.Point(79, 351)
        Me.GroupBox4.Name = "GroupBox4"
        Me.GroupBox4.Size = New System.Drawing.Size(200, 57)
        Me.GroupBox4.TabIndex = 5
        Me.GroupBox4.TabStop = False
        Me.GroupBox4.Text = "Baud Rate"
        '
        'TextBox_Baud
        '
        Me.TextBox_Baud.Location = New System.Drawing.Point(86, 22)
        Me.TextBox_Baud.Name = "TextBox_Baud"
        Me.TextBox_Baud.Size = New System.Drawing.Size(42, 20)
        Me.TextBox_Baud.TabIndex = 3
        '
        'Button_Set_Baud
        '
        Me.Button_Set_Baud.Location = New System.Drawing.Point(131, 21)
        Me.Button_Set_Baud.Name = "Button_Set_Baud"
        Me.Button_Set_Baud.Size = New System.Drawing.Size(54, 23)
        Me.Button_Set_Baud.TabIndex = 7
        Me.Button_Set_Baud.Text = "Set"
        Me.Button_Set_Baud.UseVisualStyleBackColor = True
        '
        'Button_Get_Baud
        '
        Me.Button_Get_Baud.Location = New System.Drawing.Point(11, 21)
        Me.Button_Get_Baud.Name = "Button_Get_Baud"
        Me.Button_Get_Baud.Size = New System.Drawing.Size(54, 23)
        Me.Button_Get_Baud.TabIndex = 6
        Me.Button_Get_Baud.Text = "Get"
        Me.Button_Get_Baud.UseVisualStyleBackColor = True
        '
        'GroupBox5
        '
        Me.GroupBox5.Controls.Add(Me.Button_Use_Ext_Volt)
        Me.GroupBox5.Controls.Add(Me.TextBox_Volt)
        Me.GroupBox5.Controls.Add(Me.Button_Set_Volt)
        Me.GroupBox5.Controls.Add(Me.Button_Get_Volt)
        Me.GroupBox5.Location = New System.Drawing.Point(51, 414)
        Me.GroupBox5.Name = "GroupBox5"
        Me.GroupBox5.Size = New System.Drawing.Size(257, 57)
        Me.GroupBox5.TabIndex = 8
        Me.GroupBox5.TabStop = False
        Me.GroupBox5.Text = "Voltage"
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
        'GroupBox6
        '
        Me.GroupBox6.Controls.Add(Me.Button_Set_Aux_Status)
        Me.GroupBox6.Controls.Add(Me.CheckBox_Dir_2)
        Me.GroupBox6.Controls.Add(Me.CheckBox_State_2)
        Me.GroupBox6.Controls.Add(Me.CheckBox_Dir_1)
        Me.GroupBox6.Controls.Add(Me.CheckBox_State_1)
        Me.GroupBox6.Controls.Add(Me.Button_Get_Aux_Status)
        Me.GroupBox6.Location = New System.Drawing.Point(51, 477)
        Me.GroupBox6.Name = "GroupBox6"
        Me.GroupBox6.Size = New System.Drawing.Size(257, 57)
        Me.GroupBox6.TabIndex = 8
        Me.GroupBox6.TabStop = False
        Me.GroupBox6.Text = "Aux Status"
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
        'Button_Set_Aux_Status
        '
        Me.Button_Set_Aux_Status.Location = New System.Drawing.Point(197, 21)
        Me.Button_Set_Aux_Status.Name = "Button_Set_Aux_Status"
        Me.Button_Set_Aux_Status.Size = New System.Drawing.Size(54, 23)
        Me.Button_Set_Aux_Status.TabIndex = 13
        Me.Button_Set_Aux_Status.Text = "Set"
        Me.Button_Set_Aux_Status.UseVisualStyleBackColor = True
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(359, 542)
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
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents Button_TX As System.Windows.Forms.Button
    Friend WithEvents Button_RX As System.Windows.Forms.Button
    Friend WithEvents TextBox_TX As System.Windows.Forms.TextBox
    Friend WithEvents GroupBox1 As System.Windows.Forms.GroupBox
    Friend WithEvents RichTextBox_Display As System.Windows.Forms.RichTextBox
    Friend WithEvents GroupBox2 As System.Windows.Forms.GroupBox
    Friend WithEvents CheckBox_Timer As System.Windows.Forms.CheckBox
    Friend WithEvents Timer_Read As System.Windows.Forms.Timer
    Friend WithEvents GroupBox3 As System.Windows.Forms.GroupBox
    Friend WithEvents CheckBox_Event As System.Windows.Forms.CheckBox
    Friend WithEvents GroupBox4 As System.Windows.Forms.GroupBox
    Friend WithEvents TextBox_Baud As System.Windows.Forms.TextBox
    Friend WithEvents Button_Set_Baud As System.Windows.Forms.Button
    Friend WithEvents Button_Get_Baud As System.Windows.Forms.Button
    Friend WithEvents GroupBox5 As System.Windows.Forms.GroupBox
    Friend WithEvents TextBox_Volt As System.Windows.Forms.TextBox
    Friend WithEvents Button_Set_Volt As System.Windows.Forms.Button
    Friend WithEvents Button_Get_Volt As System.Windows.Forms.Button
    Friend WithEvents Button_Use_Ext_Volt As System.Windows.Forms.Button
    Friend WithEvents GroupBox6 As System.Windows.Forms.GroupBox
    Friend WithEvents CheckBox_Dir_1 As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBox_State_1 As System.Windows.Forms.CheckBox
    Friend WithEvents Button_Get_Aux_Status As System.Windows.Forms.Button
    Friend WithEvents Button_Set_Aux_Status As System.Windows.Forms.Button
    Friend WithEvents CheckBox_Dir_2 As System.Windows.Forms.CheckBox
    Friend WithEvents CheckBox_State_2 As System.Windows.Forms.CheckBox

End Class

Public Class Form1
    Private WithEvents DataAvailableTrigger As PICkitS.USBRead
    Public Delegate Sub Threaded_Textbox_Update(ByVal pText As String)
    Dim DisplayResultsLock As New DataObject() ' used to lock richtextbox from multiple threads 
    ' create event that will be triggered when data arrives
    Public Sub RegisterEvents(ByVal s As PICkitS.USBRead)
        DataAvailableTrigger = s
        ' register event with dll
    End Sub
    Public Sub UnregisterEvents()
        DataAvailableTrigger = Nothing
    End Sub
    Private Sub HandleData() Handles DataAvailableTrigger.DataAvailable
        ' this subroutine will be triggered by the dll when data is available
        '
        ' only run when CheckBox_Event is checked
        If (CheckBox_Event.Checked = True) Then
            Dim ByteCount As UInt32 = 0
            ' how many bytes are available to read
            Dim index As Integer
            ' used as counter
            Dim DataArray(256) As Byte
            ' will store retrieved data here
            ' size is arbitrary, but we must be sure not to exceed size
            Dim ReturnStr As String = ""
            ' convert returned data from byte to string and put here
            Array.Clear(DataArray, 0, DataArray.Length)
            ' clear array
            ByteCount = PICkitS.USART.Retrieve_Data_Byte_Count()
            If (ByteCount = 0) Then
                ' no data to read
                UpdateTextBox("No data available to read" & vbCrLf)
            Else
                ' retrieve data
                If (ByteCount > DataArray.Length) Then
                    ' oops - more data than we can hold
                    ' either need bigger array or retrieve in chunks
                    UpdateTextBox("Too much data to retrieve" & vbCrLf)
                Else
                    If (PICkitS.USART.Retrieve_Data(ByteCount, DataArray)) Then
                        ' convert data to string for display
                        For index = 0 To ByteCount - 1
                            ReturnStr += Convert.ToChar(DataArray(index))
                            ' convert string data into byte array
                        Next
                        UpdateTextBox("'" & ReturnStr & "' from HandleData" & vbCrLf)
                    Else
                        ' error retrieving data
                        UpdateTextBox("Error retrieving data" & vbCrLf)
                        PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
                    End If
                End If
            End If
        End If
    End Sub
    Private Sub UpdateTextBox(ByVal pText As String)
        ' method to for thread other than main
        ' form to put text in the richtextbox
        If InvokeRequired Then
            Dim del As New Threaded_Textbox_Update(AddressOf Me.UpdateTextBox)
            ' Fire and Forget 
            Me.BeginInvoke(del, pText)
            Return
        End If
        SyncLock DisplayResultsLock  ' allow only one thread at a time
            RichTextBox_Display.Text += pText
        End SyncLock
    End Sub
    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        If (PICkitS.Device.Initialize_PICkitSerial()) Then
            If (PICkitS.USART.Configure_PICkitSerial_For_USARTAsync()) Then
                RichTextBox_Display.Text += "PICkit Serial correctly configured for USART Async" & vbCrLf
            Else
                RichTextBox_Display.Text += "Error configuring PICkit Serial for USART Async" & vbCrLf
            End If
        Else
            RichTextBox_Display.Text += "Error initializing PICkit Serial" & vbCrLf
        End If
        Dim HandleDataTemp As PICkitS.USBRead = New PICkitS.USBRead
        RegisterEvents(HandleDataTemp)
    End Sub

    Private Sub Form1_FormClosed(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosedEventArgs) Handles MyBase.FormClosed
        PICkitS.Device.Cleanup()
        UnregisterEvents()
    End Sub

    Private Sub Button_TX_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_TX.Click
        Dim DataArray(256) As Byte
        ' will store our data here
        Dim Return_Str As String = ""
        ' returns string representation of command - we won't use this
        Dim index As Integer
        ' used as counter
        Array.Clear(DataArray, 0, DataArray.Length)
        ' clear array
        Dim SendData As String = TextBox_TX.Text
        ' get data from textbox
        For index = 0 To SendData.Length - 1
            DataArray(index) = Convert.ToByte(SendData(index))
            ' convert string data into byte array
        Next
        If (PICkitS.USART.Send_Data(SendData.Length, DataArray, Return_Str)) Then
            RichTextBox_Display.Text += "Sent string '" & SendData & "' to PICkit Serial" & vbCrLf
        Else
            RichTextBox_Display.Text += "Error sending data" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If

    End Sub

    Private Sub Button_RX_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_RX.Click
        Dim ByteCount As UInt32 = 0
        ' how many bytes are available to read
        Dim index As Integer
        ' used as counter
        Dim DataArray(256) As Byte
        ' will store retrieved data here
        ' size is arbitrary, but we must be sure not to exceed size
        Dim ReturnStr As String = ""
        ' convert returned data from byte to string and put here
        Array.Clear(DataArray, 0, DataArray.Length)
        ' clear array
        ByteCount = PICkitS.USART.Retrieve_Data_Byte_Count()
        If (ByteCount = 0) Then
            ' no data to read
            RichTextBox_Display.Text += "No data available to read" & vbCrLf
        Else
            ' retrieve data
            If (ByteCount > DataArray.Length) Then
                ' oops - more data than we can hold
                ' either need bigger array or retrieve in chunks
                RichTextBox_Display.Text += "Too much data to retrieve" & vbCrLf
            Else
                If (PICkitS.USART.Retrieve_Data(ByteCount, DataArray)) Then
                    ' convert data to string for display
                    For index = 0 To ByteCount - 1
                        ReturnStr += Convert.ToChar(DataArray(index))
                        ' convert string data into byte array
                    Next
                    RichTextBox_Display.Text += "'" & ReturnStr & "' found in PICkit Serial buffer" & vbCrLf
                Else
                    ' error retrieving data
                    RichTextBox_Display.Text += "Error retrieving data" & vbCrLf
                    PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
                End If
            End If
        End If
    End Sub

    Private Sub CheckBox_Timer_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox_Timer.CheckedChanged
        If (CheckBox_Timer.Checked) Then
            Timer_Read.Enabled = True
        Else
            Timer_Read.Enabled = False
        End If
    End Sub

    Private Sub Timer_Read_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer_Read.Tick
        Dim ByteCount As UInt32 = 0
        ' how many bytes are available to read
        Dim index As Integer
        ' used as counter
        Dim DataArray(256) As Byte
        ' will store retrieved data here
        Dim ReturnStr As String = ""
        ' convert returned data from byte to string and put here
        Array.Clear(DataArray, 0, DataArray.Length)
        ' clear array
        ByteCount = PICkitS.USART.Retrieve_Data_Byte_Count()
        If (ByteCount <> 0) Then
            ' retrieve data
            If (ByteCount > DataArray.Length) Then
                ' oops - more data than we can hold
                ' either need bigger array or retrieve in chunks
                RichTextBox_Display.Text += "Too much data to retrieve" & vbCrLf
            Else
                If (PICkitS.USART.Retrieve_Data(ByteCount, DataArray)) Then
                    ' convert data to string for display
                    For index = 0 To ByteCount - 1
                        ReturnStr += Convert.ToChar(DataArray(index))
                        ' convert string data into byte array
                    Next
                    RichTextBox_Display.Text += "'" & ReturnStr & "' found in PICkit Serial buffer" & vbCrLf
                Else
                    ' error retrieving data
                    RichTextBox_Display.Text += "Error retrieving data" & vbCrLf
                    PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
                    ' turn off timer so don't keep getting error msg
                    Timer_Read.Enabled = False
                    CheckBox_Timer.Checked = False
                End If
            End If
        End If
    End Sub

    Private Sub Button_Get_Baud_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Get_Baud.Click
        Dim Baud As UShort = 0
        Baud = PICkitS.USART.Get_Baud_Rate()
        If (Baud = 0) Then
            RichTextBox_Display.Text += "Error retrieving baud rate" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        Else
            RichTextBox_Display.Text += "Baud = " & Baud.ToString() & vbCrLf
        End If
    End Sub

    Private Sub Button_Set_Baud_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Set_Baud.Click
        Dim Baud As UShort
        Baud = Convert.ToUInt16(TextBox_Baud.Text)
        If (PICkitS.USART.Set_Baud_Rate(Baud)) Then
            RichTextBox_Display.Text += "Baud Set to " & Baud.ToString() & vbCrLf
        Else
            RichTextBox_Display.Text += "Error setting baud" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If
    End Sub

    Private Sub Button_Get_Volt_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Get_Volt.Click
        Dim Volt As Double = 0
        Dim PKSA_Power As Boolean
        If (PICkitS.USART.Get_Source_Voltage(Volt, PKSA_Power)) Then
            RichTextBox_Display.Text += "Source voltage set to " & String.Format("{0:0.0}V", Volt) & vbCrLf & "PKSA powering device is set to " & PKSA_Power.ToString() & vbCrLf
        Else
            RichTextBox_Display.Text += "Error retrieving source voltage" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If
    End Sub

    Private Sub Button_Set_Volt_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Set_Volt.Click
        Dim Volt As Double
        Volt = Convert.ToDouble(TextBox_Volt.Text)
        If (PICkitS.USART.Set_Source_Voltage(Volt)) Then
            RichTextBox_Display.Text += "Voltage Set to " & Volt.ToString() & vbCrLf
        Else
            RichTextBox_Display.Text += "Error setting Voltage" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If
    End Sub

    Private Sub ButtonUse_Ext_Volt_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Use_Ext_Volt.Click
        If (PICkitS.USART.Tell_PKSA_To_Use_External_Voltage_Source()) Then
            RichTextBox_Display.Text += "PKSA set to use external voltage source" & vbCrLf
        Else
            RichTextBox_Display.Text += "Error telling PKSA to use external voltage source" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If
    End Sub

    Private Sub Button_Get_Aux_Status_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Get_Aux_Status.Click
        Dim Aux1_State, Aux1_Dir, Aux2_State, Aux2_Dir As Boolean
        If (PICkitS.USART.Get_Aux_Status(Aux1_State, Aux2_State, Aux1_Dir, Aux2_Dir)) Then
            CheckBox_State_1.Checked = Aux1_State
            CheckBox_State_2.Checked = Aux2_State
            CheckBox_Dir_1.Checked = Aux1_Dir
            CheckBox_Dir_2.Checked = Aux2_Dir
            RichTextBox_Display.Text += "Retrieved Aux status" & vbCrLf
        Else
            RichTextBox_Display.Text += "Error retrieving Aux status" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If
    End Sub

    Private Sub Button_Set_Aux_Status_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Set_Aux_Status.Click
        Dim Result As Boolean = False
        If (PICkitS.USART.Set_Aux1_Direction(CheckBox_Dir_1.Checked)) Then
            If (PICkitS.USART.Set_Aux2_Direction(CheckBox_Dir_2.Checked)) Then
                If (PICkitS.USART.Set_Aux1_State(CheckBox_State_1.Checked)) Then
                    If (PICkitS.USART.Set_Aux2_State(CheckBox_State_2.Checked)) Then
                        Result = True
                    End If
                End If
            End If
        End If
        If (Result = True) Then
            RichTextBox_Display.Text += "Aux status set" & vbCrLf
        Else
            RichTextBox_Display.Text += "Error setting Aux status" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If
    End Sub
End Class

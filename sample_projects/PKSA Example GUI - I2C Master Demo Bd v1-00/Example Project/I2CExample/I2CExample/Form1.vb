Public Class Form1

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        If (PICkitS.Device.Initialize_PICkitSerial()) Then
            If (PICkitS.I2CM.Configure_PICkitSerial_For_I2CMaster()) Then
                RichTextBox_Display.Text += "PICkit Serial correctly configured for I2C" & vbCrLf
            Else
                RichTextBox_Display.Text += "Error configuring PICkit Serial for I2C" & vbCrLf
            End If
        Else
            RichTextBox_Display.Text += "Error initializing PICkit Serial" & vbCrLf
        End If
    End Sub

    Private Sub Form1_FormClosed(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosedEventArgs) Handles MyBase.FormClosed
        PICkitS.Device.Cleanup()
    End Sub

    Private Sub Button_Read_EE_Execute_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Read_EE_Execute.Click
        Dim Demo_Slave_ee_Addr As Byte = &HA8
        ' determined by Demo board firmware, &H says this is a hex number
        Dim Word_Addr As Byte = Convert.ToByte(TextBox_Read_EE_Word_Addr.Text, 16)
        ' assume value in textbox is hex
        Dim Byte_Count As Byte = Convert.ToByte(TextBox_Read_EE_Byte_Count.Text, 16)
        ' assume value in textbox is hex
        Dim DataArray(256) As Byte
        ' will store returned data here
        Dim Return_Str As String = ""
        ' returns string representation of command - we won't use this
        Dim Display_Str As String = ""
        ' will display results with this
        Dim index As Integer
        ' used as counter
        '
        ' clear array
        For index = 0 To Byte_Count - 1
            DataArray(index) = 0
        Next
        '
        If (PICkitS.I2CM.Read(Demo_Slave_ee_Addr, Word_Addr, Byte_Count, DataArray, Return_Str)) Then
            ' successful, display results
            For index = 0 To Byte_Count - 1
                Display_Str += Convert.ToString(DataArray(index), 16) & " "
            Next
            RichTextBox_Display.Text += Display_Str & vbCrLf
        Else
            RichTextBox_Display.Text += "Error reading Demo Board EE" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If
    End Sub

    Private Sub Button_Write_EE_Execute_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Write_EE_Execute.Click
        ' routine to write 2 byte to Demo board EE
        Dim Demo_Slave_ee_Addr As Byte = &HA8
        ' determined by Demo board firmware, &H says this is a hex number
        Dim Word_Addr As Byte = Convert.ToByte(TextBox_Write_EE_Word_Addr.Text, 16)
        ' assume value in textbox is hex
        Dim Byte1 As Byte = Convert.ToByte(TextBox_Write_EE_Byte1.Text, 16)
        ' assume value in textbox is hex
        Dim Byte2 As Byte = Convert.ToByte(TextBox_Write_EE_Byte2.Text, 16)
        ' assume value in textbox is hex
        Dim DataArray(1) As Byte
        ' create array of length 2 bytes
        ' will store write data here - sized to fit our two bytes
        Dim Return_Str As String = ""
        ' returns string representation of command - we won't use this
        DataArray(0) = Byte1
        DataArray(1) = Byte2
        ' fill array
        If (PICkitS.I2CM.Write(Demo_Slave_ee_Addr, Word_Addr, DataArray.Length, DataArray, Return_Str)) Then
            ' success
            RichTextBox_Display.Text += "Writing to EE successful" & vbCrLf
        Else
            RichTextBox_Display.Text += "Error writing to EE" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If

    End Sub

    Private Sub Button_Receive_EE_Execute_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Receive_EE_Execute.Click
        Dim Demo_Slave_ee_Addr As Byte = &HA9
        ' determined by Demo board firmware, &H says this is a hex number
        ' IMPORTANT - addr is not same as for Read command
        ' that command automatically increments the write addr to 
        ' obtain the read addr
        Dim Byte_Count As Byte = Convert.ToByte(TextBox_Receive_EE_Byte_Count.Text, 16)
        ' assume value in textbox is hex
        Dim DataArray(256) As Byte
        ' will store returned data here
        Dim Return_Str As String = ""
        ' returns string representation of command - we won't use this
        Dim Display_Str As String = ""
        ' will display results with this
        Dim index As Integer
        ' used as counter
        '
        ' clear array
        For index = 0 To Byte_Count - 1
            DataArray(index) = 0
        Next
        '
        If (PICkitS.I2CM.Receive(Demo_Slave_ee_Addr, Byte_Count, DataArray, Return_Str)) Then
            ' successful, display results
            For index = 0 To Byte_Count - 1
                Display_Str += Convert.ToString(DataArray(index), 16) & " "
            Next
            RichTextBox_Display.Text += Display_Str & vbCrLf
        Else
            RichTextBox_Display.Text += "Error receiving from Demo Board EE" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If
    End Sub

    Private Sub Timer_RTC_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer_RTC.Tick
        ' routine to read and display RTC
        ' RTC data will be stored in DataArray as follows:
        ' DataArray(0) = seconds
        ' DataArray(1) = minutes
        ' DataArray(2) = hours
        ' DataArray(3) = weekday
        ' DataArray(4) = day of the month
        ' DataArray(5) = month
        ' DataArray(6) = year
        ' we will only be displaying hours, minutes, seconds
        '
        Dim Demo_Slave_RTC_Addr As Byte = &HA2
        ' determined by SMB firmware, &H says this is a hex number
        Dim Word_Addr As Byte = 2
        ' RTC data starts in location 2
        Dim DataArray(6) As Byte
        ' create array of length 7 bytes to hold rtc data
        Dim Return_Str As String = ""
        ' returns string representation of command - we won't use this
        Array.Clear(DataArray, 0, DataArray.Length)
        ' quicker way of clearing data
        If (PICkitS.I2CM.Read(Demo_Slave_RTC_Addr, Word_Addr, DataArray.Length, DataArray, Return_Str)) Then
            ' successful, display results
            Label_RTC.Text = Convert.ToString(DataArray(2), 16) & ":" & Convert.ToString(DataArray(1), 16) & ":" & Convert.ToString(DataArray(0), 16)
            ' this is easy to display because the data is already in binary coded decimal
            ' up to reader to force 2 digit values (ie display 01:05:37 instead of 1:5:37)
        Else
            RichTextBox_Display.Text += "Error reading RTC" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
            ' turn off timer so don't keep getting error
            Timer_RTC.Enabled = False
            CheckBox_RTC.Checked = False
        End If
    End Sub

    Private Sub CheckBox_RTC_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox_RTC.CheckedChanged
        If (CheckBox_RTC.Checked) Then
            Timer_RTC.Enabled = True
        Else
            Timer_RTC.Enabled = False
        End If
    End Sub

    Private Sub Timer_ADC_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer_ADC.Tick
        ' routine to read and display smb adc
        Dim Demo_Slave_adc_Addr As Byte = &HAA
        ' determined by SMB firmware, &H says this is a hex number
        ' we are actually reading Register1 in the Demo board
        Dim Word_Addr As Byte = 1
        ' start reading at byte 1
        Dim DataArray(0) As Byte
        ' create array of length 1 byte to hold adc data
        Dim Return_Str As String = ""
        ' returns string representation of command - we won't use this
        Dim ADC_Count As Byte
        ' byte value will store adc count
        Array.Clear(DataArray, 0, DataArray.Length)
        ' initialize array
        '
        If (PICkitS.I2CM.Read(Demo_Slave_adc_Addr, Word_Addr, DataArray.Length, DataArray, Return_Str)) Then
            ' successful, display results
            ADC_Count = DataArray(0)
            Label_ADC_Count.Text = "Count: " & ADC_Count.ToString()
            ProgressBar_ADC.Value = ADC_Count
        Else
            RichTextBox_Display.Text += "Error reading ADC" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
            ' turn off timer so don't keep getting error
            Timer_ADC.Enabled = False
            CheckBox_ADC.Checked = False
        End If

    End Sub

    Private Sub CheckBox_ADC_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox_ADC.CheckedChanged
        If (CheckBox_ADC.Checked) Then
            Timer_ADC.Enabled = True
        Else
            Timer_ADC.Enabled = False
        End If
    End Sub

   
    Private Sub Button_Get_Bit_Rate_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Get_Bit_Rate.Click
        Dim BitRate As Double
        BitRate = PICkitS.I2CM.Get_I2C_Bit_Rate()
        If (BitRate = 0) Then
            RichTextBox_Display.Text += "Error retrieving I2C bit rate" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        Else
            RichTextBox_Display.Text += "Bit rate = " & String.Format("{0:0.0}", BitRate) & " kHz" & vbCrLf
        End If
    End Sub

    Private Sub Button_Set_Bit_Rate_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Set_Bit_Rate.Click
        Dim BitRate_Set As Double
        BitRate_Set = Convert.ToDouble(TextBox_Bit_Rate_Set.Text)
        If (PICkitS.I2CM.Set_I2C_Bit_Rate(BitRate_Set) = True) Then
            RichTextBox_Display.Text += "Bit rate set" & vbCrLf
        Else
            RichTextBox_Display.Text += "Error setting I2C bit rate" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If

    End Sub

    Private Sub Button_Get_Volt_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Get_Volt.Click
        Dim Volt As Double = 0
        Dim PKSA_Power As Boolean
        If (PICkitS.I2CM.Get_Source_Voltage(Volt, PKSA_Power)) Then
            RichTextBox_Display.Text += "Source voltage set to " & String.Format("{0:0.0}V", Volt) & vbCrLf & "PKSA powering device is set to " & PKSA_Power.ToString() & vbCrLf
        Else
            RichTextBox_Display.Text += "Error retrieving source voltage" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If
    End Sub

    Private Sub Button_Set_Volt_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Set_Volt.Click
        Dim Volt As Double
        Volt = Convert.ToDouble(TextBox_Volt.Text)
        If (PICkitS.I2CM.Set_Source_Voltage(Volt)) Then
            RichTextBox_Display.Text += "Voltage Set to " & Volt.ToString() & vbCrLf
        Else
            RichTextBox_Display.Text += "Error setting Voltage" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If
    End Sub

    Private Sub Button_Use_Ext_Volt_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Use_Ext_Volt.Click
        If (PICkitS.I2CM.Tell_PKSA_To_Use_External_Voltage_Source()) Then
            RichTextBox_Display.Text += "PKSA set to use external voltage source" & vbCrLf
        Else
            RichTextBox_Display.Text += "Error telling PKSA to use external voltage source" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If
    End Sub

    Private Sub Button_Get_Aux_Status_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Get_Aux_Status.Click
        Dim Aux1_State, Aux1_Dir, Aux2_State, Aux2_Dir As Boolean
        If (PICkitS.I2CM.Get_Aux_Status(Aux1_State, Aux2_State, Aux1_Dir, Aux2_Dir)) Then
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
        If (PICkitS.I2CM.Set_Aux1_Direction(CheckBox_Dir_1.Checked)) Then
            If (PICkitS.I2CM.Set_Aux2_Direction(CheckBox_Dir_2.Checked)) Then
                If (PICkitS.I2CM.Set_Aux1_State(CheckBox_State_1.Checked)) Then
                    If (PICkitS.I2CM.Set_Aux2_State(CheckBox_State_2.Checked)) Then
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

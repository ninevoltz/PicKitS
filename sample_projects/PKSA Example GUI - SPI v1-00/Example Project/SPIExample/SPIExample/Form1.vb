Public Class Form1

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        If (PICkitS.Device.Initialize_PICkitSerial()) Then
            If (PICkitS.SPIM.Configure_PICkitSerial_For_SPIMaster()) Then
                RichTextBox_Display.Text += "PICkit Serial correctly configured for SPI Master" & vbCrLf
            Else
                RichTextBox_Display.Text += "Error configuring PICkit Serial for SPI Master" & vbCrLf
            End If
        Else
            RichTextBox_Display.Text += "Error initializing PICkit Serial" & vbCrLf
        End If
    End Sub

    Private Sub Form1_FormClosed(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosedEventArgs) Handles MyBase.FormClosed
        PICkitS.Device.Cleanup()
    End Sub

    Private Sub Button_Execute_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Execute.Click
        Dim SendDataArray(5) As Byte
        ' data we send stored here
        Dim ReceiveDataArray(255) As Byte
        ' data we receive stored here
        Dim SendByteCount As Byte = 0
        ' how many data bytes in SendDataArray
        Dim ReceiveByteCount As Byte = 0
        ' how many data bytes in RetrieveDataArray
        Dim SendTextBoxArray(5) As TextBox
        ' array to hold send textboxes
        Dim index As Integer
        ' counter
        Dim Return_Str As String = ""
        ' returns string representation of command - we won't use this
        Dim DeAssert As Boolean
        ' whether or not to append chip select de-assert
        Dim Assert As Boolean
        ' whether or not to prepend chip select assert
        Dim Display_Str As String = ""
        ' temporary storage for display
        ' clear the arrays
        Array.Clear(SendDataArray, 0, SendDataArray.Length)
        Array.Clear(ReceiveDataArray, 0, ReceiveDataArray.Length)
        ' popuplate array of textboxes
        SendTextBoxArray(0) = TextBox_Send_Data_0
        SendTextBoxArray(1) = TextBox_Send_Data_1
        SendTextBoxArray(2) = TextBox_Send_Data_2
        SendTextBoxArray(3) = TextBox_Send_Data_3
        SendTextBoxArray(4) = TextBox_Send_Data_4
        SendTextBoxArray(5) = TextBox_Send_Data_5
        ' loop through textbox array and populate SendDataArray
        For index = 0 To SendTextBoxArray.Length - 1
            If (SendTextBoxArray(index).Text <> "") Then
                ' this textbox has data, add to byte array
                SendDataArray(index) = Convert.ToByte(SendTextBoxArray(index).Text, 16)
                ' 16 assumes this is hex data
                ' increment send byte count
                SendByteCount += 1
            Else
                ' no more data, get out of for loop
                Exit For
            End If
        Next
        ' now see if we are retrieving data
        If (TextBox_Receive_Byte_Count.Text <> "") Then
            ReceiveByteCount = Convert.ToByte(TextBox_Receive_Byte_Count.Text, 16)
            ' 16 assumes this is hex data
        End If
        ' we have all our data, now send commands
        ' if SendByteCount <> 0 then first we use the 
        If (SendByteCount <> 0) Then
            ' need to use PICkitS.SPIM.Send_Data
            ' if we are retrieving data, do no send de-assert command
            If (ReceiveByteCount <> 0) Then
                DeAssert = False
                ' we will send the retrieve command also
            Else
                DeAssert = True
                ' not retrieving data, so send de-assert command
            End If
            ' first command, so p_assert_cs is true
            If (PICkitS.SPIM.Send_Data(SendByteCount, SendDataArray, True, DeAssert, Return_Str)) Then
                For index = 0 To SendByteCount - 1
                    Display_Str += SendTextBoxArray(index).Text & " "
                Next index
                RichTextBox_Display.Text += "Sent Data: " & Display_Str & vbCrLf
            Else
                RichTextBox_Display.Text += "Error sending SPI.Send_Data" & vbCrLf
            End If
        End If
        ' now see if we need to send receive command
        If (ReceiveByteCount <> 0) Then
            If (SendByteCount <> 0) Then
                Assert = False
                ' we already asserted, do not assert again
            Else
                Assert = True
                ' have not yet asserted, so need to now
            End If
            Display_Str = ""
            ' clear Display_Str for this command
            ' last commmand so p_de_assert_cs is true
            If (PICkitS.SPIM.Receive_Data(ReceiveByteCount, ReceiveDataArray, Assert, True, Return_Str)) Then
                For index = 0 To ReceiveByteCount - 1
                    Display_Str += Convert.ToString(ReceiveDataArray(index), 16) & " "
                Next index
                RichTextBox_Display.Text += "Received Data: " & Display_Str & vbCrLf
            Else
                RichTextBox_Display.Text += "Error sending SPI.Receive_Data" & vbCrLf
            End If
        End If
    End Sub

    Private Sub Button_Get_Bit_Rate_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Get_Bit_Rate.Click
        Dim BitRate As Double
        BitRate = PICkitS.SPIM.Get_SPI_Bit_Rate()
        If (BitRate = 0) Then
            RichTextBox_Display.Text += "Error retrieving spi bit rate" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        Else
            RichTextBox_Display.Text += "Bit rate = " & String.Format("{0:0.00}", BitRate) & " kHz" & vbCrLf
        End If
    End Sub

    Private Sub Button_Set_Bit_Rate_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Set_Bit_Rate.Click
        Dim BitRate_Set As Double
        BitRate_Set = Convert.ToDouble(TextBox_Bit_Rate_Set.Text)
        If (PICkitS.SPIM.Set_SPI_BitRate(BitRate_Set) = True) Then
            RichTextBox_Display.Text += "Bit rate set" & vbCrLf
        Else
            RichTextBox_Display.Text += "Error setting SPI bit rate" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If
    End Sub

    Private Sub CheckBox_Voltage_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox_Voltage.CheckedChanged
        If (CheckBox_Voltage.Checked) Then
            ' tell pksa to power device
            If (PICkitS.SPIM.Tell_PKSA_To_Power_My_Device()) Then
                RichTextBox_Display.Text += "PKSA is powering my device" & vbCrLf
            Else
                RichTextBox_Display.Text += "Error telling PKSA to power my device" & vbCrLf
                PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
            End If
        Else
            ' tell pksa not to power device
            If (PICkitS.SPIM.Tell_PKSA_To_Use_External_Voltage_Source()) Then
                RichTextBox_Display.Text += "PKSA is not powering my device" & vbCrLf
            Else
                RichTextBox_Display.Text += "Error telling PKSA not to power my device" & vbCrLf
                PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
            End If
        End If
    End Sub

   
    Private Sub Button_Get_Status_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Get_Status.Click
        If (PICkitS.SPIM.Get_SPI_Status(CheckBox_Sample_Phase.Checked, _
                                       CheckBox_Clock_Edge_Select.Checked, _
                                       CheckBox_Clock_Polarity.Checked, _
                                       CheckBox_Auto_Output_Disable.Checked, _
                                       CheckBox_SDI_State.Checked, _
                                       CheckBox_SDO_State.Checked, _
                                       CheckBox_Clock_State.Checked, _
                                       CheckBox_Chip_Select_State.Checked)) Then
            RichTextBox_Display.Text += "Retrieved Status" & vbCrLf
        Else
            RichTextBox_Display.Text += "Error getting PKSA status" & vbCrLf
            PICkitS.Device.Reset_Control_Block()  ' clear any errors in PKSA
        End If
    End Sub
End Class

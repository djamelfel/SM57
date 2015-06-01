dsPICDEM2 30F4011 Demonstration Project Readme File
---------------------------------------------------------------------


Set up the dsPICDEM2 Development Board for 30F4011 device:
--------------------------------------------------------------
1. Install a 30F4011 device in the socket labelled U2A1
2. Ensure jumpers JP1 and JP2 are installed.
3. In group of switches labelled S2, turn ON only the switch labelled M ALL.
4. Switches S3 and S4 should be turned OFF (all positions OFF).
5. Install jumpers to set the header H1 to the M40 position. This connects
    the SCK1 and SDO1 pins on the dsPIC to the LCD Controller(U3) on the board.
6. Ensure that any jumpers on header H2 are removed
7. Ensure that any jumpers on header H3 are removed
8. Install jumpers to set the header H4 to the M ALL position. This connects
   the RS232 drivers on-board to the U1ATX & U1ARX pins on the dsPIC.
9. Ensure that any jumpers on header H5 are removed
10. Install a jumper to set header H6 to the M ALL posiion. This connects switch
   S5 to the external interrupt pin INT0 on the dsPIC.
11. Install a jumper to set header H7 to the M ALL posiion. This connects
   switch S6 to the external interrupt pin INT1 on the dsPIC.
12. Install jumpers to set the headers H10 and H13 to the M ALL position.
   The temperature sensor(U5) and the potentiometer(R15) on the board
   are now connected to the Analog Input pins, AN2 and AN3 on the dsPIC.
13. Ensure any jumpers on header H11 are removed
14. Install jumpers to set the header H12 to the M positions. This connects
    LEDs D3 and D4 on the board to Digital I/O pins RB0 & RB1 on the dsPIC.

Connect dsPICDEM 2 Development Board to MPLAB ICD 2 and your PC:
---------------------------------------------------------------------
1. Connect the MPLAB ICD2 to your PC using the USB cable.
2. Connect RJ11 cable from MPLAB ICD 2 to ICD header (J1) on dsPICDEM 2 board.
3. Connect the Male end of an RS232 cable to the DB9 connector (J2) on the
   dsPICDEM 2 board. Connect other end of cable to the COM port on your PC.
4. Apply power to the board using a 9v power supply at J4

Running the accompanying demonstration software on the dsPIC30F4011
---------------------------------------------------------------------------
Following these steps in the sequence noted:
1. From MPLAB IDE, open Demo.mcw workspace file within MPLAB IDE by selecting:
   File>>Open Workspace
   The Demo workspace should open and ICD2 should be able to detect the
   30F4011 device installed on the dsPICDEM2 board.
4. Build the Project within MPLAB IDE by selecting the option:
   Project>>Build All
5. Program the sample 30F4011 device from MPLAB IDE by selecting:
   Programmer>>Program
6. Release the device from Reset from within MPLAB IDE by selecting the option:
   Programmer>>Release from Reset
7. In group of switches labelled S2 on the dsPICDEM2 development board, turn
   OFF the switch labelled M ALL.
8. The demonstration program is now running.
9. On your PC/Workstation, open the file "Hyperterminal_RS232_dsPICDEM2.ht"
   provided in this folder with HyperTerminal. You should see a scrolling display
   on the HyperTerminal window that indicates the room temperature and the
   voltage supplied by the potentiometer to the dsPIC.


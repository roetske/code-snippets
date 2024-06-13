# esp_now twoway com direct between 2 esp32
# 1.0 For esp now getting macadres of your board needed for peer to peer <br>
# 1.1 examples rui santos and iom2021 getting macadress/
First tries getting macadress i got only zeroes. <br>
wifi sta => WiFi.STA.begin(); so important.<br>
Code iom2021 while loop sta started also nice.<br>
Code and docs are a big mystery at times. You depend on snippets and puzzle from your snippet.<br>
# 1.2 two way com OnDataRecv compile error with v3 esp now on arduino ide

Onreceive compiler error when using snippets in v3 esp lib <br>
In the older compiler it was formulated <br>
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) <br>
In the revised v3 code, replace line with this code <br>
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) <br>
# 1.3 ino code twoway com esp_now 
Get mac address from both esp. Fill then for each ino the right mac address and go. <br>
Made a test with 2 esp each having: <br>
-2 inputs pushbuttons <br>
-2 outputs led red. <br>
-1 blue led for visualisation com between 2 esp <br>
    -blinkled when communication. (the blink is controlled by each other to see com) <br>
-watchdog <br>
-no com all outputs are off.<br>
-important data exchange struct must be the same for both esp32 <br>
working principle programs <br>
-------------------------------<br>
simple test when a pushbutton is pressed on one esp32 a led must go on the other esp32 and vice versa. <br>
ideal test io mirror wireless.
the blinking blue led is off when no communication between the 2 esp32.



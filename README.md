# code-snippets
interesting code snippets 
# 1.0 For esp now getting macadres of your board needed for peer to peer <br>
# 1.1 examples rui santos and iom2021 getting macadress/
First tries getting macadress i got only zeroes. <br>
wifi sta => WiFi.STA.begin(); so important.<br>
Code iom2021 while loop sta started also nice.<br>
Code and docs are a big mystery at times. You depend on snippets and puzzle from your snippet.<br>
# 1.2 two way com OnDataRecv compile error with v3 esp now on arduino ide
Get mac address from both esp. Fill then for each ino the right mac address and go. <br>
Onreceive compiler error when using snippets in v3 esp lib <br>
In the older compiler it was formulated <br>
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) <br>
In the revised v3 code, it is replace line with this code <br>
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) <br>

Made a test with 2 esp each <br>
-2 inputs pushbuttons <br>
-2 outputs led red. <br>
-1 blue led for visualisation com between 2 esp <br>
    -blinkled when communication. (the blink is controlled by each other to see com) <br>
-watchdog <br>
-no com all outputs are off.<br>



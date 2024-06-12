# code-snippets
interesting code snippets 
#For esp now getting macadres of your board needed for peer to peer <br>

Onreceive compiler error when using snippets in v3 esp lib <br>
In the older compiler it was formulated <br>
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) <br>
In the revised v3 code, it is replace line with this code <br>
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) <br>

#examples rui santos and iom2021 getting macadress/
First tries i got only zeroes. <br>
wifi sta => WiFi.STA.begin(); so important.<br>
Code iom2021 while loop sta started also nice.<br>
Code and docs are a big mystery at times. You depend on snippets and puzzle from your snippet.<br>

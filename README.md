# code-snippets
interesting code snippets 
#For esp now getting macadres of your board needed for peer to peer
onreceive compiler error when using snippets in v3 esp lib <br>
In the older compiler it was formulated <br>
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) <br>
In the revised v3 code, it is replace line with this code <br>
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) <br>

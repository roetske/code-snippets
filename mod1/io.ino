void setupio()
{
  //declaratie var en init
 
  pinMode(o_output1,OUTPUT);
  pinMode(o_output2,OUTPUT);
  pinMode(o_blueled,OUTPUT);

  pinMode(i_input1,INPUT);
  pinMode(i_input2,INPUT);

  digitalWrite(o_output1,HIGH);
  digitalWrite(o_output2,HIGH);
  digitalWrite(o_blueled,HIGH);

  delay(5000);

  digitalWrite(o_output1,LOW);
  digitalWrite(o_output2,LOW);
  digitalWrite(o_blueled,LOW);

}
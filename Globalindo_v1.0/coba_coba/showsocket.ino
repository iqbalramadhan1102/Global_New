void ShowSockStatus() {
  for(int x = 0; x < MAX_SOCK_NUM; x++) 
  {
    Serial.print(F("Socket #"));
    Serial.print(x,DEC);
    if(WiFi._state[x] == -1) Serial.print(F(": available  port "));
    else Serial.print(F(": used  port "));
    Serial.println(WiFi._server_port[x]);   
  }
}

void SetSockStatus() {
  for(int x = 0; x < MAX_SOCK_NUM; x++) 
  {
    WiFi._state[x] = -1;
  }
}

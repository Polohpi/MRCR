#ifndef PAGE_H
#define PAGE_H

//declaration of protoype
void ManualModePage();
void MainPage();

// main page
void MainPage()
{
  if(MAIN_PAGE == true)
  {
    //SerialUSB.println("MainPage");
  }
  yield();
}

// manual page
void ManualModePage()
{
  if(MANUAL_MODE_PAGE == true)
  {
    //Serial.println("ManualMode");
    if( (millis() - nextion_refresh_timer) > NEXTION_REFRESH_TIME)
    {
      Nextion.writeStr("t11.txt", String(Timeleft)+ " min");
      nextion_refresh_timer = millis();
    }
  }
  yield();
}




#endif
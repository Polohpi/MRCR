#ifndef PAGE_H
#define PAGE_H

//declaration of protoype
void ManualModePage();
void MainPage();

// main page
void MainPage()
{
  Nextion.writeStr("page 1");
  SerialUSB.println("MainPage");

  while (1)
  {  
    if(ChangePageMenu == MANUAL_MODE_PAGE)
    {
      ManualModePage();
    }
    yield();
  }
  yield();
}

// manual page
void ManualModePage()
{
  Nextion.writeStr("page 2");
  Serial.println("ManualMode");
  while(1)
  {
    if(ChangePageMenu == MAIN_PAGE)
    {
      MainPage();
    }
    if( (millis() - nextion_refresh_timer) > NEXTION_REFRESH_TIME)
    {
      Nextion.writeStr("t11.txt", String(Timeleft)+ " min");
      nextion_refresh_timer = millis();
    }

    yield();
  }
  yield();
}




#endif
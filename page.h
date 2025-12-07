#ifndef PAGE_H
#define PAGE_H

void ManualModePage()
{
  //ChangePageMenu = 0;
  Nextion.writeStr("page 2");
  Serial.println("ManualMode");
  while(1)
  {
    if(ChangePageMenu == MAIN_PAGE)
    {
      loop();
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
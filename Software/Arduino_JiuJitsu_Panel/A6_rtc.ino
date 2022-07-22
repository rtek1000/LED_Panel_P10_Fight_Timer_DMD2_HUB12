
void update_rtc(void) {
  dmd.stop();
  
  rtc.adjust(DateTime(wr_rtc_yy, wr_rtc_mm, wr_rtc_dd, wr_rtc_HH, wr_rtc_MM, 0)); // yyyy,mm,dd,HH,MM,SS
  
  Ds3231SqwPinMode mode = rtc.readSqwPinMode();
  if ( mode != DS3231_SquareWave1Hz ) {
    rtc.writeSqwPinMode( DS3231_SquareWave1Hz );
  }
  
  dmd.resume();
}

void get_rtc(void) {
  dmd.stop();
  
  now = rtc.now();

  rtc_temp = rtc.getTemperature();
  
  Ds3231SqwPinMode mode = rtc.readSqwPinMode();
  if ( mode != DS3231_SquareWave1Hz ) {
    rtc.writeSqwPinMode( DS3231_SquareWave1Hz );
  }
  
  dmd.resume();
}

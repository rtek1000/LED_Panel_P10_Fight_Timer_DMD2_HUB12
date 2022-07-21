
void update_rtc(void) {
  dmd.stop();
  rtc.adjust(DateTime(wr_rtc_yy, wr_rtc_mm, wr_rtc_dd, wr_rtc_HH, wr_rtc_MM, 0)); // yyyy,mm,dd,HH,MM,SS
  dmd.resume();
}

void get_rtc(void) {
  dmd.stop();
  now = rtc.now();
  dmd.resume();
}

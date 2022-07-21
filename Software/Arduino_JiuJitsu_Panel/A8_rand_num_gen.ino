//void seed_rand(void) {
//  pinMode(A7, INPUT);
//
//  uint32_t val_sum = 0;
//  uint32_t val_time1 = 0;
//  uint32_t val_time2 = 0;
//  uint16_t timeout = 0;
//
//  for (int i = 0; i < 5; i++) {
//    val_time1 = micros();
//
//    timeout = 0;
//
//    while ((analogRead(A7) > 512) || (timeout < 10)) {
//      delayMicroseconds(1);
//
//      wdt_reset(); //  reset watchdog
//
//      timeout++;
//    }
//
//    timeout = 0;
//
//    while ((analogRead(A7) < 512) || (timeout < 10)) {
//      delayMicroseconds(1);
//
//      wdt_reset(); //  reset watchdog
//
//      timeout++;
//    }
//
//    val_time2 = micros() - val_time1;
//
//    val_sum += val_time2;
//  }
//
//  val_sum /= 10;
//
//  randomSeed(val_sum);
//}

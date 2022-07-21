void temperature_init(void) {
  byte loop_init1;

  for (loop_init1 = 0; loop_init1 <= 49; loop_init1++) {
    temperature_read();
  }
}

void temperature_read(void) {
  byte loop1;
  float calc_temp1;

  temperature_array[temperature_index] = analogRead(LM35) / 9.31;

  if (temperature_index <= max_array_temp) {
    temperature_index++;
  } else {
    temperature_index = 0;
  }

  calc_temp1 = 0;

  for (loop1 = 0; loop1 <= max_array_temp; loop1++) {
    calc_temp1 = calc_temp1 + temperature_array[loop1];
  }

  temperature = calc_temp1 / (float)max_array_temp;
}

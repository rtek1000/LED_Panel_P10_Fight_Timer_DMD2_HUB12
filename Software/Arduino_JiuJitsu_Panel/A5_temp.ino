void temperature_init(void) {
  byte loop_init1;

  for (loop_init1 = 0; loop_init1 <= 49; loop_init1++) {
    temperature_read();
  }
}

void temperature_read(void) {
  byte loop1 = 0;
  float calc_temp1 = 0;

  // analogRead(LM35) / 9.31; // analog ref internal
  // Deg °C = (float(analogRead(LM35)) * (5.0 / (1024.0))) / 0.01;

  // Corrective factor:
  // 28.3°C (Arduino) / 24.7°C (0.247V using Multimeter) = 1.145748988
  // 0.48828125 / 1.145748988 = 0.426167734
//#define Corrective_Factor 0.426167734
  
  temperature_array[temperature_index] = float(analogRead(LM35)) * Corrective_Factor; // 0.48828125 / 1.145748988

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

#include "button.h"

// Mode Button
uint8_t g_current_mode_button_state = 1; // Pin is pulled high by default
uint8_t g_previous_mode_button_state = 1;
uint32_t g_last_debounce_time = 0;
uint32_t g_debounce_delay = 50;

void initButtons(void)
{
  Serial.print("initButtons:");
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP); // Pin for switching screens button
  Serial.println(" OK");
}

void handleButtons(void)
{
  g_previous_mode_button_state = g_current_mode_button_state;
  g_current_mode_button_state = digitalRead(MODE_BUTTON_PIN);

  // Check if button is now pressed and it was previously unpressed
  if (g_current_mode_button_state == LOW && g_previous_mode_button_state == HIGH)
  {
    // We haven't waited long enough so ignore this press
    if (millis() - g_last_debounce_time <= g_debounce_delay)
    {
      return;
    }
    Serial.println("Button pressed");
    // Increment display state
    g_last_debounce_time = millis();
  }
}
#include "particles.h"

// Particulate matter sensor

PMS pms(Serial2);
PMS::DATA g_data;

char g_command_topic[50];        // MQTT topic for receiving commands

#define PMS_STATE_ASLEEP 0    // Low power mode, laser and fan off
#define PMS_STATE_WAKING_UP 1 // Laser and fan on, not ready yet
#define PMS_STATE_READY 2     // Warmed up, ready to give data
uint8_t g_pms_state = PMS_STATE_WAKING_UP;

uint32_t g_pms_state_start = 0;       // Timestamp when PMS state last changed
uint8_t g_pms_readings_taken = 0;     // 0/1: whether any readings have been taken
uint8_t g_pms_ppd_readings_taken = 0; // 0/1: whether PPD readings have been taken

uint8_t samples = 3600 / g_pms_report_period;

AQSSensor g_pm1p0_sp("SP_1P0",      device_name, samples);
AQSSensor g_pm2p5_sp("SP_2P5",      device_name, samples);
AQSSensor g_pm10p0_sp("SP_10P0",    device_name, samples);

AQSSensor g_pm1p0_ae("AE_1P0",      device_name, samples);
AQSSensor g_pm2p5_ae("AE_2P5",      device_name, samples);
AQSSensor g_pm10p0_ae("AE_10P0",    device_name, samples);

AQSSensor g_pm0p3_ppd("PPD_0P3",   device_name, samples);
AQSSensor g_pm0p5_ppd("PPD_0P5",   device_name, samples);
AQSSensor g_pm1p0_ppd("PPD_1P0",   device_name, samples);
AQSSensor g_pm2p5_ppd("PPD_2P5",   device_name, samples);
AQSSensor g_pm5p0_ppd("PPD_5P0",   device_name, samples);
AQSSensor g_pm10p0_ppd("PPD_10P0", device_name, samples);

void PMS_publishSensors(void );


void PMS_AddSensors( void )
{
  addSensorToList(&g_pm1p0_sp);
  addSensorToList(&g_pm2p5_sp);
  addSensorToList(&g_pm10p0_sp);

  addSensorToList(&g_pm1p0_ae);
  addSensorToList(&g_pm2p5_ae);
  addSensorToList(&g_pm10p0_ae);
  
  addSensorToList(&g_pm0p3_ppd);
  addSensorToList(&g_pm0p5_ppd);
  addSensorToList(&g_pm1p0_ppd);
  addSensorToList(&g_pm2p5_ppd);
  addSensorToList(&g_pm5p0_ppd);
  addSensorToList(&g_pm10p0_ppd);
}

uint32_t getLCDvalue(void)
{
  return g_pm2p5_ae.value();
}


void PMS_publishSensors(void )
{
  String message_string;

  /* Report PM1.0 AE value */
  g_pm1p0_ae.publish();
  g_pm2p5_ae.publish();
  g_pm10p0_ae.publish();

  g_pm1p0_sp.publish();
  g_pm2p5_sp.publish();
  g_pm10p0_sp.publish();

  if (1 == g_pms_ppd_readings_taken)
  {
    /* Report PM0.3 PPD value */
    g_pm0p3_ppd.publish();
    g_pm0p5_ppd.publish();
    g_pm1p0_ppd.publish();
    g_pm2p5_ppd.publish();
    g_pm5p0_ppd.publish();
    g_pm10p0_ppd.publish();
  }
}

void initParticles( void )
{
  Serial.print("initPMS:");
  // Open a connection to the PMS and put it into passive mode
  Serial2.begin(PMS_BAUD_RATE, SERIAL_8N1, PMS_RX_PIN, PMS_TX_PIN); // Connection for PMS5003
  pms.activeMode();                                                 // Tell PMS to stop sending data automatically
  delay(100);
  pms.wakeUp(); // Tell PMS to wake up (turn on fan and laser)

  //Serial2.begin(PMS_BAUD_RATE, SERIAL_8N1, PMS_RX_PIN, PMS_TX_PIN);

  PMS_AddSensors();

  Serial.println(" OK");

}

void handleParticles( void )
{
 uint32_t time_now = millis();

  // Check if we've been in the sleep state for long enough
  if (PMS_STATE_ASLEEP == g_pms_state)
  {
    if (time_now - g_pms_state_start >= ((g_pms_report_period * 1000) - (g_pms_warmup_period * 1000)))
    {
      // It's time to wake up the sensor
      Serial.println("Waking up sensor");
      pms.wakeUp();
      g_pms_state_start = time_now;
      g_pms_state = PMS_STATE_WAKING_UP;
    }
  }

  // Check if we've been in the waking up state for long enough
  if (PMS_STATE_WAKING_UP == g_pms_state)
  {
    if (time_now - g_pms_state_start >= (g_pms_warmup_period * 1000))
    {
      g_pms_state_start = time_now;
      g_pms_state = PMS_STATE_READY;
    }
  }

  // Put the most recent values into globals for reference elsewhere
  if (PMS_STATE_READY == g_pms_state)
  {
    //pms.requestRead();
    if (pms.readUntil(g_data))
    {
      g_pm1p0_sp.set(g_data.PM_SP_UG_1_0);
      g_pm2p5_sp.set(g_data.PM_SP_UG_2_5);
      g_pm10p0_sp.set(g_data.PM_SP_UG_10_0);

      g_pm1p0_ae.set(g_data.PM_AE_UG_1_0);
      g_pm2p5_ae.set(g_data.PM_AE_UG_2_5);
      g_pm10p0_ae.set(g_data.PM_AE_UG_10_0);

      // This condition below should NOT be required, but currently I get all
      // 0 values for the PPD results every second time. This check only updates
      // the global values if there is a non-zero result for any of the values:
      if (g_data.PM_TOTALPARTICLES_0_3 + g_data.PM_TOTALPARTICLES_0_5 + g_data.PM_TOTALPARTICLES_1_0 + g_data.PM_TOTALPARTICLES_2_5 + g_data.PM_TOTALPARTICLES_5_0 + g_data.PM_TOTALPARTICLES_10_0 != 0)
      {
        g_pm0p3_ppd.set(g_data.PM_TOTALPARTICLES_0_3);
        g_pm0p5_ppd.set(g_data.PM_TOTALPARTICLES_0_5);
        g_pm1p0_ppd.set(g_data.PM_TOTALPARTICLES_1_0);
        g_pm2p5_ppd.set(g_data.PM_TOTALPARTICLES_2_5);
        g_pm5p0_ppd.set(g_data.PM_TOTALPARTICLES_5_0);
        g_pm10p0_ppd.set(g_data.PM_TOTALPARTICLES_10_0);
        g_pms_ppd_readings_taken = 1;
      }
      else
      {
        Serial.println("PMS: zero's issue");
      }
      pms.sleep();

      // Report the new values
      PMS_publishSensors();

      g_pms_readings_taken = 1;
      g_pms_state_start = time_now;
      g_pms_state = PMS_STATE_ASLEEP;
    }
  }
}
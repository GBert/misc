#include <odroid_go.h>
#include <ESP32CAN.h>
#include <CAN_config.h>

CAN_device_t CAN_cfg;               // CAN Config
unsigned long previousMillis = 0;   // will store last time a CAN Message was send
const int interval = 4000;          // interval at which send CAN Messages (milliseconds)
const int rx_queue_size = 10;       // Receive Queue size

void setup() {
  GO.begin();
  GO.lcd.setTextFont(4);
  GO.lcd.setCursor(0, 0);
  GO.lcd.setTextColor(WHITE);
  GO.lcd.print("==== ESP32-Arduino-CAN ====");
  CAN_cfg.speed = CAN_SPEED_250KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_12;
  CAN_cfg.rx_pin_id = GPIO_NUM_15;
  CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
  // Init CAN Module
  ESP32Can.CANInit();
}

void loop() {

  CAN_frame_t rx_frame;

  unsigned long currentMillis = millis();

  // Receive next CAN frame from queue
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {
    GO.lcd.setTextSize(0);
    if (rx_frame.FIR.B.FF == CAN_frame_std) {
      GO.lcd.print("New standard frame");
    }
    else {
      GO.lcd.print("New extended frame");
    }

    if (rx_frame.FIR.B.RTR == CAN_RTR) {
      GO.lcd.printf(" RTR from 0x%08X, DLC %d\r\n", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
    }
    else {
      GO.lcd.printf(" from 0x%08X, DLC %d, Data ", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
      for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {
        GO.lcd.printf("0x%02X ", rx_frame.data.u8[i]);
      }
      GO.lcd.print("\n");
    }
  }
  // Send CAN Message
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    CAN_frame_t tx_frame;
    tx_frame.FIR.B.FF = CAN_frame_ext;
    tx_frame.MsgID = 0x00300700;
    tx_frame.FIR.B.DLC = 0;
    tx_frame.data.u8[0] = 0x00;
    tx_frame.data.u8[1] = 0x00;
    tx_frame.data.u8[2] = 0x00;
    tx_frame.data.u8[3] = 0x00;
    tx_frame.data.u8[4] = 0x00;
    tx_frame.data.u8[5] = 0x00;
    tx_frame.data.u8[6] = 0x00;
    tx_frame.data.u8[7] = 0x00;
    ESP32Can.CANWriteFrame(&tx_frame);
  }
}

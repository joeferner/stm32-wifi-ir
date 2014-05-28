
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_usart.h>
#include <misc.h>
#include <string.h>
#include "cc3000-host-driver/socket.h"
#include "cc3000-host-driver/netapp.h"
#include "stm32_cc3000.h"
#include "debug.h"
#include "delay.h"
#include "time.h"
#include "platform_config.h"
#include "ring_buffer.h"
#include "ir_tx.h"
#include "ir_rx.h"
#include "ir_code.h"
#include "connection_info.h"

char cc3000_device_name[] = "WiFi IR";

void setup();
void loop();
void wifi_connect();

#define MAX_LINE_LENGTH 100
#define INPUT_BUFFER_SIZE 100
uint8_t g_usartInputBuffer[INPUT_BUFFER_SIZE];
ring_buffer_u8 g_usartInputRingBuffer;

int main(void) {
  setup();
  while (1) {
    loop();
  }
  return 0;
}

void setup() {
  // Configure the NVIC Preemption Priority Bits
  // 2 bit for pre-emption priority, 2 bits for subpriority
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  debug_setup();
  debug_led_set(1);

  ring_buffer_u8_init(&g_usartInputRingBuffer, g_usartInputBuffer, INPUT_BUFFER_SIZE);

  ir_code_setup();
  time_setup();
  ir_tx_setup();
  ir_rx_setup();

  cc3000_setup(0, 0);
  wifi_connect();

  debug_led_set(0);
  debug_write_line("?END setup");
}

void loop() {
  IrRecv* irRecv = ir_rx_recv();
  if(irRecv != NULL) {
    IrCode* code = ir_code_decode(irRecv->buffer, irRecv->bufferLength);
    debug_write("?rx: ");
    if(code != NULL) {
      debug_write_u16(code->brand, 16);
      debug_write_u16(code->key, 16);
    } else {
      for(int i = 0; i < irRecv->bufferLength; i++) {
        debug_write_u16(irRecv->buffer[i], 10);
        debug_write(",");
      }
    }
    debug_write_line("");
  }
}

void assert_failed(uint8_t* file, uint32_t line) {
  debug_write("-assert_failed: file ");
  debug_write((const char*) file);
  debug_write(" on line ");
  debug_write_u32(line, 10);
  debug_write_line("");

  /* Infinite loop */
  while (1) {
  }
}

//uint16_t testcode0[] = { 2388, 541, 1232, 541, 615, 541, 1232, 540, 616, 541, 1232, 541, 616, 541, 615, 541, 1231, 540, 615, 541, 615, 541, 615, 541, 616 };
uint16_t testcode0[] = { 2440, 516, 1232, 541, 616, 541, 615, 542, 1231, 542, 615, 542, 614, 543, 614, 542, 1230, 543, 615, 542, 613, 543, 614, 543, 614 };

void on_usart1_irq() {
  char line[MAX_LINE_LENGTH];

  if (USART_GetITStatus(DEBUG_USART, USART_IT_RXNE) != RESET) {
    uint8_t data[1];
    data[0] = USART_ReceiveData(DEBUG_USART);

    ring_buffer_u8_write(&g_usartInputRingBuffer, data, 1);
    while (ring_buffer_u8_readline(&g_usartInputRingBuffer, line, MAX_LINE_LENGTH) > 0) {
      if(strcmp(line, "!CONNECT\n") == 0) {
        debug_write_line("+OK");
        debug_write_line("!clear");
        debug_write_line("!set name,stm32-wifi-ir");
        debug_write_line("!set description,'IR TX/RX over WiFi'");

        debug_write_line("?add widgets");
        debug_write_line("!add label,code,1,0,1,1");

        debug_write_line("!code.set minWidth,150");
        debug_write_line("!code.set title,'Code'");
        debug_write_line("!code.set text,'xxxxxxxxxxxxxxxxxxxx'");
      } else if(strncmp(line, "!TX", 3) == 0) {
        debug_write_line("+OK");
        ir_tx_send(testcode0, 25);
        delay_us(5000);
        ir_tx_send(testcode0, 25);
        delay_us(5000);
        ir_tx_send(testcode0, 25);
      } else {
        debug_write("?Unknown command: ");
        debug_write_line(line);
      }
    }
  }
}

void wifi_connect() {
  uint8_t cc3000MajorFirmwareVersion, cc3000MinorFirmwareVersion;

  cc3000_get_firmware_version(&cc3000MajorFirmwareVersion, &cc3000MinorFirmwareVersion);
  debug_write("?major: 0x");
  debug_write_u8(cc3000MajorFirmwareVersion, 16);
  debug_write_line("");
  debug_write("?minor: 0x");
  debug_write_u8(cc3000MinorFirmwareVersion, 16);
  debug_write_line("");
  if (cc3000MajorFirmwareVersion != 0x01 || cc3000MinorFirmwareVersion != 0x18) {
    debug_write_line("?Wrong firmware version!");
    while (1);
  }

  cc3000_display_mac_address();

  debug_write_line("?Deleting old connection profiles");
  if (cc3000_delete_profiles() != 0) {
    debug_write_line("?Failed!");
    while (1);
  }

#ifdef STATIC_IP_ADDRESS
  unsigned long aucIP = STATIC_IP_ADDRESS;
  unsigned long aucSubnetMask = STATIC_SUBNET_MASK;
  unsigned long aucDefaultGateway = STATIC_DEFAULT_GATEWAY;
  unsigned long aucDNSServer = STATIC_DNS_SERVER;
  if (netapp_dhcp(&aucIP, &aucSubnetMask, &aucDefaultGateway, &aucDNSServer) != 0) {
    debug_write_line("?netapp_dhcp Failed!");
    while (1);
  }
#else
  unsigned long aucIP = 0;
  unsigned long aucSubnetMask = 0;
  unsigned long aucDefaultGateway = 0;
  unsigned long aucDNSServer = 0;
  if (netapp_dhcp(&aucIP, &aucSubnetMask, &aucDefaultGateway, &aucDNSServer) != 0) {
    debug_write_line("?netapp_dhcp Failed!");
    while (1);
  }
#endif

  // Attempt to connect to an access point
  char *ssid = WLAN_SSID; /* Max 32 chars */
  debug_write("?Attempting to connect to ");
  debug_write_line(ssid);

  // NOTE: Secure connections are not available in 'Tiny' mode!
  if (cc3000_connect_to_ap(WLAN_SSID, WLAN_PASS, WLAN_SECURITY) != 0) {
    debug_write_line("?Connect Failed!");
    while (1);
  }

  debug_write_line("?Connected!");

#ifndef STATIC_IP_ADDRESS
  // Wait for DHCP to complete
  debug_write_line("?Request DHCP");
  while (cc3000_check_dhcp() != 0) {
    delay_ms(100);
  }
#endif

  while (!cc3000_is_connected()) {
    delay_ms(100);
  }

  cc3000_display_ipconfig();
}

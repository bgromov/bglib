#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <bglib/cmd_def.h>
#include <bglib/uart.h>

#define UART_TIMEOUT 1000

volatile int tty_fd = -1;

struct gap_ad_t {
  uint8 len;
  uint8 ad_type; // See enum gap_ad_types
  uint8 data[];  // length: len - 1
};

void print_raw_packet(struct ble_header *hdr, unsigned char *data)
{
  printf("Incoming packet: ");
  int i;
  for (i = 0; i < sizeof(*hdr); i++)
  {
    unsigned char c = ((unsigned char *)hdr)[i];
    printf("%02x ", c);
  }
  for (i = 0; i < hdr->lolen; i++)
  {
    unsigned char c = data[i];
    printf("%02x ", c);
  }
  printf("\n");
}

void parse_gap_ad(const uint8array* data)
{
  const uint8* p = data->data;
  const uint8* end_p = data->data + data->len;

  struct gap_ad_t* ad = (struct gap_ad_t*)p;

  char str[256] = {};

  int i;
  while (p < end_p)
  {
    switch (ad->ad_type)
    {
      case gap_ad_type_localname_short:
      case gap_ad_type_localname_complete:
        strncpy(str, ad->data, ad->len - 1);
        printf("name: \"%s\" ", str);
        break;
      case gap_ad_type_flags:
        printf("flags: %d ", ad->data[0]);
        break;
      case gap_ad_type_services_16bit_all:
      case gap_ad_type_services_16bit_more:
      case gap_ad_type_services_32bit_all:
      case gap_ad_type_services_32bit_more:
      case gap_ad_type_services_128bit_all:
      case gap_ad_type_services_128bit_more:
        printf("UUIDs: ");
        for (i = ad->len - 2; i >= 0 ; i--)
          printf("%02x ", ad->data[i]);
        break;
      default:
        printf("unsupported: (type = 0x%02x, len = %d) ", ad->ad_type, ad->len);
    }
    p += ad->len + 1;
    ad = (struct gap_ad_t*)p;
  }
}

void ble_evt_gap_scan_response(const struct ble_msg_gap_scan_response_evt_t* msg)
{
  printf("RSSI: %d dB MAC: ", msg->rssi);
  int i;
  int addr_ln = sizeof(msg->sender);
  for (i = addr_ln - 1; i >= 0; i--)
  {
    printf("%02x", ((uint8*)&msg->sender)[i]);
    if (i > 0) printf(":");
    else printf(" ");
  }
  parse_gap_ad(&msg->data);
  printf("\n");
}

// function to be used by bglib to send commands
void output(uint8 len1, uint8* data1, uint16 len2, uint8* data2)
{
  if (uart_tx(len1, data1) || uart_tx(len2, data2))
  {
    printf("ERROR: Writing to serial port failed\n");
    exit(1);
  }
}

int read_message(int timeout_ms)
{
  unsigned char data[256]; // enough for BLE
  struct ble_header hdr;
  int r;

  r = uart_rx(sizeof(hdr), (unsigned char *)&hdr, UART_TIMEOUT);
  if (!r)
  {
    return -1; // timeout
  }
  else if (r < 0)
  {
    printf("ERROR: Reading header failed. Error code: %d\n", r);
    return 1;
  }

  if (hdr.lolen)
  {
    r = uart_rx(hdr.lolen, data, UART_TIMEOUT);
    if (r <= 0)
    {
      printf("ERROR: Reading data failed. Error code: %d\n", r);
      return 1;
    }
  }

  const struct ble_msg *msg = ble_get_msg_hdr(hdr);

#ifdef DEBUG
  print_raw_packet(&hdr, data);
#endif

  if (!msg)
  {
    printf("ERROR: Unknown message received\n");
    exit(1);
  }

  msg->handler(data);

  return 0;
}

int main(int argc, char** argv)
{
  char* uart_port = NULL;

  if (argc == 2)
    uart_port = argv[1];
  else
  {
    printf("Usage: %s <serial_port>\n\n", argv[0]);
    return 1;
  }

  bglib_output = output;

  if (uart_open(uart_port))
  {
    printf("ERROR: Unable to open serial port\n");
    return 1;
  }

  // Reset dongle to get it into known state
  ble_cmd_system_reset(0);
  uart_close();
  do
  {
    usleep(500000); // 0.5s
  } while (uart_open(uart_port));

  ble_cmd_gap_end_procedure();
  ble_cmd_gap_discover(gap_discover_observation);

  while (1)
  {
    if (read_message(UART_TIMEOUT) > 0)
      break;
  }

  uart_close();

  return 0;
}

# BGLib
BlueGiga BLE (Bluetooth Low-Energy aka Bluetooth Smart) library.

Current version is based on BlueGiga SDK v1.3.2-122.

# How to install

1. Clone

        $ git clone https://github.com/bgromov/bglib.git

2. Build

        $ cd bglib
        $ mkdir build && cd build
        $ cmake .. -DCMAKE_BUILD_TYPE=Release
        $ make
  
3. Run example (optional)

        ## You may need to run this as root
        $ ./scan_example /dev/ttyACM0

4. Install

        ## On Debian or Ubuntu
        $ make package
        $ sudo dpkg -i bglib-1.3.2-122-Linux.deb
    
    *Caution*: The following won't generate uninstall targets, so be carefull to run it as root
    
        ## On other systems
        $ make install

    These commands will install files to the following directories:
    
    - Library `libbglib.so` -> `${prefix}/lib/`
    - Header files -> `${prefix}/include/bglib/`
    - Examples -> `${prefix}/share/bglib/examples/`
    - pkg-config file `libbglib.pc` -> `${prefix}/lib/pkgconfig/`
    
# How to use

The library uses GCC weak symbols (see `weak` function attributes [here](https://gcc.gnu.org/onlinedocs/gcc-4.8.5/gcc/Function-Attributes.html)) to allow user to reimplement default empty handlers.

For example, to handle `ble_evt_gap_scan_response` one just need to add corresponding function implementation in the code.

Here is a possible implementation (taken from [`examples/scan_example/main.c`](https://github.com/bgromov/bglib/blob/master/examples/scan_example/main.c#L79)):

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

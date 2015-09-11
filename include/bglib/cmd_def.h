#ifndef cmd_def_h
#define cmd_def_h

#include <functional>
#include <map>
#include <memory>
#include <typeindex>

#include <stdarg.h>

/*****************************************************************************
 *
 *      Support for compilers other than GCC and MSVC:
 *
 *      To support specific compiler add compiler's structure packing directives to following macro:
 *          PACKSTRUCT( decl )
 *
 *      BGLIB uses PACKSTRUCT macro to add packing information for structures:
 *      PACKSTRUCT(struct wifi_msg_dfu_reset_cmd_t
 *      {
 *          uint8 dfu;
 *      });
 *
 *
 ****************************************************************************/

#include "apitypes.h"

namespace bglib {

/* Compability */
#ifndef PACKSTRUCT
#ifdef PACKED
#define packstruct(a) a PACKED
#else
/*Default packed configuration*/
#ifdef __GNUC__
#ifdef _WIN32
#define packstruct( decl ) decl __attribute__((__packed__,gcc_struct))
#else
#define packstruct( decl ) decl __attribute__((__packed__))
#endif
#define ALIGNED __attribute__((aligned(0x4)))

#else //msvc

#define packstruct( decl ) __pragma( pack(push, 1) ) decl __pragma( pack(pop) )
#define ALIGNED

#endif
#endif
#endif

struct ble_cmd_packet;

class MsgHandler
{
public:
  virtual ~MsgHandler() {};
  virtual void call(const void* msg) = 0;
};

using MsgHandlerPtr = std::shared_ptr<MsgHandler>;

template <typename T>
using MsgHandlerFunc = std::function<void(const T*)>;

template <class T>
class MsgHandlerT: public MsgHandler
{
  const MsgHandlerFunc<T> handler_;
public:
  MsgHandlerT(const MsgHandlerFunc<T>& handler): handler_(handler) {}

  void call(const void* msg)
  {
    handler_(static_cast<const T*>(msg));
  }
};

struct ble_header
{
    uint8  type_hilen;
    uint8  lolen;
    uint8  cls;
    uint8  command;
};

struct ble_msg
{
    struct ble_header    hdr;
    uint32               params;
    MsgHandlerPtr        handler;
};

enum system_endpoints
{
  system_endpoint_api    = 0,
  system_endpoint_test   = 1,
  system_endpoint_script = 2,
  system_endpoint_usb    = 3,
  system_endpoint_uart0  = 4,
  system_endpoint_uart1  = 5,
  system_endpoints_max   = 6
};

enum attributes_attribute_change_reason
{
  attributes_attribute_change_reason_write_request      = 0,
  attributes_attribute_change_reason_write_command      = 1,
  attributes_attribute_change_reason_write_request_user = 2,
  attributes_attribute_change_reason_max                = 3
};

enum attributes_attribute_status_flag
{
  attributes_attribute_status_flag_notify   = 1,
  attributes_attribute_status_flag_indicate = 2,
  attributes_attribute_status_flag_max      = 3
};

enum connection_connstatus
{
  connection_connected         = 1,
  connection_encrypted         = 2,
  connection_completed         = 4,
  connection_parameters_change = 8,
  connection_connstatus_max    = 9
};

enum attclient_attribute_value_types
{
  attclient_attribute_value_type_read             = 0,
  attclient_attribute_value_type_notify           = 1,
  attclient_attribute_value_type_indicate         = 2,
  attclient_attribute_value_type_read_by_type     = 3,
  attclient_attribute_value_type_read_blob        = 4,
  attclient_attribute_value_type_indicate_rsp_req = 5,
  attclient_attribute_value_types_max             = 6
};

enum sm_bonding_key
{
  sm_bonding_key_ltk         = 0x01,
  sm_bonding_key_addr_public = 0x02,
  sm_bonding_key_addr_static = 0x04,
  sm_bonding_key_irk         = 0x08,
  sm_bonding_key_edivrand    = 0x10,
  sm_bonding_key_csrk        = 0x20,
  sm_bonding_key_masterid    = 0x40,
  sm_bonding_key_max         = 65
};

enum sm_io_capability
{
  sm_io_capability_displayonly     = 0,
  sm_io_capability_displayyesno    = 1,
  sm_io_capability_keyboardonly    = 2,
  sm_io_capability_noinputnooutput = 3,
  sm_io_capability_keyboarddisplay = 4,
  sm_io_capability_max             = 5
};

#define GAP_SCAN_HEADER_ADV_IND          0
#define GAP_SCAN_HEADER_ADV_DIRECT_IND   1
#define GAP_SCAN_HEADER_ADV_NONCONN_IND  2
#define GAP_SCAN_HEADER_SCAN_REQ         3
#define GAP_SCAN_HEADER_SCAN_RSP         4
#define GAP_SCAN_HEADER_CONNECT_REQ      5
#define GAP_SCAN_HEADER_ADV_DISCOVER_IND 6

#define GAP_AD_FLAG_LIMITED_DISCOVERABLE      0x01
#define GAP_AD_FLAG_GENERAL_DISCOVERABLE      0x02
#define GAP_AD_FLAG_BREDR_NOT_SUPPORTED       0x04
#define GAP_AD_FLAG_SIMULTANEOUS_LEBREDR_CTRL 0x10
#define GAP_AD_FLAG_SIMULTANEOUS_LEBREDR_HOST 0x20
#define GAP_AD_FLAG_MASK                      0x1f

enum gap_address_type
{
  gap_address_type_public = 0,
  gap_address_type_random = 1,
  gap_address_type_max    = 2
};

enum gap_discoverable_mode
{
  gap_non_discoverable     = 0,
  gap_limited_discoverable = 1,
  gap_general_discoverable = 2,
  gap_broadcast            = 3,
  gap_user_data            = 4,
  gap_discoverable_mode_max= 5
};

enum gap_connectable_mode
{
  gap_non_connectable           = 0,
  gap_directed_connectable      = 1,
  gap_undirected_connectable    = 2,
  gap_scannable_non_connectable = 3,
  gap_connectable_mode_max      = 4
};

enum gap_discover_mode
{
  gap_discover_limited     = 0,
  gap_discover_generic     = 1,
  gap_discover_observation = 2,
  gap_discover_mode_max    = 3
};

enum gap_ad_types
{
  gap_ad_type_none                 = 0,
  gap_ad_type_flags                = 1,
  gap_ad_type_services_16bit_more  = 2,
  gap_ad_type_services_16bit_all   = 3,
  gap_ad_type_services_32bit_more  = 4,
  gap_ad_type_services_32bit_all   = 5,
  gap_ad_type_services_128bit_more = 6,
  gap_ad_type_services_128bit_all  = 7,
  gap_ad_type_localname_short      = 8,
  gap_ad_type_localname_complete   = 9,
  gap_ad_type_txpower              = 10,
  gap_ad_types_max                 = 11
};

enum gap_advertising_policy
{
  gap_adv_policy_all               = 0,
  gap_adv_policy_whitelist_scan    = 1,
  gap_adv_policy_whitelist_connect = 2,
  gap_adv_policy_whitelist_all     = 3,
  gap_advertising_policy_max       = 4
};

enum gap_scan_policy
{
  gap_scan_policy_all       = 0,
  gap_scan_policy_whitelist = 1,
  gap_scan_policy_max       = 2
};


enum ble_parameter_types
{
    ble_msg_parameter_uint8=2,
    ble_msg_parameter_int8=3,
    ble_msg_parameter_uint16=4,
    ble_msg_parameter_int16=5,
    ble_msg_parameter_uint32=6,
    ble_msg_parameter_int32=7,
    ble_msg_parameter_uint8array=8,
    ble_msg_parameter_string=9,
    ble_msg_parameter_hwaddr=10,
    ble_msg_parameter_uint16array=11
};

enum ble_msg_types
{
    ble_msg_type_cmd=0x00,
    ble_msg_type_rsp=0x00,
    ble_msg_type_evt=0x80
};
enum ble_dev_types
{
    ble_dev_type_ble    =0x00,
    ble_dev_type_wifi   =0x08,
    ble_dev_type_iwrap  =0x10,
    ble_dev_type_dumo   =0x20,
    ble_dev_type_test   =0x20
};

enum ble_classes
{
  ble_cls_system,
  ble_cls_flash,
  ble_cls_attributes,
  ble_cls_connection,
  ble_cls_attclient,
  ble_cls_sm,
  ble_cls_gap,
  ble_cls_hardware,
  ble_cls_test,
  ble_cls_dfu,
  ble_cls_last
};
enum ble_command_ids
{
  ble_cmd_system_reset_id=0,
  ble_cmd_system_hello_id=1,
  ble_cmd_system_address_get_id=2,
  ble_cmd_system_reg_write_id=3,
  ble_cmd_system_reg_read_id=4,
  ble_cmd_system_get_counters_id=5,
  ble_cmd_system_get_connections_id=6,
  ble_cmd_system_read_memory_id=7,
  ble_cmd_system_get_info_id=8,
  ble_cmd_system_endpoint_tx_id=9,
  ble_cmd_system_whitelist_append_id=10,
  ble_cmd_system_whitelist_remove_id=11,
  ble_cmd_system_whitelist_clear_id=12,
  ble_cmd_system_endpoint_rx_id=13,
  ble_cmd_system_endpoint_set_watermarks_id=14,
  ble_cmd_system_aes_setkey_id=15,
  ble_cmd_system_aes_encrypt_id=16,
  ble_cmd_system_aes_decrypt_id=17,
  ble_cmd_flash_ps_defrag_id=0,
  ble_cmd_flash_ps_dump_id=1,
  ble_cmd_flash_ps_erase_all_id=2,
  ble_cmd_flash_ps_save_id=3,
  ble_cmd_flash_ps_load_id=4,
  ble_cmd_flash_ps_erase_id=5,
  ble_cmd_flash_erase_page_id=6,
  ble_cmd_flash_write_data_id=7,
  ble_cmd_flash_read_data_id=8,
  ble_cmd_attributes_write_id=0,
  ble_cmd_attributes_read_id=1,
  ble_cmd_attributes_read_type_id=2,
  ble_cmd_attributes_user_read_response_id=3,
  ble_cmd_attributes_user_write_response_id=4,
  ble_cmd_attributes_send_id=5,
  ble_cmd_connection_disconnect_id=0,
  ble_cmd_connection_get_rssi_id=1,
  ble_cmd_connection_update_id=2,
  ble_cmd_connection_version_update_id=3,
  ble_cmd_connection_channel_map_get_id=4,
  ble_cmd_connection_channel_map_set_id=5,
  ble_cmd_connection_features_get_id=6,
  ble_cmd_connection_get_status_id=7,
  ble_cmd_connection_raw_tx_id=8,
  ble_cmd_attclient_find_by_type_value_id=0,
  ble_cmd_attclient_read_by_group_type_id=1,
  ble_cmd_attclient_read_by_type_id=2,
  ble_cmd_attclient_find_information_id=3,
  ble_cmd_attclient_read_by_handle_id=4,
  ble_cmd_attclient_attribute_write_id=5,
  ble_cmd_attclient_write_command_id=6,
  ble_cmd_attclient_indicate_confirm_id=7,
  ble_cmd_attclient_read_long_id=8,
  ble_cmd_attclient_prepare_write_id=9,
  ble_cmd_attclient_execute_write_id=10,
  ble_cmd_attclient_read_multiple_id=11,
  ble_cmd_sm_encrypt_start_id=0,
  ble_cmd_sm_set_bondable_mode_id=1,
  ble_cmd_sm_delete_bonding_id=2,
  ble_cmd_sm_set_parameters_id=3,
  ble_cmd_sm_passkey_entry_id=4,
  ble_cmd_sm_get_bonds_id=5,
  ble_cmd_sm_set_oob_data_id=6,
  ble_cmd_sm_whitelist_bonds_id=7,
  ble_cmd_gap_set_privacy_flags_id=0,
  ble_cmd_gap_set_mode_id=1,
  ble_cmd_gap_discover_id=2,
  ble_cmd_gap_connect_direct_id=3,
  ble_cmd_gap_end_procedure_id=4,
  ble_cmd_gap_connect_selective_id=5,
  ble_cmd_gap_set_filtering_id=6,
  ble_cmd_gap_set_scan_parameters_id=7,
  ble_cmd_gap_set_adv_parameters_id=8,
  ble_cmd_gap_set_adv_data_id=9,
  ble_cmd_gap_set_directed_connectable_mode_id=10,
  ble_cmd_hardware_io_port_config_irq_id=0,
  ble_cmd_hardware_set_soft_timer_id=1,
  ble_cmd_hardware_adc_read_id=2,
  ble_cmd_hardware_io_port_config_direction_id=3,
  ble_cmd_hardware_io_port_config_function_id=4,
  ble_cmd_hardware_io_port_config_pull_id=5,
  ble_cmd_hardware_io_port_write_id=6,
  ble_cmd_hardware_io_port_read_id=7,
  ble_cmd_hardware_spi_config_id=8,
  ble_cmd_hardware_spi_transfer_id=9,
  ble_cmd_hardware_i2c_read_id=10,
  ble_cmd_hardware_i2c_write_id=11,
  ble_cmd_hardware_set_txpower_id=12,
  ble_cmd_hardware_timer_comparator_id=13,
  ble_cmd_hardware_io_port_irq_enable_id=14,
  ble_cmd_hardware_io_port_irq_direction_id=15,
  ble_cmd_hardware_analog_comparator_enable_id=16,
  ble_cmd_hardware_analog_comparator_read_id=17,
  ble_cmd_hardware_analog_comparator_config_irq_id=18,
  ble_cmd_hardware_set_rxgain_id=19,
  ble_cmd_hardware_usb_enable_id=20,
  ble_cmd_test_phy_tx_id=0,
  ble_cmd_test_phy_rx_id=1,
  ble_cmd_test_phy_end_id=2,
  ble_cmd_test_phy_reset_id=3,
  ble_cmd_test_get_channel_map_id=4,
  ble_cmd_test_debug_id=5,
  ble_cmd_test_channel_mode_id=6,
  ble_cmd_dfu_reset_id=0,
  ble_cmd_dfu_flash_set_address_id=1,
  ble_cmd_dfu_flash_upload_id=2,
  ble_cmd_dfu_flash_upload_finish_id=3
};
enum ble_response_ids
{
  ble_rsp_system_reset_id=0,
  ble_rsp_system_hello_id=1,
  ble_rsp_system_address_get_id=2,
  ble_rsp_system_reg_write_id=3,
  ble_rsp_system_reg_read_id=4,
  ble_rsp_system_get_counters_id=5,
  ble_rsp_system_get_connections_id=6,
  ble_rsp_system_read_memory_id=7,
  ble_rsp_system_get_info_id=8,
  ble_rsp_system_endpoint_tx_id=9,
  ble_rsp_system_whitelist_append_id=10,
  ble_rsp_system_whitelist_remove_id=11,
  ble_rsp_system_whitelist_clear_id=12,
  ble_rsp_system_endpoint_rx_id=13,
  ble_rsp_system_endpoint_set_watermarks_id=14,
  ble_rsp_system_aes_setkey_id=15,
  ble_rsp_system_aes_encrypt_id=16,
  ble_rsp_system_aes_decrypt_id=17,
  ble_rsp_flash_ps_defrag_id=0,
  ble_rsp_flash_ps_dump_id=1,
  ble_rsp_flash_ps_erase_all_id=2,
  ble_rsp_flash_ps_save_id=3,
  ble_rsp_flash_ps_load_id=4,
  ble_rsp_flash_ps_erase_id=5,
  ble_rsp_flash_erase_page_id=6,
  ble_rsp_flash_write_data_id=7,
  ble_rsp_flash_read_data_id=8,
  ble_rsp_attributes_write_id=0,
  ble_rsp_attributes_read_id=1,
  ble_rsp_attributes_read_type_id=2,
  ble_rsp_attributes_user_read_response_id=3,
  ble_rsp_attributes_user_write_response_id=4,
  ble_rsp_attributes_send_id=5,
  ble_rsp_connection_disconnect_id=0,
  ble_rsp_connection_get_rssi_id=1,
  ble_rsp_connection_update_id=2,
  ble_rsp_connection_version_update_id=3,
  ble_rsp_connection_channel_map_get_id=4,
  ble_rsp_connection_channel_map_set_id=5,
  ble_rsp_connection_features_get_id=6,
  ble_rsp_connection_get_status_id=7,
  ble_rsp_connection_raw_tx_id=8,
  ble_rsp_attclient_find_by_type_value_id=0,
  ble_rsp_attclient_read_by_group_type_id=1,
  ble_rsp_attclient_read_by_type_id=2,
  ble_rsp_attclient_find_information_id=3,
  ble_rsp_attclient_read_by_handle_id=4,
  ble_rsp_attclient_attribute_write_id=5,
  ble_rsp_attclient_write_command_id=6,
  ble_rsp_attclient_indicate_confirm_id=7,
  ble_rsp_attclient_read_long_id=8,
  ble_rsp_attclient_prepare_write_id=9,
  ble_rsp_attclient_execute_write_id=10,
  ble_rsp_attclient_read_multiple_id=11,
  ble_rsp_sm_encrypt_start_id=0,
  ble_rsp_sm_set_bondable_mode_id=1,
  ble_rsp_sm_delete_bonding_id=2,
  ble_rsp_sm_set_parameters_id=3,
  ble_rsp_sm_passkey_entry_id=4,
  ble_rsp_sm_get_bonds_id=5,
  ble_rsp_sm_set_oob_data_id=6,
  ble_rsp_sm_whitelist_bonds_id=7,
  ble_rsp_gap_set_privacy_flags_id=0,
  ble_rsp_gap_set_mode_id=1,
  ble_rsp_gap_discover_id=2,
  ble_rsp_gap_connect_direct_id=3,
  ble_rsp_gap_end_procedure_id=4,
  ble_rsp_gap_connect_selective_id=5,
  ble_rsp_gap_set_filtering_id=6,
  ble_rsp_gap_set_scan_parameters_id=7,
  ble_rsp_gap_set_adv_parameters_id=8,
  ble_rsp_gap_set_adv_data_id=9,
  ble_rsp_gap_set_directed_connectable_mode_id=10,
  ble_rsp_hardware_io_port_config_irq_id=0,
  ble_rsp_hardware_set_soft_timer_id=1,
  ble_rsp_hardware_adc_read_id=2,
  ble_rsp_hardware_io_port_config_direction_id=3,
  ble_rsp_hardware_io_port_config_function_id=4,
  ble_rsp_hardware_io_port_config_pull_id=5,
  ble_rsp_hardware_io_port_write_id=6,
  ble_rsp_hardware_io_port_read_id=7,
  ble_rsp_hardware_spi_config_id=8,
  ble_rsp_hardware_spi_transfer_id=9,
  ble_rsp_hardware_i2c_read_id=10,
  ble_rsp_hardware_i2c_write_id=11,
  ble_rsp_hardware_set_txpower_id=12,
  ble_rsp_hardware_timer_comparator_id=13,
  ble_rsp_hardware_io_port_irq_enable_id=14,
  ble_rsp_hardware_io_port_irq_direction_id=15,
  ble_rsp_hardware_analog_comparator_enable_id=16,
  ble_rsp_hardware_analog_comparator_read_id=17,
  ble_rsp_hardware_analog_comparator_config_irq_id=18,
  ble_rsp_hardware_set_rxgain_id=19,
  ble_rsp_hardware_usb_enable_id=20,
  ble_rsp_test_phy_tx_id=0,
  ble_rsp_test_phy_rx_id=1,
  ble_rsp_test_phy_end_id=2,
  ble_rsp_test_phy_reset_id=3,
  ble_rsp_test_get_channel_map_id=4,
  ble_rsp_test_debug_id=5,
  ble_rsp_test_channel_mode_id=6,
  ble_rsp_dfu_reset_id=0,
  ble_rsp_dfu_flash_set_address_id=1,
  ble_rsp_dfu_flash_upload_id=2,
  ble_rsp_dfu_flash_upload_finish_id=3
};
enum ble_event_ids
{
  ble_evt_system_boot_id=0,
  ble_evt_system_debug_id=1,
  ble_evt_system_endpoint_watermark_rx_id=2,
  ble_evt_system_endpoint_watermark_tx_id=3,
  ble_evt_system_script_failure_id=4,
  ble_evt_system_no_license_key_id=5,
  ble_evt_system_protocol_error_id=6,
  ble_evt_flash_ps_key_id=0,
  ble_evt_attributes_value_id=0,
  ble_evt_attributes_user_read_request_id=1,
  ble_evt_attributes_status_id=2,
  ble_evt_connection_status_id=0,
  ble_evt_connection_version_ind_id=1,
  ble_evt_connection_feature_ind_id=2,
  ble_evt_connection_raw_rx_id=3,
  ble_evt_connection_disconnected_id=4,
  ble_evt_attclient_indicated_id=0,
  ble_evt_attclient_procedure_completed_id=1,
  ble_evt_attclient_group_found_id=2,
  ble_evt_attclient_attribute_found_id=3,
  ble_evt_attclient_find_information_found_id=4,
  ble_evt_attclient_attribute_value_id=5,
  ble_evt_attclient_read_multiple_response_id=6,
  ble_evt_sm_smp_data_id=0,
  ble_evt_sm_bonding_fail_id=1,
  ble_evt_sm_passkey_display_id=2,
  ble_evt_sm_passkey_request_id=3,
  ble_evt_sm_bond_status_id=4,
  ble_evt_gap_scan_response_id=0,
  ble_evt_gap_mode_changed_id=1,
  ble_evt_hardware_io_port_status_id=0,
  ble_evt_hardware_soft_timer_id=1,
  ble_evt_hardware_adc_result_id=2,
  ble_evt_hardware_analog_comparator_status_id=3,
  ble_evt_dfu_boot_id=0
};

enum ble_msg_idx
{
  ble_cmd_system_reset_idx =0,
  ble_cmd_system_hello_idx =1,
  ble_cmd_system_address_get_idx =2,
  ble_cmd_system_reg_write_idx =3,
  ble_cmd_system_reg_read_idx =4,
  ble_cmd_system_get_counters_idx =5,
  ble_cmd_system_get_connections_idx =6,
  ble_cmd_system_read_memory_idx =7,
  ble_cmd_system_get_info_idx =8,
  ble_cmd_system_endpoint_tx_idx =9,
  ble_cmd_system_whitelist_append_idx =10,
  ble_cmd_system_whitelist_remove_idx =11,
  ble_cmd_system_whitelist_clear_idx =12,
  ble_cmd_system_endpoint_rx_idx =13,
  ble_cmd_system_endpoint_set_watermarks_idx =14,
  ble_cmd_system_aes_setkey_idx =15,
  ble_cmd_system_aes_encrypt_idx =16,
  ble_cmd_system_aes_decrypt_idx =17,
  ble_cmd_flash_ps_defrag_idx =18,
  ble_cmd_flash_ps_dump_idx =19,
  ble_cmd_flash_ps_erase_all_idx =20,
  ble_cmd_flash_ps_save_idx =21,
  ble_cmd_flash_ps_load_idx =22,
  ble_cmd_flash_ps_erase_idx =23,
  ble_cmd_flash_erase_page_idx =24,
  ble_cmd_flash_write_data_idx =25,
  ble_cmd_flash_read_data_idx =26,
  ble_cmd_attributes_write_idx =27,
  ble_cmd_attributes_read_idx =28,
  ble_cmd_attributes_read_type_idx =29,
  ble_cmd_attributes_user_read_response_idx =30,
  ble_cmd_attributes_user_write_response_idx =31,
  ble_cmd_attributes_send_idx =32,
  ble_cmd_connection_disconnect_idx =33,
  ble_cmd_connection_get_rssi_idx =34,
  ble_cmd_connection_update_idx =35,
  ble_cmd_connection_version_update_idx =36,
  ble_cmd_connection_channel_map_get_idx =37,
  ble_cmd_connection_channel_map_set_idx =38,
  ble_cmd_connection_features_get_idx =39,
  ble_cmd_connection_get_status_idx =40,
  ble_cmd_connection_raw_tx_idx =41,
  ble_cmd_attclient_find_by_type_value_idx =42,
  ble_cmd_attclient_read_by_group_type_idx =43,
  ble_cmd_attclient_read_by_type_idx =44,
  ble_cmd_attclient_find_information_idx =45,
  ble_cmd_attclient_read_by_handle_idx =46,
  ble_cmd_attclient_attribute_write_idx =47,
  ble_cmd_attclient_write_command_idx =48,
  ble_cmd_attclient_indicate_confirm_idx =49,
  ble_cmd_attclient_read_long_idx =50,
  ble_cmd_attclient_prepare_write_idx =51,
  ble_cmd_attclient_execute_write_idx =52,
  ble_cmd_attclient_read_multiple_idx =53,
  ble_cmd_sm_encrypt_start_idx =54,
  ble_cmd_sm_set_bondable_mode_idx =55,
  ble_cmd_sm_delete_bonding_idx =56,
  ble_cmd_sm_set_parameters_idx =57,
  ble_cmd_sm_passkey_entry_idx =58,
  ble_cmd_sm_get_bonds_idx =59,
  ble_cmd_sm_set_oob_data_idx =60,
  ble_cmd_sm_whitelist_bonds_idx =61,
  ble_cmd_gap_set_privacy_flags_idx =62,
  ble_cmd_gap_set_mode_idx =63,
  ble_cmd_gap_discover_idx =64,
  ble_cmd_gap_connect_direct_idx =65,
  ble_cmd_gap_end_procedure_idx =66,
  ble_cmd_gap_connect_selective_idx =67,
  ble_cmd_gap_set_filtering_idx =68,
  ble_cmd_gap_set_scan_parameters_idx =69,
  ble_cmd_gap_set_adv_parameters_idx =70,
  ble_cmd_gap_set_adv_data_idx =71,
  ble_cmd_gap_set_directed_connectable_mode_idx =72,
  ble_cmd_hardware_io_port_config_irq_idx =73,
  ble_cmd_hardware_set_soft_timer_idx =74,
  ble_cmd_hardware_adc_read_idx =75,
  ble_cmd_hardware_io_port_config_direction_idx =76,
  ble_cmd_hardware_io_port_config_function_idx =77,
  ble_cmd_hardware_io_port_config_pull_idx =78,
  ble_cmd_hardware_io_port_write_idx =79,
  ble_cmd_hardware_io_port_read_idx =80,
  ble_cmd_hardware_spi_config_idx =81,
  ble_cmd_hardware_spi_transfer_idx =82,
  ble_cmd_hardware_i2c_read_idx =83,
  ble_cmd_hardware_i2c_write_idx =84,
  ble_cmd_hardware_set_txpower_idx =85,
  ble_cmd_hardware_timer_comparator_idx =86,
  ble_cmd_hardware_io_port_irq_enable_idx =87,
  ble_cmd_hardware_io_port_irq_direction_idx =88,
  ble_cmd_hardware_analog_comparator_enable_idx =89,
  ble_cmd_hardware_analog_comparator_read_idx =90,
  ble_cmd_hardware_analog_comparator_config_irq_idx =91,
  ble_cmd_hardware_set_rxgain_idx =92,
  ble_cmd_hardware_usb_enable_idx =93,
  ble_cmd_test_phy_tx_idx =94,
  ble_cmd_test_phy_rx_idx =95,
  ble_cmd_test_phy_end_idx =96,
  ble_cmd_test_phy_reset_idx =97,
  ble_cmd_test_get_channel_map_idx =98,
  ble_cmd_test_debug_idx =99,
  ble_cmd_test_channel_mode_idx =100,
  ble_cmd_dfu_reset_idx =101,
  ble_cmd_dfu_flash_set_address_idx =102,
  ble_cmd_dfu_flash_upload_idx =103,
  ble_cmd_dfu_flash_upload_finish_idx =104,
  ble_rsp_system_reset_idx =105,
  ble_rsp_system_hello_idx =106,
  ble_rsp_system_address_get_idx =107,
  ble_rsp_system_reg_write_idx =108,
  ble_rsp_system_reg_read_idx =109,
  ble_rsp_system_get_counters_idx =110,
  ble_rsp_system_get_connections_idx =111,
  ble_rsp_system_read_memory_idx =112,
  ble_rsp_system_get_info_idx =113,
  ble_rsp_system_endpoint_tx_idx =114,
  ble_rsp_system_whitelist_append_idx =115,
  ble_rsp_system_whitelist_remove_idx =116,
  ble_rsp_system_whitelist_clear_idx =117,
  ble_rsp_system_endpoint_rx_idx =118,
  ble_rsp_system_endpoint_set_watermarks_idx =119,
  ble_rsp_system_aes_setkey_idx =120,
  ble_rsp_system_aes_encrypt_idx =121,
  ble_rsp_system_aes_decrypt_idx =122,
  ble_rsp_flash_ps_defrag_idx =123,
  ble_rsp_flash_ps_dump_idx =124,
  ble_rsp_flash_ps_erase_all_idx =125,
  ble_rsp_flash_ps_save_idx =126,
  ble_rsp_flash_ps_load_idx =127,
  ble_rsp_flash_ps_erase_idx =128,
  ble_rsp_flash_erase_page_idx =129,
  ble_rsp_flash_write_data_idx =130,
  ble_rsp_flash_read_data_idx =131,
  ble_rsp_attributes_write_idx =132,
  ble_rsp_attributes_read_idx =133,
  ble_rsp_attributes_read_type_idx =134,
  ble_rsp_attributes_user_read_response_idx =135,
  ble_rsp_attributes_user_write_response_idx =136,
  ble_rsp_attributes_send_idx =137,
  ble_rsp_connection_disconnect_idx =138,
  ble_rsp_connection_get_rssi_idx =139,
  ble_rsp_connection_update_idx =140,
  ble_rsp_connection_version_update_idx =141,
  ble_rsp_connection_channel_map_get_idx =142,
  ble_rsp_connection_channel_map_set_idx =143,
  ble_rsp_connection_features_get_idx =144,
  ble_rsp_connection_get_status_idx =145,
  ble_rsp_connection_raw_tx_idx =146,
  ble_rsp_attclient_find_by_type_value_idx =147,
  ble_rsp_attclient_read_by_group_type_idx =148,
  ble_rsp_attclient_read_by_type_idx =149,
  ble_rsp_attclient_find_information_idx =150,
  ble_rsp_attclient_read_by_handle_idx =151,
  ble_rsp_attclient_attribute_write_idx =152,
  ble_rsp_attclient_write_command_idx =153,
  ble_rsp_attclient_indicate_confirm_idx =154,
  ble_rsp_attclient_read_long_idx =155,
  ble_rsp_attclient_prepare_write_idx =156,
  ble_rsp_attclient_execute_write_idx =157,
  ble_rsp_attclient_read_multiple_idx =158,
  ble_rsp_sm_encrypt_start_idx =159,
  ble_rsp_sm_set_bondable_mode_idx =160,
  ble_rsp_sm_delete_bonding_idx =161,
  ble_rsp_sm_set_parameters_idx =162,
  ble_rsp_sm_passkey_entry_idx =163,
  ble_rsp_sm_get_bonds_idx =164,
  ble_rsp_sm_set_oob_data_idx =165,
  ble_rsp_sm_whitelist_bonds_idx =166,
  ble_rsp_gap_set_privacy_flags_idx =167,
  ble_rsp_gap_set_mode_idx =168,
  ble_rsp_gap_discover_idx =169,
  ble_rsp_gap_connect_direct_idx =170,
  ble_rsp_gap_end_procedure_idx =171,
  ble_rsp_gap_connect_selective_idx =172,
  ble_rsp_gap_set_filtering_idx =173,
  ble_rsp_gap_set_scan_parameters_idx =174,
  ble_rsp_gap_set_adv_parameters_idx =175,
  ble_rsp_gap_set_adv_data_idx =176,
  ble_rsp_gap_set_directed_connectable_mode_idx =177,
  ble_rsp_hardware_io_port_config_irq_idx =178,
  ble_rsp_hardware_set_soft_timer_idx =179,
  ble_rsp_hardware_adc_read_idx =180,
  ble_rsp_hardware_io_port_config_direction_idx =181,
  ble_rsp_hardware_io_port_config_function_idx =182,
  ble_rsp_hardware_io_port_config_pull_idx =183,
  ble_rsp_hardware_io_port_write_idx =184,
  ble_rsp_hardware_io_port_read_idx =185,
  ble_rsp_hardware_spi_config_idx =186,
  ble_rsp_hardware_spi_transfer_idx =187,
  ble_rsp_hardware_i2c_read_idx =188,
  ble_rsp_hardware_i2c_write_idx =189,
  ble_rsp_hardware_set_txpower_idx =190,
  ble_rsp_hardware_timer_comparator_idx =191,
  ble_rsp_hardware_io_port_irq_enable_idx =192,
  ble_rsp_hardware_io_port_irq_direction_idx =193,
  ble_rsp_hardware_analog_comparator_enable_idx =194,
  ble_rsp_hardware_analog_comparator_read_idx =195,
  ble_rsp_hardware_analog_comparator_config_irq_idx =196,
  ble_rsp_hardware_set_rxgain_idx =197,
  ble_rsp_hardware_usb_enable_idx =198,
  ble_rsp_test_phy_tx_idx =199,
  ble_rsp_test_phy_rx_idx =200,
  ble_rsp_test_phy_end_idx =201,
  ble_rsp_test_phy_reset_idx =202,
  ble_rsp_test_get_channel_map_idx =203,
  ble_rsp_test_debug_idx =204,
  ble_rsp_test_channel_mode_idx =205,
  ble_rsp_dfu_reset_idx =206,
  ble_rsp_dfu_flash_set_address_idx =207,
  ble_rsp_dfu_flash_upload_idx =208,
  ble_rsp_dfu_flash_upload_finish_idx =209,
  ble_evt_system_boot_idx =210,
  ble_evt_system_debug_idx =211,
  ble_evt_system_endpoint_watermark_rx_idx =212,
  ble_evt_system_endpoint_watermark_tx_idx =213,
  ble_evt_system_script_failure_idx =214,
  ble_evt_system_no_license_key_idx =215,
  ble_evt_system_protocol_error_idx =216,
  ble_evt_flash_ps_key_idx =217,
  ble_evt_attributes_value_idx =218,
  ble_evt_attributes_user_read_request_idx =219,
  ble_evt_attributes_status_idx =220,
  ble_evt_connection_status_idx =221,
  ble_evt_connection_version_ind_idx =222,
  ble_evt_connection_feature_ind_idx =223,
  ble_evt_connection_raw_rx_idx =224,
  ble_evt_connection_disconnected_idx =225,
  ble_evt_attclient_indicated_idx =226,
  ble_evt_attclient_procedure_completed_idx =227,
  ble_evt_attclient_group_found_idx =228,
  ble_evt_attclient_attribute_found_idx =229,
  ble_evt_attclient_find_information_found_idx =230,
  ble_evt_attclient_attribute_value_idx =231,
  ble_evt_attclient_read_multiple_response_idx =232,
  ble_evt_sm_smp_data_idx =233,
  ble_evt_sm_bonding_fail_idx =234,
  ble_evt_sm_passkey_display_idx =235,
  ble_evt_sm_passkey_request_idx =236,
  ble_evt_sm_bond_status_idx =237,
  ble_evt_gap_scan_response_idx =238,
  ble_evt_gap_mode_changed_idx =239,
  ble_evt_hardware_io_port_status_idx =240,
  ble_evt_hardware_soft_timer_idx =241,
  ble_evt_hardware_adc_result_idx =242,
  ble_evt_hardware_analog_comparator_status_idx =243,
  ble_evt_dfu_boot_idx =244
};
#ifndef BG_ERRORCODES
#define BG_ERRORCODES
enum ble_error_spaces
{
  ble_errspc_bt=512,
  ble_errspc_att=1024,
  ble_errspc_bg=256,
  ble_errspc_smp=768,
};
typedef enum ble_error
{
  ble_err_bt_error_success                                    =ble_errspc_bt+0,   //Command completed succesfully
  ble_err_bt_bt_error_authentication_failure                  =ble_errspc_bt+5,   //Pairing or authentication failed due to incorrect results in the pairing or authentication procedure. This could be due to an incorrect PIN or Link Key
  ble_err_bt_pin_or_key_missing                               =ble_errspc_bt+6,   //Pairing failed because of missing PIN, or authentication failed because of missing Key
  ble_err_bt_bt_error_memory_capacity_exceeded                =ble_errspc_bt+7,   //Controller is out of memory.
  ble_err_bt_connection_timeout                               =ble_errspc_bt+8,   //Link supervision timeout has expired.
  ble_err_bt_connection_limit_exceeded                        =ble_errspc_bt+9,   //Controller is at limit of connections it can support.
  ble_err_bt_bt_error_command_disallowed                      =ble_errspc_bt+12,  //Command requested cannot be executed because the Controller is in a state where it cannot process this command at this time.
  ble_err_bt_bt_error_invalid_command_parameters              =ble_errspc_bt+18,  //Command contained invalid parameters.
  ble_err_bt_bt_error_remote_user_terminated                  =ble_errspc_bt+19,  //User on the remote device terminated the connection.
  ble_err_bt_bt_error_connection_terminated_by_local_host     =ble_errspc_bt+22,  //Local device terminated the connection.
  ble_err_bt_bt_error_ll_response_timeout                     =ble_errspc_bt+34,  //Connection terminated due to link-layer procedure timeout.
  ble_err_bt_bt_error_ll_instant_passed                       =ble_errspc_bt+40,  //Received link-layer control packet where instant was in the past.
  ble_err_bt_bt_error_controller_busy                         =ble_errspc_bt+58,  //Operation was rejected because the controller is busy and unable to process the request.
  ble_err_bt_bt_error_unacceptable_connection_interval        =ble_errspc_bt+59,  //Remote evice terminated the connection because of an unacceptable connection interval.
  ble_err_bt_bt_error_directed_advertising_timeout            =ble_errspc_bt+60,  //Directed advertising completed without a connection being created.
  ble_err_bt_bt_error_connection_terminated_due_to_mic_failure=ble_errspc_bt+61,  //Connection was terminated because the Message Integrity Check (MIC) failed on a received packet.
  ble_err_bt_bt_error_connection_failed_to_be_established     =ble_errspc_bt+62,  //LL initiated a connection but the connection has failed to be established. Controller did not receive any packets from remote end.
  ble_err_att_invalid_handle                                  =ble_errspc_att+1,  //The attribute handle given was not valid on this server
  ble_err_att_read_not_permitted                              =ble_errspc_att+2,  //The attribute cannot be read
  ble_err_att_write_not_permitted                             =ble_errspc_att+3,  //The attribute cannot be written
  ble_err_att_invalid_pdu                                     =ble_errspc_att+4,  //The attribute PDU was invalid
  ble_err_att_insufficient_authentication                     =ble_errspc_att+5,  //The attribute requires authentication before it can be read or written.
  ble_err_att_request_not_supported                           =ble_errspc_att+6,  //Attribute Server does not support the request received from the client.
  ble_err_att_invalid_offset                                  =ble_errspc_att+7,  //Offset specified was past the end of the attribute
  ble_err_att_insufficient_authorization                      =ble_errspc_att+8,  //The attribute requires authorization before it can be read or written.
  ble_err_att_prepare_queue_full                              =ble_errspc_att+9,  //Too many prepare writes have been queueud
  ble_err_att_att_not_found                                   =ble_errspc_att+10, //No attribute found within the given attribute handle range.
  ble_err_att_att_not_long                                    =ble_errspc_att+11, //The attribute cannot be read or written using the Read Blob Request
  ble_err_att_insufficient_enc_key_size                       =ble_errspc_att+12, //The Encryption Key Size used for encrypting this link is insufficient.
  ble_err_att_invalid_att_length                              =ble_errspc_att+13, //The attribute value length is invalid for the operation
  ble_err_att_unlikely_error                                  =ble_errspc_att+14, //The attribute request that was requested has encountered an error that was unlikely, and therefore could not be completed as requested.
  ble_err_att_insufficient_encryption                         =ble_errspc_att+15, //The attribute requires encryption before it can be read or written.
  ble_err_att_unsupported_group_type                          =ble_errspc_att+16, //The attribute type is not a supported grouping attribute as defined by a higher layer specification.
  ble_err_att_insufficient_resources                          =ble_errspc_att+17, //Insufficient Resources to complete the request
  ble_err_att_application                                     =ble_errspc_att+128,//Application error code defined by a higher layer specification.
  ble_err_success                                             =0,                 //No error
  ble_err_invalid_param                                       =ble_errspc_bg+128, //Command contained invalid parameter
  ble_err_wrong_state                                         =ble_errspc_bg+129, //Device is in wrong state to receive command
  ble_err_out_of_memory                                       =ble_errspc_bg+130, //Device has run out of memory
  ble_err_not_implemented                                     =ble_errspc_bg+131, //Feature is not implemented
  ble_err_invalid_command                                     =ble_errspc_bg+132, //Command was not recognized
  ble_err_timeout                                             =ble_errspc_bg+133, //Command or Procedure failed due to timeout
  ble_err_not_connected                                       =ble_errspc_bg+134, //Connection handle passed is to command is not a valid handle
  ble_err_flow                                                =ble_errspc_bg+135, //Command would cause either underflow or overflow error
  ble_err_user_attribute                                      =ble_errspc_bg+136, //User attribute was accessed through API which is not supported
  ble_err_invalid_license_key                                 =ble_errspc_bg+137, //No valid license key found
  ble_err_command_too_long                                    =ble_errspc_bg+138, //Command maximum length exceeded
  ble_err_out_of_bonds                                        =ble_errspc_bg+139, //Bonding procedure can't be started because device has no space left for bond.
  ble_err_smp_passkey_entry_failed                            =ble_errspc_smp+1,  //The user input of passkey failed, for example, the user cancelled the operation
  ble_err_smp_oob_not_available                               =ble_errspc_smp+2,  //Out of Band data is not available for authentication
  ble_err_smp_authentication_requirements                     =ble_errspc_smp+3,  //The pairing procedure cannot be performed as authentication requirements cannot be met due to IO capabilities of one or both devices
  ble_err_smp_confirm_value_failed                            =ble_errspc_smp+4,  //The confirm value does not match the calculated compare value
  ble_err_smp_pairing_not_supported                           =ble_errspc_smp+5,  //Pairing is not supported by the device
  ble_err_smp_encryption_key_size                             =ble_errspc_smp+6,  //The resultant encryption key size is insufficient for the security requirements of this device
  ble_err_smp_command_not_supported                           =ble_errspc_smp+7,  //The SMP command received is not supported on this device
  ble_err_smp_unspecified_reason                              =ble_errspc_smp+8,  //Pairing failed due to an unspecified reason
  ble_err_smp_repeated_attempts                               =ble_errspc_smp+9,  //Pairing or authentication procedure is disallowed because too little time has elapsed since last pairing request or security request
  ble_err_smp_invalid_parameters                              =ble_errspc_smp+10, //The Invalid Parameters error code indicates: the command length is invalid or a parameter is outside of the specified range.
  ble_err_last
}errorcode_t;
#endif
PACKSTRUCT(struct ble_msg_system_reset_cmd_t
{
  uint8 boot_in_dfu;
});

PACKSTRUCT(struct ble_msg_system_address_get_rsp_t
{
  bd_addr address;
});

PACKSTRUCT(struct ble_msg_system_reg_write_cmd_t
{
  uint16  address;
  uint8 value;
});

PACKSTRUCT(struct ble_msg_system_reg_write_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_system_reg_read_cmd_t
{
  uint16  address;
});

PACKSTRUCT(struct ble_msg_system_reg_read_rsp_t
{
  uint16  address;
  uint8 value;
});

PACKSTRUCT(struct ble_msg_system_get_counters_rsp_t
{
  uint8 txok;
  uint8 txretry;
  uint8 rxok;
  uint8 rxfail;
  uint8 mbuf;
});

PACKSTRUCT(struct ble_msg_system_get_connections_rsp_t
{
  uint8 maxconn;
});

PACKSTRUCT(struct ble_msg_system_read_memory_cmd_t
{
  uint32  address;
  uint8 length;
});

PACKSTRUCT(struct ble_msg_system_read_memory_rsp_t
{
  uint32  address;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_system_get_info_rsp_t
{
  uint16  major;
  uint16  minor;
  uint16  patch;
  uint16  build;
  uint16  ll_version;
  uint8 protocol_version;
  uint8 hw;
});

PACKSTRUCT(struct ble_msg_system_endpoint_tx_cmd_t
{
  uint8 endpoint;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_system_endpoint_tx_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_system_whitelist_append_cmd_t
{
  bd_addr address;
  uint8 address_type;
});

PACKSTRUCT(struct ble_msg_system_whitelist_append_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_system_whitelist_remove_cmd_t
{
  bd_addr address;
  uint8 address_type;
});

PACKSTRUCT(struct ble_msg_system_whitelist_remove_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_system_endpoint_rx_cmd_t
{
  uint8 endpoint;
  uint8 size;
});

PACKSTRUCT(struct ble_msg_system_endpoint_rx_rsp_t
{
  uint16  result;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_system_endpoint_set_watermarks_cmd_t
{
  uint8 endpoint;
  uint8 rx;
  uint8 tx;
});

PACKSTRUCT(struct ble_msg_system_endpoint_set_watermarks_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_system_aes_setkey_cmd_t
{
  uint8array  key;
});

PACKSTRUCT(struct ble_msg_system_aes_encrypt_cmd_t
{
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_system_aes_encrypt_rsp_t
{
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_system_aes_decrypt_cmd_t
{
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_system_aes_decrypt_rsp_t
{
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_system_boot_evt_t
{
  uint16  major;
  uint16  minor;
  uint16  patch;
  uint16  build;
  uint16  ll_version;
  uint8 protocol_version;
  uint8 hw;
});

PACKSTRUCT(struct ble_msg_system_debug_evt_t
{
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_system_endpoint_watermark_rx_evt_t
{
  uint8 endpoint;
  uint8 data;
});

PACKSTRUCT(struct ble_msg_system_endpoint_watermark_tx_evt_t
{
  uint8 endpoint;
  uint8 data;
});

PACKSTRUCT(struct ble_msg_system_script_failure_evt_t
{
  uint16  address;
  uint16  reason;
});

PACKSTRUCT(struct ble_msg_system_protocol_error_evt_t
{
  uint16  reason;
});

PACKSTRUCT(struct ble_msg_flash_ps_save_cmd_t
{
  uint16  key;
  uint8array  value;
});

PACKSTRUCT(struct ble_msg_flash_ps_save_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_flash_ps_load_cmd_t
{
  uint16  key;
});

PACKSTRUCT(struct ble_msg_flash_ps_load_rsp_t
{
  uint16  result;
  uint8array  value;
});

PACKSTRUCT(struct ble_msg_flash_ps_erase_cmd_t
{
  uint16  key;
});

PACKSTRUCT(struct ble_msg_flash_erase_page_cmd_t
{
  uint8 page;
});

PACKSTRUCT(struct ble_msg_flash_erase_page_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_flash_write_data_cmd_t
{
  uint32  address;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_flash_write_data_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_flash_read_data_cmd_t
{
  uint32  address;
  uint8 length;
});

PACKSTRUCT(struct ble_msg_flash_read_data_rsp_t
{
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_flash_ps_key_evt_t
{
  uint16  key;
  uint8array  value;
});

PACKSTRUCT(struct ble_msg_attributes_write_cmd_t
{
  uint16  handle;
  uint8 offset;
  uint8array  value;
});

PACKSTRUCT(struct ble_msg_attributes_write_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_attributes_read_cmd_t
{
  uint16  handle;
  uint16  offset;
});

PACKSTRUCT(struct ble_msg_attributes_read_rsp_t
{
  uint16  handle;
  uint16  offset;
  uint16  result;
  uint8array  value;
});

PACKSTRUCT(struct ble_msg_attributes_read_type_cmd_t
{
  uint16  handle;
});

PACKSTRUCT(struct ble_msg_attributes_read_type_rsp_t
{
  uint16  handle;
  uint16  result;
  uint8array  value;
});

PACKSTRUCT(struct ble_msg_attributes_user_read_response_cmd_t
{
  uint8 connection;
  uint8 att_error;
  uint8array  value;
});

PACKSTRUCT(struct ble_msg_attributes_user_write_response_cmd_t
{
  uint8 connection;
  uint8 att_error;
});

PACKSTRUCT(struct ble_msg_attributes_send_cmd_t
{
  uint8 connection;
  uint16  handle;
  uint8array  value;
});

PACKSTRUCT(struct ble_msg_attributes_send_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_attributes_value_evt_t
{
  uint8 connection;
  uint8 reason;
  uint16  handle;
  uint16  offset;
  uint8array  value;
});

PACKSTRUCT(struct ble_msg_attributes_user_read_request_evt_t
{
  uint8 connection;
  uint16  handle;
  uint16  offset;
  uint8 maxsize;
});

PACKSTRUCT(struct ble_msg_attributes_status_evt_t
{
  uint16  handle;
  uint8 flags;
});

PACKSTRUCT(struct ble_msg_connection_disconnect_cmd_t
{
  uint8 connection;
});

PACKSTRUCT(struct ble_msg_connection_disconnect_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_connection_get_rssi_cmd_t
{
  uint8 connection;
});

PACKSTRUCT(struct ble_msg_connection_get_rssi_rsp_t
{
  uint8 connection;
  int8  rssi;
});

PACKSTRUCT(struct ble_msg_connection_update_cmd_t
{
  uint8 connection;
  uint16  interval_min;
  uint16  interval_max;
  uint16  latency;
  uint16  timeout;
});

PACKSTRUCT(struct ble_msg_connection_update_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_connection_version_update_cmd_t
{
  uint8 connection;
});

PACKSTRUCT(struct ble_msg_connection_version_update_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_connection_channel_map_get_cmd_t
{
  uint8 connection;
});

PACKSTRUCT(struct ble_msg_connection_channel_map_get_rsp_t
{
  uint8 connection;
  uint8array  map;
});

PACKSTRUCT(struct ble_msg_connection_channel_map_set_cmd_t
{
  uint8 connection;
  uint8array  map;
});

PACKSTRUCT(struct ble_msg_connection_channel_map_set_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_connection_features_get_cmd_t
{
  uint8 connection;
});

PACKSTRUCT(struct ble_msg_connection_features_get_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_connection_get_status_cmd_t
{
  uint8 connection;
});

PACKSTRUCT(struct ble_msg_connection_get_status_rsp_t
{
  uint8 connection;
});

PACKSTRUCT(struct ble_msg_connection_raw_tx_cmd_t
{
  uint8 connection;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_connection_raw_tx_rsp_t
{
  uint8 connection;
});

PACKSTRUCT(struct ble_msg_connection_status_evt_t
{
  uint8 connection;
  uint8 flags;
  bd_addr address;
  uint8 address_type;
  uint16  conn_interval;
  uint16  timeout;
  uint16  latency;
  uint8 bonding;
});

PACKSTRUCT(struct ble_msg_connection_version_ind_evt_t
{
  uint8 connection;
  uint8 vers_nr;
  uint16  comp_id;
  uint16  sub_vers_nr;
});

PACKSTRUCT(struct ble_msg_connection_feature_ind_evt_t
{
  uint8 connection;
  uint8array  features;
});

PACKSTRUCT(struct ble_msg_connection_raw_rx_evt_t
{
  uint8 connection;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_connection_disconnected_evt_t
{
  uint8 connection;
  uint16  reason;
});

PACKSTRUCT(struct ble_msg_attclient_find_by_type_value_cmd_t
{
  uint8 connection;
  uint16  start;
  uint16  end;
  uint16  uuid;
  uint8array  value;
});

PACKSTRUCT(struct ble_msg_attclient_find_by_type_value_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_attclient_read_by_group_type_cmd_t
{
  uint8 connection;
  uint16  start;
  uint16  end;
  uint8array  uuid;
});

PACKSTRUCT(struct ble_msg_attclient_read_by_group_type_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_attclient_read_by_type_cmd_t
{
  uint8 connection;
  uint16  start;
  uint16  end;
  uint8array  uuid;
});

PACKSTRUCT(struct ble_msg_attclient_read_by_type_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_attclient_find_information_cmd_t
{
  uint8 connection;
  uint16  start;
  uint16  end;
});

PACKSTRUCT(struct ble_msg_attclient_find_information_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_attclient_read_by_handle_cmd_t
{
  uint8 connection;
  uint16  chrhandle;
});

PACKSTRUCT(struct ble_msg_attclient_read_by_handle_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_attclient_attribute_write_cmd_t
{
  uint8 connection;
  uint16  atthandle;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_attclient_attribute_write_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_attclient_write_command_cmd_t
{
  uint8 connection;
  uint16  atthandle;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_attclient_write_command_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_attclient_indicate_confirm_cmd_t
{
  uint8 connection;
});

PACKSTRUCT(struct ble_msg_attclient_indicate_confirm_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_attclient_read_long_cmd_t
{
  uint8 connection;
  uint16  chrhandle;
});

PACKSTRUCT(struct ble_msg_attclient_read_long_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_attclient_prepare_write_cmd_t
{
  uint8 connection;
  uint16  atthandle;
  uint16  offset;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_attclient_prepare_write_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_attclient_execute_write_cmd_t
{
  uint8 connection;
  uint8 commit;
});

PACKSTRUCT(struct ble_msg_attclient_execute_write_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_attclient_read_multiple_cmd_t
{
  uint8 connection;
  uint8array  handles;
});

PACKSTRUCT(struct ble_msg_attclient_read_multiple_rsp_t
{
  uint8 connection;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_attclient_indicated_evt_t
{
  uint8 connection;
  uint16  attrhandle;
});

PACKSTRUCT(struct ble_msg_attclient_procedure_completed_evt_t
{
  uint8 connection;
  uint16  result;
  uint16  chrhandle;
});

PACKSTRUCT(struct ble_msg_attclient_group_found_evt_t
{
  uint8 connection;
  uint16  start;
  uint16  end;
  uint8array  uuid;
});

PACKSTRUCT(struct ble_msg_attclient_attribute_found_evt_t
{
  uint8 connection;
  uint16  chrdecl;
  uint16  value;
  uint8 properties;
  uint8array  uuid;
});

PACKSTRUCT(struct ble_msg_attclient_find_information_found_evt_t
{
  uint8 connection;
  uint16  chrhandle;
  uint8array  uuid;
});

PACKSTRUCT(struct ble_msg_attclient_attribute_value_evt_t
{
  uint8 connection;
  uint16  atthandle;
  uint8 type;
  uint8array  value;
});

PACKSTRUCT(struct ble_msg_attclient_read_multiple_response_evt_t
{
  uint8 connection;
  uint8array  handles;
});

PACKSTRUCT(struct ble_msg_sm_encrypt_start_cmd_t
{
  uint8 handle;
  uint8 bonding;
});

PACKSTRUCT(struct ble_msg_sm_encrypt_start_rsp_t
{
  uint8 handle;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_sm_set_bondable_mode_cmd_t
{
  uint8 bondable;
});

PACKSTRUCT(struct ble_msg_sm_delete_bonding_cmd_t
{
  uint8 handle;
});

PACKSTRUCT(struct ble_msg_sm_delete_bonding_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_sm_set_parameters_cmd_t
{
  uint8 mitm;
  uint8 min_key_size;
  uint8 io_capabilities;
});

PACKSTRUCT(struct ble_msg_sm_passkey_entry_cmd_t
{
  uint8 handle;
  uint32  passkey;
});

PACKSTRUCT(struct ble_msg_sm_passkey_entry_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_sm_get_bonds_rsp_t
{
  uint8 bonds;
});

PACKSTRUCT(struct ble_msg_sm_set_oob_data_cmd_t
{
  uint8array  oob;
});

PACKSTRUCT(struct ble_msg_sm_whitelist_bonds_rsp_t
{
  uint16  result;
  uint8 count;
});

PACKSTRUCT(struct ble_msg_sm_smp_data_evt_t
{
  uint8 handle;
  uint8 packet;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_sm_bonding_fail_evt_t
{
  uint8 handle;
  uint16  result;
});

PACKSTRUCT(struct ble_msg_sm_passkey_display_evt_t
{
  uint8 handle;
  uint32  passkey;
});

PACKSTRUCT(struct ble_msg_sm_passkey_request_evt_t
{
  uint8 handle;
});

PACKSTRUCT(struct ble_msg_sm_bond_status_evt_t
{
  uint8 bond;
  uint8 keysize;
  uint8 mitm;
  uint8 keys;
});

PACKSTRUCT(struct ble_msg_gap_set_privacy_flags_cmd_t
{
  uint8 peripheral_privacy;
  uint8 central_privacy;
});

PACKSTRUCT(struct ble_msg_gap_set_mode_cmd_t
{
  uint8 discover;
  uint8 connect;
});

PACKSTRUCT(struct ble_msg_gap_set_mode_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_gap_discover_cmd_t
{
  uint8 mode;
});

PACKSTRUCT(struct ble_msg_gap_discover_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_gap_connect_direct_cmd_t
{
  bd_addr address;
  uint8 addr_type;
  uint16  conn_interval_min;
  uint16  conn_interval_max;
  uint16  timeout;
  uint16  latency;
});

PACKSTRUCT(struct ble_msg_gap_connect_direct_rsp_t
{
  uint16  result;
  uint8 connection_handle;
});

PACKSTRUCT(struct ble_msg_gap_end_procedure_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_gap_connect_selective_cmd_t
{
  uint16  conn_interval_min;
  uint16  conn_interval_max;
  uint16  timeout;
  uint16  latency;
});

PACKSTRUCT(struct ble_msg_gap_connect_selective_rsp_t
{
  uint16  result;
  uint8 connection_handle;
});

PACKSTRUCT(struct ble_msg_gap_set_filtering_cmd_t
{
  uint8 scan_policy;
  uint8 adv_policy;
  uint8 scan_duplicate_filtering;
});

PACKSTRUCT(struct ble_msg_gap_set_filtering_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_gap_set_scan_parameters_cmd_t
{
  uint16  scan_interval;
  uint16  scan_window;
  uint8 active;
});

PACKSTRUCT(struct ble_msg_gap_set_scan_parameters_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_gap_set_adv_parameters_cmd_t
{
  uint16  adv_interval_min;
  uint16  adv_interval_max;
  uint8 adv_channels;
});

PACKSTRUCT(struct ble_msg_gap_set_adv_parameters_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_gap_set_adv_data_cmd_t
{
  uint8 set_scanrsp;
  uint8array  adv_data;
});

PACKSTRUCT(struct ble_msg_gap_set_adv_data_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_gap_set_directed_connectable_mode_cmd_t
{
  bd_addr address;
  uint8 addr_type;
});

PACKSTRUCT(struct ble_msg_gap_set_directed_connectable_mode_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_gap_scan_response_evt_t
{
  int8  rssi;
  uint8 packet_type;
  bd_addr sender;
  uint8 address_type;
  uint8 bond;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_gap_mode_changed_evt_t
{
  uint8 discover;
  uint8 connect;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_config_irq_cmd_t
{
  uint8 port;
  uint8 enable_bits;
  uint8 falling_edge;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_config_irq_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_hardware_set_soft_timer_cmd_t
{
  uint32  time;
  uint8 handle;
  uint8 single_shot;
});

PACKSTRUCT(struct ble_msg_hardware_set_soft_timer_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_hardware_adc_read_cmd_t
{
  uint8 input;
  uint8 decimation;
  uint8 reference_selection;
});

PACKSTRUCT(struct ble_msg_hardware_adc_read_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_config_direction_cmd_t
{
  uint8 port;
  uint8 direction;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_config_direction_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_config_function_cmd_t
{
  uint8 port;
  uint8 function;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_config_function_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_config_pull_cmd_t
{
  uint8 port;
  uint8 tristate_mask;
  uint8 pull_up;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_config_pull_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_write_cmd_t
{
  uint8 port;
  uint8 mask;
  uint8 data;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_write_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_read_cmd_t
{
  uint8 port;
  uint8 mask;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_read_rsp_t
{
  uint16  result;
  uint8 port;
  uint8 data;
});

PACKSTRUCT(struct ble_msg_hardware_spi_config_cmd_t
{
  uint8 channel;
  uint8 polarity;
  uint8 phase;
  uint8 bit_order;
  uint8 baud_e;
  uint8 baud_m;
});

PACKSTRUCT(struct ble_msg_hardware_spi_config_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_hardware_spi_transfer_cmd_t
{
  uint8 channel;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_hardware_spi_transfer_rsp_t
{
  uint16  result;
  uint8 channel;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_hardware_i2c_read_cmd_t
{
  uint8 address;
  uint8 stop;
  uint8 length;
});

PACKSTRUCT(struct ble_msg_hardware_i2c_read_rsp_t
{
  uint16  result;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_hardware_i2c_write_cmd_t
{
  uint8 address;
  uint8 stop;
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_hardware_i2c_write_rsp_t
{
  uint8 written;
});

PACKSTRUCT(struct ble_msg_hardware_set_txpower_cmd_t
{
  uint8 power;
});

PACKSTRUCT(struct ble_msg_hardware_timer_comparator_cmd_t
{
  uint8 timer;
  uint8 channel;
  uint8 mode;
  uint16  comparator_value;
});

PACKSTRUCT(struct ble_msg_hardware_timer_comparator_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_irq_enable_cmd_t
{
  uint8 port;
  uint8 enable_bits;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_irq_enable_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_irq_direction_cmd_t
{
  uint8 port;
  uint8 falling_edge;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_irq_direction_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_hardware_analog_comparator_enable_cmd_t
{
  uint8 enable;
});

PACKSTRUCT(struct ble_msg_hardware_analog_comparator_read_rsp_t
{
  uint16  result;
  uint8 output;
});

PACKSTRUCT(struct ble_msg_hardware_analog_comparator_config_irq_cmd_t
{
  uint8 enabled;
});

PACKSTRUCT(struct ble_msg_hardware_analog_comparator_config_irq_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_hardware_set_rxgain_cmd_t
{
  uint8 gain;
});

PACKSTRUCT(struct ble_msg_hardware_usb_enable_cmd_t
{
  uint8 enable;
});

PACKSTRUCT(struct ble_msg_hardware_usb_enable_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_hardware_io_port_status_evt_t
{
  uint32  timestamp;
  uint8 port;
  uint8 irq;
  uint8 state;
});

PACKSTRUCT(struct ble_msg_hardware_soft_timer_evt_t
{
  uint8 handle;
});

PACKSTRUCT(struct ble_msg_hardware_adc_result_evt_t
{
  uint8 input;
  int16 value;
});

PACKSTRUCT(struct ble_msg_hardware_analog_comparator_status_evt_t
{
  uint32  timestamp;
  uint8 output;
});

PACKSTRUCT(struct ble_msg_test_phy_tx_cmd_t
{
  uint8 channel;
  uint8 length;
  uint8 type;
});

PACKSTRUCT(struct ble_msg_test_phy_rx_cmd_t
{
  uint8 channel;
});

PACKSTRUCT(struct ble_msg_test_phy_end_rsp_t
{
  uint16  counter;
});

PACKSTRUCT(struct ble_msg_test_get_channel_map_rsp_t
{
  uint8array  channel_map;
});

PACKSTRUCT(struct ble_msg_test_debug_cmd_t
{
  uint8array  input;
});

PACKSTRUCT(struct ble_msg_test_debug_rsp_t
{
  uint8array  output;
});

PACKSTRUCT(struct ble_msg_test_channel_mode_cmd_t
{
  uint8 mode;
});

PACKSTRUCT(struct ble_msg_dfu_reset_cmd_t
{
  uint8 dfu;
});

PACKSTRUCT(struct ble_msg_dfu_flash_set_address_cmd_t
{
  uint32  address;
});

PACKSTRUCT(struct ble_msg_dfu_flash_set_address_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_dfu_flash_upload_cmd_t
{
  uint8array  data;
});

PACKSTRUCT(struct ble_msg_dfu_flash_upload_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_dfu_flash_upload_finish_rsp_t
{
  uint16  result;
});

PACKSTRUCT(struct ble_msg_dfu_boot_evt_t
{
  uint32  version;
});

PACKSTRUCT(
struct ble_cmd_packet
{
  struct ble_header header;

union{
  uint8 handle;

  uint8 payload;

  struct ble_msg_system_reset_cmd_t                          system_reset_cmd;
  struct ble_msg_system_reg_write_cmd_t                      system_reg_write_cmd;
  struct ble_msg_system_reg_read_cmd_t                       system_reg_read_cmd;
  struct ble_msg_system_read_memory_cmd_t                    system_read_memory_cmd;
  struct ble_msg_system_endpoint_tx_cmd_t                    system_endpoint_tx_cmd;
  struct ble_msg_system_whitelist_append_cmd_t               system_whitelist_append_cmd;
  struct ble_msg_system_whitelist_remove_cmd_t               system_whitelist_remove_cmd;
  struct ble_msg_system_endpoint_rx_cmd_t                    system_endpoint_rx_cmd;
  struct ble_msg_system_endpoint_set_watermarks_cmd_t        system_endpoint_set_watermarks_cmd;
  struct ble_msg_system_aes_setkey_cmd_t                     system_aes_setkey_cmd;
  struct ble_msg_system_aes_encrypt_cmd_t                    system_aes_encrypt_cmd;
  struct ble_msg_system_aes_decrypt_cmd_t                    system_aes_decrypt_cmd;
  struct ble_msg_system_boot_evt_t                           system_boot_evt;
  struct ble_msg_system_debug_evt_t                          system_debug_evt;
  struct ble_msg_system_endpoint_watermark_rx_evt_t          system_endpoint_watermark_rx_evt;
  struct ble_msg_system_endpoint_watermark_tx_evt_t          system_endpoint_watermark_tx_evt;
  struct ble_msg_system_script_failure_evt_t                 system_script_failure_evt;
  struct ble_msg_system_protocol_error_evt_t                 system_protocol_error_evt;
  struct ble_msg_system_address_get_rsp_t                    system_address_get_rsp;
  struct ble_msg_system_reg_write_rsp_t                      system_reg_write_rsp;
  struct ble_msg_system_reg_read_rsp_t                       system_reg_read_rsp;
  struct ble_msg_system_get_counters_rsp_t                   system_get_counters_rsp;
  struct ble_msg_system_get_connections_rsp_t                system_get_connections_rsp;
  struct ble_msg_system_read_memory_rsp_t                    system_read_memory_rsp;
  struct ble_msg_system_get_info_rsp_t                       system_get_info_rsp;
  struct ble_msg_system_endpoint_tx_rsp_t                    system_endpoint_tx_rsp;
  struct ble_msg_system_whitelist_append_rsp_t               system_whitelist_append_rsp;
  struct ble_msg_system_whitelist_remove_rsp_t               system_whitelist_remove_rsp;
  struct ble_msg_system_endpoint_rx_rsp_t                    system_endpoint_rx_rsp;
  struct ble_msg_system_endpoint_set_watermarks_rsp_t        system_endpoint_set_watermarks_rsp;
  struct ble_msg_system_aes_encrypt_rsp_t                    system_aes_encrypt_rsp;
  struct ble_msg_system_aes_decrypt_rsp_t                    system_aes_decrypt_rsp;
  struct ble_msg_flash_ps_save_cmd_t                         flash_ps_save_cmd;
  struct ble_msg_flash_ps_load_cmd_t                         flash_ps_load_cmd;
  struct ble_msg_flash_ps_erase_cmd_t                        flash_ps_erase_cmd;
  struct ble_msg_flash_erase_page_cmd_t                      flash_erase_page_cmd;
  struct ble_msg_flash_write_data_cmd_t                      flash_write_data_cmd;
  struct ble_msg_flash_read_data_cmd_t                       flash_read_data_cmd;
  struct ble_msg_flash_ps_key_evt_t                          flash_ps_key_evt;
  struct ble_msg_flash_ps_save_rsp_t                         flash_ps_save_rsp;
  struct ble_msg_flash_ps_load_rsp_t                         flash_ps_load_rsp;
  struct ble_msg_flash_erase_page_rsp_t                      flash_erase_page_rsp;
  struct ble_msg_flash_write_data_rsp_t                      flash_write_data_rsp;
  struct ble_msg_flash_read_data_rsp_t                       flash_read_data_rsp;
  struct ble_msg_attributes_write_cmd_t                      attributes_write_cmd;
  struct ble_msg_attributes_read_cmd_t                       attributes_read_cmd;
  struct ble_msg_attributes_read_type_cmd_t                  attributes_read_type_cmd;
  struct ble_msg_attributes_user_read_response_cmd_t         attributes_user_read_response_cmd;
  struct ble_msg_attributes_user_write_response_cmd_t        attributes_user_write_response_cmd;
  struct ble_msg_attributes_send_cmd_t                       attributes_send_cmd;
  struct ble_msg_attributes_value_evt_t                      attributes_value_evt;
  struct ble_msg_attributes_user_read_request_evt_t          attributes_user_read_request_evt;
  struct ble_msg_attributes_status_evt_t                     attributes_status_evt;
  struct ble_msg_attributes_write_rsp_t                      attributes_write_rsp;
  struct ble_msg_attributes_read_rsp_t                       attributes_read_rsp;
  struct ble_msg_attributes_read_type_rsp_t                  attributes_read_type_rsp;
  struct ble_msg_attributes_send_rsp_t                       attributes_send_rsp;
  struct ble_msg_connection_disconnect_cmd_t                 connection_disconnect_cmd;
  struct ble_msg_connection_get_rssi_cmd_t                   connection_get_rssi_cmd;
  struct ble_msg_connection_update_cmd_t                     connection_update_cmd;
  struct ble_msg_connection_version_update_cmd_t             connection_version_update_cmd;
  struct ble_msg_connection_channel_map_get_cmd_t            connection_channel_map_get_cmd;
  struct ble_msg_connection_channel_map_set_cmd_t            connection_channel_map_set_cmd;
  struct ble_msg_connection_features_get_cmd_t               connection_features_get_cmd;
  struct ble_msg_connection_get_status_cmd_t                 connection_get_status_cmd;
  struct ble_msg_connection_raw_tx_cmd_t                     connection_raw_tx_cmd;
  struct ble_msg_connection_status_evt_t                     connection_status_evt;
  struct ble_msg_connection_version_ind_evt_t                connection_version_ind_evt;
  struct ble_msg_connection_feature_ind_evt_t                connection_feature_ind_evt;
  struct ble_msg_connection_raw_rx_evt_t                     connection_raw_rx_evt;
  struct ble_msg_connection_disconnected_evt_t               connection_disconnected_evt;
  struct ble_msg_connection_disconnect_rsp_t                 connection_disconnect_rsp;
  struct ble_msg_connection_get_rssi_rsp_t                   connection_get_rssi_rsp;
  struct ble_msg_connection_update_rsp_t                     connection_update_rsp;
  struct ble_msg_connection_version_update_rsp_t             connection_version_update_rsp;
  struct ble_msg_connection_channel_map_get_rsp_t            connection_channel_map_get_rsp;
  struct ble_msg_connection_channel_map_set_rsp_t            connection_channel_map_set_rsp;
  struct ble_msg_connection_features_get_rsp_t               connection_features_get_rsp;
  struct ble_msg_connection_get_status_rsp_t                 connection_get_status_rsp;
  struct ble_msg_connection_raw_tx_rsp_t                     connection_raw_tx_rsp;
  struct ble_msg_attclient_find_by_type_value_cmd_t          attclient_find_by_type_value_cmd;
  struct ble_msg_attclient_read_by_group_type_cmd_t          attclient_read_by_group_type_cmd;
  struct ble_msg_attclient_read_by_type_cmd_t                attclient_read_by_type_cmd;
  struct ble_msg_attclient_find_information_cmd_t            attclient_find_information_cmd;
  struct ble_msg_attclient_read_by_handle_cmd_t              attclient_read_by_handle_cmd;
  struct ble_msg_attclient_attribute_write_cmd_t             attclient_attribute_write_cmd;
  struct ble_msg_attclient_write_command_cmd_t               attclient_write_command_cmd;
  struct ble_msg_attclient_indicate_confirm_cmd_t            attclient_indicate_confirm_cmd;
  struct ble_msg_attclient_read_long_cmd_t                   attclient_read_long_cmd;
  struct ble_msg_attclient_prepare_write_cmd_t               attclient_prepare_write_cmd;
  struct ble_msg_attclient_execute_write_cmd_t               attclient_execute_write_cmd;
  struct ble_msg_attclient_read_multiple_cmd_t               attclient_read_multiple_cmd;
  struct ble_msg_attclient_indicated_evt_t                   attclient_indicated_evt;
  struct ble_msg_attclient_procedure_completed_evt_t         attclient_procedure_completed_evt;
  struct ble_msg_attclient_group_found_evt_t                 attclient_group_found_evt;
  struct ble_msg_attclient_attribute_found_evt_t             attclient_attribute_found_evt;
  struct ble_msg_attclient_find_information_found_evt_t      attclient_find_information_found_evt;
  struct ble_msg_attclient_attribute_value_evt_t             attclient_attribute_value_evt;
  struct ble_msg_attclient_read_multiple_response_evt_t      attclient_read_multiple_response_evt;
  struct ble_msg_attclient_find_by_type_value_rsp_t          attclient_find_by_type_value_rsp;
  struct ble_msg_attclient_read_by_group_type_rsp_t          attclient_read_by_group_type_rsp;
  struct ble_msg_attclient_read_by_type_rsp_t                attclient_read_by_type_rsp;
  struct ble_msg_attclient_find_information_rsp_t            attclient_find_information_rsp;
  struct ble_msg_attclient_read_by_handle_rsp_t              attclient_read_by_handle_rsp;
  struct ble_msg_attclient_attribute_write_rsp_t             attclient_attribute_write_rsp;
  struct ble_msg_attclient_write_command_rsp_t               attclient_write_command_rsp;
  struct ble_msg_attclient_indicate_confirm_rsp_t            attclient_indicate_confirm_rsp;
  struct ble_msg_attclient_read_long_rsp_t                   attclient_read_long_rsp;
  struct ble_msg_attclient_prepare_write_rsp_t               attclient_prepare_write_rsp;
  struct ble_msg_attclient_execute_write_rsp_t               attclient_execute_write_rsp;
  struct ble_msg_attclient_read_multiple_rsp_t               attclient_read_multiple_rsp;
  struct ble_msg_sm_encrypt_start_cmd_t                      sm_encrypt_start_cmd;
  struct ble_msg_sm_set_bondable_mode_cmd_t                  sm_set_bondable_mode_cmd;
  struct ble_msg_sm_delete_bonding_cmd_t                     sm_delete_bonding_cmd;
  struct ble_msg_sm_set_parameters_cmd_t                     sm_set_parameters_cmd;
  struct ble_msg_sm_passkey_entry_cmd_t                      sm_passkey_entry_cmd;
  struct ble_msg_sm_set_oob_data_cmd_t                       sm_set_oob_data_cmd;
  struct ble_msg_sm_smp_data_evt_t                           sm_smp_data_evt;
  struct ble_msg_sm_bonding_fail_evt_t                       sm_bonding_fail_evt;
  struct ble_msg_sm_passkey_display_evt_t                    sm_passkey_display_evt;
  struct ble_msg_sm_passkey_request_evt_t                    sm_passkey_request_evt;
  struct ble_msg_sm_bond_status_evt_t                        sm_bond_status_evt;
  struct ble_msg_sm_encrypt_start_rsp_t                      sm_encrypt_start_rsp;
  struct ble_msg_sm_delete_bonding_rsp_t                     sm_delete_bonding_rsp;
  struct ble_msg_sm_passkey_entry_rsp_t                      sm_passkey_entry_rsp;
  struct ble_msg_sm_get_bonds_rsp_t                          sm_get_bonds_rsp;
  struct ble_msg_sm_whitelist_bonds_rsp_t                    sm_whitelist_bonds_rsp;
  struct ble_msg_gap_set_privacy_flags_cmd_t                 gap_set_privacy_flags_cmd;
  struct ble_msg_gap_set_mode_cmd_t                          gap_set_mode_cmd;
  struct ble_msg_gap_discover_cmd_t                          gap_discover_cmd;
  struct ble_msg_gap_connect_direct_cmd_t                    gap_connect_direct_cmd;
  struct ble_msg_gap_connect_selective_cmd_t                 gap_connect_selective_cmd;
  struct ble_msg_gap_set_filtering_cmd_t                     gap_set_filtering_cmd;
  struct ble_msg_gap_set_scan_parameters_cmd_t               gap_set_scan_parameters_cmd;
  struct ble_msg_gap_set_adv_parameters_cmd_t                gap_set_adv_parameters_cmd;
  struct ble_msg_gap_set_adv_data_cmd_t                      gap_set_adv_data_cmd;
  struct ble_msg_gap_set_directed_connectable_mode_cmd_t     gap_set_directed_connectable_mode_cmd;
  struct ble_msg_gap_scan_response_evt_t                     gap_scan_response_evt;
  struct ble_msg_gap_mode_changed_evt_t                      gap_mode_changed_evt;
  struct ble_msg_gap_set_mode_rsp_t                          gap_set_mode_rsp;
  struct ble_msg_gap_discover_rsp_t                          gap_discover_rsp;
  struct ble_msg_gap_connect_direct_rsp_t                    gap_connect_direct_rsp;
  struct ble_msg_gap_end_procedure_rsp_t                     gap_end_procedure_rsp;
  struct ble_msg_gap_connect_selective_rsp_t                 gap_connect_selective_rsp;
  struct ble_msg_gap_set_filtering_rsp_t                     gap_set_filtering_rsp;
  struct ble_msg_gap_set_scan_parameters_rsp_t               gap_set_scan_parameters_rsp;
  struct ble_msg_gap_set_adv_parameters_rsp_t                gap_set_adv_parameters_rsp;
  struct ble_msg_gap_set_adv_data_rsp_t                      gap_set_adv_data_rsp;
  struct ble_msg_gap_set_directed_connectable_mode_rsp_t     gap_set_directed_connectable_mode_rsp;
  struct ble_msg_hardware_io_port_config_irq_cmd_t           hardware_io_port_config_irq_cmd;
  struct ble_msg_hardware_set_soft_timer_cmd_t               hardware_set_soft_timer_cmd;
  struct ble_msg_hardware_adc_read_cmd_t                     hardware_adc_read_cmd;
  struct ble_msg_hardware_io_port_config_direction_cmd_t     hardware_io_port_config_direction_cmd;
  struct ble_msg_hardware_io_port_config_function_cmd_t      hardware_io_port_config_function_cmd;
  struct ble_msg_hardware_io_port_config_pull_cmd_t          hardware_io_port_config_pull_cmd;
  struct ble_msg_hardware_io_port_write_cmd_t                hardware_io_port_write_cmd;
  struct ble_msg_hardware_io_port_read_cmd_t                 hardware_io_port_read_cmd;
  struct ble_msg_hardware_spi_config_cmd_t                   hardware_spi_config_cmd;
  struct ble_msg_hardware_spi_transfer_cmd_t                 hardware_spi_transfer_cmd;
  struct ble_msg_hardware_i2c_read_cmd_t                     hardware_i2c_read_cmd;
  struct ble_msg_hardware_i2c_write_cmd_t                    hardware_i2c_write_cmd;
  struct ble_msg_hardware_set_txpower_cmd_t                  hardware_set_txpower_cmd;
  struct ble_msg_hardware_timer_comparator_cmd_t             hardware_timer_comparator_cmd;
  struct ble_msg_hardware_io_port_irq_enable_cmd_t           hardware_io_port_irq_enable_cmd;
  struct ble_msg_hardware_io_port_irq_direction_cmd_t        hardware_io_port_irq_direction_cmd;
  struct ble_msg_hardware_analog_comparator_enable_cmd_t     hardware_analog_comparator_enable_cmd;
  struct ble_msg_hardware_analog_comparator_config_irq_cmd_t hardware_analog_comparator_config_irq_cmd;
  struct ble_msg_hardware_set_rxgain_cmd_t                   hardware_set_rxgain_cmd;
  struct ble_msg_hardware_usb_enable_cmd_t                   hardware_usb_enable_cmd;
  struct ble_msg_hardware_io_port_status_evt_t               hardware_io_port_status_evt;
  struct ble_msg_hardware_soft_timer_evt_t                   hardware_soft_timer_evt;
  struct ble_msg_hardware_adc_result_evt_t                   hardware_adc_result_evt;
  struct ble_msg_hardware_analog_comparator_status_evt_t     hardware_analog_comparator_status_evt;
  struct ble_msg_hardware_io_port_config_irq_rsp_t           hardware_io_port_config_irq_rsp;
  struct ble_msg_hardware_set_soft_timer_rsp_t               hardware_set_soft_timer_rsp;
  struct ble_msg_hardware_adc_read_rsp_t                     hardware_adc_read_rsp;
  struct ble_msg_hardware_io_port_config_direction_rsp_t     hardware_io_port_config_direction_rsp;
  struct ble_msg_hardware_io_port_config_function_rsp_t      hardware_io_port_config_function_rsp;
  struct ble_msg_hardware_io_port_config_pull_rsp_t          hardware_io_port_config_pull_rsp;
  struct ble_msg_hardware_io_port_write_rsp_t                hardware_io_port_write_rsp;
  struct ble_msg_hardware_io_port_read_rsp_t                 hardware_io_port_read_rsp;
  struct ble_msg_hardware_spi_config_rsp_t                   hardware_spi_config_rsp;
  struct ble_msg_hardware_spi_transfer_rsp_t                 hardware_spi_transfer_rsp;
  struct ble_msg_hardware_i2c_read_rsp_t                     hardware_i2c_read_rsp;
  struct ble_msg_hardware_i2c_write_rsp_t                    hardware_i2c_write_rsp;
  struct ble_msg_hardware_timer_comparator_rsp_t             hardware_timer_comparator_rsp;
  struct ble_msg_hardware_io_port_irq_enable_rsp_t           hardware_io_port_irq_enable_rsp;
  struct ble_msg_hardware_io_port_irq_direction_rsp_t        hardware_io_port_irq_direction_rsp;
  struct ble_msg_hardware_analog_comparator_read_rsp_t       hardware_analog_comparator_read_rsp;
  struct ble_msg_hardware_analog_comparator_config_irq_rsp_t hardware_analog_comparator_config_irq_rsp;
  struct ble_msg_hardware_usb_enable_rsp_t                   hardware_usb_enable_rsp;
  struct ble_msg_test_phy_tx_cmd_t                           test_phy_tx_cmd;
  struct ble_msg_test_phy_rx_cmd_t                           test_phy_rx_cmd;
  struct ble_msg_test_debug_cmd_t                            test_debug_cmd;
  struct ble_msg_test_channel_mode_cmd_t                     test_channel_mode_cmd;
  struct ble_msg_test_phy_end_rsp_t                          test_phy_end_rsp;
  struct ble_msg_test_get_channel_map_rsp_t                  test_get_channel_map_rsp;
  struct ble_msg_test_debug_rsp_t                            test_debug_rsp;
  struct ble_msg_dfu_reset_cmd_t                             dfu_reset_cmd;
  struct ble_msg_dfu_flash_set_address_cmd_t                 dfu_flash_set_address_cmd;
  struct ble_msg_dfu_flash_upload_cmd_t                      dfu_flash_upload_cmd;
  struct ble_msg_dfu_boot_evt_t                              dfu_boot_evt;
  struct ble_msg_dfu_flash_set_address_rsp_t                 dfu_flash_set_address_rsp;
  struct ble_msg_dfu_flash_upload_rsp_t                      dfu_flash_upload_rsp;
  struct ble_msg_dfu_flash_upload_finish_rsp_t               dfu_flash_upload_finish_rsp;
};

}ALIGNED);

struct ble_class_handler_t
{
    const struct ble_msg * const *msgs;
    uint8 maxhandlers;
};

extern const struct ble_class_handler_t ble_class_rsp_handlers[ble_cls_last];
extern const struct ble_class_handler_t ble_class_evt_handlers[ble_cls_last];

std::map<std::type_index, uint8> ble_msg_typeid = {
  {typeid(struct ble_msg_system_reset_cmd_t), ble_cmd_system_reset_idx},
  {typeid(struct ble_msg_system_reg_write_cmd_t), ble_cmd_system_reg_write_idx},
  {typeid(struct ble_msg_system_reg_read_cmd_t), ble_cmd_system_reg_read_idx},
  {typeid(struct ble_msg_system_read_memory_cmd_t), ble_cmd_system_read_memory_idx},
  {typeid(struct ble_msg_system_endpoint_tx_cmd_t), ble_cmd_system_endpoint_tx_idx},
  {typeid(struct ble_msg_system_whitelist_append_cmd_t), ble_cmd_system_whitelist_append_idx},
  {typeid(struct ble_msg_system_whitelist_remove_cmd_t), ble_cmd_system_whitelist_remove_idx},
  {typeid(struct ble_msg_system_endpoint_rx_cmd_t), ble_cmd_system_endpoint_rx_idx},
  {typeid(struct ble_msg_system_endpoint_set_watermarks_cmd_t), ble_cmd_system_endpoint_set_watermarks_idx},
  {typeid(struct ble_msg_system_aes_setkey_cmd_t), ble_cmd_system_aes_setkey_idx},
  {typeid(struct ble_msg_system_aes_encrypt_cmd_t), ble_cmd_system_aes_encrypt_idx},
  {typeid(struct ble_msg_system_aes_decrypt_cmd_t), ble_cmd_system_aes_decrypt_idx},
  {typeid(struct ble_msg_system_boot_evt_t), ble_evt_system_boot_idx},
  {typeid(struct ble_msg_system_debug_evt_t), ble_evt_system_debug_idx},
  {typeid(struct ble_msg_system_endpoint_watermark_rx_evt_t), ble_evt_system_endpoint_watermark_rx_idx},
  {typeid(struct ble_msg_system_endpoint_watermark_tx_evt_t), ble_evt_system_endpoint_watermark_tx_idx},
  {typeid(struct ble_msg_system_script_failure_evt_t), ble_evt_system_script_failure_idx},
  {typeid(struct ble_msg_system_protocol_error_evt_t), ble_evt_system_protocol_error_idx},
  {typeid(struct ble_msg_system_address_get_rsp_t), ble_rsp_system_address_get_idx},
  {typeid(struct ble_msg_system_reg_write_rsp_t), ble_rsp_system_reg_write_idx},
  {typeid(struct ble_msg_system_reg_read_rsp_t), ble_rsp_system_reg_read_idx},
  {typeid(struct ble_msg_system_get_counters_rsp_t), ble_rsp_system_get_counters_idx},
  {typeid(struct ble_msg_system_get_connections_rsp_t), ble_rsp_system_get_connections_idx},
  {typeid(struct ble_msg_system_read_memory_rsp_t), ble_rsp_system_read_memory_idx},
  {typeid(struct ble_msg_system_get_info_rsp_t), ble_rsp_system_get_info_idx},
  {typeid(struct ble_msg_system_endpoint_tx_rsp_t), ble_rsp_system_endpoint_tx_idx},
  {typeid(struct ble_msg_system_whitelist_append_rsp_t), ble_rsp_system_whitelist_append_idx},
  {typeid(struct ble_msg_system_whitelist_remove_rsp_t), ble_rsp_system_whitelist_remove_idx},
  {typeid(struct ble_msg_system_endpoint_rx_rsp_t), ble_rsp_system_endpoint_rx_idx},
  {typeid(struct ble_msg_system_endpoint_set_watermarks_rsp_t), ble_rsp_system_endpoint_set_watermarks_idx},
  {typeid(struct ble_msg_system_aes_encrypt_rsp_t), ble_rsp_system_aes_encrypt_idx},
  {typeid(struct ble_msg_system_aes_decrypt_rsp_t), ble_rsp_system_aes_decrypt_idx},
  {typeid(struct ble_msg_flash_ps_save_cmd_t), ble_cmd_flash_ps_save_idx},
  {typeid(struct ble_msg_flash_ps_load_cmd_t), ble_cmd_flash_ps_load_idx},
  {typeid(struct ble_msg_flash_ps_erase_cmd_t), ble_cmd_flash_ps_erase_idx},
  {typeid(struct ble_msg_flash_erase_page_cmd_t), ble_cmd_flash_erase_page_idx},
  {typeid(struct ble_msg_flash_write_data_cmd_t), ble_cmd_flash_write_data_idx},
  {typeid(struct ble_msg_flash_read_data_cmd_t), ble_cmd_flash_read_data_idx},
  {typeid(struct ble_msg_flash_ps_key_evt_t), ble_evt_flash_ps_key_idx},
  {typeid(struct ble_msg_flash_ps_save_rsp_t), ble_rsp_flash_ps_save_idx},
  {typeid(struct ble_msg_flash_ps_load_rsp_t), ble_rsp_flash_ps_load_idx},
  {typeid(struct ble_msg_flash_erase_page_rsp_t), ble_rsp_flash_erase_page_idx},
  {typeid(struct ble_msg_flash_write_data_rsp_t), ble_rsp_flash_write_data_idx},
  {typeid(struct ble_msg_flash_read_data_rsp_t), ble_rsp_flash_read_data_idx},
  {typeid(struct ble_msg_attributes_write_cmd_t), ble_cmd_attributes_write_idx},
  {typeid(struct ble_msg_attributes_read_cmd_t), ble_cmd_attributes_read_idx},
  {typeid(struct ble_msg_attributes_read_type_cmd_t), ble_cmd_attributes_read_type_idx},
  {typeid(struct ble_msg_attributes_user_read_response_cmd_t), ble_cmd_attributes_user_read_response_idx},
  {typeid(struct ble_msg_attributes_user_write_response_cmd_t), ble_cmd_attributes_user_write_response_idx},
  {typeid(struct ble_msg_attributes_send_cmd_t), ble_cmd_attributes_send_idx},
  {typeid(struct ble_msg_attributes_value_evt_t), ble_evt_attributes_value_idx},
  {typeid(struct ble_msg_attributes_user_read_request_evt_t), ble_evt_attributes_user_read_request_idx},
  {typeid(struct ble_msg_attributes_status_evt_t), ble_evt_attributes_status_idx},
  {typeid(struct ble_msg_attributes_write_rsp_t), ble_rsp_attributes_write_idx},
  {typeid(struct ble_msg_attributes_read_rsp_t), ble_rsp_attributes_read_idx},
  {typeid(struct ble_msg_attributes_read_type_rsp_t), ble_rsp_attributes_read_type_idx},
  {typeid(struct ble_msg_attributes_send_rsp_t), ble_rsp_attributes_send_idx},
  {typeid(struct ble_msg_connection_disconnect_cmd_t), ble_cmd_connection_disconnect_idx},
  {typeid(struct ble_msg_connection_get_rssi_cmd_t), ble_cmd_connection_get_rssi_idx},
  {typeid(struct ble_msg_connection_update_cmd_t), ble_cmd_connection_update_idx},
  {typeid(struct ble_msg_connection_version_update_cmd_t), ble_cmd_connection_version_update_idx},
  {typeid(struct ble_msg_connection_channel_map_get_cmd_t), ble_cmd_connection_channel_map_get_idx},
  {typeid(struct ble_msg_connection_channel_map_set_cmd_t), ble_cmd_connection_channel_map_set_idx},
  {typeid(struct ble_msg_connection_features_get_cmd_t), ble_cmd_connection_features_get_idx},
  {typeid(struct ble_msg_connection_get_status_cmd_t), ble_cmd_connection_get_status_idx},
  {typeid(struct ble_msg_connection_raw_tx_cmd_t), ble_cmd_connection_raw_tx_idx},
  {typeid(struct ble_msg_connection_status_evt_t), ble_evt_connection_status_idx},
  {typeid(struct ble_msg_connection_version_ind_evt_t), ble_evt_connection_version_ind_idx},
  {typeid(struct ble_msg_connection_feature_ind_evt_t), ble_evt_connection_feature_ind_idx},
  {typeid(struct ble_msg_connection_raw_rx_evt_t), ble_evt_connection_raw_rx_idx},
  {typeid(struct ble_msg_connection_disconnected_evt_t), ble_evt_connection_disconnected_idx},
  {typeid(struct ble_msg_connection_disconnect_rsp_t), ble_rsp_connection_disconnect_idx},
  {typeid(struct ble_msg_connection_get_rssi_rsp_t), ble_rsp_connection_get_rssi_idx},
  {typeid(struct ble_msg_connection_update_rsp_t), ble_rsp_connection_update_idx},
  {typeid(struct ble_msg_connection_version_update_rsp_t), ble_rsp_connection_version_update_idx},
  {typeid(struct ble_msg_connection_channel_map_get_rsp_t), ble_rsp_connection_channel_map_get_idx},
  {typeid(struct ble_msg_connection_channel_map_set_rsp_t), ble_rsp_connection_channel_map_set_idx},
  {typeid(struct ble_msg_connection_features_get_rsp_t), ble_rsp_connection_features_get_idx},
  {typeid(struct ble_msg_connection_get_status_rsp_t), ble_rsp_connection_get_status_idx},
  {typeid(struct ble_msg_connection_raw_tx_rsp_t), ble_rsp_connection_raw_tx_idx},
  {typeid(struct ble_msg_attclient_find_by_type_value_cmd_t), ble_cmd_attclient_find_by_type_value_idx},
  {typeid(struct ble_msg_attclient_read_by_group_type_cmd_t), ble_cmd_attclient_read_by_group_type_idx},
  {typeid(struct ble_msg_attclient_read_by_type_cmd_t), ble_cmd_attclient_read_by_type_idx},
  {typeid(struct ble_msg_attclient_find_information_cmd_t), ble_cmd_attclient_find_information_idx},
  {typeid(struct ble_msg_attclient_read_by_handle_cmd_t), ble_cmd_attclient_read_by_handle_idx},
  {typeid(struct ble_msg_attclient_attribute_write_cmd_t), ble_cmd_attclient_attribute_write_idx},
  {typeid(struct ble_msg_attclient_write_command_cmd_t), ble_cmd_attclient_write_command_idx},
  {typeid(struct ble_msg_attclient_indicate_confirm_cmd_t), ble_cmd_attclient_indicate_confirm_idx},
  {typeid(struct ble_msg_attclient_read_long_cmd_t), ble_cmd_attclient_read_long_idx},
  {typeid(struct ble_msg_attclient_prepare_write_cmd_t), ble_cmd_attclient_prepare_write_idx},
  {typeid(struct ble_msg_attclient_execute_write_cmd_t), ble_cmd_attclient_execute_write_idx},
  {typeid(struct ble_msg_attclient_read_multiple_cmd_t), ble_cmd_attclient_read_multiple_idx},
  {typeid(struct ble_msg_attclient_indicated_evt_t), ble_evt_attclient_indicated_idx},
  {typeid(struct ble_msg_attclient_procedure_completed_evt_t), ble_evt_attclient_procedure_completed_idx},
  {typeid(struct ble_msg_attclient_group_found_evt_t), ble_evt_attclient_group_found_idx},
  {typeid(struct ble_msg_attclient_attribute_found_evt_t), ble_evt_attclient_attribute_found_idx},
  {typeid(struct ble_msg_attclient_find_information_found_evt_t), ble_evt_attclient_find_information_found_idx},
  {typeid(struct ble_msg_attclient_attribute_value_evt_t), ble_evt_attclient_attribute_value_idx},
  {typeid(struct ble_msg_attclient_read_multiple_response_evt_t), ble_evt_attclient_read_multiple_response_idx},
  {typeid(struct ble_msg_attclient_find_by_type_value_rsp_t), ble_rsp_attclient_find_by_type_value_idx},
  {typeid(struct ble_msg_attclient_read_by_group_type_rsp_t), ble_rsp_attclient_read_by_group_type_idx},
  {typeid(struct ble_msg_attclient_read_by_type_rsp_t), ble_rsp_attclient_read_by_type_idx},
  {typeid(struct ble_msg_attclient_find_information_rsp_t), ble_rsp_attclient_find_information_idx},
  {typeid(struct ble_msg_attclient_read_by_handle_rsp_t), ble_rsp_attclient_read_by_handle_idx},
  {typeid(struct ble_msg_attclient_attribute_write_rsp_t), ble_rsp_attclient_attribute_write_idx},
  {typeid(struct ble_msg_attclient_write_command_rsp_t), ble_rsp_attclient_write_command_idx},
  {typeid(struct ble_msg_attclient_indicate_confirm_rsp_t), ble_rsp_attclient_indicate_confirm_idx},
  {typeid(struct ble_msg_attclient_read_long_rsp_t), ble_rsp_attclient_read_long_idx},
  {typeid(struct ble_msg_attclient_prepare_write_rsp_t), ble_rsp_attclient_prepare_write_idx},
  {typeid(struct ble_msg_attclient_execute_write_rsp_t), ble_rsp_attclient_execute_write_idx},
  {typeid(struct ble_msg_attclient_read_multiple_rsp_t), ble_rsp_attclient_read_multiple_idx},
  {typeid(struct ble_msg_sm_encrypt_start_cmd_t), ble_cmd_sm_encrypt_start_idx},
  {typeid(struct ble_msg_sm_set_bondable_mode_cmd_t), ble_cmd_sm_set_bondable_mode_idx},
  {typeid(struct ble_msg_sm_delete_bonding_cmd_t), ble_cmd_sm_delete_bonding_idx},
  {typeid(struct ble_msg_sm_set_parameters_cmd_t), ble_cmd_sm_set_parameters_idx},
  {typeid(struct ble_msg_sm_passkey_entry_cmd_t), ble_cmd_sm_passkey_entry_idx},
  {typeid(struct ble_msg_sm_set_oob_data_cmd_t), ble_cmd_sm_set_oob_data_idx},
  {typeid(struct ble_msg_sm_smp_data_evt_t), ble_evt_sm_smp_data_idx},
  {typeid(struct ble_msg_sm_bonding_fail_evt_t), ble_evt_sm_bonding_fail_idx},
  {typeid(struct ble_msg_sm_passkey_display_evt_t), ble_evt_sm_passkey_display_idx},
  {typeid(struct ble_msg_sm_passkey_request_evt_t), ble_evt_sm_passkey_request_idx},
  {typeid(struct ble_msg_sm_bond_status_evt_t), ble_evt_sm_bond_status_idx},
  {typeid(struct ble_msg_sm_encrypt_start_rsp_t), ble_rsp_sm_encrypt_start_idx},
  {typeid(struct ble_msg_sm_delete_bonding_rsp_t), ble_rsp_sm_delete_bonding_idx},
  {typeid(struct ble_msg_sm_passkey_entry_rsp_t), ble_rsp_sm_passkey_entry_idx},
  {typeid(struct ble_msg_sm_get_bonds_rsp_t), ble_rsp_sm_get_bonds_idx},
  {typeid(struct ble_msg_sm_whitelist_bonds_rsp_t), ble_rsp_sm_whitelist_bonds_idx},
  {typeid(struct ble_msg_gap_set_privacy_flags_cmd_t), ble_cmd_gap_set_privacy_flags_idx},
  {typeid(struct ble_msg_gap_set_mode_cmd_t), ble_cmd_gap_set_mode_idx},
  {typeid(struct ble_msg_gap_discover_cmd_t), ble_cmd_gap_discover_idx},
  {typeid(struct ble_msg_gap_connect_direct_cmd_t), ble_cmd_gap_connect_direct_idx},
  {typeid(struct ble_msg_gap_connect_selective_cmd_t), ble_cmd_gap_connect_selective_idx},
  {typeid(struct ble_msg_gap_set_filtering_cmd_t), ble_cmd_gap_set_filtering_idx},
  {typeid(struct ble_msg_gap_set_scan_parameters_cmd_t), ble_cmd_gap_set_scan_parameters_idx},
  {typeid(struct ble_msg_gap_set_adv_parameters_cmd_t), ble_cmd_gap_set_adv_parameters_idx},
  {typeid(struct ble_msg_gap_set_adv_data_cmd_t), ble_cmd_gap_set_adv_data_idx},
  {typeid(struct ble_msg_gap_set_directed_connectable_mode_cmd_t), ble_cmd_gap_set_directed_connectable_mode_idx},
  {typeid(struct ble_msg_gap_scan_response_evt_t), ble_evt_gap_scan_response_idx},
  {typeid(struct ble_msg_gap_mode_changed_evt_t), ble_evt_gap_mode_changed_idx},
  {typeid(struct ble_msg_gap_set_mode_rsp_t), ble_rsp_gap_set_mode_idx},
  {typeid(struct ble_msg_gap_discover_rsp_t), ble_rsp_gap_discover_idx},
  {typeid(struct ble_msg_gap_connect_direct_rsp_t), ble_rsp_gap_connect_direct_idx},
  {typeid(struct ble_msg_gap_end_procedure_rsp_t), ble_rsp_gap_end_procedure_idx},
  {typeid(struct ble_msg_gap_connect_selective_rsp_t), ble_rsp_gap_connect_selective_idx},
  {typeid(struct ble_msg_gap_set_filtering_rsp_t), ble_rsp_gap_set_filtering_idx},
  {typeid(struct ble_msg_gap_set_scan_parameters_rsp_t), ble_rsp_gap_set_scan_parameters_idx},
  {typeid(struct ble_msg_gap_set_adv_parameters_rsp_t), ble_rsp_gap_set_adv_parameters_idx},
  {typeid(struct ble_msg_gap_set_adv_data_rsp_t), ble_rsp_gap_set_adv_data_idx},
  {typeid(struct ble_msg_gap_set_directed_connectable_mode_rsp_t), ble_rsp_gap_set_directed_connectable_mode_idx},
  {typeid(struct ble_msg_hardware_io_port_config_irq_cmd_t), ble_cmd_hardware_io_port_config_irq_idx},
  {typeid(struct ble_msg_hardware_set_soft_timer_cmd_t), ble_cmd_hardware_set_soft_timer_idx},
  {typeid(struct ble_msg_hardware_adc_read_cmd_t), ble_cmd_hardware_adc_read_idx},
  {typeid(struct ble_msg_hardware_io_port_config_direction_cmd_t), ble_cmd_hardware_io_port_config_direction_idx},
  {typeid(struct ble_msg_hardware_io_port_config_function_cmd_t), ble_cmd_hardware_io_port_config_function_idx},
  {typeid(struct ble_msg_hardware_io_port_config_pull_cmd_t), ble_cmd_hardware_io_port_config_pull_idx},
  {typeid(struct ble_msg_hardware_io_port_write_cmd_t), ble_cmd_hardware_io_port_write_idx},
  {typeid(struct ble_msg_hardware_io_port_read_cmd_t), ble_cmd_hardware_io_port_read_idx},
  {typeid(struct ble_msg_hardware_spi_config_cmd_t), ble_cmd_hardware_spi_config_idx},
  {typeid(struct ble_msg_hardware_spi_transfer_cmd_t), ble_cmd_hardware_spi_transfer_idx},
  {typeid(struct ble_msg_hardware_i2c_read_cmd_t), ble_cmd_hardware_i2c_read_idx},
  {typeid(struct ble_msg_hardware_i2c_write_cmd_t), ble_cmd_hardware_i2c_write_idx},
  {typeid(struct ble_msg_hardware_set_txpower_cmd_t), ble_cmd_hardware_set_txpower_idx},
  {typeid(struct ble_msg_hardware_timer_comparator_cmd_t), ble_cmd_hardware_timer_comparator_idx},
  {typeid(struct ble_msg_hardware_io_port_irq_enable_cmd_t), ble_cmd_hardware_io_port_irq_enable_idx},
  {typeid(struct ble_msg_hardware_io_port_irq_direction_cmd_t), ble_cmd_hardware_io_port_irq_direction_idx},
  {typeid(struct ble_msg_hardware_analog_comparator_enable_cmd_t), ble_cmd_hardware_analog_comparator_enable_idx},
  {typeid(struct ble_msg_hardware_analog_comparator_config_irq_cmd_t), ble_cmd_hardware_analog_comparator_config_irq_idx},
  {typeid(struct ble_msg_hardware_set_rxgain_cmd_t), ble_cmd_hardware_set_rxgain_idx},
  {typeid(struct ble_msg_hardware_usb_enable_cmd_t), ble_cmd_hardware_usb_enable_idx},
  {typeid(struct ble_msg_hardware_io_port_status_evt_t), ble_evt_hardware_io_port_status_idx},
  {typeid(struct ble_msg_hardware_soft_timer_evt_t), ble_evt_hardware_soft_timer_idx},
  {typeid(struct ble_msg_hardware_adc_result_evt_t), ble_evt_hardware_adc_result_idx},
  {typeid(struct ble_msg_hardware_analog_comparator_status_evt_t), ble_evt_hardware_analog_comparator_status_idx},
  {typeid(struct ble_msg_hardware_io_port_config_irq_rsp_t), ble_rsp_hardware_io_port_config_irq_idx},
  {typeid(struct ble_msg_hardware_set_soft_timer_rsp_t), ble_rsp_hardware_set_soft_timer_idx},
  {typeid(struct ble_msg_hardware_adc_read_rsp_t), ble_rsp_hardware_adc_read_idx},
  {typeid(struct ble_msg_hardware_io_port_config_direction_rsp_t), ble_rsp_hardware_io_port_config_direction_idx},
  {typeid(struct ble_msg_hardware_io_port_config_function_rsp_t), ble_rsp_hardware_io_port_config_function_idx},
  {typeid(struct ble_msg_hardware_io_port_config_pull_rsp_t), ble_rsp_hardware_io_port_config_pull_idx},
  {typeid(struct ble_msg_hardware_io_port_write_rsp_t), ble_rsp_hardware_io_port_write_idx},
  {typeid(struct ble_msg_hardware_io_port_read_rsp_t), ble_rsp_hardware_io_port_read_idx},
  {typeid(struct ble_msg_hardware_spi_config_rsp_t), ble_rsp_hardware_spi_config_idx},
  {typeid(struct ble_msg_hardware_spi_transfer_rsp_t), ble_rsp_hardware_spi_transfer_idx},
  {typeid(struct ble_msg_hardware_i2c_read_rsp_t), ble_rsp_hardware_i2c_read_idx},
  {typeid(struct ble_msg_hardware_i2c_write_rsp_t), ble_rsp_hardware_i2c_write_idx},
  {typeid(struct ble_msg_hardware_timer_comparator_rsp_t), ble_rsp_hardware_timer_comparator_idx},
  {typeid(struct ble_msg_hardware_io_port_irq_enable_rsp_t), ble_rsp_hardware_io_port_irq_enable_idx},
  {typeid(struct ble_msg_hardware_io_port_irq_direction_rsp_t), ble_rsp_hardware_io_port_irq_direction_idx},
  {typeid(struct ble_msg_hardware_analog_comparator_read_rsp_t), ble_rsp_hardware_analog_comparator_read_idx},
  {typeid(struct ble_msg_hardware_analog_comparator_config_irq_rsp_t), ble_rsp_hardware_analog_comparator_config_irq_idx},
  {typeid(struct ble_msg_hardware_usb_enable_rsp_t), ble_rsp_hardware_usb_enable_idx},
  {typeid(struct ble_msg_test_phy_tx_cmd_t), ble_cmd_test_phy_tx_idx},
  {typeid(struct ble_msg_test_phy_rx_cmd_t), ble_cmd_test_phy_rx_idx},
  {typeid(struct ble_msg_test_debug_cmd_t), ble_cmd_test_debug_idx},
  {typeid(struct ble_msg_test_channel_mode_cmd_t), ble_cmd_test_channel_mode_idx},
  {typeid(struct ble_msg_test_phy_end_rsp_t), ble_rsp_test_phy_end_idx},
  {typeid(struct ble_msg_test_get_channel_map_rsp_t), ble_rsp_test_get_channel_map_idx},
  {typeid(struct ble_msg_test_debug_rsp_t), ble_rsp_test_debug_idx},
  {typeid(struct ble_msg_dfu_reset_cmd_t), ble_cmd_dfu_reset_idx},
  {typeid(struct ble_msg_dfu_flash_set_address_cmd_t), ble_cmd_dfu_flash_set_address_idx},
  {typeid(struct ble_msg_dfu_flash_upload_cmd_t), ble_cmd_dfu_flash_upload_idx},
  {typeid(struct ble_msg_dfu_boot_evt_t), ble_evt_dfu_boot_idx},
  {typeid(struct ble_msg_dfu_flash_set_address_rsp_t), ble_rsp_dfu_flash_set_address_idx},
  {typeid(struct ble_msg_dfu_flash_upload_rsp_t), ble_rsp_dfu_flash_upload_idx},
  {typeid(struct ble_msg_dfu_flash_upload_finish_rsp_t), ble_rsp_dfu_flash_upload_finish_idx},
};

class Ble
{
  const struct ble_msg* findMsgHdr(struct ble_header hdr);
  const struct ble_msg* getMsg(uint8 idx);

  void sendMessage(uint8 msgid, ...);
  void setMsgHandler(uint8 idx, const MsgHandlerPtr& handler);

  virtual void rawWrite(uint8 len1, const uint8* data1, uint16 len2, const uint8* data2) = 0;

  /**Reset device**/
  virtual void rspSystemReset(const void *nul) {}

  /**Hello - command for testing**/
  virtual void rspSystemHello(const void *nul) {}

  /**Get device bluetooth address**/
  virtual void rspSystemAddressGet(const struct ble_msg_system_address_get_rsp_t *msg) {}

  /**write register**/
  virtual void rspSystemRegWrite(const struct ble_msg_system_reg_write_rsp_t *msg) {}

  /**read register**/
  virtual void rspSystemRegRead(const struct ble_msg_system_reg_read_rsp_t *msg) {}

  /**get and reset packet counters**/
  virtual void rspSystemGetCounters(const struct ble_msg_system_get_counters_rsp_t *msg) {}

  /**Get status from all connections**/
  virtual void rspSystemGetConnections(const struct ble_msg_system_get_connections_rsp_t *msg) {}

  /**Read Memory**/
  virtual void rspSystemReadMemory(const struct ble_msg_system_read_memory_rsp_t *msg) {}

  /**Get Device info**/
  virtual void rspSystemGetInfo(const struct ble_msg_system_get_info_rsp_t *msg) {}

  /**Send data to endpoint, error is returned if endpoint does not have enough space**/
  virtual void rspSystemEndpointTx(const struct ble_msg_system_endpoint_tx_rsp_t *msg) {}

  /**Add entry to whitelist**/
  virtual void rspSystemWhitelistAppend(const struct ble_msg_system_whitelist_append_rsp_t *msg) {}

  /**Remove entry from whitelist**/
  virtual void rspSystemWhitelistRemove(const struct ble_msg_system_whitelist_remove_rsp_t *msg) {}

  /**Add entry to whitelist**/
  virtual void rspSystemWhitelistClear(const void *nul) {}

  /**Read data from endpoint, error is returned if endpoint does not have enough data.**/
  virtual void rspSystemEndpointRx(const struct ble_msg_system_endpoint_rx_rsp_t *msg) {}

  /**Set watermarks on both input and output side**/
  virtual void rspSystemEndpointSetWatermarks(const struct ble_msg_system_endpoint_set_watermarks_rsp_t *msg) {}

  /**Set encryption key that will be used with aes_encrypt and aes_decrypt commands.**/
  virtual void rspSystemAesSetkey(const void *nul) {}

  /**Encrypt one block of data**/
  virtual void rspSystemAesEncrypt(const struct ble_msg_system_aes_encrypt_rsp_t *msg) {}

  /**Decrypt one block of data**/
  virtual void rspSystemAesDecrypt(const struct ble_msg_system_aes_decrypt_rsp_t *msg) {}

  /**Defragment persistent store**/
  virtual void rspFlashPsDefrag(const void *nul) {}

  /**Dump all ps keys**/
  virtual void rspFlashPsDump(const void *nul) {}

  /**erase all ps keys**/
  virtual void rspFlashPsEraseAll(const void *nul) {}

  /**save ps key**/
  virtual void rspFlashPsSave(const struct ble_msg_flash_ps_save_rsp_t *msg) {}

  /**load ps key**/
  virtual void rspFlashPsLoad(const struct ble_msg_flash_ps_load_rsp_t *msg) {}

  /**erase ps key**/
  virtual void rspFlashPsErase(const void *nul) {}

  /**erase flash page**/
  virtual void rspFlashErasePage(const struct ble_msg_flash_erase_page_rsp_t *msg) {}

  /**write data to user data area**/
  virtual void rspFlashWriteData(const struct ble_msg_flash_write_data_rsp_t *msg) {}

  /**read data from user data area.**/
  virtual void rspFlashReadData(const struct ble_msg_flash_read_data_rsp_t *msg) {}

  /**Write to attribute database**/
  virtual void rspAttributesWrite(const struct ble_msg_attributes_write_rsp_t *msg) {}

  /**Read from attribute database**/
  virtual void rspAttributesRead(const struct ble_msg_attributes_read_rsp_t *msg) {}

  /**Read attribute type from database**/
  virtual void rspAttributesReadType(const struct ble_msg_attributes_read_type_rsp_t *msg) {}

  /**Respond to user attribute read request**/
  virtual void rspAttributesUserReadResponse(const void *nul) {}

  /**Response to attribute_changed event where reason is user-attribute write.**/
  virtual void rspAttributesUserWriteResponse(const void *nul) {}

  /**Send notification or indication to remote device.**/
  virtual void rspAttributesSend(const struct ble_msg_attributes_send_rsp_t *msg) {}

  /**Disconnect connection, starts a disconnection procedure on connection**/
  virtual void rspConnectionDisconnect(const struct ble_msg_connection_disconnect_rsp_t *msg) {}

  /**Get Link RSSI**/
  virtual void rspConnectionGetRssi(const struct ble_msg_connection_get_rssi_rsp_t *msg) {}

  /**Update connection parameters**/
  virtual void rspConnectionUpdate(const struct ble_msg_connection_update_rsp_t *msg) {}

  /**Request version exchange**/
  virtual void rspConnectionVersionUpdate(const struct ble_msg_connection_version_update_rsp_t *msg) {}

  /**Get Current channel map**/
  virtual void rspConnectionChannelMapGet(const struct ble_msg_connection_channel_map_get_rsp_t *msg) {}

  /**Set Channel map**/
  virtual void rspConnectionChannelMapSet(const struct ble_msg_connection_channel_map_set_rsp_t *msg) {}

  /**Remote feature request**/
  virtual void rspConnectionFeaturesGet(const struct ble_msg_connection_features_get_rsp_t *msg) {}

  /**Get Connection Status Parameters**/
  virtual void rspConnectionGetStatus(const struct ble_msg_connection_get_status_rsp_t *msg) {}

  /**Raw TX**/
  virtual void rspConnectionRawTx(const struct ble_msg_connection_raw_tx_rsp_t *msg) {}

  /**Discover attributes by type and value**/
  virtual void rspAttclientFindByTypeValue(const struct ble_msg_attclient_find_by_type_value_rsp_t *msg) {}

  /**Discover attributes by type and value**/
  virtual void rspAttclientReadByGroupType(const struct ble_msg_attclient_read_by_group_type_rsp_t *msg) {}

  /**Read all attributes where type matches**/
  virtual void rspAttclientReadByType(const struct ble_msg_attclient_read_by_type_rsp_t *msg) {}

  /**Discover Attribute handle and type mappings**/
  virtual void rspAttclientFindInformation(const struct ble_msg_attclient_find_information_rsp_t *msg) {}

  /**Read Characteristic value using handle**/
  virtual void rspAttclientReadByHandle(const struct ble_msg_attclient_read_by_handle_rsp_t *msg) {}

  /**write data to attribute**/
  virtual void rspAttclientAttributeWrite(const struct ble_msg_attclient_attribute_write_rsp_t *msg) {}

  /**write data to attribute using ATT write command**/
  virtual void rspAttclientWriteCommand(const struct ble_msg_attclient_write_command_rsp_t *msg) {}

  /**Send confirmation for received indication, use only if manual indications are enabled**/
  virtual void rspAttclientIndicateConfirm(const struct ble_msg_attclient_indicate_confirm_rsp_t *msg) {}

  /**Read Long Characteristic value**/
  virtual void rspAttclientReadLong(const struct ble_msg_attclient_read_long_rsp_t *msg) {}

  /**Send prepare write request to remote host**/
  virtual void rspAttclientPrepareWrite(const struct ble_msg_attclient_prepare_write_rsp_t *msg) {}

  /**Send prepare write request to remote host**/
  virtual void rspAttclientExecuteWrite(const struct ble_msg_attclient_execute_write_rsp_t *msg) {}

  /**Read multiple attributes from server**/
  virtual void rspAttclientReadMultiple(const struct ble_msg_attclient_read_multiple_rsp_t *msg) {}

  /**Enable encryption on link**/
  virtual void rspSmEncryptStart(const struct ble_msg_sm_encrypt_start_rsp_t *msg) {}

  /**Set device to bondable mode**/
  virtual void rspSmSetBondableMode(const void *nul) {}

  /**delete bonding information from ps store**/
  virtual void rspSmDeleteBonding(const struct ble_msg_sm_delete_bonding_rsp_t *msg) {}

  /**set pairing requirements**/
  virtual void rspSmSetParameters(const void *nul) {}

  /**Passkey entered**/
  virtual void rspSmPasskeyEntry(const struct ble_msg_sm_passkey_entry_rsp_t *msg) {}

  /**List all bonded devices**/
  virtual void rspSmGetBonds(const struct ble_msg_sm_get_bonds_rsp_t *msg) {}

  /**
                      Set out-of-band encryption data for device
                      Device does not allow any other kind of pairing except oob if oob data is set.
                      **/
  virtual void rspSmSetOobData(const void *nul) {}

  /**
                                  Add all bonded devices with a known public or static address to
                                  the whitelist. Previous entries in the whitelist are first
                                  cleared. This command can't be used while advertising, scanning
                                  or being connected.
                          **/
  virtual void rspSmWhitelistBonds(const struct ble_msg_sm_whitelist_bonds_rsp_t *msg) {}

  /**Set GAP central/peripheral privacy flags
                          **/
  virtual void rspGapSetPrivacyFlags(const void *nul) {}

  /**Set discoverable and connectable mode**/
  virtual void rspGapSetMode(const struct ble_msg_gap_set_mode_rsp_t *msg) {}

  /**start or stop discover procedure**/
  virtual void rspGapDiscover(const struct ble_msg_gap_discover_rsp_t *msg) {}

  /**Direct connection**/
  virtual void rspGapConnectDirect(const struct ble_msg_gap_connect_direct_rsp_t *msg) {}

  /**End current GAP procedure**/
  virtual void rspGapEndProcedure(const struct ble_msg_gap_end_procedure_rsp_t *msg) {}

  /**Connect to any device on whitelist**/
  virtual void rspGapConnectSelective(const struct ble_msg_gap_connect_selective_rsp_t *msg) {}

  /**Set scan and advertising filtering parameters**/
  virtual void rspGapSetFiltering(const struct ble_msg_gap_set_filtering_rsp_t *msg) {}

  /**Set scan parameters**/
  virtual void rspGapSetScanParameters(const struct ble_msg_gap_set_scan_parameters_rsp_t *msg) {}

  /**Set advertising parameters**/
  virtual void rspGapSetAdvParameters(const struct ble_msg_gap_set_adv_parameters_rsp_t *msg) {}

  /**Set advertisement or scan response data. Use broadcast mode to advertise data**/
  virtual void rspGapSetAdvData(const struct ble_msg_gap_set_adv_data_rsp_t *msg) {}

  /**Sets device to Directed Connectable Mode
                         Uses fast advertisement procedure for 1.28s after which device enters nonconnectable mode.
                         If device has valid reconnection characteristic value, it is used for connection
                         otherwise passed address and address type are used
             **/
  virtual void rspGapSetDirectedConnectableMode(const struct ble_msg_gap_set_directed_connectable_mode_rsp_t *msg) {}

  /**Configure I/O-port interrupts**/
  virtual void rspHardwareIoPortConfigIrq(const struct ble_msg_hardware_io_port_config_irq_rsp_t *msg) {}

  /**Set soft timer to send events**/
  virtual void rspHardwareSetSoftTimer(const struct ble_msg_hardware_set_soft_timer_rsp_t *msg) {}

  /**Read A/D conversion**/
  virtual void rspHardwareAdcRead(const struct ble_msg_hardware_adc_read_rsp_t *msg) {}

  /**Configure I/O-port direction**/
  virtual void rspHardwareIoPortConfigDirection(const struct ble_msg_hardware_io_port_config_direction_rsp_t *msg) {}

  /**Configure I/O-port function**/
  virtual void rspHardwareIoPortConfigFunction(const struct ble_msg_hardware_io_port_config_function_rsp_t *msg) {}

  /**Configure I/O-port pull-up/pull-down**/
  virtual void rspHardwareIoPortConfigPull(const struct ble_msg_hardware_io_port_config_pull_rsp_t *msg) {}

  /**Write I/O-port**/
  virtual void rspHardwareIoPortWrite(const struct ble_msg_hardware_io_port_write_rsp_t *msg) {}

  /**Read I/O-port**/
  virtual void rspHardwareIoPortRead(const struct ble_msg_hardware_io_port_read_rsp_t *msg) {}

  /**Configure SPI**/
  virtual void rspHardwareSpiConfig(const struct ble_msg_hardware_spi_config_rsp_t *msg) {}

  /**Transfer SPI data**/
  virtual void rspHardwareSpiTransfer(const struct ble_msg_hardware_spi_transfer_rsp_t *msg) {}

  /**Read data from I2C bus using bit-bang in cc2540. I2C clk is in P1.7 data in P1.6. Pull-up must be enabled on pins**/
  virtual void rspHardwareI2cRead(const struct ble_msg_hardware_i2c_read_rsp_t *msg) {}

  /**Write data to I2C bus using bit-bang in cc2540. I2C clk is in P1.7 data in P1.6. Pull-up must be enabled on pins**/
  virtual void rspHardwareI2cWrite(const struct ble_msg_hardware_i2c_write_rsp_t *msg) {}

  /**Set TX Power**/
  virtual void rspHardwareSetTxpower(const void *nul) {}

  /**Set comparator for timer channel**/
  virtual void rspHardwareTimerComparator(const struct ble_msg_hardware_timer_comparator_rsp_t *msg) {}

  /**Enable I/O-port interrupts**/
  virtual void rspHardwareIoPortIrqEnable(const struct ble_msg_hardware_io_port_irq_enable_rsp_t *msg) {}

  /**Set I/O-port interrupt direction**/
  virtual void rspHardwareIoPortIrqDirection(const struct ble_msg_hardware_io_port_irq_direction_rsp_t *msg) {}

  /**Enable or disable analog comparator**/
  virtual void rspHardwareAnalogComparatorEnable(const void *nul) {}

  /**Read analog comparator output**/
  virtual void rspHardwareAnalogComparatorRead(const struct ble_msg_hardware_analog_comparator_read_rsp_t *msg) {}

  /**Configure analog comparator interrupt**/
  virtual void rspHardwareAnalogComparatorConfigIrq(const struct ble_msg_hardware_analog_comparator_config_irq_rsp_t *msg) {}

  /**Set RX Sensitivity**/
  virtual void rspHardwareSetRxgain(const void *nul) {}

  /**Enable or disable USB**/
  virtual void rspHardwareUsbEnable(const struct ble_msg_hardware_usb_enable_rsp_t *msg) {}

  /**Start packet transmission, send one packet at every 625us**/
  virtual void rspTestPhyTx(const void *nul) {}

  /**Start receive test**/
  virtual void rspTestPhyRx(const void *nul) {}

  /**End test, and report received packets**/
  virtual void rspTestPhyEnd(const struct ble_msg_test_phy_end_rsp_t *msg) {}

  /**Reset test**/
  virtual void rspTestPhyReset(const void *nul) {}

  /**Get current channel quality map**/
  virtual void rspTestGetChannelMap(const struct ble_msg_test_get_channel_map_rsp_t *msg) {}

  /**Debug command**/
  virtual void rspTestDebug(const struct ble_msg_test_debug_rsp_t *msg) {}

  /**Set channel quality measurement mode**/
  virtual void rspTestChannelMode(const void *nul) {}

  /**Reset system**/
  virtual void rspDfuReset(const void *nul) {}

  /**set address for flashing**/
  virtual void rspDfuFlashSetAddress(const struct ble_msg_dfu_flash_set_address_rsp_t *msg) {}

  /**Upload binary for flashing. Address will be updated automatically.**/
  virtual void rspDfuFlashUpload(const struct ble_msg_dfu_flash_upload_rsp_t *msg) {}

  /**Uploading is finished.**/
  virtual void rspDfuFlashUploadFinish(const struct ble_msg_dfu_flash_upload_finish_rsp_t *msg) {}

  /**Device booted up, and is ready to receive commands**/
  virtual void evtSystemBoot(const struct ble_msg_system_boot_evt_t *msg) {}

  /**Debug data output**/
  virtual void evtSystemDebug(const struct ble_msg_system_debug_evt_t *msg) {}

  /**Endpoint receive size crossed watermark**/
  virtual void evtSystemEndpointWatermarkRx(const struct ble_msg_system_endpoint_watermark_rx_evt_t *msg) {}

  /**Endpoint transmit size crossed watermark**/
  virtual void evtSystemEndpointWatermarkTx(const struct ble_msg_system_endpoint_watermark_tx_evt_t *msg) {}

  /**Script failure detected**/
  virtual void evtSystemScriptFailure(const struct ble_msg_system_script_failure_evt_t *msg) {}

  /**No valid license key found**/
  virtual void evtSystemNoLicenseKey(const void *nul) {}

  /**Protocol error in command parser**/
  virtual void evtSystemProtocolError(const struct ble_msg_system_protocol_error_evt_t *msg) {}

  /**Dump key result**/
  virtual void evtFlashPsKey(const struct ble_msg_flash_ps_key_evt_t *msg) {}

  /**Attribute value has changed**/
  virtual void evtAttributesValue(const struct ble_msg_attributes_value_evt_t *msg) {}

  /**User-backed attribute data requested**/
  virtual void evtAttributesUserReadRequest(const struct ble_msg_attributes_user_read_request_evt_t *msg) {}

  /**attribute status flags has changed**/
  virtual void evtAttributesStatus(const struct ble_msg_attributes_status_evt_t *msg) {}

  /**Connection opened**/
  virtual void evtConnectionStatus(const struct ble_msg_connection_status_evt_t *msg) {}

  /**Remote version information**/
  virtual void evtConnectionVersionInd(const struct ble_msg_connection_version_ind_evt_t *msg) {}

  /**Remote feature information**/
  virtual void evtConnectionFeatureInd(const struct ble_msg_connection_feature_ind_evt_t *msg) {}

  /**Raw RX**/
  virtual void evtConnectionRawRx(const struct ble_msg_connection_raw_rx_evt_t *msg) {}

  /**Link Disconnected**/
  virtual void evtConnectionDisconnected(const struct ble_msg_connection_disconnected_evt_t *msg) {}

  /**Attribute indication has been acknowledged**/
  virtual void evtAttclientIndicated(const struct ble_msg_attclient_indicated_evt_t *msg) {}

  /**GATT procedure completed**/
  virtual void evtAttclientProcedureCompleted(const struct ble_msg_attclient_procedure_completed_evt_t *msg) {}

  /**group discovery return**/
  virtual void evtAttclientGroupFound(const struct ble_msg_attclient_group_found_evt_t *msg) {}

  /**characteristics found**/
  virtual void evtAttclientAttributeFound(const struct ble_msg_attclient_attribute_found_evt_t *msg) {}

  /**Handle type mapping found**/
  virtual void evtAttclientFindInformationFound(const struct ble_msg_attclient_find_information_found_evt_t *msg) {}

  /**attribute value returned**/
  virtual void evtAttclientAttributeValue(const struct ble_msg_attclient_attribute_value_evt_t *msg) {}

  /**Response to read multiple request**/
  virtual void evtAttclientReadMultipleResponse(const struct ble_msg_attclient_read_multiple_response_evt_t *msg) {}

  /**SMP data output**/
  virtual void evtSmSmpData(const struct ble_msg_sm_smp_data_evt_t *msg) {}

  /**Link bonding has failed**/
  virtual void evtSmBondingFail(const struct ble_msg_sm_bonding_fail_evt_t *msg) {}

  /**Passkey to be entered to remote device**/
  virtual void evtSmPasskeyDisplay(const struct ble_msg_sm_passkey_display_evt_t *msg) {}

  /**Passkey Requested**/
  virtual void evtSmPasskeyRequest(const struct ble_msg_sm_passkey_request_evt_t *msg) {}

  /**Bond status information**/
  virtual void evtSmBondStatus(const struct ble_msg_sm_bond_status_evt_t *msg) {}

  /**Scan Response**/
  virtual void evtGapScanResponse(const struct ble_msg_gap_scan_response_evt_t *msg) {}

  /**Not used**/
  virtual void evtGapModeChanged(const struct ble_msg_gap_mode_changed_evt_t *msg) {}

  /**I/O-port state**/
  virtual void evtHardwareIoPortStatus(const struct ble_msg_hardware_io_port_status_evt_t *msg) {}

  /**soft timer event**/
  virtual void evtHardwareSoftTimer(const struct ble_msg_hardware_soft_timer_evt_t *msg) {}

  /**adc result**/
  virtual void evtHardwareAdcResult(const struct ble_msg_hardware_adc_result_evt_t *msg) {}

  /**This event is produced when analog comparator output changes**/
  virtual void evtHardwareAnalogComparatorStatus(const struct ble_msg_hardware_analog_comparator_status_evt_t *msg) {}

  /**Device booted up in dfu, and is ready to receive commands**/
  virtual void evtDfuBoot(const struct ble_msg_dfu_boot_evt_t *msg) {}

public:
  Ble();
  virtual ~Ble() {}

  template <typename T>
  void setMsgHandler(const MsgHandlerFunc<T>& func)
  {
    uint8 idx = ble_msg_typeid.find(typeid(T))->second;
    MsgHandlerPtr handler = std::make_shared<MsgHandlerT<T>>(func);

    setMsgHandler(idx, handler);
  }

  const struct ble_msg* getMsgHdr(struct ble_header hdr);

  /**Reset device**/
  inline void cmdSystemReset(uint8 boot_in_dfu) {sendMessage (ble_cmd_system_reset_idx, boot_in_dfu);}
  /**Hello - command for testing**/
  inline void cmdSystemHello() { sendMessage (ble_cmd_system_hello_idx); }
  /**Get device bluetooth address**/
  inline void cmdSystemAddressGet() { sendMessage (ble_cmd_system_address_get_idx); }

  // /**write register**/
  // #define cmdSystemRegWrite(address,value) { sendMessage (ble_cmd_system_reg_write_idx,address,value); }
  // /**read register**/
  // #define cmdSystemRegRead(address) { sendMessage (ble_cmd_system_reg_read_idx,address); }

  /**get and reset packet counters**/
  inline void cmdSystemGetCounters() { sendMessage (ble_cmd_system_get_counters_idx); }
  /**Get status from all connections**/
  inline void cmdSystemGetConnections() { sendMessage (ble_cmd_system_get_connections_idx); }

  // /**Read Memory**/
  // #define cmdSystemReadMemory(address,length) { sendMessage (ble_cmd_system_read_memory_idx,address,length); }

  /**Get Device info**/
  inline void cmdSystemGetInfo() { sendMessage (ble_cmd_system_get_info_idx); }
  /**Send data to endpoint, error is returned if endpoint does not have enough space**/
  inline void cmdSystemEndpointTx(uint8 endpoint, uint8 data_len, const uint8* data_data) { sendMessage (ble_cmd_system_endpoint_tx_idx,endpoint,data_len,data_data); }
  /**Add entry to whitelist**/
  inline void cmdSystemWhitelistAppend(bd_addr address, uint8 address_type) { sendMessage (ble_cmd_system_whitelist_append_idx,address,address_type); }
  /**Remove entry from whitelist**/
  inline void cmdSystemWhitelistRemove(bd_addr address, uint8 address_type) { sendMessage (ble_cmd_system_whitelist_remove_idx,address,address_type); }
  /**Add entry to whitelist**/
  inline void cmdSystemWhitelistClear() { sendMessage (ble_cmd_system_whitelist_clear_idx); }
  /**Read data from endpoint, error is returned if endpoint does not have enough data.**/
  inline void cmdSystemEndpointRx(uint8 endpoint, uint8 size) { sendMessage (ble_cmd_system_endpoint_rx_idx,endpoint,size); }
  /**Set watermarks on both input and output side**/
  inline void cmdSystemEndpointSetWatermarks(uint8 endpoint, uint8 rx, uint8 tx) { sendMessage (ble_cmd_system_endpoint_set_watermarks_idx,endpoint,rx,tx); }
  /**Set encryption key that will be used with aes_encrypt and aes_decrypt commands.**/
  inline void cmdSystemAesSetkey(uint8 key_len, const uint8* key_data) { sendMessage (ble_cmd_system_aes_setkey_idx,key_len,key_data); }
  /**Encrypt one block of data**/
  inline void cmdSystemAesEncrypt(uint8 data_len, const uint8* data_data) { sendMessage (ble_cmd_system_aes_encrypt_idx,data_len,data_data); }
  /**Decrypt one block of data**/
  inline void cmdSystemAesDecrypt(uint8 data_len, const uint8* data_data) { sendMessage (ble_cmd_system_aes_decrypt_idx,data_len,data_data); }
  /**Defragment persistent store**/
  inline void cmdFlashPsDefrag() { sendMessage (ble_cmd_flash_ps_defrag_idx); }
  /**Dump all ps keys**/
  inline void cmdFlashPsDump() { sendMessage (ble_cmd_flash_ps_dump_idx); }
  /**erase all ps keys**/
  inline void cmdFlashPsEraseAll() { sendMessage (ble_cmd_flash_ps_erase_all_idx); }
  /**save ps key**/
  inline void cmdFlashPsSave(uint16 key, uint8 value_len, const uint8* value_data) { sendMessage (ble_cmd_flash_ps_save_idx,key,value_len,value_data); }
  /**load ps key**/
  inline void cmdFlashPsLoad(uint16 key) { sendMessage (ble_cmd_flash_ps_load_idx,key); }
  /**erase ps key**/
  inline void cmdFlashPsErase(uint16 key) { sendMessage (ble_cmd_flash_ps_erase_idx,key); }
  /**erase flash page**/
  inline void cmdFlashErasePage(uint8 page) { sendMessage (ble_cmd_flash_erase_page_idx,page); }
  /**write data to user data area**/
  inline void cmdFlashWriteData(uint32 address, uint8 data_len, const uint8* data_data) { sendMessage (ble_cmd_flash_write_data_idx,address,data_len,data_data); }
  /**read data from user data area.**/
  inline void cmdFlashReadData(uint32 address, uint8 length) { sendMessage (ble_cmd_flash_read_data_idx,address,length); }
  /**Write to attribute database**/
  inline void cmdAttributesWrite(uint16 handle, uint8 offset, uint8 value_len, const uint8* value_data) { sendMessage (ble_cmd_attributes_write_idx,handle,offset,value_len,value_data); }
  /**Read from attribute database**/
  inline void cmdAttributesRead(uint16 handle, uint16 offset) { sendMessage (ble_cmd_attributes_read_idx,handle,offset); }
  /**Read attribute type from database**/
  inline void cmdAttributesReadType(uint16 handle) { sendMessage (ble_cmd_attributes_read_type_idx,handle); }
  /**Respond to user attribute read request**/
  inline void cmdAttributesUserReadResponse(uint8 connection, uint8 att_error, uint8 value_len, const uint8* value_data) { sendMessage (ble_cmd_attributes_user_read_response_idx,connection,att_error,value_len,value_data); }
  /**Response to attribute_changed event where reason is user-attribute write.**/
  inline void cmdAttributesUserWriteResponse(uint8 connection, uint8 att_error) { sendMessage (ble_cmd_attributes_user_write_response_idx,connection,att_error); }
  /**Send notification or indication to remote device.**/
  inline void cmdAttributesSend(uint8 connection, uint16 handle, uint8 value_len,const uint8* value_data) { sendMessage (ble_cmd_attributes_send_idx,connection,handle,value_len,value_data); }
  /**Disconnect connection, starts a disconnection procedure on connection**/
  inline void cmdConnectionDisconnect(uint8 connection) { sendMessage (ble_cmd_connection_disconnect_idx,connection); }
  /**Get Link RSSI**/
  inline void cmdConnectionGetRssi(uint8 connection) { sendMessage (ble_cmd_connection_get_rssi_idx,connection); }
  /**Update connection parameters**/
  inline void cmdConnectionUpdate(uint8 connection, uint16 interval_min, uint16 interval_max, uint16 latency, uint16 timeout) { sendMessage (ble_cmd_connection_update_idx,connection,interval_min,interval_max,latency,timeout); }
  /**Request version exchange**/
  inline void cmdConnectionVersionUpdate(uint8 connection) { sendMessage (ble_cmd_connection_version_update_idx,connection); }

  // /**Get Current channel map**/
  // inline void cmdConnectionChannelMapGet(uint8 connection) { sendMessage (ble_cmd_connection_channel_map_get_idx,connection); }
  // /**Set Channel map**/
  // inline void cmdConnectionChannelMapSet(uint8 connection, uint8 map_len, map_data) { sendMessage (ble_cmd_connection_channel_map_set_idx,connection,map_len,map_data); }
  // /**Remote feature request**/
  // inline void cmdConnectionFeaturesGet(uint8 connection) { sendMessage (ble_cmd_connection_features_get_idx,connection); }
  // /**Get Connection Status Parameters**/
  // inline void cmdConnectionGetStatus(uint8 connection) { sendMessage (ble_cmd_connection_get_status_idx,connection); }
  // /**Raw TX**/
  // inline void cmdConnectionRawTx(uint8 connection, uint8 data_len, const uint8* data_data) { sendMessage (ble_cmd_connection_raw_tx_idx,connection,data_len,data_data); }

  /**Discover attributes by type and value**/
  inline void cmdAttclientFindByTypeValue(uint8 connection, uint16 start, uint16 end, uint16 uuid, uint8 value_len, const uint8* value_data) { sendMessage (ble_cmd_attclient_find_by_type_value_idx,connection,start,end,uuid,value_len,value_data); }
  /**Discover attributes by type and value**/
  inline void cmdAttclientReadByGroupType(uint8 connection, uint16 start, uint16 end, uint8 uuid_len, const uint8* uuid_data) { sendMessage (ble_cmd_attclient_read_by_group_type_idx,connection,start,end,uuid_len,uuid_data); }
  /**Read all attributes where type matches**/
  inline void cmdAttclientReadByType(uint8 connection, uint16 start, uint16 end, uint8 uuid_len, const uint8* uuid_data) { sendMessage (ble_cmd_attclient_read_by_type_idx,connection,start,end,uuid_len,uuid_data); }
  /**Discover Attribute handle and type mappings**/
  inline void cmdAttclientFindInformation(uint8 connection, uint16 start, uint16 end) { sendMessage (ble_cmd_attclient_find_information_idx,connection,start,end); }
  /**Read Characteristic value using handle**/
  inline void cmdAttclientReadByHandle(uint8 connection, uint16 chrhandle) { sendMessage (ble_cmd_attclient_read_by_handle_idx,connection,chrhandle); }
  /**write data to attribute**/
  inline void cmdAttclientAttributeWrite(uint8 connection, uint16 atthandle, uint8 data_len, const uint8* data_data) { sendMessage (ble_cmd_attclient_attribute_write_idx,connection,atthandle,data_len,data_data); }
  /**write data to attribute using ATT write command**/
  inline void cmdAttclientWriteCommand(uint8 connection, uint16 atthandle, uint8 data_len, const uint8* data_data) { sendMessage (ble_cmd_attclient_write_command_idx,connection,atthandle,data_len,data_data); }
  /**Send confirmation for received indication, use only if manual indications are enabled**/
  inline void cmdAttclientIndicateConfirm(uint8 connection) { sendMessage (ble_cmd_attclient_indicate_confirm_idx,connection); }
  /**Read Long Characteristic value**/
  inline void cmdAttclientReadLong(uint8 connection, uint16 chrhandle) { sendMessage (ble_cmd_attclient_read_long_idx,connection,chrhandle); }
  /**Send prepare write request to remote host**/
  inline void cmdAttclientPrepareWrite(uint8 connection, uint16 atthandle, uint16 offset, uint8 data_len, const uint8* data_data) { sendMessage (ble_cmd_attclient_prepare_write_idx,connection,atthandle,offset,data_len,data_data); }
  /**Send prepare write request to remote host**/
  inline void cmdAttclientExecuteWrite(uint8 connection, uint8 commit) { sendMessage (ble_cmd_attclient_execute_write_idx,connection,commit); }
  /**Read multiple attributes from server**/
  inline void cmdAttclientReadMultiple(uint8 connection, uint8 handles_len, const uint8* handles_data) { sendMessage (ble_cmd_attclient_read_multiple_idx,connection,handles_len,handles_data); }
  /**Enable encryption on link**/
  inline void cmdSmEncryptStart(uint8 handle, uint8 bonding) { sendMessage (ble_cmd_sm_encrypt_start_idx,handle,bonding); }
  /**Set device to bondable mode**/
  inline void cmdSmSetBondableMode(uint8 bondable) { sendMessage (ble_cmd_sm_set_bondable_mode_idx,bondable); }
  /**delete bonding information from ps store**/
  inline void cmdSmDeleteBonding(uint8 handle) { sendMessage (ble_cmd_sm_delete_bonding_idx,handle); }
  /**set pairing requirements**/
  inline void cmdSmSetParameters(uint8 mitm, uint8 min_key_size, uint8 io_capabilities) { sendMessage (ble_cmd_sm_set_parameters_idx,mitm,min_key_size,io_capabilities); }
  /**Passkey entered**/
  inline void cmdSmPasskeyEntry(uint8 handle, uint32 passkey) { sendMessage (ble_cmd_sm_passkey_entry_idx,handle,passkey); }
  /**List all bonded devices**/
  inline void cmdSmGetBonds() { sendMessage (ble_cmd_sm_get_bonds_idx); }
  /**
          Set out-of-band encryption data for device
          Device does not allow any other kind of pairing except oob if oob data is set.
  **/
  inline void cmdSmSetOobData(uint8 oob_len, const uint8* oob_data) { sendMessage (ble_cmd_sm_set_oob_data_idx,oob_len,oob_data); }
  /**
          Add all bonded devices with a known public or static address to
          the whitelist. Previous entries in the whitelist are first
          cleared. This command can't be used while advertising, scanning
          or being connected.
  **/
  inline void cmdSmWhitelistBonds() { sendMessage (ble_cmd_sm_whitelist_bonds_idx); }
  /**Set GAP central/peripheral privacy flags**/
  inline void cmdGapSetPrivacyFlags(uint8 peripheral_privacy, uint8 central_privacy) { sendMessage (ble_cmd_gap_set_privacy_flags_idx,peripheral_privacy,central_privacy); }
  /**Set discoverable and connectable mode**/
  inline void cmdGapSetMode(uint8 discover, uint8 connect) { sendMessage (ble_cmd_gap_set_mode_idx,discover,connect); }
  /**start or stop discover procedure**/
  inline void cmdGapDiscover(uint8 mode) { sendMessage (ble_cmd_gap_discover_idx,mode); }
  /**Direct connection**/
  inline void cmdGapConnectDirect(bd_addr address, uint8 addr_type, uint16 conn_interval_min, uint16 conn_interval_max, uint16 timeout, uint16 latency) { sendMessage (ble_cmd_gap_connect_direct_idx,address,addr_type,conn_interval_min,conn_interval_max,timeout,latency); }
  /**End current GAP procedure**/
  inline void cmdGapEndProcedure() { sendMessage (ble_cmd_gap_end_procedure_idx); }
  /**Connect to any device on whitelist**/
  inline void cmdGapConnectSelective(uint16 conn_interval_min, uint16 conn_interval_max, uint16 timeout, uint16 latency) { sendMessage (ble_cmd_gap_connect_selective_idx,conn_interval_min,conn_interval_max,timeout,latency); }
  /**Set scan and advertising filtering parameters**/
  inline void cmdGapSetFiltering(uint8 scan_policy, uint8 adv_policy, uint8 scan_duplicate_filtering) { sendMessage (ble_cmd_gap_set_filtering_idx,scan_policy,adv_policy,scan_duplicate_filtering); }
  /**Set scan parameters**/
  inline void cmdGapSetScanParameters(uint16 scan_interval, uint16 scan_window, uint8 active) { sendMessage (ble_cmd_gap_set_scan_parameters_idx,scan_interval,scan_window,active); }
  /**Set advertising parameters**/
  inline void cmdGapSetAdvParameters(uint16 adv_interval_min, uint16 adv_interval_max, uint8 adv_channels) { sendMessage (ble_cmd_gap_set_adv_parameters_idx,adv_interval_min,adv_interval_max,adv_channels); }
  /**Set advertisement or scan response data. Use broadcast mode to advertise data**/
  inline void cmdGapSetAdvData(uint8 set_scanrsp, uint8 adv_data_len, const uint8* adv_data_data) { sendMessage (ble_cmd_gap_set_adv_data_idx,set_scanrsp,adv_data_len,adv_data_data); }
  /**Sets device to Directed Connectable Mode
                          Uses fast advertisement procedure for 1.28s after which device enters nonconnectable mode.
                          If device has valid reconnection characteristic value, it is used for connection
                          otherwise passed address and address type are used
  **/
  inline void cmdGapSetDirectedConnectableMode(bd_addr address, uint8 addr_type) { sendMessage (ble_cmd_gap_set_directed_connectable_mode_idx,address,addr_type); }
  /**Configure I/O-port interrupts**/
  inline void cmdHardwareIoPortConfigIrq(uint8 port, uint8 enable_bits, uint8 falling_edge) { sendMessage (ble_cmd_hardware_io_port_config_irq_idx,port,enable_bits,falling_edge); }
  /**Set soft timer to send events**/
  inline void cmdHardwareSetSoftTimer(uint32 time, uint8 handle, uint8 single_shot) { sendMessage (ble_cmd_hardware_set_soft_timer_idx,time,handle,single_shot); }
  /**Read A/D conversion**/
  inline void cmdHardwareAdcRead(uint8 input, uint8 decimation, uint8 reference_selection) { sendMessage (ble_cmd_hardware_adc_read_idx,input,decimation,reference_selection); }
  /**Configure I/O-port direction**/
  inline void cmdHardwareIoPortConfigDirection(uint8 port, uint8 direction) { sendMessage (ble_cmd_hardware_io_port_config_direction_idx,port,direction); }
  /**Configure I/O-port function**/
  inline void cmdHardwareIoPortConfigFunction(uint8 port, uint8 function) { sendMessage (ble_cmd_hardware_io_port_config_function_idx,port,function); }
  /**Configure I/O-port pull-up/pull-down**/
  inline void cmdHardwareIoPortConfigPull(uint8 port, uint8 tristate_mask, uint8 pull_up) { sendMessage (ble_cmd_hardware_io_port_config_pull_idx,port,tristate_mask,pull_up); }
  /**Write I/O-port**/
  inline void cmdHardwareIoPortWrite(uint8 port, uint8 mask, uint8 data) { sendMessage (ble_cmd_hardware_io_port_write_idx,port,mask,data); }
  /**Read I/O-port**/
  inline void cmdHardwareIoPortRead(uint8 port, uint8 mask) { sendMessage (ble_cmd_hardware_io_port_read_idx,port,mask); }
  /**Configure SPI**/
  inline void cmdHardwareSpiConfig(uint8 channel, uint8 polarity, uint8 phase, uint8 bit_order, uint8 baud_e, uint8 baud_m) { sendMessage (ble_cmd_hardware_spi_config_idx,channel,polarity,phase,bit_order,baud_e,baud_m); }
  /**Transfer SPI data**/
  inline void cmdHardwareSpiTransfer(uint8 channel, uint8 data_len, const uint8* data_data) { sendMessage (ble_cmd_hardware_spi_transfer_idx,channel,data_len,data_data); }
  /**Read data from I2C bus using bit-bang in cc2540. I2C clk is in P1.7 data in P1.6. Pull-up must be enabled on pins**/
  inline void cmdHardwareI2cRead(uint8 address, uint8 stop, uint8 length) { sendMessage (ble_cmd_hardware_i2c_read_idx,address,stop,length); }
  /**Write data to I2C bus using bit-bang in cc2540. I2C clk is in P1.7 data in P1.6. Pull-up must be enabled on pins**/
  inline void cmdHardwareI2cWrite(uint8 address, uint8 stop, uint8 data_len, const uint8* data_data) { sendMessage (ble_cmd_hardware_i2c_write_idx,address,stop,data_len,data_data); }
  /**Set TX Power**/
  inline void cmdHardwareSetTxpower(uint8 power) { sendMessage (ble_cmd_hardware_set_txpower_idx,power); }
  /**Set comparator for timer channel**/
  inline void cmdHardwareTimerComparator(uint8 timer, uint8 channel, uint8 mode, uint16 comparator_value) { sendMessage (ble_cmd_hardware_timer_comparator_idx,timer,channel,mode,comparator_value); }
  /**Enable I/O-port interrupts**/
  inline void cmdHardwareIoPortIrqEnable(uint8 port, uint8 enable_bits) { sendMessage (ble_cmd_hardware_io_port_irq_enable_idx,port,enable_bits); }
  /**Set I/O-port interrupt direction**/
  inline void cmdHardwareIoPortIrqDirection(uint8 port, uint8 falling_edge) { sendMessage (ble_cmd_hardware_io_port_irq_direction_idx,port,falling_edge); }
  /**Enable or disable analog comparator**/
  inline void cmdHardwareAnalogComparatorEnable(uint8 enable) { sendMessage (ble_cmd_hardware_analog_comparator_enable_idx,enable); }
  /**Read analog comparator output**/
  inline void cmdHardwareAnalogComparatorRead() { sendMessage (ble_cmd_hardware_analog_comparator_read_idx); }
  /**Configure analog comparator interrupt**/
  inline void cmdHardwareAnalogComparatorConfigIrq(uint8 enabled) { sendMessage (ble_cmd_hardware_analog_comparator_config_irq_idx,enabled); }
  /**Set RX Sensitivity**/
  inline void cmdHardwareSetRxgain(uint8 gain) { sendMessage (ble_cmd_hardware_set_rxgain_idx,gain); }
  /**Enable or disable USB**/
  inline void cmdHardwareUsbEnable(uint8 enable) { sendMessage (ble_cmd_hardware_usb_enable_idx,enable); }
  /**Start packet transmission, send one packet at every 625us**/
  inline void cmdTestPhyTx(uint8 channel, uint8 length, uint8 type) { sendMessage (ble_cmd_test_phy_tx_idx,channel,length,type); }
  /**Start receive test**/
  inline void cmdTestPhyRx(uint8 channel) { sendMessage (ble_cmd_test_phy_rx_idx,channel); }
  /**End test, and report received packets**/
  inline void cmdTestPhyEnd() { sendMessage (ble_cmd_test_phy_end_idx); }
  /**Reset test**/
  inline void cmdTestPhyReset() { sendMessage (ble_cmd_test_phy_reset_idx); }
  /**Get current channel quality map**/
  inline void cmdTestGetChannelMap() { sendMessage (ble_cmd_test_get_channel_map_idx); }

  // /**Debug command**/
  // inline void cmdTestDebug(uint8 input_len, const uint8* input_data) { sendMessage (ble_cmd_test_debug_idx,input_len,input_data); }

  /**Set channel quality measurement mode**/
  inline void cmdTestChannelMode(uint8 mode) { sendMessage (ble_cmd_test_channel_mode_idx,mode); }
  /**Reset system**/
  inline void cmdDfuReset(uint8 dfu) { sendMessage (ble_cmd_dfu_reset_idx,dfu); }
  /**set address for flashing**/
  inline void cmdDfuFlashSetAddress(uint32 address) { sendMessage (ble_cmd_dfu_flash_set_address_idx,address); }
  /**Upload binary for flashing. Address will be updated automatically.**/
  inline void cmdDfuFlashUpload(uint8 data_len, const uint8* data_data) { sendMessage (ble_cmd_dfu_flash_upload_idx,data_len,data_data); }
  /**Uploading is finished.**/
  inline void cmdDfuFlashUploadFinish() { sendMessage (ble_cmd_dfu_flash_upload_finish_idx); }
};

} /* namespace bglib */

#endif

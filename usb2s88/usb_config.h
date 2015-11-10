/*
    example minimal CDC serial port adapter using PIC16F1454 microcontroller

    based on M-Stack by Alan Ott, Signal 11 Software

    culled from USB CDC-ACM Demo (by Alan Ott, Signal 11 Software)
    and ANSI C12.18 optical interface (by Peter Lawrence)

    Copyright (C) 2014,2015 Peter Lawrence

    Permission is hereby granted, free of charge, to any person obtaining a 
    copy of this software and associated documentation files (the "Software"), 
    to deal in the Software without restriction, including without limitation 
    the rights to use, copy, modify, merge, publish, distribute, sublicense, 
    and/or sell copies of the Software, and to permit persons to whom the 
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in 
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
    DEALINGS IN THE SOFTWARE.
*/

#ifndef USB_CONFIG_H__
#define USB_CONFIG_H__

#define NUM_ENDPOINT_NUMBERS 2

/* Only 8, 16, 32 and 64 are supported for endpoint zero length. */
#define EP_0_LEN 8

#define EP_1_OUT_LEN 1
#define EP_1_IN_LEN 10 /* May need to be longer, depending on the notifications you support. */

#define EP_2_LEN     64
#define EP_2_OUT_LEN EP_2_LEN
#define EP_2_IN_LEN  EP_2_LEN

#define NUMBER_OF_CONFIGURATIONS 1

#define PPB_MODE PPB_NONE /* Do not ping-pong any endpoints */

//#define USB_USE_INTERRUPTS

/* Objects from usb_descriptors.c */
#define USB_DEVICE_DESCRIPTOR this_device_descriptor
#define USB_CONFIG_DESCRIPTOR_MAP usb_application_config_descs
#define USB_STRING_DESCRIPTOR_FUNC usb_application_get_string

/* Optional callbacks from usb.c. Leave them commented if you don't want to
   use them. For the prototypes and documentation for each one, see usb.h. */

#define SET_CONFIGURATION_CALLBACK app_set_configuration_callback
#define GET_DEVICE_STATUS_CALLBACK app_get_device_status_callback
#define ENDPOINT_HALT_CALLBACK     app_endpoint_halt_callback
#define SET_INTERFACE_CALLBACK     app_set_interface_callback
#define GET_INTERFACE_CALLBACK     app_get_interface_callback
#define OUT_TRANSACTION_CALLBACK   app_out_transaction_callback
#define IN_TRANSACTION_COMPLETE_CALLBACK   app_in_transaction_complete_callback
#define UNKNOWN_SETUP_REQUEST_CALLBACK app_unknown_setup_request_callback
#define UNKNOWN_GET_DESCRIPTOR_CALLBACK app_unknown_get_descriptor_callback
#define START_OF_FRAME_CALLBACK    app_start_of_frame_callback
#define USB_RESET_CALLBACK         app_usb_reset_callback

/* CDC Configuration functions. See usb_cdc.h for documentation. */
//#define CDC_SEND_ENCAPSULATED_COMMAND_CALLBACK app_send_encapsulated_command
//#define CDC_GET_ENCAPSULATED_RESPONSE_CALLBACK app_get_encapsulated_response
//#define CDC_SET_COMM_FEATURE_CALLBACK app_set_comm_feature_callback
//#define CDC_CLEAR_COMM_FEATURE_CALLBACK app_clear_comm_feature_callback
//#define CDC_GET_COMM_FEATURE_CALLBACK app_get_comm_feature_callback
#define CDC_SET_LINE_CODING_CALLBACK app_set_line_coding_callback
#define CDC_GET_LINE_CODING_CALLBACK app_get_line_coding_callback
//#define CDC_SET_CONTROL_LINE_STATE_CALLBACK app_set_control_line_state_callback
//#define CDC_SEND_BREAK_CALLBACK app_send_break_callback

#endif /* USB_CONFIG_H__ */

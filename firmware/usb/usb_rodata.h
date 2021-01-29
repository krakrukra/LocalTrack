#ifndef USB_RODATA_H
#define USB_RODATA_H

static DeviceDescriptor_TypeDef DeviceDescriptor __attribute__(( aligned(2), section(".rodata,\"a\",%progbits@") )) =
{
  .bLength            = sizeof(DeviceDescriptor_TypeDef),
  .bDescriptorType    = 0x01,//DEVICE descriptor type
  .bcdUSB             = 0x0200,//USB 2.0
  .bDeviceClass       = 0,//class is specified at interface level
  .bDeviceSubClass    = 0,//class is specified at interface level
  .bDeviceProtocol    = 0,//protocol is specified at interface level
  .bMaxPacketSize0    = MAXPACKET_0,//max packet size for EP0 (default = 64 bytes)
  .idVendor           = 0x0951,//Kingston
  .idProduct          = 0x1603,//DataTraveler
  .bcdDevice          = 0x0100,//device version 1.0
  .iManufacturer      = 0,//no symbolic name reserved in string descriptor
  .iProduct           = 0,//no symbolic name reserved in string descriptor
  .iSerialNumber      = 1,//serial number is in StringDescriptor_1
  .bNumConfigurations = 1//only one configuration supported
};

static GetConfigResponse_TypeDef GetConfigResponse __attribute__(( aligned(2), section(".rodata,\"a\",%progbits@") )) =
{
  .ConfigurationDescriptor_1 =
  {
    .bLength                = sizeof(ConfigurationDescriptor_TypeDef),
    .bDescriptorType        = 0x02,//CONFIGURATION descriptor type
    .wTotalLength           = sizeof(GetConfigResponse_TypeDef),
    .bNumInterfaces         = 1,//1 interface supported in this configuration
    .bConfigurationValue    = 1,//configuration number is 1
    .iConfiguration         = 0,//no symbolic name reserved in string descriptor
    .bmAttributes           = 0x80,//not self-powered, no remote wakeup support
    .bMaxPower              = 45//45 * 2mA = 90mA max current consumption
  },
  .InterfaceDescriptor_0 =
  {
    .bLength                = sizeof(InterfaceDescriptor_TypeDef),
    .bDescriptorType        = 0x04,//INTERFACE descriptor type
    .bInterfaceNumber       = 0,//interface number is 0
    .bAlternateSetting      = 0,//alternate setting 0
    .bNumEndpoints          = 2,//two endpoints in this interface
    .bInterfaceClass        = 0x08,//MSD class
    .bInterfaceSubClass     = 0x06,//SCSI transparent command set
    .bInterfaceProtocol     = 0x50,//BULK-ONLY transport
    .iInterface             = 0x00//no symbolic name reserved in string descriptor
  },
  .EndpointDescriptor_2_OUT =
  {
    .bLength                = sizeof(EndpointDescriptor_TypeDef),
    .bDescriptorType        = 0x05,//ENDPOINT descriptor type
    .bEndpointAddress       = 0x02,//EP2_OUT endpoint
    .bmAttributes           = 0x02,//BULK endpoint
    .wMaxPacketSize         = MAXPACKET_MSD,//max packet size for EP2 (default = 64 bytes)
    .bInterval              = 0x00//this field is not used
  },
  .EndpointDescriptor_3_IN =
  {
    .bLength                = sizeof(EndpointDescriptor_TypeDef),
    .bDescriptorType        = 0x05,//ENDPOINT descriptor type
    .bEndpointAddress       = 0x83,//EP3_IN endpoint
    .bmAttributes           = 0x02,//BULK endpoint
    .wMaxPacketSize         = MAXPACKET_MSD,//max packet size for EP3 (default = 64 bytes)
    .bInterval              = 0x00//this field is not used
  }
  
};

//string descriptors. only english (0x0409) is present in supported LANGID list
static unsigned short StringDescriptor_0[2] __attribute__(( aligned(2), section(".rodata,\"a\",%progbits@") )) = { 0x0304, 0x0409 };
static unsigned short StringDescriptor_1[13] __attribute__(( aligned(2), section(".rodata,\"a\",%progbits@") )) = { 0x031A, '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C' };

#endif //USB_RODATA_H

/*
 * usb_desc.c - USB Descriptor
 */

#include <stdint.h>
#include <string.h>

#include "config.h"

#include "sys.h"
#include "usb_lld.h"
#include "usb_conf.h"
#include "usb-cdc.h"


/* HID report descriptor.  */
#define HID_REPORT_DESC_SIZE (sizeof (hid_report_desc))

static const uint8_t hid_report_desc[] = {
  0x05, 0x01,	    /* USAGE_PAGE (Generic Desktop) */
  0x09, 0x07,	    /* USAGE (Keypad) */
  0xa1, 0x01,	    /* COLLECTION (Application) */
  0x05, 0x07,	    /*   USAGE_PAGE (Key Codes) */
  0x19, 0xe0,	    /*   USAGE_MINIMUM (Keyboard LeftControl) */
  0x29, 0xe7,	    /*   USAGE_MAXIMUM (Keyboard Right GUI) */
  0x15, 0x00,	    /*   LOGICAL_MINIMUM (0) */
  0x25, 0x01,	    /*   LOGICAL_MAXIMUM (1) */
  0x75, 0x01,	    /*   REPORT_SIZE (1) */
  0x95, 0x08,	    /*   REPORT_COUNT (8) */
  0x81, 0x02,	    /*   INPUT (Data,Var,Abs); Modifier byte */

  /*
   * NumLock, CapsLock, ScrollLock, Compose, Kana, Power, Shift,
   * Do Not Disturb
   */
  0x05, 0x08,	    /*   USAGE_PAGE (LEDs) */
  0x19, 0x01,	    /*   USAGE_MINIMUM (1) */
  0x29, 0x08,	    /*   USAGE_MAXIMUM (8) */
  0x95, 0x08,	    /*   REPORT_COUNT (8) */
  0x75, 0x01,	    /*   REPORT_SIZE (1) */
  0x91, 0x02,	    /*   OUTPUT (Data,Var,Abs); LED report */

  0x05, 0x07,	    /*   USAGE_PAGE (Key Codes) */
  0x19, 0x00,	    /*   USAGE_MINIMUM (Reserved (no event indicated)) */
  0x29, 0x65,	    /*   USAGE_MAXIMUM (Keyboard Application) */
  0x15, 0x00,	    /*   LOGICAL_MINIMUM (0) */
  0x25, 0x65,	    /*   LOGICAL_MAXIMUM (101) */
  0x95, 0x06,	    /*   REPORT_COUNT (1) */
  0x75, 0x08,	    /*   REPORT_SIZE (8) */
  0x81, 0x00,	    /*   INPUT (Data,Ary,Abs); Key arrays (1 bytes) */
  0xc0		    /* END_COLLECTION */
};

>>>>>>> hid
#define USB_ICC_INTERFACE_CLASS 0x0B
#define USB_ICC_INTERFACE_SUBCLASS 0x00
#define USB_ICC_INTERFACE_BULK_PROTOCOL 0x00
#define USB_ICC_DATA_SIZE 64

/* USB Standard Device Descriptor */
static const uint8_t gnukDeviceDescriptor[] = {
  18,   /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE,     /* bDescriptorType */
  0x10, 0x01,   /* bcdUSB = 1.1 */
  0x00,   /* bDeviceClass: 0 means deferred to interface */
  0x00,   /* bDeviceSubClass */
  0x00,   /* bDeviceProtocol */
  0x40,   /* bMaxPacketSize0 */
#include "usb-vid-pid-ver.c.inc"
  1, /* Index of string descriptor describing manufacturer */
  2, /* Index of string descriptor describing product */
  3, /* Index of string descriptor describing the device's serial number */
  0x01    /* bNumConfigurations */
};

#define ICC_TOTAL_LENGTH (9+9+54+7+7+7)
#define ICC_NUM_INTERFACES 1

#define HID_TOTAL_LENGTH (9+9+7)
#define HID_NUM_INTERFACES 1

#ifdef ENABLE_VIRTUAL_COM_PORT
#define VCOM_TOTAL_LENGTH (9+5+5+4+5+7+9+7+7)
#define VCOM_NUM_INTERFACES 2
#else
#define VCOM_TOTAL_LENGTH 0
#define VCOM_NUM_INTERFACES 0
#endif

#ifdef PINPAD_DND_SUPPORT
#define MSC_TOTAL_LENGTH (9+7+7)
#define MSC_NUM_INTERFACES 1
#else
#define MSC_TOTAL_LENGTH 0
#define MSC_NUM_INTERFACES 0
#endif

#define W_TOTAL_LENGTH (ICC_TOTAL_LENGTH + HID_TOTAL_LENGTH     \
			+ VCOM_TOTAL_LENGTH + MSC_TOTAL_LENGTH)
#define NUM_INTERFACES (ICC_NUM_INTERFACES + HID_NUM_INTERFACES \
			+ VCOM_NUM_INTERFACES + MSC_NUM_INTERFACES)



#if defined(USB_SELF_POWERED)
#define USB_INITIAL_FEATURE 0xC0   /* bmAttributes: self powered */
#else
#define USB_INITIAL_FEATURE 0x80   /* bmAttributes: bus powered */
#endif

const uint8_t usb_initial_feature = USB_INITIAL_FEATURE;

/* Configuation Descriptor */
static const uint8_t gnukConfigDescriptor[] = {
  9,			   /* bLength: Configuation Descriptor size */
  USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */
  W_TOTAL_LENGTH, 0x00,   /* wTotalLength:no of returned bytes */
  NUM_INTERFACES,   /* bNumInterfaces: */
  0x01,   /* bConfigurationValue: Configuration value */
  0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
  USB_INITIAL_FEATURE,  /* bmAttributes*/
  50,	  /* MaxPower 100 mA */

  /* Interface Descriptor */
  9,			         /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  0,			         /* bInterfaceNumber: Index of this interface */
  0,			         /* Alternate setting for this interface */
  3,			         /* bNumEndpoints: Bulk-IN, Bulk-OUT, Intr-IN */
  USB_ICC_INTERFACE_CLASS,
  USB_ICC_INTERFACE_SUBCLASS,
  USB_ICC_INTERFACE_BULK_PROTOCOL,
  0,				 /* string index for interface */

  /* ICC Descriptor */
  54,			  /* bLength: */
  0x21,			  /* bDescriptorType: USBDESCR_ICC */
  0x10, 0x01,		  /* bcdCCID: revision 1.1 (of CCID) */
  0,			  /* bMaxSlotIndex: */
  1,			  /* bVoltageSupport: FIXED VALUE */
  0x02, 0, 0, 0,	  /* dwProtocols: T=1 */
  0xf3, 0x0d, 0, 0,	  /* dwDefaultClock: 3571 (non-ICCD): 3580 (ICCD) */
  0xf3, 0x0d, 0, 0,	  /* dwMaximumClock: 3571 (non-ICCD): 3580 (ICCD) */
  1,			  /* bNumClockSupported: FIXED VALUE */
  0x80, 0x25, 0, 0,	  /* dwDataRate: 9600: FIXED VALUE */
  0x80, 0x25, 0, 0,	  /* dwMaxDataRate: 9600: FIXED VALUE */
  1,			  /* bNumDataRateSupported: FIXED VALUE */
  0xfe, 0, 0, 0,	  /* dwMaxIFSD: 254 */
  0, 0, 0, 0,		  /* dwSynchProtocols: FIXED VALUE */
  0, 0, 0, 0,		  /* dwMechanical: FIXED VALUE */
  /*
   * According to Specification for USB ICCD (revision 1.0),
   * dwFeatures should be 0x00040840.
   *
   * It is different now for better interaction to GPG's in-stock
   * ccid-driver.
   */
  0x42, 0x08, 0x02, 0x00, /* dwFeatures (not ICCD):
			   *  Short APDU level             : 0x20000 *
			   *  (what? means ICCD?)          : 0x00800 *
			   *  Automatic IFSD               : 0x00400
			   *  NAD value other than 0x00    : 0x00200
			   *  Can set ICC in clock stop    : 0x00100
			   *  Automatic PPS CUR            : 0x00080
			   *  Automatic PPS PROP           : 0x00040 *
			   *  Auto baud rate change	   : 0x00020
			   *  Auto clock change		   : 0x00010
			   *  Auto voltage selection	   : 0x00008
			   *  Auto activaction of ICC	   : 0x00004
			   *  Automatic conf. based on ATR : 0x00002  g
			   */
  0x0f, 0x01, 0, 0,	  /* dwMaxCCIDMessageLength: 271 */
  0xff,			  /* bClassGetResponse: */
  0xff,			  /* bClassEnvelope: */
  0, 0,			  /* wLCDLayout: FIXED VALUE */
#if defined(PINPAD_SUPPORT)
#if defined(PINPAD_CIR_SUPPORT) || defined(PINPAD_DND_SUPPORT)
  1,			  /* bPinSupport: with PIN pad (verify) */
#elif defined(PINPAD_DIAL_SUPPORT)
  3,			  /* bPinSupport: with PIN pad (verify, modify) */
#endif
#else
  0,			  /* bPinSupport: No PIN pad */
#endif
  1,			  /* bMaxCCIDBusySlots: 1 */
  /*Endpoint IN1 Descriptor*/
  7,			       /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: Endpoint */
  0x81,				/* bEndpointAddress: (IN1) */
  0x02,				/* bmAttributes: Bulk */
  USB_ICC_DATA_SIZE, 0x00,      /* wMaxPacketSize: */
  0x00,				/* bInterval */
  /*Endpoint OUT1 Descriptor*/
  7,			       /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: Endpoint */
  0x01,				/* bEndpointAddress: (OUT1) */
  0x02,				/* bmAttributes: Bulk */
  USB_ICC_DATA_SIZE, 0x00,	/* wMaxPacketSize: */
  0x00,				/* bInterval */
  /*Endpoint IN2 Descriptor*/
  7,			       /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: Endpoint */
  0x82,				/* bEndpointAddress: (IN2) */
  0x03,				/* bmAttributes: Interrupt */
  4, 0x00,			/* wMaxPacketSize: */
  0xFF,				/* bInterval (255ms) */

  /* Interface Descriptor */
  9,			         /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  0x01,		  /* bInterfaceNumber: Number of Interface */
  0x00,		  /* bAlternateSetting: Alternate setting */
  0x01,		  /* bNumEndpoints: One endpoint used */
  0x03,		  /* bInterfaceClass: HID */
  0x00,		  /* bInterfaceSubClass: no boot */
  0x00,		  /* bInterfaceProtocol: 0=none */
  0x00,		  /* iInterface: no string for this interface */

  9,	        /* bLength: HID Descriptor size */
  0x21,	        /* bDescriptorType: HID */
  0x10, 0x01,   /* bcdHID: HID Class Spec release number */
  0x00,	        /* bCountryCode: Hardware target country */
  0x01,         /* bNumDescriptors: Number of HID class descriptors to follow */
  0x22,         /* bDescriptorType */
  HID_REPORT_DESC_SIZE, 0, /* wItemLength: Total length of Report descriptor */

  /*Endpoint IN7 Descriptor*/
  7,                            /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: Endpoint */
  0x87,				/* bEndpointAddress: (IN7) */
  0x03,				/* bmAttributes: Interrupt */
  0x02, 0x00,			/* wMaxPacketSize: 2 */
  0x20,				/* bInterval (32ms) */

#ifdef ENABLE_VIRTUAL_COM_PORT
  /* Interface Descriptor */
  9,			      /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  0x02,		  /* bInterfaceNumber: Number of Interface */
  0x00,		  /* bAlternateSetting: Alternate setting */
  0x01,		  /* bNumEndpoints: One endpoints used */
  0x02,		  /* bInterfaceClass: Communication Interface Class */
  0x02,		  /* bInterfaceSubClass: Abstract Control Model */
  0x01,		  /* bInterfaceProtocol: Common AT commands */
  0x00,		  /* iInterface: */
  /*Header Functional Descriptor*/
  5,			    /* bLength: Endpoint Descriptor size */
  0x24,			    /* bDescriptorType: CS_INTERFACE */
  0x00,			    /* bDescriptorSubtype: Header Func Desc */
  0x10,			    /* bcdCDC: spec release number */
  0x01,
  /*Call Managment Functional Descriptor*/
  5,	    /* bFunctionLength */
  0x24,	    /* bDescriptorType: CS_INTERFACE */
  0x01,	    /* bDescriptorSubtype: Call Management Func Desc */
  0x03,	    /* bmCapabilities: D0+D1 */
  0x02,	    /* bDataInterface: 2 */
  /*ACM Functional Descriptor*/
  4,	    /* bFunctionLength */
  0x24,	    /* bDescriptorType: CS_INTERFACE */
  0x02,	    /* bDescriptorSubtype: Abstract Control Management desc */
  0x02,	    /* bmCapabilities */
  /*Union Functional Descriptor*/
  5,		 /* bFunctionLength */
  0x24,		 /* bDescriptorType: CS_INTERFACE */
  0x06,		 /* bDescriptorSubtype: Union func desc */
  0x01,		 /* bMasterInterface: Communication class interface */
  0x02,		 /* bSlaveInterface0: Data Class Interface */
  /*Endpoint 4 Descriptor*/
  7,			       /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,	   /* bDescriptorType: Endpoint */
  0x84,				   /* bEndpointAddress: (IN4) */
  0x03,				   /* bmAttributes: Interrupt */
  VIRTUAL_COM_PORT_INT_SIZE, 0x00, /* wMaxPacketSize: */
  0xFF,				   /* bInterval: */

  /*Data class interface descriptor*/
  9,			       /* bLength: Endpoint Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: */
  0x03,			   /* bInterfaceNumber: Number of Interface */
  0x00,			   /* bAlternateSetting: Alternate setting */
  0x02,			   /* bNumEndpoints: Two endpoints used */
  0x0A,			   /* bInterfaceClass: CDC */
  0x00,			   /* bInterfaceSubClass: */
  0x00,			   /* bInterfaceProtocol: */
  0x00,			   /* iInterface: */
  /*Endpoint 5 Descriptor*/
  7,			       /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,	    /* bDescriptorType: Endpoint */
  0x05,				    /* bEndpointAddress: (OUT5) */
  0x02,				    /* bmAttributes: Bulk */
  VIRTUAL_COM_PORT_DATA_SIZE, 0x00, /* wMaxPacketSize: */
  0x00,			     /* bInterval: ignore for Bulk transfer */
  /*Endpoint 3 Descriptor*/
  7,			       /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,	    /* bDescriptorType: Endpoint */
  0x83,				    /* bEndpointAddress: (IN3) */
  0x02,				    /* bmAttributes: Bulk */
  VIRTUAL_COM_PORT_DATA_SIZE, 0x00, /* wMaxPacketSize: */
  0x00,				    /* bInterval */
#endif
#ifdef PINPAD_DND_SUPPORT
  /* Interface Descriptor.*/
  9,			      /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
#ifdef ENABLE_VIRTUAL_COM_PORT
  0x04,				/* bInterfaceNumber.                */
#else
  0x02,				/* bInterfaceNumber.                */
#endif
  0x00,				/* bAlternateSetting.               */
  0x02,				/* bNumEndpoints.                   */
  0x08,				/* bInterfaceClass (Mass Stprage).  */
  0x06,				/* bInterfaceSubClass (SCSI
				   transparent command set, MSCO
				   chapter 2).                      */
  0x50,				/* bInterfaceProtocol (Bulk-Only
				   Mass Storage, MSCO chapter 3).  */
  0x00,				/* iInterface.                      */
  /* Endpoint Descriptor.*/
  7,			        /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: Endpoint */
  0x86,				/* bEndpointAddress: (IN6)   */
  0x02,				/* bmAttributes (Bulk).             */
  0x40, 0x00,			/* wMaxPacketSize.                  */
  0x00,				/* bInterval (ignored for bulk).    */
  /* Endpoint Descriptor.*/
  7,			        /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: Endpoint */
  0x06,				/* bEndpointAddress: (OUT6)    */
  0x02,			        /* bmAttributes (Bulk).             */
  0x40, 0x00,		        /* wMaxPacketSize.                  */
  0x00,                         /* bInterval (ignored for bulk).    */
#endif
};


/* USB String Descriptors */
static const uint8_t gnukStringLangID[] = {
  4,				/* bLength */
  USB_STRING_DESCRIPTOR_TYPE,
  0x09, 0x04			/* LangID = 0x0409: US-English */
};

#define USB_STRINGS_FOR_GNUK 1
#include "usb-strings.c.inc"

struct desc
{
  const uint8_t *desc;
  uint16_t size;
};

static const struct desc String_Descriptors[NUM_STRING_DESC] = {
  {gnukStringLangID, sizeof (gnukStringLangID)},
  {gnukStringVendor, sizeof (gnukStringVendor)},
  {gnukStringProduct, sizeof (gnukStringProduct)},
  {gnukStringSerial, sizeof (gnukStringSerial)},
  {gnuk_revision_detail, sizeof (gnuk_revision_detail)},
  {gnuk_config_options, sizeof (gnuk_config_options)},
  {sys_version, sizeof (sys_version)},
};

#define USB_DT_HID			0x21
#define USB_DT_REPORT			0x22

int
usb_cb_get_descriptor (uint8_t rcp, uint8_t desc_type, uint8_t desc_index,
		       uint16_t index)
{
  if (rcp == DEVICE_RECIPIENT)
    {
      if (desc_type == DEVICE_DESCRIPTOR)
	{
	  usb_lld_set_data_to_send (gnukDeviceDescriptor,
				    sizeof (gnukDeviceDescriptor));
	  return USB_SUCCESS;
	}
      else if (desc_type == CONFIG_DESCRIPTOR)
	{
	  usb_lld_set_data_to_send (gnukConfigDescriptor,
				    sizeof (gnukConfigDescriptor));
	  return USB_SUCCESS;
	}
      else if (desc_type == STRING_DESCRIPTOR)
	{
	  if (desc_index < NUM_STRING_DESC)
	    {
	      usb_lld_set_data_to_send (String_Descriptors[desc_index].desc,
					String_Descriptors[desc_index].size);
	      return USB_SUCCESS;
	    }
	}
    }
  else if (rcp == INTERFACE_RECIPIENT)
    {
      if (index == 1)
	{
	  if (desc_type == USB_DT_HID)
	    {
	      usb_lld_set_data_to_send (gnukConfigDescriptor+ICC_TOTAL_LENGTH+9,
					9);
	      return USB_SUCCESS;
	    }
	  else if (desc_type == USB_DT_REPORT)
	    {
	      usb_lld_set_data_to_send (hid_report_desc, HID_REPORT_DESC_SIZE);
	      return USB_SUCCESS;
	    }
	}
      else if (desc_type == STRING_DESCRIPTOR)
	{
	  if (desc_index < NUM_STRING_DESC)
	    {
	      usb_lld_set_data_to_send (String_Descriptors[desc_index].desc,
					String_Descriptors[desc_index].size);
	      return USB_SUCCESS;
	    }
	}
    }

  return USB_UNSUPPORT;
}

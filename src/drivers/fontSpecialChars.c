
#include <drivers/fonts.h>


const uint8_t FontSpecial_Table[] =
{
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0b00101000, 0b00010100,
	0b01010010, 0b01001010,
	0b00101001, 0b10010100,
	0b01010011, 0b11001010,
	0b00101111, 0b11110100,
	0b01010111, 0b11101010,
	0b00101011, 0b11010100,
	0b01010000, 0b00001010,
	0b00101000, 0b00010100,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
};

sFONT FontSpecialChar = {
  FontSpecial_Table,
  16, /* Width */
  16, /* Height */
};

/**
  * @}
  */ 


/** @defgroup FONTS_Private_Function_Prototypes
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup FONTS_Private_Functions
  * @{
  */
    
/**
  * @}
  */
  
/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

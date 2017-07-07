/**************************************************************************
 *
 * FILE NAME:			bitmapDb.h
 * FILE DESCRIPTION:	Database of all bitmaps in the system
 *
 * FILE CREATION DATE:	24-07-2009
 *
 *==========================================================================
 *
 * Modification history:
 * --------------------
 * 01a,24jul09 erd written
 *
 ***************************************************************************/

#ifndef __BITMAP_DB_H_
#define __BITMAP_DB_H_
#include <stdint.h>
// ==========================================================================
// structure definition
// ==========================================================================

// This structure describes a single character's display information
typedef struct
{
	const uint8_t widthBits;					// width, in bits (or pixels), of the character
	const uint8_t heightBits;					// width, in bits (or pixels), of the character
	const uint16_t offset;					// offset of the character's bitmap, in bytes, into the the FONT_INFO's data array
	
} FONT_CHAR_INFO;	

// Describes a single font
typedef struct
{
	const uint8_t 			heightPages;	// height, in bits
	const uint8_t 			startChar;		// the first character in the font (e.g. in charInfo and data)
	const uint8_t 			endChar;		// the last character in the font
	const FONT_CHAR_INFO*	charInfo;		// pointer to array of char information
	const uint8_t*			data;			// pointer to generated array of character visual representation
		
} FONT_INFO;	



/* Font data for Arial 10pt */
extern const uint8_t arial_10ptBitmaps[];
extern const FONT_INFO arial_10ptFontInfo;
extern const FONT_CHAR_INFO arial_10ptDescriptors[];

/* Font data for Arial 10pt Bold*/
extern const uint8_t arial_10BptBitmaps[];
extern const FONT_INFO arial_10BptFontInfo;
extern const FONT_CHAR_INFO arial_10BptDescriptors[];

/* Font data for Arial 18pt */
extern const uint8_t arial_18ptBitmaps[];
extern const FONT_INFO arial_18ptFontInfo;
extern const FONT_CHAR_INFO arial_18ptDescriptors[];

/* Font data for Arial 18pt Bold*/
extern const uint8_t arial_18BptBitmaps[];
extern const FONT_INFO arial_18BptFontInfo;
extern const FONT_CHAR_INFO arial_18BptDescriptors[];

/* Font data for Arial 24pt */
extern const uint8_t arial_24ptBitmaps[];
extern const FONT_INFO arial_24ptFontInfo;
extern const FONT_CHAR_INFO arial_24ptDescriptors[];

/* Font data for Arial 24pt Bold*/
extern const uint8_t arial_24BptBitmaps[];
extern const FONT_INFO arial_24BptFontInfo;
extern const FONT_CHAR_INFO arial_24BptDescriptors[];




#endif /* __BITMAP_DB_H_ */

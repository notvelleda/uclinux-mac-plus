
/*
 * arch/m68k/console/fonts.c -- `Soft' font definitions
 *
 *    Created 1995 by Geert Uytterhoeven
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */


#include <linux/types.h>
#include <linux/string.h>
#include <asm/font.h>


   /*
    *    External Font Definitions
    */

/* VGA8x8 */
extern char fontname_8x8[];
extern int fontwidth_8x8, fontheight_8x8;
extern u_char fontdata_8x8[];

/* VGA8x16 */
extern char fontname_8x16[];
extern int fontwidth_8x16, fontheight_8x16;
extern u_char fontdata_8x16[];

/* PEARL8x8 */
extern char fontname_pearl8x8[];
extern int fontwidth_pearl8x8, fontheight_pearl8x8;
extern u_char fontdata_pearl8x8[];

/* MINI4x6 */
extern char fontname_mini4x6[];
extern int fontwidth_mini4x6, fontheight_mini4x6;
extern u_char fontdata_mini4x6[];

/* TERMINUS6x12 */
extern char fontname_terminus6x12[];
extern int fontwidth_terminus6x12, fontheight_terminus6x12;
extern u_char fontdata_terminus6x12[];


   /*
    *    Font Descriptor Array
    */

struct softfontdesc {
   char *name;
   int *width;
   int *height;
   u_char *data;
};

#define VGA8x8_IDX         0
#define VGA8x16_IDX        1
#define PEARL8x8_IDX       2
#define MINI4x6_IDX        3
#define TERMINUS6x12_IDX   4

static struct softfontdesc softfonts[] = {
   { fontname_8x8, &fontwidth_8x8, &fontheight_8x8, fontdata_8x8 },
   { fontname_8x16, &fontwidth_8x16, &fontheight_8x16, fontdata_8x16 },
   { fontname_pearl8x8, &fontwidth_pearl8x8, &fontheight_pearl8x8,
     fontdata_pearl8x8 },
   { fontname_mini4x6, &fontwidth_mini4x6, &fontheight_mini4x6,
     fontdata_mini4x6 },
   { fontname_terminus6x12, &fontwidth_terminus6x12, &fontheight_terminus6x12,
     fontdata_terminus6x12 },
};

static u_long numsoftfonts = sizeof(softfonts)/sizeof(*softfonts);


   /*
    *    Find a font with a specific name
    */

int findsoftfont(char *name, int *width, int *height, u_char *data[])
{
   int i;

   for (i = 0; i < numsoftfonts; i++)
      if (!strcmp(softfonts[i].name, name)) {
         if (width)
            *width = *softfonts[i].width;
         if (height)
            *height = *softfonts[i].height;
         if (data)
            *data = softfonts[i].data;
			return(1);
      }
	return(0);
}


   /*
    *    Get the default font for a specific screen size
    */

void getdefaultfont(int xres, int yres, char *name[], int *width, int *height,
                    u_char *data[])
{
   int i;
   
   i = TERMINUS6x12_IDX;

   if (name)
      *name = softfonts[i].name;
   if (width)
      *width = *softfonts[i].width;
   if (height)
      *height = *softfonts[i].height;
   if (data)
      *data = softfonts[i].data;
}

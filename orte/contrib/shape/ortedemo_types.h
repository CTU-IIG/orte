#ifndef ORTEDEMO_TYPES_H
#define ORTEDEMO_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

//#include <stdint.h>
#include <orte.h>


#define ELLIPSE		0
#define RECTANGLE	1
#define TRIANGLE	2

#define BLUE		0
#define GREEN		1
#define RED		2
#define BLACK		3
#define YELLOW          4

typedef struct BoxRect {
    int top_left_x;
    int top_left_y;
    int bottom_right_x;
    int bottom_right_y;
} BoxRect;
	
typedef struct BoxType {
    int      color;
    int      shape;
    BoxRect  rectangle;
} BoxType;
			    
extern Boolean 
ORTETypeRegisterBoxType(ORTEDomain *d);

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif

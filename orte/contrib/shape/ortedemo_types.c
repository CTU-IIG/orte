#include "ortedemo_types.h"

/*****************************************************************************/
void
BoxTypeSerialize(ORTECDRStream *cdr_stream, void *instance) {
  BoxType  *boxType=(BoxType*)instance;

  //color
  *(int*)cdr_stream->bufferPtr=boxType->color;
  cdr_stream->bufferPtr+=sizeof(int);
  //shape
  *(int*)cdr_stream->bufferPtr=boxType->shape;
  cdr_stream->bufferPtr+=sizeof(int);
  //************************************************************
  //Box position
  //top_left_x
  *(int*)cdr_stream->bufferPtr=boxType->rectangle.top_left_x;
  cdr_stream->bufferPtr+=sizeof(int);
  //top_left_y
  *(int*)cdr_stream->bufferPtr=boxType->rectangle.top_left_y;
  cdr_stream->bufferPtr+=sizeof(int);
  //bottom_right_x
  *(int*)cdr_stream->bufferPtr=boxType->rectangle.bottom_right_x;
  cdr_stream->bufferPtr+=sizeof(int);
  //bottom_right_y
  *(int*)cdr_stream->bufferPtr=boxType->rectangle.bottom_right_y;
  cdr_stream->bufferPtr+=sizeof(int);
}

/*****************************************************************************/
void
BoxTypeDeserialize(ORTECDRStream *cdr_stream, void *instance) {
  BoxType  *boxType=(BoxType*)instance;

  //color
  boxType->color=*(int*)cdr_stream->bufferPtr;
  cdr_stream->bufferPtr+=sizeof(int);
  //shape
  boxType->shape=*(int*)cdr_stream->bufferPtr;
  cdr_stream->bufferPtr+=sizeof(int);
  //************************************************************
  //Box position
  //top_left_x
  boxType->rectangle.top_left_x=*(int*)cdr_stream->bufferPtr;
  cdr_stream->bufferPtr+=sizeof(int);
  //top_left_y
  boxType->rectangle.top_left_y=*(int*)cdr_stream->bufferPtr;
  cdr_stream->bufferPtr+=sizeof(int);
  //bottom_right_x
  boxType->rectangle.bottom_right_x=*(int*)cdr_stream->bufferPtr;
  cdr_stream->bufferPtr+=sizeof(int);
  //bottom_right_y
  boxType->rectangle.bottom_right_y=*(int*)cdr_stream->bufferPtr;
  cdr_stream->bufferPtr+=sizeof(int);
}

/*****************************************************************************/
Boolean 
ORTETypeRegisterBoxType(ORTEDomain *d) {
   return ORTETypeRegisterAdd(
                d,
                "BoxType",
	        BoxTypeSerialize, 
	        BoxTypeDeserialize,
	        sizeof(BoxType));

}


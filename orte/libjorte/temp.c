
  do
  {
    // get class MessageData
    cls_msg = (*env)->GetObjectClass(env, obj_msg);
    if(cls_msg == 0)
    {
      #ifdef TEST_STAGE
         printf(":!c: cls_msg = NULL \n");
      #endif
      break;
    }
    //
    if (obj_bb == 0)
    {
      // get buff's fieldID
      fid = (*env)->GetFieldID(env,
                               cls_msg,
                               "buffer",
                               "Ljava/nio/ByteBuffer;");
      if (fid == NULL)
      {
        #ifdef TEST_STAGE
           printf(":!c: fid = NULL \n");
        #endif
        break;
      }
      // get buff's object
      obj_bb = (*env)->GetObjectField(env, obj_msg, fid);
      if(obj_bb == NULL)
      {
        #ifdef TEST_STAGE
           printf(":!c: obj_bb = NULL \n");
        #endif
        break;
      }
      // create global reference
      callback_cont->bbuff = (*env)->NewGlobalRef(env, obj_bb);
      if (callback_cont->bbuff == 0)
      {
        #ifdef TEST_STAGE
           printf(":!c: callback_cont->bbuff = NULL \n");
        #endif
        break;
      }
      #ifdef TEST_STAGE
         printf(":c: global reference created.. \n");
      #endif
    }
    // get class ByteBuffer
    cls_bb = (*env)->GetObjectClass(env, obj_bb);
    if(cls_bb == 0)
    {
      #ifdef TEST_STAGE
         printf(":!c: cls_bb = NULL \n");
      #endif
      break;
    }
    // get buff's length
    mid = (*env)->GetMethodID(env,
                              cls_msg,
                              "getMaxDataLength",
                              "()I");
    if(mid == 0)
    {
      #ifdef TEST_STAGE
         printf(":!c: mid = NULL \n");
      #endif
      break;
    }
    buff_length = (*env)->CallIntMethod(env,
                                        obj_msg,
                                        mid);

    // rewind buffer (needs cls Buffer not ByteBuffer!)
    // get class Buffer
    cls_tmp = (*env)->FindClass(env, "java/nio/Buffer");
    if(cls_tmp == 0)
    {
      #ifdef TEST_STAGE
         printf(":!c: cls_tmp = NULL \n");
      #endif
      break;
    }
    // methodID
    mid = (*env)->GetMethodID(env,
                            cls_tmp,
                            "rewind",
                            "()Ljava/nio/Buffer;");
    if(mid == 0)
    {
      #ifdef TEST_STAGE
         printf(":!c: mid = NULL \n");
      #endif
      break;
    }
    // call method
    (*env)->CallObjectMethod(env,
                             obj_bb,
                             mid);
    #ifdef TEST_STAGE
       printf(":c: buffer successfully rewind..\n");
    #endif
    // methodID
    mid = (*env)->GetMethodID(env,
                              cls_bb,
                              "put",
                              "(B)Ljava/nio/ByteBuffer;");
    if(mid == 0)
    {
      #ifdef TEST_STAGE
         printf(":!c: mid = NULL \n");
      #endif
      break;
    }
    // copy the bytes from C to JAVA buffer
    // //////////////////////////////////////////////////////////
    buffer = (char *) vinstance;
    //
    for(i = 0; i < buff_length; i++)
    {
      #ifdef TEST_STAGE
        printf(":c: i = %d kopirovany znak = %c [%d] \n",
               i,*buffer,*buffer);
      #endif
      // call method
      obj_bb = (*env)->CallObjectMethod(env,
                                        obj_bb,
                                        mid,
                                        *buffer);
      // next char
      buffer++;
    }
    // //////////////////////////////////////////////////////////
    // methodID
    mid = (*env)->GetMethodID(env,
                              cls_msg,
                              "read",
                              "()V");
    if(mid == 0)
    {
      #ifdef TEST_STAGE
         printf(":!c: mid = NULL \n");
      #endif
      break;
    }
    // call method
    (*env)->CallVoidMethod(env,
                           obj_msg,
                           mid);

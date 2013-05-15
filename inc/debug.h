#ifndef _DEBUG_H_
#define _DEBUG_H_
    
    #include <stdio.h>
    #include "errno.h"
    
    extern int errno;
    
    void _perror ( char *function );
 
#endif

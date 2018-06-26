//
//  error.h
//  lfhs
//
//  Created by Vasiliy Sabadazh on 05/05/2018.
//  Copyright © 2018 Vasiliy Sabadazh. All rights reserved.
//

#ifndef error_h
#define error_h

typedef enum
{
    e_io_buffer_no_error = 0,
    e_io_buffer_too_small = 1,
    e_io_buffer_alloc_failed = 2,
    e_io_buffer_not_enough_data = 3,
    e_io_buffer_cannot_shrink = 4
} io_buffer_status;

char* io_buffer_error_str(io_buffer_status);

#endif /* error_h */

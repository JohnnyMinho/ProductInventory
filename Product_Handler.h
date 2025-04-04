#ifndef __Product_Handler_H__
#define __Product_Handler_H__

//This header file contains the function prototypes for handling product operations in the inventory system.
//During each of the respective functions end use, if there is any error a specific output code will be released. This output codes will have a header file with the define error_types.
//If everything is ok, the function will return 0.

int add_new_product();
int update_product();
int remove_product();

#endif
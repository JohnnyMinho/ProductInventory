#ifndef __FILEHANDLER_H__
#define __FILEHANDLER_H__

#include "DataUnits.h"

struct Product * LoadFromFile(char *filename, struct Sys_Info *info);
void SaveToFile(char *filename, struct Product *products, struct Sys_Info *info);

#endif // __FILEHANDLER_C__

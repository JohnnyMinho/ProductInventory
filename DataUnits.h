#ifndef __DATAUNITS_H__
#define __DATAUNITS_H__

struct Product{
    int id;
    char name[100];
    int quantity;
    float price;
};

struct Sys_Info{
    int Loaded_Products;
    char Last_Use_Date;
};

#endif // __DATAUNITS_H__
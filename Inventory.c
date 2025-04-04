#include <stdio.h>
#include <stdlib.h>
#include "Menu.h"
#include "FileHandler.h"
#include "Inventory.h"


int main(int argc, char *argv[]){

    int exit = 0;

    system("clear");
    //On startup the C program should be given an file name as argument for now.
    if(argc != 2){
        printf("Missing file\n");
        return 1;
    }

    struct Sys_Info Current_Info;

    struct Product* Current_Products = LoadFromFile(argv[1], &Current_Info);

    while(exit == 0){
        system("clear");

        draw_menu_norm();
        int option = getc(stdin);
        if(option != 10){
            getchar();
        }
        switch(option)
        {
            case 49:
                {
                    printf("Add New Product\n");
                    break;
                }
                
            case 50:
                {
                    printf("Update Product\n");
                    break;
                }
            case 51:
                {
                    printf("Remove Product\n");
                    break;
                }
            case 52:
                {
                    printf("Options\n");
                    break;
                }
            case 53:
                {
                    printf("Credits\n");
                    draw_menu_credits();
                    break;
                }
            case 54:
                {
                    printf("Help\n");
                    draw_menu_help();
                    break;
                }
            case 48:
                {
                    printf("Exit\n");
                    exit = 1;
                    break;
                }
            default:
                printf("Invalid Option \n");
                break;
        }
    }
    //SaveToFile(argv[1], Current_Products, &Current_Info);

    free(Current_Products);
    
    return 0;
}
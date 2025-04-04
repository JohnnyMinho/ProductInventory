#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "FileHandler.h"
#include "Inventory.h"

void ReadHeader(int filename, char *header){
    //This function will be used to read the header.
    //This way we can simplify the loading process.
    
}

char *Check_File(char *filename, int *new_file_name){
    //This auxiliary function will check the given filename and if it is an available file, the same name will be returned.
    //Otherwise, a new file should be either be formed or the name of an existing file should be given.
    int filename_fd = open(filename, O_RDONLY,0666);
    printf("Filename: %s\n", filename);
    if(filename_fd == -1){
        int Keep_Option_Search = 1;
        while(Keep_Option_Search){
            //system("clear");
            perror("File not found");
            printf("Create a new file (c), search for another file (s) or exit (e)?\n");
            Keep_Option_Search = 0;
            int option = getc(stdin);
            if(option != 10){
                getchar();
            }
            switch(option){
                case 99:
                    {
                        char *New_File_Name = (char *)malloc(100*sizeof(char));
                        printf("Type the New File Name\n");
                        fgets(New_File_Name, 100, stdin);
                        for(int New_Line_Finder = 0; New_File_Name[New_Line_Finder] != '\0' && New_Line_Finder < 100; New_Line_Finder++){
                            if(New_File_Name[New_Line_Finder] == 10){
                                New_File_Name[New_Line_Finder] = '\0';
                                New_Line_Finder = 100;
                            }
                        }
                        int New_File_fd = open(New_File_Name, O_WRONLY | O_CREAT | O_TRUNC ,0666);
                        if(New_File_fd != -1){
                            printf("New File Created\n");
                            close(New_File_fd);
                            (*new_file_name) = 1;
                            return New_File_Name;
                        }else{
                            perror("No New File\n");
                            Keep_Option_Search = 1;
                        }
                        free(New_File_Name);
                        break;
                    }
                case 115:
                    {   
                        char *New_Search_Name = (char *)malloc(100*sizeof(char));
                        fgets(New_Search_Name, 100, stdin);
                        for(int New_Line_Finder = 0; New_Search_Name[New_Line_Finder] != '\0' && New_Line_Finder < 100; New_Line_Finder++){
                            if(New_Search_Name[New_Line_Finder] == 10){
                                New_Search_Name[New_Line_Finder] = '\0';
                                New_Line_Finder = 100;
                            }
                        }
                        int New_Search_Fd = open(New_Search_Name, O_RDONLY ,0666);
                        if(New_Search_Fd != -1){
                            printf("Found File\n");
                            close(New_Search_Fd);
                            (*new_file_name) = 0;
                            return New_Search_Name;
                        }else{

                            Keep_Option_Search = 1;
                        }
                        free(New_Search_Name);
                        break;
                    }
                case 101:
                    printf("Leaving the program\n");
                    exit(0);
                    break;
                default:
                    Keep_Option_Search = 1;
                    printf("Invalid option\n");
                    break;
            }
        }
    }else{
        close(filename_fd);
        return filename;
    }

    return filename;
}

struct Product ReadProductFromFile(int filename_fd, int read_products, int *EOF_Checker){
    char *product_data = (char *)malloc(512*sizeof(char));
    struct Product Loaded_Product;
    //The data is tokeninzed with "," as the delimiter between parameters. The last one is delimited with a newline.

    int Loaded_Id = 0;
    int Loaded_Quantity = 0;
    float Loaded_Price = 0.0;
    char Loaded_Name[100];
    int filled_data = 0; 

    int data_current_pos = 0;
    int EOF_VALUE = 0;
    int STOP = 0;

    while(STOP != 1 && (EOF_VALUE = read(filename_fd, &product_data[data_current_pos], 1)) > 0){
        //This way we advance the newline character.
        if(product_data[data_current_pos] == 10){
            product_data[data_current_pos] = '\0';
            STOP = 1;
        }
            data_current_pos++;
    }
    if(data_current_pos <= 1){
        (*EOF_Checker) = -1;
        //Nothing was read, so we know that we reached EOF.
    }
    printf("%s\n",product_data);
    if((*EOF_Checker) != -1){
        printf("Valid loaded %d , %c\n",EOF_VALUE, product_data[0]);
        for(int data_reader = 0,token_pos = 0, data_pos_start = 0;  data_reader != data_current_pos && EOF_VALUE != 0; data_reader++){
            if(product_data[data_reader] == ',' || product_data[data_reader] == 10 || product_data[data_reader] == '\0'){
                token_pos = data_reader;
            }
            if(token_pos != 0){
                    switch(filled_data){
                        
                        case 0:
                            {   
                                int Size_Of_Int = token_pos - data_pos_start;
                                
                                char Id[Size_Of_Int+1];
                                memmove(Id, product_data, Size_Of_Int);
                                Loaded_Id = atoi(Id);
                                break;
                            }
                        case 1:
                            {  
                                int Loaded_Name_size = token_pos - data_pos_start;
                                memmove(Loaded_Name, product_data+data_pos_start, Loaded_Name_size);
                                break;
                            }
                        case 2:
                            {
                                int Size_Of_Int = token_pos - data_pos_start;
                                char Quantity[Size_Of_Int+1];
                                memmove(Quantity, product_data+data_pos_start, Size_Of_Int);
                                Loaded_Quantity = atoi(Quantity);
                                break;
                            }
                        case 3:
                            {
                                int Size_Of_Price = token_pos - data_pos_start;
                                char Price[Size_Of_Price+1];
                                memmove(Price, product_data+data_pos_start, Size_Of_Price);
                                Loaded_Price = atof(Price);
                                //Loaded_Price = roundf(Loaded_Price* 100.0) / 100.0;
                                break;
                            }
                        default:
                            break;

                    }
                //memset(product_data,'\0',512);
                filled_data++;
                data_pos_start = token_pos+1;
                token_pos = 0;
            }
        }

        Loaded_Product.id = Loaded_Id;
        strcpy(Loaded_Product.name, Loaded_Name);
        Loaded_Product.quantity = Loaded_Quantity;
        Loaded_Product.price = Loaded_Price;

        printf("Id: %d\n", Loaded_Product.id);
        printf("Name: %s\n", Loaded_Product.name);
        printf("Quantity: %d\n", Loaded_Product.quantity);
        printf("Price: %.2f\n", Loaded_Product.price);
    }
    free(product_data);

    return Loaded_Product;
}

struct Product *LoadFromFile(char *filename, struct Sys_Info *info){
    //Load previous stored data from file.
    //The file has an very small header on which the first lines have the amount of products already registered 
    //All the products are stored per line and with the following format: id;name;quantity;price;

    int can_free_filename = 0;

    char *checked_filename = Check_File(filename, &can_free_filename);

    int filename_fd = open(checked_filename, O_RDONLY,0666);

    if(filename_fd == -1){
        //The file shouldn't have any problems where. If it does have them, something is very wrong.
        //This is just an extra security step.
        perror("Error with file");
        exit(1);
    }

    char header[100];
    int header_length = 0;
    int Amount_of_Products = 0;

    while((read(filename_fd, &header[header_length], 1)) > 0 && header[header_length] != 10){
        //This way we advance the newline character.
        header_length++;
    }
    header[header_length] = '\0';
    printf("%s\n", header);
    for(int Ten_X_Increment = header_length-1,runner = 0; Ten_X_Increment >= 0; Ten_X_Increment--){
        int Ten_Multiplier = 1;
        for(int Times_Multiplied = 0; Times_Multiplied < Ten_X_Increment; Times_Multiplied++){
            Ten_Multiplier = Ten_Multiplier*10;
        }
        Amount_of_Products = Amount_of_Products + (Ten_Multiplier*(header[runner]-48));
        runner++;
    }

    printf("Amount of Products: %d\n", Amount_of_Products);

    info->Loaded_Products = Amount_of_Products;

    struct Product *products = (struct Product *)malloc(Amount_of_Products*sizeof(struct Product));
    int EOF_Reached = 0;
    //Now we have to safely load the products from the file.
    //If one the files is missing a data field, the program should log it on the terminal as an warning.
    //In the future the user should either be prompted to either fix the missing data or remove the product

    for(int read_products = 0; read_products < Amount_of_Products && EOF_Reached != -1; read_products++){
        struct Product temp_product = ReadProductFromFile(filename_fd, read_products, &EOF_Reached);
        if(EOF_Reached != -1){
            printf("New Product Loaded -> %d\n",read_products);
            products[read_products] = temp_product;
        }
    }

    close(filename_fd);

    if(can_free_filename){
        free(checked_filename);
    }

    printf("Press Enter to Continue");
    while( getchar() != '\n' );

    return products;
}

void SaveToFile(char *filename, struct Product *products, struct Sys_Info *info){
    //We have to collect the info from each product and then store it in the file.
    //We have two ways to do this.
    //Leave the making of the header to a later stage. Or store the entire product as a massive string.
    //The latter is not duable if the amount of products is too high.

    int can_free_filename = 0;

    char *checked_filename = Check_File(filename, &can_free_filename);

    int file_fd = 0;

    if((file_fd = open(checked_filename, O_WRONLY,0666)) == 1){
        perror("Failed to write to save file, please retry");
    }

        
    char Header[100];

    sprintf(Header, "%d\n",info->Loaded_Products);

    write(file_fd, Header, strlen(Header));

    for(int Stored_Products = 0; Stored_Products != info->Loaded_Products; Stored_Products++){
        struct Product temp_product = products[Stored_Products];
        char Product_Data[512];
        sprintf(Product_Data, "%d,%s,%d,%.2f\n", temp_product.id, temp_product.name, temp_product.quantity, temp_product.price);
        write(file_fd, Product_Data, strlen(Product_Data));
    }

    if(can_free_filename){
        free(checked_filename);
    }
}

/*int main() {

    

    return 0;
}*/
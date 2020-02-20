#include <ctype.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct diff {
    long offset; /* offset of the difference in file starting from zero*/
    unsigned char orig_value;     /* value of the byte in ORIG */
    unsigned char new_value;     /* value of the byte in NEW */
}diff;
typedef struct node node;

struct node {
    diff *diff_data; /* pointer to a struct containing the offset and the value of the bytes in each of the files*/
    node *next;
};


void list_print(node *diff_list,FILE* output)/* Print the nodes in diff_list in the following format: byte POSITION ORIG_VALUE NEW_VALUE.
Each item followed by a newline character. */
{
    node *temp=diff_list;
    if(temp==NULL)
        exit(8);
    while(temp!=NULL){
        printf("%ld",temp->diff_data->offset);
        printf(" ");
        printf("%c",temp->diff_data->orig_value);
        printf(" ");
        printf("%c",temp->diff_data->new_value);
        printf("%c",'\n');
        temp=temp->next;
    }
}

    void list_free(node *diff_list) /* Free the memory allocated by and for the list. */
{
    if(diff_list==NULL)
        free(diff_list);
    else{
        while(diff_list!=NULL){
            node *temp=diff_list;
            diff_list=diff_list->next;
            free(temp->diff_data);
            free(temp);
        }
        free(diff_list);
    }
}

int main(int argc, char **argv) {
    return 0;
}

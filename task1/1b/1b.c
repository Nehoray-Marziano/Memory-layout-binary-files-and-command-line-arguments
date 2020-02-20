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
    if(output!=NULL) {
        while (temp->next != NULL) {
            fprintf(output,"%s", "byte ");
            fprintf(output, "%ld", temp->diff_data->offset);
            fprintf(output, " ");
            fprintf(output, "%u", temp->diff_data->orig_value);
            fprintf(output, " ");
            fprintf(output, "%u", temp->diff_data->new_value);
            fprintf(output,"%c", '\n');
            temp = temp->next;
        }
    }
    if(output==NULL) {
        while (temp!= NULL) {
            printf("%s", "byte ");
            printf("%ld", temp->diff_data->offset);
            printf(" ");
            printf("%u", temp->diff_data->orig_value);
            printf(" ");
            printf("%u", temp->diff_data->new_value);
            printf("%c",'\n');
            temp = temp->next;
        }
    }
}
node* list_append(node* diff_list, diff* data)
/* Add a new node with the given data to the list,
   and return a pointer to the list (i.e., the first node in the list).
   If the list is null - create a new entry and return a pointer to the entry.*/
{
    node *toReturn=diff_list;
    node *temp=diff_list;

    if (diff_list==NULL){
        diff_list=malloc(sizeof(node));
        diff_list->next=NULL;
        struct diff *new1=malloc(sizeof(diff));
        diff_list->diff_data=new1;
        diff_list->diff_data->new_value=data->new_value;
        diff_list->diff_data->orig_value=data->orig_value;
        diff_list->diff_data->offset=data->offset;
        toReturn=diff_list;
        return toReturn;
    }


    else {
        while (temp->next != NULL) {
            temp = temp->next;
        }
        node *updated=malloc(sizeof(node));
        struct diff *new2=malloc(sizeof(diff));
        updated->diff_data=new2;
        updated->next=NULL;
        updated->diff_data->offset=data->offset;
        updated->diff_data->orig_value=data->orig_value;
        updated->diff_data->new_value=data->new_value;
        temp->next=updated;
    }
    return toReturn;
}

void list_free(node *diff_list) /* Free the memory allocated by and for the list. */
{
    if(diff_list==NULL)
        free(diff_list);
    else{
        while(diff_list->next!=NULL){
            node *temp=diff_list;
            diff_list=diff_list->next;
            free(temp->diff_data);
            free(temp);
        }
        free(diff_list);
    }
}



void bcmp(char *origin, char *new){
    FILE *orig_ptr;
    FILE *new_ptr;
    orig_ptr=fopen(origin,"rb");
    new_ptr=fopen(new,"rb");
    int ch;

    int orig_size=0;
    int new_size=0;
    while (1) {
        ch = fgetc(orig_ptr);
        if (ch == EOF)
            break;
        ++orig_size;
    }
    while (1) {
        ch = fgetc(new_ptr);
        if (ch == EOF)
            break;
        ++new_size;
    }
    node *head=NULL;
    orig_ptr=fopen(origin,"rb");
    new_ptr=fopen(new,"rb");
    unsigned char *orig_buffer=malloc(orig_size*sizeof(char));
    unsigned char *new_buffer=malloc(new_size* sizeof(char));
    fread(orig_buffer,1,orig_size,orig_ptr);
    fread(new_buffer,1,new_size,new_ptr);
    int i=0;
    if(orig_size>=new_size){
        while(i<orig_size){
            if(orig_buffer[i]!=new_buffer[i]){
                struct diff to_add;
                to_add.new_value=new_buffer[i];
                to_add.orig_value=orig_buffer[i];
                to_add.offset=i;
                if(head==NULL)
                    head=list_append(head,&to_add);
                else
                    list_append(head,&to_add);
            }
            i++;
        }
    }
    else{
        while(i<new_size){
            if(orig_buffer[i]!=new_buffer[i]){
                struct diff to_add;
                to_add.new_value=new_buffer[i];
                to_add.orig_value=orig_buffer[i];
                to_add.offset=i;
                if(head==NULL)
                    head=list_append(head,&to_add);
                else
                    list_append(head,&to_add);
            }
            i++;
        }
    }
    list_print(head,NULL);
    list_free(head);
    free(orig_buffer);
    free(new_buffer);


}
int main(int argc, char **argv) {
    bcmp(argv[1],argv[2]);


}

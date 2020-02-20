#include <ctype.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void change(char *new, int howmany, node *head){
    FILE *new_ptr=fopen(new,"wb");
    node *temp=head;
    if(howmany==-1) {
        while (temp != NULL) {
            char c[1] = {(char) temp->diff_data->orig_value};
            fseek(new_ptr, temp->diff_data->offset, SEEK_SET);
            fwrite(c, sizeof(char), 1, new_ptr);
            temp = temp->next;
        }
    }
    else{
        int i=0;
        while (temp != NULL&&i<howmany) {
            char c[1] = {(char) temp->diff_data->orig_value};
            fseek(new_ptr, temp->diff_data->offset, SEEK_SET);
            fwrite(c, sizeof(char), 1, new_ptr);
            temp = temp->next;
            i++;
        }
    }

    fclose(new_ptr);

}
void list_print(node *diff_list,FILE* output,int howmany)/* Print the nodes in diff_list in the following format: byte POSITION ORIG_VALUE NEW_VALUE.
Each item followed by a newline character. */
{
    node *temp=diff_list;
    if(temp==NULL)
        exit(8);
    if(output!=NULL&&howmany==-1) {/*just print everything to a file*/
        while (temp != NULL) {
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
    else if(output!=NULL&&howmany!=-1){
        int i=0;
        while (temp!= NULL&&i<howmany) {
            fprintf(output,"%s", "byte ");
            fprintf(output, "%ld", temp->diff_data->offset);
            fprintf(output, " ");
            fprintf(output, "%u", temp->diff_data->orig_value);
            fprintf(output, " ");
            fprintf(output, "%u", temp->diff_data->new_value);
            fprintf(output,"%c", '\n');
            temp = temp->next;
            i++;
        }

    }
    else if(output==NULL&&howmany!=-1) {
        int i = 0;
        while (temp != NULL && i < howmany) {
            printf("%s", "byte ");
            printf("%ld", temp->diff_data->offset);
            printf(" ");
            printf("%u", temp->diff_data->orig_value);
            printf(" ");
            printf("%u", temp->diff_data->new_value);
            printf("%c",'\n');
            temp = temp->next;
            i++;
        }
    }
    else if(output==NULL) {
        while (temp!= NULL) {
            printf("%s", "byte ");
            printf("%ld", temp->diff_data->offset);
            printf(" ");
            printf("%u", temp->diff_data->orig_value);
            printf(" ");
            printf("%u", temp->diff_data->new_value);
            printf("\n");
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



void mybcmp(char *origin, char *new, char *link,int firstflag,int secondflag,int howmany){
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
    if(howmany==-1&&firstflag==0) {
        if (link == NULL) {/*basic case-just print*/
            list_print(head, NULL, -1);
            list_free(head);
            free(orig_buffer);
            free(new_buffer);
        }
        else{/*printing to a file*/
            FILE *out=fopen(link,"wb");
            list_print(head, out, -1);
            list_free(head);
            free(orig_buffer);
            free(new_buffer);
        }
    }

    else if(firstflag==1&&secondflag==-1){/*prints the number of differences*/
        int i=0;
        if(head==NULL)
            printf("%d",0);
        node *temp=head;
        while(temp!=NULL){
            temp=temp->next;
            i++;
        }
        printf("%d",i);
        list_free(head);
        free(orig_buffer);
        free(new_buffer);
    }
    else if(firstflag==1&&secondflag!=-1){/*prints the number of differences into a file*/
        int i=1;
        FILE *out=fopen(link,"wb");
        if(head==NULL)
            fprintf(out,"%d",0);
        node *temp=head;
        while(temp!=NULL){
            temp=temp->next;
            i++;
        }
        fprintf(out,"%d",i);
        list_free(head);
        free(orig_buffer);
        free(new_buffer);
    }
    else if(firstflag==2&&secondflag==-1) {/*print howmany differences*/
        list_print(head, NULL,howmany);
        list_free(head);
        free(orig_buffer);
        free(new_buffer);
    }
    else if(howmany!=-1&&secondflag!=-1){/*meaning printing howmany differences to a file*/
        FILE *out=fopen(link,"wb");
        list_print(head, out,howmany);
        list_free(head);
        free(orig_buffer);
        free(new_buffer);
    }
    else if(firstflag==5&&secondflag==-1){/*just changing the 'new' file*/
        change(new,-1,head);
        list_free(head);
        free(orig_buffer);
        free(new_buffer);
    }
    else if(firstflag==5&&secondflag==2){/*changing the new file to a certain point*/
        change(new, howmany,head);
        list_free(head);
        free(orig_buffer);
        free(new_buffer);
    }
}
int main(int argc, char **argv) {

    if (argc == 3) {/*basic case*/
        mybcmp(argv[1], argv[2], " ", 0, -1, -1);
    } else if (argc == 4) {
        if (strcmp(argv[3], "-r") == 0) {/*change the 'new' file all the way*/
            mybcmp(argv[1], argv[2], " ", 5, -1, -1);
        } else if (strcmp(argv[3], "-t") == 0) {/*print number of differences*/
            mybcmp(argv[1], argv[2], " ", 1, -1, -1);
        }
    } else if (argc == 5) {
        if (strcmp(argv[3], "-r") == 0) {/*change 'new' to a certain point*/
            mybcmp(argv[1], argv[2], " ", 5, -1, atoi(argv[4]));
        } else if (strcmp(argv[3], "-o") == 0) {/*print differences to a file*/
            mybcmp(argv[1], argv[2], argv[4], 3, -1, -1);
        } else if (strcmp(argv[3], "-k") == 0) {/*print differences to a certain point*/
            mybcmp(argv[1], argv[2], " ", 2, -1, atoi(argv[4]));
        }
    } else if (argc == 6) {/*print number of differences into a file. We check to find where '-t' is*/
        if (strcmp(argv[3], "-t") == 0) {
            mybcmp(argv[1], argv[2], argv[5], 1, 3, -1);
        } else if (strcmp(argv[5], "-t") == 0) {
            mybcmp(argv[1], argv[2], argv[4], 1, 3, -1);
        }
    } else if (argc == 7) {/*print the differences into a file. We check to find where '-k' is*/
        if (strcmp(argv[3], "-k") == 0) {
            mybcmp(argv[1], argv[2], argv[6], 2, 3, atoi(argv[4]));
        } else if (strcmp(argv[5], "-k") == 0) {
            mybcmp(argv[1], argv[2], argv[4], 2, 3, atoi(argv[6]));
        }
    }
    return 0;
}

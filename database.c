#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct data{
	int id;
	char name[20];
};

int main()
{
	printf("Database implementation in c\n");
	printf("1. Insert new data\n");   
	printf("2. Read all data\n");
	printf("3. View last entry\n");
	printf("4. Delete last entry\n");
	printf("5. Modify entry by id\n");
	int ch;
	scanf("%d",&ch);
	getchar();
	switch(ch){
		case 1:{	
			struct data d; 
			printf("Enter your name\n");
			char s[20];
			fgets(s,20,stdin);
			s[strlen(s)-1]='\0';
			if(s[0]=='\0')
			{
				printf("Error! Null string entered. Exiting.\n");
				exit(1);
			}
			FILE *fptr;

			if((fptr=fopen("database.bin","rb+"))==NULL){
				printf("Error opening file!");
				exit(1);
 			}
			fseek(fptr,0,SEEK_END);
			unsigned int size=ftell(fptr); 

			if(size==0){
				d.id=1;
				strcpy(d.name,s);
				fwrite(&d,sizeof(struct data),1,fptr);
				fclose(fptr);
			}
			else
			{
				fseek(fptr,-sizeof(struct data), SEEK_END);
				struct data prev;
				fread(&prev, sizeof(struct data), 1, fptr);
				int id=prev.id+1;
				d.id=id;
				strcpy(d.name,s);
				fwrite(&d,sizeof(struct data),1,fptr);
				fclose(fptr);
			}
			break;
		}
	        case 2:{	
			struct data d;
			FILE *fptr;

			if((fptr=fopen("database.bin","rb"))==NULL){
				printf("Error opening file!");
				exit(1);
 			}
			while(fread(&d, sizeof(struct data),1, fptr)!=0)
			{
				printf("id: %d\n",d.id);
				printf("name: %s\n",d.name);
			}
			fclose(fptr);
			break;
		}
		case 3:{
			struct data d;
			FILE *fptr;

			if((fptr=fopen("database.bin","rb+"))==NULL){
				printf("Error opening file!");
				exit(1); 
			}
			fseek(fptr,-sizeof(struct data),SEEK_END);
			if(fread(&d,sizeof(struct data),1,fptr))
			{
				printf("Printing last entry::\n\n");
				printf("id: %d\n",d.id);
				printf("name: %s\n\n",d.name);
				fclose(fptr);
			}
			else
			{
				printf("Error!No entries in database");
			}
			break;
		}
		case 4:{
			struct data d;
			FILE *fptr;

			if((fptr=fopen("database.bin","rb+"))==NULL){
				printf("Error opening file!");
				exit(1);
			}
			fseek(fptr,-sizeof(struct data),SEEK_END);
			if(fread(&d,sizeof(struct data),1,fptr))
			{
				int id=d.id;
				FILE *fptr2;
				fptr2=fopen("temp.bin","wb");
				fseek(fptr,0,SEEK_SET);
				while(fread(&d,sizeof(struct data),1,fptr))
				{
					if(d.id!=id){
						fwrite(&d,sizeof(struct data),1,fptr2);
					}
				}
				fclose(fptr);
				fclose(fptr2);
				remove("database.bin");
				rename("temp.bin","database.bin");
				printf("Last Record successfully deleted\n");
			}
			else
			{
				printf("Error! Database empty");
			}
			break;
		}
		case 5:{
			struct data d;
			FILE *fptr;

			if((fptr=fopen("database.bin","rb+"))==NULL){
				printf("Error opening file!");
				exit(1);
			}
			printf("Enter the id you want to change");
			int id;  
			scanf("%d",&id);
			getchar();
			int found=0;
			while(fread(&d,sizeof(struct data),1,fptr))           
			{
				if(d.id==id)
				{
					found=1;
					break;
				}
			}
			if(found)
			{
				fseek(fptr, -sizeof(struct data), SEEK_CUR);
				printf("Record found!!\n");
				printf("Enter new name\n");
				char s[20];
				fgets(s,20,stdin);
				s[strlen(s)-1]='\0';
				if(s[0]=='\0')
				{
					printf("Error! Null string entered. Exiting");
					exit(1);
				}
				strcpy(d.name,s);
				if(fwrite(&d,sizeof(struct data),1,fptr))
				{
					printf("Record updated successfully");
				}
			}
			else
			{
				printf("Error! Record not found!");
			}
			fclose(fptr);
			break;
		}

		default: 
			printf("Wrong option selected\n");
	}
	return 0;
}





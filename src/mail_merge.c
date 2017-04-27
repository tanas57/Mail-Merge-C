/*
 * Created by Tayyip on 15.04.2017
*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/types.h>
#include <sys/stat.h>

// read XML files
void print_element_names(xmlNode * a_node);
void template_file_content(xmlNode * a_node);
// control and print
bool commandControl(int nums, char *com[]);
bool FileControl(char *file);
void saveFile(char *template, char *items, char *output);
void print(char *mail, char *id);
// string operations
char *replace_str(char *str, char *orig, char *rep);
char * combineStr(const char *s1, const char *s2);

int counter = 0, changeCounter = 0;
/*
 * counter: sum of elements in template.xml
 * changeCounter : all elements that is needed do student have? 
 * fileContent : template.xml
 * mail : changed template.xml
 * id : student id
*/
char *fileContent, *mail, *id, *outputPath;

void print(char *mail, char *id)// save as txt file (mail content, student number)
{
	char *savePath = malloc(100);
	strcpy(savePath, outputPath);
	strcat(savePath, "/");
	strcat(savePath, id);
	strcat(savePath, ".txt");
	//printf("Student ID : %s | outputPath : %s \n", id, savePath);
	
    FILE *fp = fopen(savePath, "w");
    if(!fp) {
        //printf("Could not open file!");
        return;
    }
    free(savePath);
    strcpy(mail, strchr(mail, '\n'));
    //*mail = '\0';
    fprintf(fp,"%s", mail);
    fclose(fp);
}
int main(int argc, char *args[])
{
	fileContent = malloc(4096);
	outputPath  = malloc(4096);
	mail 		= malloc(4096);
	id	 		= malloc(30);
    if(!commandControl(argc, args))
    {
		printf("Command is invalid");
	}
	//printf("%s \n\n", tempFile);
	free(fileContent);
	free(outputPath);
	free(mail);
	free(id);
	return 0;
}
char* combineStr(const char *s1, const char *s2) // combine two strings
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
char *replace_str(char *str, char *orig, char *rep) // find a string, replace it
{
  static char buffer[1024];
  char *p;

  if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
    return str;

  strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
  buffer[p-str] = '\0';

  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

  return buffer;
}
bool commandControl(int nums, char *com[]) // arguments are controled, 
{
    int counter = 1, args = 0; 
    // for each argument  -t -o -v
    bool nameControl[] = { 0, 0, 0 };
    bool fileControl[] = { 0, 0, 0 };
    // strcmp
	if(nums > 1)
	{
			char *arg1, *arg2, *arg3; // save file function arguments
	        while(counter < nums)
	        {
	                // -t asdasd.xml
	                if( nameControl[0] == 0 && strcmp("-t", com[counter]) == 0 )
	                {
	                       if(com[counter + 1] != NULL) // is file entered ?
	                       {
								fileControl[0] =  FileControl(com[counter + 1]);
								arg1 = com[counter + 1]; // template file name
								nameControl[0] =  true;
						   }
						   else{
								nameControl[0] =  false;
								fileControl[0] =  false;
						   }
	                       
	                }
	                else if( nameControl[1] == 0 && strcmp("-v", com[counter]) == 0 )
	                {
							if(com[counter + 1] != NULL)
							{
								fileControl[1] =  FileControl(com[counter + 1]);
								arg2 = com[counter + 1]; // items file name
								nameControl[1] =  true;
							}
							else{
								nameControl[1] =  false;
								fileControl[1] =  false;
							}
	                }
	                else if( nameControl[2] == 0 && strcmp("-o", com[counter]) == 0 )
	                {
							if(com[counter + 1] != NULL)
							{
								arg3 = com[counter + 1]; // items file name
								if(strcmp(arg3, " ") != 0)
								{
									nameControl[2] =  true;
									mkdir(arg3, 0755);
									nameControl[2] = true;
									fileControl[2] = true;
									// file output path is copied to outputh common variable
									strcpy(outputPath, arg3);
								}
								else{
									nameControl[2] =  false;
									fileControl[2] =  false;
								}
							}
							else{
								nameControl[2] =  false;
								fileControl[2] =  false;
							}
	                }
	                counter++;
	        }
	        int i = 0;
	        bool lineControl = true;
	        while( i < 3)
	        {
	                if( !(nameControl[i] && fileControl[i])) lineControl = false;
	                
	                i++;
	        }
	        // nameControl and fileControl's each index value must be true, otherwise this command is invalid
	        if(lineControl) saveFile(arg1,arg2,arg3); // after controls, file operations are made in there
	        return lineControl;
	}
	else
	{
	        return false;
	}
}
void print_element_names(xmlNode * a_node) // read variables. XML, control template.xml, change operations, and save..
{
	xmlNode *cur_node = NULL;
	xmlAttr *attributes = NULL;
	bool control = true;
	// control : is there a ID
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) 
	{	
		if (cur_node->type == XML_ELEMENT_NODE ) 
		{
			if(strcmp(cur_node->name, "root") != 0 && strcmp(cur_node->name, "student") != 0)
			{
				
				char *name = malloc(sizeof(cur_node->name) + 1);
				char *content = malloc(sizeof(cur_node->children->content) + 1);
				
				strcpy(name, cur_node->name);
				
				char *tag = malloc(100);
				
				strcat(tag, "<");
				strcat(tag, name);
				strcat(tag, "/>");
				strcpy(name, tag);
				
				free(tag);
				
				strcpy(content,cur_node->children->content);
				strcpy(mail, replace_str(mail, name, content));
				
				free(name);
				free(content);
				
				// attributlere bakmayı unutma
				changeCounter++;
			}
			else if(strcmp(cur_node->name, "student") == 0)
			{
				changeCounter = 0;
				strcpy(mail, fileContent);
				// get student ID
				attributes = cur_node->properties;
				while(attributes)
				{
					if(strcmp(attributes->name, "id") == 0) {
						
						char *name = malloc(30);
						char *content = malloc(30);
						strcpy(name, attributes->name);
						
						char *tag = malloc(100);
				
						strcat(tag, "<");
						strcat(tag, name);
						strcat(tag, "/>");
						strcpy(name, tag);
						free(tag);
						
						strcpy(id, attributes->children->content);
						
						strcpy(content, attributes->children->content);
						
						strcpy(mail, replace_str(mail, name, content));
						
						free(name); free(content);
						
						control = true; // there is ID 
						
						changeCounter++;
					}
					attributes = attributes->next;
				}
			}
            // needed elements are found in current student, and student has a ID
			if((counter + 1 == changeCounter) && control)
			{
				strcpy(mail, replace_str(mail, "<id/>", id)); // a bug fix
				print(mail, id);
				//printf("%s ", mail);
				control = false;
			}
		}
		print_element_names(cur_node->children);
	}
		
}
void saveFile(char *template, char *items, char *output) // after control operation, if argument is valid, save as a txt
{
		// for items file
		char *templateFile = "";
		// for template file
		xmlDoc *temp_doc = NULL;
		xmlNode *temp_root_element = NULL;
		const char *temp_Filename = template;
		temp_doc = xmlReadFile(temp_Filename, NULL, 0);
		temp_root_element = xmlDocGetRootElement(temp_doc);
			
		template_file_content(temp_root_element);
			
		xmlFreeDoc(temp_doc); /*free the document*/
		xmlCleanupParser(); /*Free the global variables that may have been allocated by the parser.*/
		
		////
		//printf("Counter : %d \n--- %s",counter,fileContent);
		////
		
		xmlDoc *item_doc = NULL;
		xmlNode *item_root_element = NULL;
		const char *item_Filename = items;
		item_doc = xmlReadFile(item_Filename, NULL, 0);
		item_root_element = xmlDocGetRootElement(item_doc);
		
		print_element_names(item_root_element);
		
		xmlFreeDoc(item_doc); /*free the document*/
		xmlCleanupParser(); /*Free the global variables that may have been allocated by the parser.*/
}
void template_file_content(xmlNode * a_node) // read template XML
{
	xmlNode *cur_node = NULL;
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) 
	{	
		if (cur_node->type == XML_ELEMENT_NODE)  // ELEMENT_NODE
		{
			// first node is root!
			if(cur_node != a_node/*strcmp(cur_node->name, "mail") != 0*/)
			{
				counter++;
				char* name = malloc(30);
				strcpy(name, cur_node->name);
				
				char *tag = malloc(100);
				
				strcat(tag, "<");
				strcat(tag, name);
				strcat(tag, "/>");
				strcpy(name, tag);
				free(tag);
				// templateFile
				// write element nodes
				strcpy(fileContent,combineStr(fileContent, name));
				free(name);
			}
			
		}
		else if (cur_node->type == XML_TEXT_NODE)  // TEXT_NODE
		{
			// write text nodes
            strcpy(fileContent,combineStr(fileContent, cur_node->content));
		}
		template_file_content(cur_node->children);
	}
}
bool FileControl(char *file) // is file exist ?
{
        FILE *fp = fopen(file, "r");
        if(!fp) {
                printf("Could not open file \n");
                return false;
        }
        fclose(fp);
        return true;
}

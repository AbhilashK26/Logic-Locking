#include<stdio.h>
#include<string.h>

int copyfiles(char *destfile,char *sourcefile)
{
    FILE *dest, *source;
    dest = fopen(destfile,"w");
    if(dest == NULL)
    {
        return 0;
    }
    source = fopen(sourcefile,"r");
    if(source == NULL)
    {
        return 0;
    }
    char ch;
    while((ch = fgetc(source)) != EOF)
    {
        fputc(ch,dest);
    }
    fclose(dest);
    fclose(source);
    return 1;

}
void replace(char *ch,char *position)
{
    char *ptr = strstr(ch,position);
    char dummy[50]="";
    int i;
    for(i=0;(&ch[i])!=(ptr);i++)
    {
        dummy[i]=ch[i];
    }
    i = i + strlen(position);
    strcat(dummy,position);
    strcat(dummy,"enc");
    strcat(dummy,&ch[i]);
    strcpy(ch,dummy);
}
void addgate(char *gate,char *position,char *key)
{
    FILE *srcFile,*dstFile;
    srcFile = fopen("tempc1355.bench","r+");
    dstFile = fopen("dubc1355.bench","a");
    char ch[50];
    char temp[10];
    strcpy(temp,position);
    strcat(temp,"enc");
    fgets(ch,50,srcFile);
    while((strstr(ch,"OUTPUT"))==NULL)
    {
        if(strstr(ch,"INPUT") != NULL)
        {
        fputs(ch,dstFile);
        }
        fgets(ch,50,srcFile);
    }
    fseek(srcFile,-1-strlen(ch),SEEK_CUR);
    fprintf(dstFile,"INPUT(%s)\n\n",key);
    int outindi = 1;
    fgets(ch,50,srcFile);
    do
    {
        if((strstr(ch,"OUTPUT")!=NULL) && (strstr(ch,position) != NULL))
        {
            fprintf(dstFile,"OUTPUT(%s)\n",temp);
            outindi = 0;
        }
        else if(strstr(ch,"OUTPUT") != NULL)
        {
            fputs(ch,dstFile);
        }
        else
        {
            fprintf(dstFile,"\n");
            break;
        }
    }while(fgets(ch,50,srcFile));
    while((fgets(ch,50,srcFile)) != NULL)
        {
            if((strstr(ch,position) != NULL) && (outindi))
            {
                if(ch != strstr(ch,position))
                {
                replace(ch,position);
                fputs(ch,dstFile);
                }
                else
                {
                    fputs(ch,dstFile);
                }
            }
            else
            {
                fputs(ch,dstFile);
            }
        }
    fprintf(dstFile,"\n");
    fprintf(dstFile,"%s = %s(%s,%s)",temp,gate,position,key);
    fclose(srcFile);
    fclose(dstFile);
}
char* addKey(int i,char *k)
{
    strcpy(k,"key");
    char h[5]="";
    sprintf(h,"%d",i);
    strcat(k,h);
    return k;
}
int main()
{
    copyfiles("tempc1355.bench","c1355.bench");
    FILE *ptr = fopen("c1355nodeslist.txt","r");
    char key[10] = "";
    char ch;
    int i=-1;
    ch = fgetc(ptr);
    while(ch!=EOF)
    {
        if(ch == '\'')
        {
            int j=0;
            i++;
            char temp[10];
            ch=fgetc(ptr);
            temp[j]=ch;
            j++;
            ch=fgetc(ptr);
            while(ch!='\'')
            {
                temp[j]=ch;
                j++;
                ch = fgetc(ptr);
            } 
            temp[j]='\0';
            addgate("XOR",temp,addKey(i,key));
            copyfiles("tempc1355.bench","dubc1355.bench");
            FILE *empty = fopen("dubc1355.bench","w");
            fclose(empty);
        }
        ch=fgetc(ptr);
    }
    fclose(ptr);
    return 0;
}
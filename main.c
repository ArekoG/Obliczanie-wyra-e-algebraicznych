#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <math.h>
#define MAX 100
#define EMPTY -1
#define N -9952771

struct stos
{
    char data;
    struct stos *next;
};


struct lista
{
    char data[MAX];
    struct lista *next;

};

struct drzewo
{
    struct drzewo * left, * right;
    char element[MAX];

}*root;


int isempty(struct stos *top)
{

    if(top)return 1;
    else return 0;
}


struct stos *push(struct stos *top,char element)
{

    struct stos *nowy_element=(struct stos *)malloc(sizeof(struct stos));

    if(nowy_element)
    {

        nowy_element->data=element;
        nowy_element->next=top;
        top=nowy_element;

    }

    return top;

}

char pop(struct stos **top)
{

    char result=(char)EMPTY;

    if(*top)
    {

        result=(*top)->data;
        struct stos *tmp=(*top)->next;
        free(*top);
        *top=tmp;

    }

    return result;
}


int is_operator(char element)
{


    if(element=='+' || element=='-' || element=='*' || element=='/'|| element=='^')return 1;
    else return N;
}

int priorytet_operatorow(char element)
{
    int priorytet=0;

    if(element=='*'|| element=='/' ) priorytet=2;

    if(element == '+' || element == '-')priorytet = 1;

    if(element=='^')priorytet=3;


    return priorytet;
}

void konwersjaDoONP(char *infix,char *ONP)
{
    char *tmp_infix,*tmp_ONP;
    struct stos *top=NULL;
    char pom;


    tmp_infix=&infix[0];
    tmp_ONP=&ONP[0];


    while(*tmp_infix)
    {

        while(*tmp_infix==' ')
        {

            tmp_infix++;
        }

        if(isdigit(*tmp_infix))
        {
            while(isdigit(*tmp_infix) || *tmp_infix=='.')
            {

                *tmp_ONP=*tmp_infix;
                tmp_ONP++;
                tmp_infix++;
            }

            *tmp_ONP=' ';
            tmp_ONP++;
        }

        if(*tmp_infix=='(')
        {
            top=push(top,*tmp_infix);
            tmp_infix++;
        }

        if(*tmp_infix==')')
        {

            pom=pop(&top);
            while(pom!='(')
            {

                *tmp_ONP=pom;
                tmp_ONP++;

                *tmp_ONP=' ';
                tmp_ONP++;

                pom=pop(&top);

            }
            tmp_infix++;

        }

        if(is_operator(*tmp_infix)==1)
        {

            if(isempty(top)==0)top=push(top,*tmp_infix);

            else if(isempty(top))
            {
                pom=pop(&top);

                while(priorytet_operatorow(pom)>=priorytet_operatorow(*tmp_infix))
                {
                    *tmp_ONP=pom;
                    tmp_ONP++;

                    *tmp_ONP=' ';
                    tmp_ONP++;

                    pom=pop(&top);
                }
                top=push(top,pom);
                top=push(top,*tmp_infix);
            }
            tmp_infix++;
        }
    }

    while(isempty(top))
    {

        pom=pop(&top);
        *tmp_ONP=pom;
        tmp_ONP++;

        *tmp_ONP=' ';
        tmp_ONP++;
    }
    *tmp_ONP='\0';
}

void dzielnie_przez_zero(struct lista *head)
{

    while(head)
    {

        if(head->data[0]=='0' && head->next->data[0]=='/')
        {
            puts("Nie mozna dzielic przez 0!\n");
            exit(0);
        }

        head=head->next;
    }

}

void jeden_element(struct lista *head)
{

    if(head->next==NULL)
    {

    if(is_operator(head->data[0])==1)
    {
    printf("Niepoprawne wyrazenie!");
     exit(0);
    }
    else
        printf("Wynik dzialania to :%s\n",head->data);
        exit(0);

    }
    else if (head->next->next==NULL)
    {

        puts("Niepoprawne wyrazenie!");
        exit(0);
    }

}



void dodaj_do_listy(int i,char ONP[i],struct lista **head)
{

    int j=0;
    if(i<strlen(ONP))
    {


        if(ONP[i]!=' ')
        {


            char *znak=(char*)calloc(sizeof(char),100);
            while(ONP[i]!=' ')
            {



                znak[j]=ONP[i];
                i++;
                j++;

            }
            struct lista *nowy_element=(struct lista*)malloc(sizeof(struct lista));
            nowy_element->next=NULL;
            strncpy(nowy_element->data,znak,MAX);
            free(znak);
            if(*head==NULL) (*head)=nowy_element;
            else
            {

                struct lista* tmp=(*head);
                while(tmp->next)tmp=tmp->next;
                tmp->next=nowy_element;

            }
        }

        if(ONP[i]==' ')
        {

            dodaj_do_listy(i+1,ONP,head);
        }


    }
    dzielnie_przez_zero(*head);
    jeden_element(*head);

}


struct drzewo *dodaj(struct lista *head)
{

    struct drzewo *wskazniki[100];


    int index=0;
    struct drzewo *wezel;


    for(; NULL!=head; head=head->next)
    {
        char *end;
        double licz=strtod(head->data,&end);

        wezel=(struct drzewo *)malloc(sizeof(struct drzewo));

        strncpy(wezel->element,head->data,MAX);

        if(is_operator(head->data[0])==1)
        {

            wezel->right=wskazniki[--index];
            wezel->left=wskazniki[--index];
            wskazniki[index++]=wezel;

        }

        if(licz || head->data[0]=='0')
        {

            wezel->right=NULL;
            wezel->left=NULL;
            wskazniki[index++]=wezel;

        }

    }

    return wskazniki[index-1];
}


double oblicz(double wynik,struct drzewo *root)
{
    char *end;

    int flaga1=0;
    int flaga2=0;

    double number_a;
    double number_b;

    if(is_operator(root->left->element[0])==N)
    {

        number_a=strtod(root->left->element,&end);
        flaga1++;

    }
    else number_a=-N;


    if(is_operator(root->right->element[0])==N)
    {
        number_b=strtod(root->right->element,&end);
        flaga2++;
    }
    else number_b=-N;


    if(root->element[0]=='*')
    {

        if(root->element[0]=='*' && flaga1 && flaga2)
        {

            snprintf(root->element,MAX,"%f",number_a*number_b);
            return wynik+=number_a*number_b;
        }
        else if(flaga1==0 && number_b!=-N) return wynik=oblicz(wynik,root->left)*number_b;
        else if(flaga2==0 && number_a!=-N)return wynik=number_a*oblicz(wynik,root->right);
        else return oblicz(wynik,root->left)*oblicz(wynik,root->right);
    }


    if(root->element[0]=='/')
    {

        if(root->element[0]=='/' && flaga1 && flaga2)
        {
            snprintf(root->element,MAX, "%f",number_a/number_b);

            return wynik+=number_a/number_b;
        }
        else if(flaga1==0 && number_b!=-N) return wynik=oblicz(wynik,root->left)/number_b;
        else if(flaga2==0 && number_a!=-N)return wynik=number_a/oblicz(wynik,root->right);
        else return oblicz(wynik,root->left)/oblicz(wynik,root->right);
    }

    if(root->element[0]=='+')
    {

        if(root->element[0]=='+' && flaga1 && flaga2)
        {
            snprintf(root->element,MAX, "%f",number_a+number_b);
            return wynik+=number_a+number_b;
        }
        else if(flaga1==0 && number_b!=-N) return wynik=oblicz(wynik,root->left)+number_b;
        else if(flaga2==0 && number_a!=-N) return wynik=number_a+oblicz(wynik,root->right);
        else return oblicz(wynik,root->left)+oblicz(wynik,root->right);
    }

    if(root->element[0]=='-')
    {
        if(root->element[0]=='-' && flaga1 && flaga2)
        {


            snprintf(root->element,MAX, "%f",number_a-number_b);
            return wynik+=number_a-number_b;

        }
        else if(flaga1==0 && number_b!=-N)return wynik=oblicz(wynik,root->left)-number_b;
        else if(flaga2==0 && number_a!=-N) return wynik=number_a-oblicz(wynik,root->right);
        else return oblicz(wynik,root->left)-oblicz(wynik,root->right);
    }

    if(root->element[0]=='^')
    {
        if(flaga1 && flaga2)
        {
            if(number_a==0)
            {
                snprintf(root->element,MAX, "%f",0.0);
                return wynik+=1;
            }
            snprintf(root->element,MAX, "%f",pow(number_a,number_b));
            return wynik+=pow(number_a,number_b);
        }
        else
            return wynik=pow(oblicz(wynik,root->left),number_b);
    }

    return -1;

}

void remove_list(struct lista **head)
{

    while(*head)
    {

        struct lista *next=(*head)->next;
        free(*head);
        *head=next;
    }
}

void remove_tree(struct drzewo **korzen)
{

    if(*korzen!=NULL)
    {

        remove_tree(&(*korzen)->left);
        remove_tree(&(*korzen)->right);
        free(*korzen);
        *korzen=NULL;
    }
}

void kontrola_bledow(char *wyrazenie)
{


char *tmp_infix=&wyrazenie[0];
tmp_infix++;
    while(*wyrazenie)
    {


        if((!isdigit(*wyrazenie) && is_operator(*wyrazenie)!=1 &&  *wyrazenie!='.' && *wyrazenie!='(' && *wyrazenie!=')') || (is_operator(*wyrazenie)==1 && is_operator(*tmp_infix)==1))
        {

            printf("Niepoprawne wyrazenie!");
            exit(0);

        }
        wyrazenie++;
        tmp_infix++;

    }


}


int main()
{
    char wejsciowe_wyrazenie[50];
    char wyjsciowe_wyrazenie[50];
    struct lista *head=NULL;
    wyjsciowe_wyrazenie[0]=0;


    puts("-------------------------------------------------------------------------------");
    puts("               Obliczanie wyrazen arytmetycznych za pomoca drzewa");
    puts("-------------------------------------------------------------------------------");
    printf("\nPodaj swoje wyrazenie,zakoncz je enterem: ");
    fflush(stdin);
    scanf("%s",wejsciowe_wyrazenie);

    kontrola_bledow(wejsciowe_wyrazenie);
    konwersjaDoONP(wejsciowe_wyrazenie,wyjsciowe_wyrazenie);

    dodaj_do_listy(0,wyjsciowe_wyrazenie,&head);
    root=dodaj(head);


    printf("\nWynik dzialania to:%lf ",oblicz(0,root));

    puts("\n");
    remove_list(&head);
    remove_tree(&root);
    system("pause");
    return 0;
}

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct globalordersnode{
    int agentid;
    char order[100];
    int restid;
    struct globalordersnode*next;
}globalordersnode;

globalordersnode* globalordershead=0;
globalordersnode* globalorderstail=0;

void printliveorders()
{
    globalordersnode*mov=globalordershead;
    while(mov!=0)
    {
        printf("order=%s , agentid=%d\n",mov->order,mov->agentid);
        mov=mov->next;
    }
}

globalordersnode* makeglobalordersnode(int id,char*order,int restid)
{
    globalordersnode* temp=(globalordersnode*)malloc(sizeof(globalordersnode));
    temp->agentid=id;
    strcpy(temp->order,order);
    temp->restid=restid;
    temp->next=0;
    return temp;
}

typedef struct prevordersnode{

    char order[100];
    int month;
    int count;
    struct prevordersnode* next;
}prevordersnode;

prevordersnode* makeprevordersnode(char* order,int month)
{
    prevordersnode* temp=(prevordersnode*)malloc(sizeof(prevordersnode));
    strcpy(temp->order,order);
    temp->month=month;
    temp->next=0;
    temp->count=0;
    return temp;
}
typedef struct restordersnode{

    char order[100];
    int month;
    int count;
    struct restordersnode* next;
}restordersnode;

restordersnode* makerestordersnode(char* order,int month)
{
    restordersnode* temp=(restordersnode*)malloc(sizeof(restordersnode));
    strcpy(temp->order,order);
    temp->month=month;
    temp->next=0;
    temp->count=0;
    return temp;
}
typedef struct restaurantnode{
    int restid;
    char address[100];
    int seats;
    char facility[100];
    char menu[100];
    restordersnode* ordershead;
    struct restaurantnode*next;
}restaurantnode;
restaurantnode* makerestaurantnode(int id,char* address,int s,char*facility,char*menu)
{
    restaurantnode* temp=(restaurantnode*)malloc(sizeof(restaurantnode));
    strcpy(temp->address,address);
    temp->seats=s;
    strcpy(temp->facility,facility);
    strcpy(temp->menu,menu);
    temp->ordershead=NULL;
    temp->next=0;
    temp->restid=id;
    return temp;
}

typedef struct agentnode{
    int agentid;
    char name[100];
    char phoneno[15];
    int accucommision;
    int working;
    struct agentnode*next;
}agentnode;
agentnode* makeagentnode(char* name,int id,char*phoneno)
{
    agentnode* temp=(agentnode*)malloc(sizeof(agentnode));
    strcpy(temp->name,name);
    temp->agentid=id;
    strcpy(temp->phoneno,phoneno);
    temp->accucommision=0;
    temp->working=0;
    temp->next=0;
    return temp;
}
typedef struct usernode{
    int userid;
    char name[100];
    char address[100];
    char phoneno[15];
    prevordersnode* prevordershead;
    struct usernode*next;
}usernode;
usernode* makeusernode(char*name,int id,char*phoneno,char* address)
{
    usernode* temp=(usernode*)malloc(sizeof(usernode));
    strcpy(temp->address,address);
    temp->userid=id;
    strcpy(temp->name,name);
    strcpy(temp->phoneno,phoneno);
    temp->prevordershead=NULL;
    temp->next=0;
    return temp;
}





void search(char* category,char* food,char*area,restaurantnode*restaurantnodehead)
{
    restaurantnode *mov=restaurantnodehead;
    while(mov!=0)
    {
        if(strcmp(mov->address,area)==0) printf("%d\n",mov->restid);
        else if(strcmp(mov->menu,food)==0) printf("%d\n",mov->restid);
        else if(strcmp(mov->facility,category)==0) printf("%d\n",mov->restid);
        mov=mov->next;  
    }
}

void order(char* name,char* address,char* phoneno,char* menu,agentnode*agentnodehead,int restid,usernode*usernodehead,int userid,int month,restaurantnode*restauranthead)
{
    //step1 travel the agentlist and see who is free and you got exit from loop and 
    //insert the order in gloalorderlist with the agent and mark agent as busy.
    //if no agent available the say no agent available and we will not take that order(because to insert in global list we need the agent id).
    
    //step 2 userlist mai ek prevorders list hai usee update karna hai by inserting(special way by incrementing count of that menu by searching and not creating copy of same menu )
    
    //step 3 restaurantlist mai ek orders list hai usee update karna hai by inserting(special way by incrementing count of that menu by searching and not creating copy of same menu )
    
    agentnode*mov=agentnodehead;
    while(mov!=0&&mov->working) mov=mov->next;
    if(mov)
    {
        if(globalorderstail)
        {
            globalorderstail->next=makeglobalordersnode(mov->agentid,menu,restid);
            mov->working=1;
            globalorderstail=globalorderstail->next;
        }
        else
        {
            mov->working=1;
            globalordershead=makeglobalordersnode(mov->agentid,menu,restid);
            globalorderstail=globalordershead;
        }
    }
    else
    {
        printf("no agent available\n");
    }
    //printliveorders();
    //printf("age1\n");
    usernode* mov2=usernodehead;
    while(mov2&&mov2->userid!=userid) mov2=mov2->next;
    //printf("%s\n",mov2->name);
    prevordersnode* mov3=mov2->prevordershead;
    //if(mov3) printf("%s\n",mov3->order);
    //printf("%s\n",menu);
    if(mov3)
    {
        prevordersnode*prev=0;
        int flag=1;
        while(mov3&&flag) 
        {
            if(strcmp(mov3->order,menu)==0)
            {
                mov3->count++;
                mov3->month=month;
                flag=0;
            }
            prev=mov3;
            mov3=mov3->next;
        }
        if(flag)
        {
            prev->next=makeprevordersnode(menu,month);
            prev->next->count++;
        }
        
    }
    else
    {
        mov2->prevordershead=makeprevordersnode(menu,month);
        mov2->prevordershead->count++;
    }
    
    restaurantnode*mov4=restauranthead;
    while(mov4->restid!=restid) mov4=mov4->next;
    restordersnode* mov5=mov4->ordershead;
    if(mov5)
    {
        restordersnode*prev=0;
        int flag=1;
        while(mov5&&flag) 
        {
            if(strcmp(mov5->order,menu)==0)
            {
                mov5->count++;
                mov5->month=month;
                flag=0;
            }
            prev=mov5;
            mov5=mov5->next;
        }
        if(flag)
        {
            prev->next=makerestordersnode(menu,month);
            prev->next->count++;
        }
        
    }
    else
    {
        mov4->ordershead=makerestordersnode(menu,month);
        mov4->ordershead->count++;
    }

}
void delivery(int agentid,agentnode*agentnodehead,int comm)
{
    
    //step1 agentlist ko travel karo and uss agent ko commission add karo and uss agent ko not working mark karo.
    //step2 globallist ko travel karo and remove that order from globalorderlist.
    agentnode*mov1=agentnodehead;
    while(mov1!=0)
    {
        if(mov1->agentid==agentid)
        {
            mov1->working=0;
            printf("I have deliverd my name is %s\n",mov1->name);
            mov1->accucommision=mov1->accucommision+comm;
        }
        mov1=mov1->next;
    }
    globalordersnode*mov2=globalordershead;
    globalordersnode*prev=0;
    while(mov2->agentid!=agentid)
    {
        prev=mov2;
        mov2=mov2->next;
    }
    if(mov2==globalordershead)
    {
        globalordershead=globalordershead->next;
        free(mov2);
    }
    else
    {
        prev->next=mov2->next;
        free(mov2);
        if(mov2==globalorderstail) globalorderstail=prev;
    }
}
void cancelorder(int agentid,agentnode*agentnodehead)
{
    //step1 agentlist ko travel karo and uss agent ko not working mark karo.
    //step2 globallist ko travel karo and remove that order from globalorderlist.
    agentnode*mov1=agentnodehead;
    while(mov1!=0)
    {
        if(mov1->agentid==agentid)
        {
            printf("I have recieved cancel order, my agent id is %d\n",mov1->agentid);
            mov1->working=0;
        }
        mov1=mov1->next;
    }
    globalordersnode*mov2=globalordershead;
    globalordersnode*prev=0;
    while(mov2->agentid!=agentid)
    {
        prev=mov2;
        mov2=mov2->next;
    }
    if(mov2==globalordershead)
    {
        globalordershead=globalordershead->next;
        free(mov2);
    }
    else
    {
        prev->next=mov2->next;
        free(mov2);
        if(mov2==globalorderstail) globalorderstail=prev;
    }
}
void findfavoritefoodsofuser(int userid,usernode*usernodehead)
{
    //step1 first find that user in userlist
    //step2 travel the prevorder list of that user and find top 3 elements.    
    usernode* mov=usernodehead;
    while(mov->userid!=userid) mov=mov->next;
    prevordersnode* mov2=mov->prevordershead;
    int max=0;
    int secondmax=0;
    int thirdmax=0;
    char first[100]="";
    char second[100]="";
    char third[100]="";
    while(mov2!=0)
    {
        if(mov2->month<=3)
        {
            if(mov2->count>max)
            {
                thirdmax=secondmax;
                secondmax=max;
                max=mov2->count;
                strcpy(third,second);
                strcpy(second,first);
                strcpy(first,mov2->order);
            }
            else if(mov2->count>secondmax)
            {
                thirdmax=secondmax;
                secondmax=mov2->count;
                strcpy(third,second);
                strcpy(second,mov2->order);
            }
            else if(mov2->count>thirdmax)
            {
                thirdmax=mov2->count;
                strcpy(third,mov2->order);
            }
        }
        mov2=mov2->next;
    }
    printf("first=%s\n",first);
    printf("second=%s\n",second);
    printf("third=%s\n",third);
}
void findfavoritefoodsacrossrestaurants(restaurantnode*restnodehead)
{
    //step1 for each of the restaurant travel its orderslist and find max,secondmax,thirdmax and keep these max, secondmax,thirdmax. 
    //so at last you will get max,secondmax, thirdmax accross all restarants.
    int max=0;
    int secondmax=0;
    int thirdmax=0;
    char first[100]="";
    char second[100]="";
    char third[100]="";
    restaurantnode*mov=restnodehead;
    while(mov!=0)
    {
        restordersnode*mov2=mov->ordershead;
        while(mov2!=0)
        {
            if(mov2->month<=3)
            {
                if(mov2->count>max)
                {
                    thirdmax=secondmax;
                    secondmax=max;
                    max=mov2->count;
                    strcpy(third,second);
                    strcpy(second,first);
                    strcpy(first,mov2->order);
                }
                else if(mov2->count>secondmax)
                {
                    thirdmax=secondmax;
                    secondmax=mov2->count;
                    strcpy(third,second);
                    strcpy(second,mov2->order);
                }
                else if(mov2->count>thirdmax)
                {
                    thirdmax=mov2->count;
                    strcpy(third,mov2->order);
                }
            }
            mov2=mov2->next;
        }
        mov=mov->next;
    }
    printf("first=%s\n",first);
    printf("second=%s\n",second);
    printf("third=%s\n",third);
}

void findfavoriterestuarants(restaurantnode*restnodehead)
{
    //step1 for each of the restaurant travel its orderslist and find max,secondmax,thirdmax and keep these max, secondmax,thirdmax. 
    //so at last you will get max,secondmax, thirdmax accross all restarants.
    int max=0;
    int secondmax=0;
    int thirdmax=0;
    int firstid=0;
    int secondid=0;
    int thirdid=0;
    restaurantnode*mov=restnodehead;
    while(mov!=0)
    {
        restordersnode*mov2=mov->ordershead;
        while(mov2!=0)
        {
            if(mov2->month<=3)
            {
                if(mov2->count>max)
            {
                thirdmax=secondmax;
                secondmax=max;
                max=mov2->count;
                thirdid=secondid;
                secondid=firstid;
                firstid=mov->restid;
            }
            else if(mov2->count>secondmax)
            {
                thirdmax=secondmax;
                secondmax=mov2->count;
                thirdid=secondid;
                secondid=mov->restid;
            }
            else if(mov2->count>thirdmax)
            {
                thirdmax=mov2->count;
                thirdid=mov->restid;
            }
            }
            mov2=mov2->next;
        }
        mov=mov->next;
    }
    printf("first=%d\n",firstid);
    printf("second=%d\n",secondid);
    printf("third=%d\n",thirdid);
}
void printprevorderlistofparticularuser(usernode*userhead,int id)
{
    usernode*mov=userhead;
    while(mov->userid!=id) mov=mov->next;
    prevordersnode* mov1=mov->prevordershead;
    while(mov1!=0) 
    {
        printf("%s  %d\n",mov1->order,mov1->count);
        mov1=mov1->next;
    }
}
int main()
{
    restaurantnode*restauranthead=makerestaurantnode(1,"bajajnagar",100,"restaurant","chinese");
    restauranthead->next=makerestaurantnode(2,"vijaynagar",50,"pub","northindian");
    restauranthead->next->next=makerestaurantnode(3,"shivajinagar",500,"cafe","southindian");
    
    agentnode*agenthead=makeagentnode("ramu",1,"8686245374");
    agenthead->next=makeagentnode("shamu",2,"7842111628");
    agenthead->next->next=makeagentnode("bablu",3,"7865456280");
    
    usernode*userhead=makeusernode("rock",1,"4546247891","balramnagar");
    userhead->next=makeusernode("aditya",2,"7846247891","ramnagar");
    userhead->next->next=makeusernode("ganesh",3,"7966247891","ganeshnagar");
    
    //Q1
    
    //search("restaurant","chinese","bajajnagar",restauranthead);
    //search("restaurant","northindian","shivajinagar",restauranthead);
    
    //Q2,Q3
    //order("ganesh","ganeshnagar","7966247891","chinese",agenthead,1,userhead,3,1,restauranthead);
    //order("aditya","ramnagar","7846247891","northindian",agenthead,2,userhead,2,2,restauranthead);
    //printliveorders();
    
    //delivery(2,agenthead,100);
    
    //printliveorders();
    
    //Q4
    // order("ganesh","ganeshnagar","7966247891","chinese",agenthead,1,userhead,3,1,restauranthead);
    // order("aditya","ramnagar","7846247891","northindian",agenthead,2,userhead,2,2,restauranthead);
    // order("rock","balramnagar","4546247891","northindian",agenthead,2,userhead,1,1,restauranthead);
    
    // printliveorders();
    // delivery(1,agenthead,100);
    // printliveorders();
    
    // cancelorder(2,agenthead);
    // printliveorders();
    
    //Q5
    //order("ganesh","ganeshnagar","7966247891","chinese",agenthead,1,userhead,3,1,restauranthead);
    //order("ganesh","ganeshnagar","7966247891","northindian",agenthead,2,userhead,3,1,restauranthead);
    //order("ganesh","ganeshnagar","7966247891","chinese",agenthead,1,userhead,3,1,restauranthead);
    /////dontuncommentjustfordebug///////order("ganesh","ganeshnagar","7966247891","chinese",agenthead,1,userhead,3,1,restauranthead);
    /////dontuncommentjustfordebug///////order("ganesh","ganeshnagar","7966247891","chinese",agenthead,1,userhead,3,1,restauranthead);
    /////dontuncommentjustfordebug/////order("aditya","ramnagar","7846247891","northindian",agenthead,2,userhead,2,2,restauranthead);
    //findfavoritefoodsofuser(3,userhead);
    
    ////////printliveorders();
    ////////delivery(1,agenthead,100);
    ////////printprevorderlistofparticularuser(userhead,1);
    ///////printliveorders();
    
    //Q6
    // order("ganesh","ganeshnagar","7966247891","chinese",agenthead,1,userhead,3,1,restauranthead);
    // order("ganesh","ganeshnagar","7966247891","northindian",agenthead,2,userhead,3,1,restauranthead);
    // order("ganesh","ganeshnagar","7966247891","chinese",agenthead,1,userhead,3,1,restauranthead);
    ///////printf("%d",restauranthead->ordershead->count);
    /////////printf("%d",restauranthead->next->ordershead->count);
    // findfavoriterestuarants(restauranthead);
    
    //Q7
    // order("ganesh","ganeshnagar","7966247891","chinese",agenthead,1,userhead,3,1,restauranthead);
    // order("ganesh","ganeshnagar","7966247891","northindian",agenthead,2,userhead,3,1,restauranthead);
    // order("ganesh","ganeshnagar","7966247891","chinese",agenthead,1,userhead,3,1,restauranthead);
    // findfavoritefoodsacrossrestaurants(restauranthead);
    
    
    return 0;
}










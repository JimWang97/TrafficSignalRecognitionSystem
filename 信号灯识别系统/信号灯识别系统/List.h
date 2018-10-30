#include<stdio.h>
#include<stdlib.h>
typedef int elemType;
struct SeqList{
	elemType *list;
	int size;
	int maxSize;
};
void initList(struct SeqList *L,int ms)
{
	if(ms<=0)
	{
		printf("MaxSize 非法");
		exit(1);
	}
	L->list=(elemType *)malloc(ms *sizeof(elemType));
	if(!L->list)
	{
		printf("空间分配失败！");
		exit(1);	
	}
	L->maxSize=ms;
	L->size=0;
	return ;
}
void againMalloc(struct SeqList *L)
{
	elemType *p=(elemType *)realloc(L->list,2*L->maxSize*sizeof(elemType));
	if(!p)
	{
		printf("空间分配失败！ ");
		exit(1); 
	}
	L->list=p;
	L->maxSize=2*L->maxSize;
	
}
void clearList(struct SeqList *L)
{
	if(L->list!=NULL){
		free(L->list);
		L->list=NULL;
		L->size=L->maxSize=0;
		
	}
	
}
int sizeList(struct SeqList *L)
{
	if(L->size==0)
		return 1;
	else 
		return 0;
		
}
elemType getElem(struct SeqList *L,int pos)
{
	if(pos<1||pos>L->size)
	{
		printf("元素序号越界！");
		exit(1);
	}
	return L->list[pos-1];
	
}
void traverseList(struct SeqList *L)
{
	int i;
	for(i=0;i<L->size;i++)
	{
		printf("%d",L->list[i]);
		printf(" ");
	}
	return;
	
}
int  search(struct SeqList *L,elemType x)
{
	int i;
	for(i=0;i<L->size;i++)
		if(L->list[i]==x)
			return i;
	return -1;
	
}
int updataPosList(struct SeqList *L,int pos ,elemType x)
{
	if(pos<1||pos>L->size)
		return 0;
	L->list[pos-1]=x;
	return 1;
	
}
void inserFirstList(struct SeqList *L,elemType x)
{
	int i;
	if(L->size==L->maxSize)
		againMalloc(L);
	for(i=L->size-1;i>=0;i--)
		L->list[i+1]=L->list[i];
	L->list[0]=x;
	L->size++;
	
}
void insertLastList(struct SeqList *L,elemType x)
{
	if(L->size==L->maxSize)
		againMalloc(L);
	L->list[L->size]=x;
	L->size++;
	
}
int insertPosList(struct SeqList *L,int pos,elemType x)
{
	int i;
	if(pos<1||pos>L->size+1)
		return 0;
	if(L->size==L->maxSize)
		againMalloc(L);
	for(i=L->size-1;i>=pos;i--)
		L->list[i+1]=L->list[i];
	L->list[pos-1]=x;
	L->size++;
	return 1;
}
void insertOrderList(struct SeqList *L,elemType x)
{
	int i,j;
	if(L->size==L->maxSize)
		againMalloc(L);
	for(i=0;i<L->size;i++)
		if(x<L->list[i])
			break;
	for(j=L->size-1;j>=i;j--)
		L->list[i]=x;
	L->size++;
	return ;
	
}
elemType deleteFirstList(struct SeqList *L)
{
	elemType temp;
	int i;
	if(L->size==0)
	{
		printf("线性表为空,不能进行删除操作！");
		exit(1);
	}
	temp=L->list[0];
	for(i=1;i<L->size;i++)
		L->list[i-1]=L->list[i];
	L->size--;
	return temp;

}
elemType deleteLastList(struct SeqList *L)
{
	if(L->size==0)
	{
		printf("线性表为空,不能进行删除操作");
		exit(1);
		
	}
	L->size--;
	return L->list[L->size];
	
}
elemType deletePosList(struct SeqList *L,int pos)
{
	elemType temp;
	int i;
	if(pos<1||pos>L->size)
	{
		printf("pos值越界，不能进行删除操作！");
		exit(1);
	}
	temp=L->list[pos-1];
	for(i=pos;i<L->size;i++)
		L->list[i-1]=L->list[i];
	L->size--;
	return temp;
	
}
int deleteValueList(struct SeqList *L,elemType x)
{
	int i,j;
	for(i=0;i<L->size;i++)
		if(L->list[i]==x)
			break;
	if(i==L->size)
		return 0;
	for(j=i+1;j<L->size;j++)
		L->list[j-1]=L->list[j];
	L->size--;
	return 1;
	
}
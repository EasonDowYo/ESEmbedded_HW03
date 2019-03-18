

int OneArgAdd(int a,int b){return a+b;}
int MultiArgAdd(int a,int b,int c,int d,int e){return a+b+c+d+e;}
void reset_handler(void)
{
    int a=1,b=2,c,d,e,f;
    c=OneArgAdd(a,b);
    d=OneArgAdd(c,10);
    e=20;
    f=MultiArgAdd(a,b,c,d,e);
    while(1);
		
        

}

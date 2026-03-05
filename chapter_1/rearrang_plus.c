/*
*这个程序用于读入一条条乱七八糟的字符串，然后根据起始数字代码和终止数字代码来打印字符串
*起始数字代码成对出现
*最后一个负数表示数字代码输入完毕
*/

#include<stdio.h>//包含fgets函数和scanf和printf
#include<string.h>//用于字符串处理
#include<stdlib.h>//提供EXIT_SUCCESS等宏
#define MAX_COLS 20//处理最大行列号
#define MAX_INPUT 1000//每行输入最大长度

/*
*读取配对的起始数字代码和终止数字代码(保证配对)，返回读取行列号个数
*int columns[]数组保存存的数字代码
*max，保证num不超过max导致超出MAX_COLS处理范围，也是column数组容量
*/

int read_column_numbers(int columns[],int max);

/*“重排”
*output容纳
*n_columns是刚刚read_columns_numbers的返回值
*columns数组是刚刚存入数字代码（配对的）数组
*/
void rearrange(char*output,char const*input,int n_columns,int const columns[]);

int main()
{
    int n_columns;
    int columns[MAX_COLS];
    char input[MAX_INPUT];
    char output[MAX_INPUT];//最坏的情况是从头到尾

    n_columns=read_column_numbers(columns,MAX_COLS);

    while(fgets(input,sizeof(input),stdin)!=NULL)
    /*
    *其实 fgets 会保证字符串以 \0 结尾，读到EOF返回NULL
    *表示读取失败或到达文件末尾（EOF）*/
    {
        input[strcspn(input,"\n")]='\0';//去掉可能存在的换行符
        //input[MAX_INPUT-1]='\0';//做打印前的保险工作,但是其实是多余的
        printf("Original input :%s\n",input);
     rearrange(output,input,n_columns,columns);
     printf("Rearranged line:%s\n",output);
    }
    return EXIT_SUCCESS;

}

int read_column_numbers(int columns[],int max)
{
    int num=0;//统计输入起始数字代码和终止数字代码个数
    int ch;//用于清屏

    /*
    *读取条件是num不超过max，并且行标号大于等于0，读取一个，num就自增一次
    *注意到读取一个columns就num++，所以说num和column是同步的
    *由于等会column数组传参时传容量，所以说不能越界，num<max
    */
    while(num<max&&scanf("%d",&columns[num])==1&&columns[num]>=0)
    {
        num++;
    }//注意最后一个数据负数没有读进去

    if(num%2!=0)
    {
        printf("The last number is not paired.\n");
        exit(EXIT_FAILURE);//异常退出
    }

    /*清屏，清除包括负数在内的一切*/
    while((ch=getchar())!='\n'&&ch!=EOF);//空循环

    return num;
}

void rearrange(char*output,char const*input,int n_columns,int const columns[])
{
    int col;/*columns数组下标，即将用于for循环*/
    int output_col;/*输出列计数器，和后面字符数量挂钩*/
    int len;/*输入行长度*/

    len=strlen(input);;
    output_col=0;

    /*
    *处理每队列标号
    */
   for(col=0;col<n_columns;col+=2)//0,2,4,6……
   {

    /*计算字符的前置判断*/
    if(columns[col]>=len||columns[col+1]>=len||columns[col]>columns[col+1])
    {
        continue;/*如果columns数组中的数字终止代码超过输入行长度，则处理下一对*/
    }
    /*计算一下要打印多少个字符*/
    int nchars=columns[col+1]-columns[col]+1;

    /*
    *如果columns数组中的数字终止代码超过输入行长度，结束任务()
    *如果输出行数组满了，不能再装了，即将越界，结束任务
    */
   if(columns[col]>=len||output_col==MAX_INPUT-1)
   break;

   /*
   *输出行数据空间不够，只复制可以容纳的内容
   */
  if(output_col+nchars>=MAX_INPUT)//nchars最大值是
  {
    nchars=MAX_INPUT-output_col-1;
  }
/*
*复制相关数据
*/
strncpy(output+output_col,input+columns[col],nchars);
   output_col+=nchars;
}

output[output_col]='\0';
/*
*假如传了10个数据，output_col值就是10，output数组0~9有数据，10号位就是终止符
*/
}
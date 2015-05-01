#include "sudokuentity.h"
# include <cstdlib>
# include <ctime>
# include <memory.h>

/********************************** INNER FUNCTION *********************************************/
void cddstatistic(int a[9][9],int x,int y,int cdd[9][9][10])	//统计一个格子的候选数个数
//此函数不需修改
{
    int i,j;
    int z[10];
    cdd[x][y][0]=0;
    for (i=1;i<=9;i++) z[i]=1;
    for (i=0;i<=8;i++) z[a[i][y]]=0;
    for (j=0;j<=8;j++) z[a[x][j]]=0;
    for (i=x/3*3;i<=x/3*3+2;i++)
        for (j=y/3*3;j<=y/3*3+2;j++)
            z[a[i][j]]=0;
    for (i=1;i<=9;i++)
    {
        if (z[i]!=0)
        {
            cdd[x][y][0]++;
            cdd[x][y][i]=1;
        }
        else cdd[x][y][i]=0;
    }
}

/* 功能：INNER FUNCTION
 */
int DEF(int a[9][9])
{
    int totalblank=0,x,y,n;
    n=0;
    for (x=0;x<=8;x++)
        for (y=0;y<=8;y++)
        {
            if (a[x][y]==0)
            {
                totalblank++;
            }
        }//对所有空格，统计其候选数
    //填空初始化完毕。
    n=totalblank;
    return n;
}

/* 功能：INNER FUNCTION
 */
int getresult(int a[9][9])
{
    int totalblank=0,i,j,x,y,t,mx=0,my,mc,record;
    int z[10],rx[81],ry[81];
    int candidate[9][9][10];
    memset(candidate, 0, sizeof(candidate));
    mc=10;
    for (x=0;x<=8;x++)
        for (y=0;y<=8;y++)
        {
            if (a[x][y]==0)
            {
                totalblank++;
                cddstatistic(a,x,y,candidate);
            }
        }//对所有空格，统计其候选数
    record=0;
    //record记录已填的数，填空初始化完毕。
    do
    {
        mc=10;
        //找出候选数最少的空格
        for (x=0;x<=8;x++)
            for (y=0;y<=8;y++)
            {
                if (a[x][y]==0)
                {
                    if (candidate[x][y][0]<mc)
                    {
                        mc=candidate[x][y][0];
                        mx=x;
                        my=y;
                    }
                }
            }
        if (mc>0)
        {
            //对候选数最少的空格进行随机填写
            t=0;
            for (i=1;i<=9;i++)
            {
                if (candidate[mx][my][i]==1)
                {
                    z[t]=i;
                    t++;
                }
            }
            t=rand()%mc;
            a[mx][my]=z[t];
            //记录填写路径
            rx[record]=mx;
            ry[record]=my;
            record++;
            totalblank--;
            candidate[mx][my][0]--;
            candidate[mx][my][z[t]]=0;
//删除受影响空格的该候选数
            for (i=0;i<=8;i++)
            {
                if (a[i][my]==0&&candidate[i][my][z[t]]==1)
                {
                    candidate[i][my][z[t]]=0;
                    candidate[i][my][0]--;
                }
            }
            for (j=0;j<=8;j++)
            {
                if (a[mx][j]==0&&candidate[mx][j][z[t]]==1)
                {
                    candidate[mx][j][z[t]]=0;
                    candidate[mx][j][0]--;
                }
            }
            for (i=mx/3*3;i<=mx/3*3+2;i++)
                for (j=my/3*3;j<=my/3*3+2;j++)
                {
                    if (a[i][j]==0&&candidate[i][j][z[t]]==1)
                    {
                        candidate[i][j][z[t]]=0;
                        candidate[i][j][0]--;
                    }
                }
        }
        else		//候选数为0，说明求解产生问题，回溯。
        {
            //回溯。上一个如果还有其他候选数,重填。
            while (mc==0&&record!=0)
            {
                record--;
                mx=rx[record];
                my=ry[record];
                mc=candidate[mx][my][0];
                if (mc>0)
                {
                    //重填
                    for (i=1;i<=9;i++)
                    {
                        if (candidate[mx][my][i]==1)
                        {
                            a[mx][my]=i;
                            record++;
                            break;
                        }
                    }
                    candidate[mx][my][i]=0;
                    candidate[mx][my][0]--;
                }
                else
                {
                    a[mx][my]=0;
                    totalblank++;
                }                //重新统计受该位置影响的所有数的候选数
                for (x=0;x<=8;x++)
                {
                    if (a[x][my]==0)
                    {
                        cddstatistic(a,x,my,candidate);
                    }
                }
                for (y=0;y<=8;y++)
                {
                    if (a[mx][y]==0)
                    {
                        cddstatistic(a,mx,y,candidate);
                    }
                }
                for (x=mx/3*3;x<=mx/3*3+2;x++)
                    for (y=my/3*3;y<=my/3*3+2;y++)
                    {
                        if (a[x][y]==0)
                        {
                            cddstatistic(a,x,y,candidate);
                        }
                    }
            }
        }
    }while (totalblank>=1&&record>0);
    if (totalblank==0)
        return 1;		//此盘有解
    else
        return 0;
}

/* 功能：生成初盘
 * 输入：终盘数组（承载输出结果），终盘数组（由resultgenerator输出），难度的上限
 * 输出：将改变终盘数组，复写为初盘数组，返回难度系数
 */
int puzzlegenerator(int a[9][9],int m)
{
    int k,t,i,j,r,x,y,p,n,flag;
    int cdd[9][9][10],rx[81],ry[81],a0[9][9];//rx,ry记录能删的格子的横纵坐标
    t=0;
    n=0;
    for (i=0;i<=8;i++)
        for (j=0;j<=8;j++)
        {
            rx[t]=i;
            ry[t]=j;
            t++;
        }
    do
    {
        r=rand()%t;
        x=rx[r];
        y=ry[r];
        t--;
        //无论能删或不能删，下次都不用再考虑该格子了。
        for (i=r;i<=t;i++)
        {
            rx[i]=rx[i+1];
            ry[i]=ry[i+1];
        }
        p=1;
        flag=0;//flag用来判断要不要重新统计难度系数
        cddstatistic(a,x,y,cdd);
        if (cdd[x][y][0]>0)//该格子除了目前的数还有其他候选数
        {
            flag=1;
            for (k=1;k<=9;k++)
            {
                if (cdd[x][y][k]==1)
                {
                    for (i=0;i<=8;i++)
                        for (j=0;j<=8;j++)
                            a0[i][j]=a[i][j];
                    a0[x][y]=k;
                    if (getresult(a0))
                    {
                        p=0;
                        break;
                    }
                }
            }
        }
        if (p)
        {
            a[x][y]=0;
            if (flag)//新删除的格子不止一个候选数。难度可能有所增加
            {
                for (i=0;i<=8;i++)
                    for (j=0;j<=8;j++)
                a0[i][j]=a[i][j];
                n=DEF(a0);
            }
        }
    }while (t>=1&&n<m);
    return n;
}

void print(int a[][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
}

/* 功能：生成终盘
 * 输入：任意数组
 * 输出：将改变输入数组，复写为终盘数字
 */
static void resultgenerator(int a[9][9])
{
    int i,j,r;
    int z[9];
    for (i=0;i<=8;i++) z[i]=i+1;

    for (i=0;i<=8;i++)
    {
        r=rand()%(9-i);
        a[i][0]=z[r];
        for (j=r;j<=7;j++)
            z[j]=z[j+1];
    }
    //print(a);
    getresult(a);
    //print(a);
}
/********************************** INNER FUNCTION *********************************************/
void SudokuEntity::generateHavingFun()
{
    memset(endStatus, 0, sizeof(endStatus));

    srand((int)time(0));
    resultgenerator(endStatus);

    for (int i=0;i<=8;i++)
        for (int j=0;j<=8;j++)
            startStatus[i][j]=endStatus[i][j];

    startStatus[0][0] = 0;
    startStatus[0][1] = 0;
    holeCount = 2;
}

int SudokuEntity::getHoleCount()
{
    return this->holeCount;
}

void SudokuEntity::regenerate(int difficulty)
{
    this->difficulty = difficulty;

    if (difficulty == 0)
    {
        generateHavingFun();
        return;
    }

    int i,j;
    int upperLimit,lowerLimit;

    memset(endStatus, 0, sizeof(endStatus));

    /*生成终盘
     */
    srand((int)time(0));
    resultgenerator(endStatus);
    /*将终盘复制至初盘数组，便于挖孔生成初盘
     */
    for (i=0;i<=8;i++)
        for (j=0;j<=8;j++)
            startStatus[i][j]=endStatus[i][j];
    /*使用判断难度的上下界（挖孔书目的上下界）检验数组
     */
    lowerLimit = difficulty*14;
    upperLimit = (difficulty+1)*12;

    holeCount = puzzlegenerator(startStatus, upperLimit);
    /*如果得到的初盘难度小于下界，则重新开始生成
     */
    while (holeCount < lowerLimit)
    {
        for (i=0;i<=8;i++)
            for (j=0;j<=8;j++)
                startStatus[i][j]=endStatus[i][j];
        holeCount=puzzlegenerator(startStatus, upperLimit);
    }
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"lexical_analysis.h"

/*
语法分析部分
作者：方银城
2016.12.20
*/
/*
这是符号在LR分析表中对应的下标
  +  -  *  /  (  )  d(int/double)  #      P   A   B   C
 1  2  3  4  6  7    5  /8        0      9   10   11   12
*/
struct Formula {
	int aim;//目的符号代码
	int sum;//归约单词数
} f[12];//产生式数组

//int LR[19][13];//LR分析表
int LR[20][13];//LR分析表

typedef struct List_y* list_y_pointer;
struct List_y {
	char s;
	char* left;
	char* right;
	char* result;
	list_y_pointer next;
};//语法分析单行四元式序列链表
 
typedef struct Result_y* y_pointer;
struct Result_y {
	int index;//行数
	list_y_pointer list;
	char* error;
	char* r;//计算结果
	y_pointer next;
};//程序语法分析结果链表
typedef y_pointer result_y_p;

typedef struct State {
	int a[100];
	int top;
} State_stack;//状态栈

typedef struct Symbol {
	list_pointer b[100];
	int top;
} Symbol_stack;//文法符号栈

/*
产生式数组初始化
*/
void create_F() {
	f[1].aim = 9;
	f[1].sum = 1;

	f[2].aim = 9;
	f[2].sum = 3;

	f[3].aim = 9;
	f[3].sum = 3;

	f[4].aim = 9;
	f[4].sum = 1;

	f[5].aim = 10;
	f[5].sum = 3;

	f[6].aim = 10;
	f[6].sum = 3;

	f[7].aim = 10;
	f[7].sum = 1;

	f[8].aim = 11;
	f[8].sum = 1;

	f[9].aim = 11;
	f[9].sum = 2;

	f[10].aim = 12;
	f[10].sum = 1;

	f[11].aim = 12;
	f[11].sum = 3;
}

/*
初始化LR分析表(其中100+的都表示归约，100为acc)
如：10表示跳到状态10
	104表示使用产生式4归约

这部分是这个文法下的LR分析表：
	P：表达式    A：项    B：因子   C：算术量    d：常数（包括int和double）
	拓广文法：P'-->P (1)    A-->B*A (5)   B-->-B (9)
		      P-->A+P (2)   A-->B/A (6)   C-->d (10)
			  P-->A-P (3)   A-->B (7)     C-->(P) (11)
			  P-->A (4)     B-->C (8)

void create_LR() {
	int i,j;
	for(i = 0;i < 19;i++) {
		for(j = 0;j < 13;j++) {
			LR[i][j] = -1;
		}
	}
	LR[0][2] = 12;
	LR[0][5] = 15;
	LR[0][6] = 16;
	LR[0][8] = 15;
	LR[0][9] = 1;
	LR[0][10] = 2;
	LR[0][11] = 7;
	LR[0][12] = 14;

	LR[1][0] = 100;

	LR[2][0] = 104;
	LR[2][1] = 3;
	LR[2][2] = 5;
	LR[2][3] = 104;
	LR[2][4] = 104;
	LR[2][5] = 104;
	LR[2][6] = 104;
	LR[2][7] = 104;
	LR[2][8] = 104;

	LR[3][2] = 12;
	LR[3][5] = 15;
	LR[3][6] = 16;
	LR[3][8] = 15;
	LR[3][9] = 4;
	LR[3][10] = 2;
	LR[3][11] = 7;
	LR[3][12] = 14;

	for(i = 0;i < 9;i++) {
		LR[4][i] = 102;
	}

	LR[5][2] = 12;
	LR[5][5] = 15;
	LR[5][6] = 16;
	LR[5][8] = 15;
	LR[5][9] = 6;
	LR[5][10] = 2;
	LR[5][11] = 7;
	LR[5][12] = 14;

	for(i = 0;i < 9;i++) {
		LR[6][i] = 103;
	}

	LR[7][0] = 107;
	LR[7][1] = 107;
	LR[7][2] = 107;
	LR[7][3] = 8;
	LR[7][4] = 10;
	LR[7][5] = 107;
	LR[7][6] = 107;
	LR[7][7] = 107;
	LR[7][8] = 107;

	LR[8][2] = 12;
	LR[8][5] = 15;
	LR[8][6] = 16;
	LR[8][8] = 15;
	LR[8][10] = 9;
	LR[8][11] = 7;
	LR[8][12] = 14;

	for(i = 0;i < 9;i++) {
		LR[9][i] = 105;
	}

	LR[10][2] = 12;
	LR[10][5] = 15;
	LR[10][6] = 16;
	LR[10][8] = 15;
	LR[10][10] = 11;
	LR[10][11] = 7;
	LR[10][12] = 14;

	for(i = 0;i < 9;i++) {
		LR[11][i] = 106;
	}

	LR[12][2] = 12;
	LR[12][5] = 15;
	LR[12][6] = 16;
	LR[12][8] = 15;
	LR[12][11] = 13;
	LR[12][12] = 14;

	for(i = 0;i < 9;i++) {
		LR[13][i] = 109;
	}

	for(i = 0;i < 9;i++) {
		LR[14][i] = 108;
	}

	for(i = 0;i < 9;i++) {
		LR[15][i] = 110;
	}

	LR[16][2] = 12;
	LR[16][5] = 15;
	LR[16][6] = 16;
	LR[16][8] = 15;
	LR[16][9] = 17;
	LR[16][10] = 2;
	LR[16][11] = 7;
	LR[16][12] = 14;

	LR[17][7] = 18;

	for(i = 0;i < 9;i++) {
		LR[18][i] = 111;
	}
}*/
/*
初始化LR分析表(其中100+的都表示归约，100为acc)
如：10表示跳到状态10
	104表示使用产生式4归约

 这部分是这个文法下的LR分析表：
	P：表达式    A：项    B：因子   C：算术量    d：常数（包括int和double）
	拓广文法：P'-->P (1)    A-->A*B (5)   B-->-B (9)
		      P-->P+A (2)   A-->A/B (6)   C-->d (10)
			  P-->P-A (3)   A-->B (7)     C-->(P) (11)
			  P-->A (4)     B-->C (8)
*/
void create_LR() {
	int i,j;
	for(i = 0;i < 19;i++) {
		for(j = 0;j < 13;j++) {
			LR[i][j] = -1;
		}
	}
	LR[0][2] = 5;
	LR[0][5] = 6;
	LR[0][6] = 7;
	LR[0][8] = 6;
	LR[0][9] = 1;
	LR[0][10] = 2;
	LR[0][11] = 3;
	LR[0][12] = 4;

	LR[1][0] = 100;
	LR[1][1] = 8;
	LR[1][2] = 9;

	LR[2][0] = 104;
	LR[2][1] = 104;
	LR[2][2] = 104;
	LR[2][3] = 10;
	LR[2][4] = 11;
	LR[2][5] = 104;
	LR[2][6] = 104;
	LR[2][7] = 104;
	LR[2][8] = 104;

	for(i = 0;i < 9;i++) {
		LR[3][i] = 107;
	}

	for(i = 0;i < 9;i++) {
		LR[4][i] = 108;
	}

	LR[5][2] = 5;
	LR[5][5] = 6;
	LR[5][6] = 7;
	LR[5][8] = 6;
	LR[5][11] = 12;
	LR[5][12] = 4;

	for(i = 0;i < 9;i++) {
		LR[6][i] = 110;
	}

	LR[7][2] = 5;
	LR[7][5] = 6;
	LR[7][6] = 7;
	LR[7][8] = 6;
	LR[7][9] = 14;
	LR[7][10] = 2;
	LR[7][11] = 3;
	LR[7][12] = 4;

	LR[8][2] = 5;
	LR[8][5] = 6;
	LR[8][6] = 7;
	LR[8][8] = 6;
	LR[8][10] = 15;
	LR[8][11] = 3;
	LR[8][12] = 4;

	LR[9][2] = 5;
	LR[9][5] = 6;
	LR[9][6] = 7;
	LR[9][8] = 6;
	LR[9][10] = 16;
	LR[9][11] = 3;
	LR[9][12] = 4;

	LR[10][2] = 5;
	LR[10][5] = 6;
	LR[10][6] = 7;
	LR[10][8] = 6;
	LR[10][11] = 17;
	LR[10][12] = 4;

	LR[11][2] = 5;
	LR[11][5] = 6;
	LR[11][6] = 7;
	LR[11][8] = 6;
	LR[11][11] = 18;
	LR[11][12] = 4;

	for(i = 0;i < 9;i++) {
		LR[12][i] = 109;
	}

	for(i = 0;i < 9;i++) {
		LR[13][i] = 108;
	}

	LR[14][1] = 8;
	LR[14][2] = 9;
	LR[14][7] = 19;

	for(i = 0;i < 9;i++) {
		LR[15][i] = 102;
	}
	LR[15][3] = 10;
	LR[15][4] = 11;

	for(i = 0;i < 9;i++) {
		LR[16][i] = 103;
	}
	LR[16][3] = 10;
	LR[16][4] = 11;

	for(i = 0;i < 9;i++) {
		LR[17][i] = 105;
	}

	for(i = 0;i < 9;i++) {
		LR[18][i] = 106;
	}
	for(i = 0;i < 9;i++) {
		LR[19][i] = 111;
	}
}

/*
初始化程序语法分析结果链表
*/
result_y_p create_y() {
	y_pointer head = (y_pointer)malloc(sizeof(struct Result_y));
	head->index = 0;
	head->list = NULL;
	head->error = NULL;
	head->r = NULL;
	head->next = NULL;
	create_F();
	create_LR();
	return head;
}

/*
建立四元式
*/
list_y_pointer setData_2(Symbol_stack* symbol,list_y_pointer list,int m) {
	list_y_pointer list_y = (list_y_pointer)malloc(sizeof(struct List_y));
	list_pointer list_head = (list_pointer)malloc(sizeof(struct List));

	list_y->s = symbol->b[symbol->top-1]->s[0];
	list_y->left = (char *)malloc(sizeof(char)*51);
	list_y->left[0] = '\0';
	list_y->right = (char *)malloc(sizeof(char)*51);
	list_y->right[0] = '\0';
	list_y->result = (char *)malloc(sizeof(char)*51);
	list_y->result[0] = '\0';

	list_head->num = 8;
	list_head->s = NULL;
	list_head->value = (char *)malloc(sizeof(char)*51);
	list_head->value[0] = '\0';
	list_head->type = NULL;
	list_head->error = NULL;
	list_head->next = NULL;

	if(m == 2) {
		char* index = "-";
		list_y->s = '@';
		strcpy(list_y->left,symbol->b[symbol->top]->value);
		list_y->right = "NULL";
		if(list_y->left[0] != '-')
			strcpy(list_y->result,index);
		else
			list_y->left = list_y->left+1;
		strcat(list_y->result,list_y->left);
		strcpy(list_head->value,list_y->result);
		symbol->b[symbol->top-1] = list_head;
	} else if(m == 3) {
		double d1 = strtod(symbol->b[symbol->top-2]->value,NULL);
		double d2 = strtod(symbol->b[symbol->top]->value,NULL);
		char * k = (char *)malloc(sizeof(char)*51);

		strcpy(list_y->left,symbol->b[symbol->top-2]->value);
		//list_y->left = symbol->b[symbol->top-1]->s;
		strcpy(list_y->right,symbol->b[symbol->top]->value);
		//list_y->right = symbol->b[symbol->top+1]->s;
		
		if(symbol->b[symbol->top-1]->num == 1) {
			double d3 = d1 + d2;
			gcvt(d3,50,k);
			strcpy(list_y->result,k);
			strcpy(list_head->value,list_y->result);
			symbol->b[symbol->top-2] = list_head;
		} else if(symbol->b[symbol->top-1]->num == 2) {
			double d3 = d1 - d2;
			gcvt(d3,50,k);
			strcpy(list_y->result,k);
			strcpy(list_head->value,list_y->result);
			symbol->b[symbol->top-2] = list_head;
		} else if(symbol->b[symbol->top-1]->num == 3) {
			double d3 = d1 * d2;
			gcvt(d3,50,k);
			strcpy(list_y->result,k);
			strcpy(list_head->value,list_y->result);
			symbol->b[symbol->top-2] = list_head;
		} else if(symbol->b[symbol->top-1]->num == 4) {
			double d3 = d1 / d2;
			gcvt(d3,50,k);
			strcpy(list_y->result,k);
			strcpy(list_head->value,list_y->result);
			symbol->b[symbol->top-2] = list_head;
		} else {
			printf("运行到这里代表错了！！！");
		}
	}
	//去掉doule转为字符串后最后出现的点
	if(list_y->left[strlen(list_y->left)-1] == '.')
		list_y->left[strlen(list_y->left)-1] = '\0';
	if(list_y->right[strlen(list_y->right)-1] == '.')
		list_y->right[strlen(list_y->right)-1] = '\0';
	if(list_y->result[strlen(list_y->result)-1] == '.')
		list_y->result[strlen(list_y->result)-1] = '\0';
	list_y->next = NULL;
	list->next = list_y;
	return list_y;
}

/*
语法分析
*/
void grammatical_analysis(result_y_p head,result_p head_2) {
    pointer p = head_2->next;
	y_pointer q = head;
	char * a = "缺少操作数";
	char * b = "运算符出错";
	char * c = "左括号不匹配";
	char * d = "词法分析出错";
	char * k = "表达式结果为";
	while(p != NULL) {
		y_pointer y = (y_pointer)malloc(sizeof(struct Result_y));
		State_stack state;
		Symbol_stack symbol;
		list_pointer l = p->list->next;
		list_y_pointer list_y = (list_y_pointer)malloc(sizeof(struct List_y));
		
		int index;

		list_y->next = NULL;

		y->index = p->index;
		y->list = list_y;
		y->r = NULL;
		y->error = NULL;
		y->next = NULL;

		q->next = y;
		q = y;

		//初始化状态栈
		state.a[0] = 0;
		state.top = 0;
		//初始化符号栈
		symbol.top = -1;
		if(l == NULL) {
			p = p->next;
			continue;
		}
		while(1) {
			if(l != NULL) {
				if(l->error != NULL) {
					index = -1;
					break;
				}
				index = l->num;
			} else
				index = 0;

			if(LR[state.a[state.top]][index] > 0 && LR[state.a[state.top]][index] < 100) {//移进
				state.a[state.top+1] = LR[state.a[state.top]][index];
				state.top = state.top+1;
				symbol.b[symbol.top+1] = l;
				symbol.top = symbol.top+1;
			} else if(LR[state.a[state.top]][index] > 100) {//归约
				int t = LR[state.a[state.top]][index]-100;
				int s = f[t].sum;
				state.a[state.top-s+1] = LR[state.a[state.top-s]][f[t].aim];
				state.top = state.top-s+1;

				if(s>1 && symbol.b[symbol.top-1]->num < 5) {
					list_y = setData_2(&symbol,list_y,s);//这里是生成四元式。。。。。。。。。。。。
				} else if(s==3 && symbol.b[symbol.top]->num == 7) {
					symbol.b[symbol.top-s+1] = symbol.b[symbol.top-1];
				}
				symbol.top = symbol.top-s+1;
				continue;
			} else {
				//错误。。。。。。
				break;
			}
			l = l->next;
		}

		if(index == -1) {
			y->error = (char *)malloc(sizeof(char)*13);
			y->error[0] = '\0';
			strcat(y->error,d);
			p = p->next;
			continue;
		}
		if(l == NULL && LR[state.a[state.top]][index]==100) {
			y->r = (char *)malloc(sizeof(char)*61);
			y->r[0] = '\0';
			strcpy(y->r,k);
			strcat(y->r,symbol.b[symbol.top]->value);
		} else {
			//错误。。。。。。
			if(index == 0) {
				if(symbol.b[symbol.top]->num < 5) {
					y->error = (char *)malloc(sizeof(char)*15);
					y->error[0] = '\0';
					strcat(y->error,symbol.b[symbol.top]->s);
					strcat(y->error,a);
				}
				if(symbol.b[symbol.top-1]->num == 6) {
					y->error = (char *)malloc(sizeof(char)*17);
					y->error[0] = '\0';
					strcat(y->error,c);
				}
			} else {
				if(index < 5) {
					y->error = (char *)malloc(sizeof(char)*15);
					y->error[0] = '\0';
					strcat(y->error,l->s);
					strcat(y->error,b);
				}
			}
		}
		p = p->next;
	}
}



/*
语法分析结果输出
*/
void output_grammatical(result_p head,result_y_p head_2) {
	y_pointer p = head_2->next;
	pointer pp = head->next;
	int sum = 0;
	printf("\n语法分析结果：\n");
	while(p != NULL) {
		list_y_pointer q = p->list->next;
		if(p->error != NULL) {
			p = p->next;
			continue;
		}
		printf("-----------第%d行--------\n",p->index);
		if(q == NULL) {
			if(p->r == NULL)
				printf("****空行****\n");
		}
		while(q != NULL) {
			printf("    (%d) (%c, %s, %s, %s)\n",++sum,q->s,q->left,q->right,q->result);
			q = q->next;
		}
		if(p->r != NULL)
			printf("%s\n",p->r);
		p = p->next;
	}

	printf("——————词法错误—————————\n");

	//显示词法错误信息
	sum = 0;
	pp = head->next;
	while(pp != NULL) {
		list_pointer q = pp->list->next;
		while(q != NULL) {
			if(q->error != NULL)
				printf(" Error%d：第%d行，%s\n",++sum,pp->index,q->error);
			q = q->next;
		}
		pp = pp->next;
	}
	printf("\n");

	printf("——————语法错误—————————\n");

	//显示语法错误信息
	sum = 0;
	p = head_2->next;
	while(p != NULL) {
		if(p->error != NULL)
			printf(" Error%d：第%d行，%s\n",++sum,p->index,p->error);
		p = p->next;
	}
	printf("\n");
}

int main() {
	result_p head;
	result_y_p head_2;
	printf("作者：方银城\n班级：软件工程2班\n学号：201430098164\n\n");
	head = create();
	head_2 = create_y();
	lexical_analysis(head);
	//output_lexical(head);//输出词法分析结果
	grammatical_analysis(head_2,head);
	output_grammatical(head,head_2);//输出语法分析结果
	return 0;
}
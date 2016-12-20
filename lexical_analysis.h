#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
这部分是词法分析，作为头文件给dfa.c文件调用
作者：方银城
*/

typedef struct List* list_pointer;
struct List {
	int num;
	char* s;
	char* value;
	char* type;
	char* error;
	list_pointer next;
};//程序词法分析单行结果链表

typedef struct Result* pointer;
struct Result {
	int index;//行数
	list_pointer list;
	pointer next;
};//程序词法分析结果链表
typedef pointer result_p;

/*
初始化程序词法分析结果链表
*/
result_p create() {
	pointer head = (pointer)malloc(sizeof(struct Result));
	head->index = 0;
	head->list = NULL;
	head->next = NULL;
	return head;
}

/*
建立四元组
*/
list_pointer setData(int num, char* s, char* value, char* type, list_pointer p, char* error) {
	list_pointer list_head = (list_pointer)malloc(sizeof(struct List));
	list_head->num = num;
	list_head->s = s;
	list_head->value = value;
	list_head->type = type;
	list_head->error = error;
	list_head->next = NULL;

	p->next = list_head;
	p = list_head;
	return p;
}

/*
单行词法分析
*/
pointer single_analysis(char* str, pointer h) {
	char* a = "int";
	char* b = "double";
	char* c = "后面存在非法字符";
	char* d = "开头存在非法字符";
	char* e = "数值非法";
	int i = 0;
	char *s,*value,*type,*error;
	list_pointer p = (list_pointer)malloc(sizeof(struct List));
	pointer r = (pointer)malloc(sizeof(struct Result));
	p->next = NULL;
	r->index = h->index+1;
	r->list = p;
	r->next = NULL;
	h->next = r;
	h = r;

	while(*(str+i) != '\0') {
		switch(*(str+i)) {
			case ' ':
				break;
			case '\n'://末尾换行符
				break;
			case '\t':
				break;
			case '+':
				s = (char*)malloc(sizeof(char)*2);
				strncpy(s,str+i,1);
				s[1] = '\0';
				value = NULL;
				type = NULL;
				error = NULL;
				p = setData(1,s,value,type,p,error);
				break;
			case '-':
				s = (char*)malloc(sizeof(char)*2);
				strncpy(s,str+i,1);
				s[1] = '\0';
				value = NULL;
				type = NULL;
				error = NULL;
				p = setData(2,s,value,type,p,error);
				break;
			case '*':
				s = (char*)malloc(sizeof(char)*2);
				strncpy(s,str+i,1);
				s[1] = '\0';
				value = NULL;
				type = NULL;
				error = NULL;
				p = setData(3,s,value,type,p,error);
				break;
			case '/':
				s = (char*)malloc(sizeof(char)*2);
				strncpy(s,str+i,1);
				s[1] = '\0';
				value = NULL;
				type = NULL;
				error = NULL;
				p = setData(4,s,value,type,p,error);
				break;
			case '(':
				s = (char*)malloc(sizeof(char)*2);
				strncpy(s,str+i,1);
				s[1] = '\0';
				value = NULL;
				type = NULL;
				error = NULL;
				p = setData(6,s,value,type,p,error);
				break;
			case ')':
				s = (char*)malloc(sizeof(char)*2);
				strncpy(s,str+i,1);
				s[1] = '\0';
				value = NULL;
				type = NULL;
				error = NULL;
				p = setData(7,s,value,type,p,error);
				break;
			case '.':
				error = (char*)malloc(sizeof(char)*10);
				strncpy(error,str+i,1);
				error[1] = '\0';
				strcat(error,e);

				s = NULL;
				value = NULL;
				type = NULL;
				p = setData(0,s,value,type,p,error);
				break;
			default:
				if(*(str+i) >= '0' && *(str+i) <= '9') {
					int num = 5,j,k;
					int flag = 0;//标识小数点
					value = NULL;
					type = NULL;
					s = (char*)malloc(sizeof(char)*51);
					strncpy(s,str+i,1);
					s[1] = '\0';
				/*	if(*(str+i) == '0' && *(str+i+1) != '.') {
						i++;
					} else {*/
					while(*(str+(++i)) == '.' || (*(str+i) >= '0' && *(str+i) <= '9')) {
						if(*(str+i) == '.') {
							if(flag == 0) {
								flag = 1;
								num = 8;
								type = (char*)malloc(sizeof(char)*7);
								strcpy(type,b);
							} else {
								break;
							}
							
						}
						if(strlen(s) < 50)
							strncat(s,str+i,1);
					}
					if(*(str+i-1) == '.')
						flag = 2;
				//	}
					if(type == NULL) {
						type = (char*)malloc(sizeof(char)*4);
						strcpy(type,a);
					}

					//数字格式错误报错
					if(flag == 2){
						error = (char*)malloc(sizeof(char)*(strlen(s)+8+1));
						error[0] = '\0';
						strcpy(error,s);
						strcat(error,e);

						free(s);
						free(type);
						s = NULL;
						value = NULL;
						type = NULL;
						p = setData(0,s,value,type,p,error);
					} else {
						//去掉数值的无用0
						for(j=0,k=strlen(s)-1; (s[j] == '0' || (s[k]=='0' && type[0]=='d')) && j<k; ) {
							if(s[j] == '0') {
								j++;
							}
							if(s[k]=='0' && type[0]=='d') {
								k--;
							}
						}
						if(s[j] == '.') {
							j--;
						}
						if(s[k] == '.') {
							k++;
						}
						value = (char*)malloc(sizeof(char)*(k-j+2));
						strncpy(value,s+j,k-j+1);
						value[k-j+1] = '\0';

						error = NULL;
						p = setData(num,s,value,type,p,error);
					}
					i--;
				} else {
					error = (char*)malloc(sizeof(char)*20);
					if(i-1 < 0) {
						error[0] = '\0';
						strcat(error,d);
					} else {
						strncpy(error,str+i-1,1);
						error[1] = '\0';
						strcat(error,c);
					}

					//判断中文字符和英文字符
					if(*(str+i) > -1 && *(str+i) < 128) {
						strncat(error,str+i,1);
						error[strlen(c)+2] = '\0';
					} else {
						strncat(error,str+i,2);
						error[strlen(c)+3] = '\0';
						i++;
					}
					
					s = NULL;
					value = NULL;
					type = NULL;
					p = setData(0,s,value,type,p,error);
				}
				break;
		}
		i++;
	}
	return h;
}

/*
程序词法分析
*/
void lexical_analysis(result_p head) {
	FILE *fp;
	pointer h = head;
	char* f = (char*)malloc(sizeof(char)*20);
	char* str = (char*)malloc(sizeof(char)*500);

	while(1) {
		printf("请输入文件名(包括路径在20字符内)：");
		gets(f);
		//文件打开并判断是否成功
		if((fp=fopen(f,"r")) == NULL) {
			printf("\n程序文件打开失败！\n\n");
		} else {
			break;
		}
	}
	//读取每一行并分析
	while(fgets(str,500,fp) != NULL) {
		h = single_analysis(str,h);
	}
	fclose(fp);
}

/*
词法分析结果输出
*/
void output_lexical(result_p head) {
	pointer p = head->next;
	int sum = 0;
	printf("\n词法分析结果：\n");
	while(p != NULL) {
		list_pointer q = p->list->next;
		printf("-----------------\n");
		if(q == NULL) {//空行
			printf("****空行****\n");
		}
		while(q != NULL) {
			if(q->error == NULL)
				printf("    (%d, %s, %s, %s)\n",q->num,q->s,q->value==NULL?"NULL":q->value,q->type==NULL?"NULL":q->type);
			q = q->next;
		}
		p = p->next;
	}
	printf("————————————————————\n");

	//显示错误信息
	p = head->next;
	while(p != NULL) {
		list_pointer q = p->list->next;
		while(q != NULL) {
			if(q->error != NULL)
				printf(" Error%d：第%d行，%s\n",++sum,p->index,q->error);
			q = q->next;
		}
		p = p->next;
	}
	printf("\n");
}
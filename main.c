#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
 
//蛇的状态，U：上 ；D：下；L:左 R：右
#define U 1
#define D 2
#define L 3 
#define R 4 
 
#define EMPTY 0
#define BIT_SELF 1 //咬到蛇身
#define TOUCH_WALL 2 //碰到墙
 
#define TRUE 1
#define FALSE 0 
 
typedef struct SNAKE //蛇身的一个节点 
{
	int x;
	int y;
	struct SNAKE *next;
}snake;
 
//head用于指向蛇的第一个节点，标识整条蛇，但不属于蛇的节点
snake head, *temp_ptr, food_node;
 
//direction						键入方向
//game_over_reason				记录游戏失败原因
//speed							移动速度
int direction = R, game_over_reason = EMPTY,  speed = 100;
 
void locateAndPrint(int x, int y);						//在光标位置输出方块
void locateAndClear(int x, int y);						//在光标位置清除方块
void creatMap();										//创建地图
void createFood();										//创建食物
void intSnake(int snake_len, int start_x, int start_y); //初始化蛇身
void endGame();											//结束游戏
void getEnteredDirection();								//获取键入的方向
void snakeMove();										//蛇移动
void startGame();										//游戏循环
 
/*在光标位置输出方块*/
void locateAndPrint(int x, int y)
{
	//定位
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);   //获取控制台的句柄:类似于指针 
	SetConsoleCursorPosition(hOutput, pos);  //作用是设置控制台光标位置 
 
	//输出
	printf("■");
}
 
/*在光标位置清除方块*/
void locateAndClear(int x, int y)
{
	//定位
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
 
	//输出
	printf(" ");
}
 
/*创建地图*/
void creatMap()
{
	//设定黑窗的大小
	system("mode con cols=100 lines=30");   //调整系统Console控制台显示的宽度和高度，高度为30个字符，宽度为100个字符  
 
	//注意这里x+2 y+1，在黑框中就是这样约定的，没办法
	int i;
	for (i = 0; i < 58; i += 2)//打印上下边框
	{
		locateAndPrint(i, 0);
		locateAndPrint(i, 26);
	}
	for (i = 1; i < 26; i++)//打印左右边框
	{
		locateAndPrint(0, i);
		locateAndPrint(56, i);
	}
}
 
/*创建食物*/
void createFood()
{
	//创建食物成功，跳出循环
	int create_food_success_flag = TRUE;
 
	//如果创建不成功，则重新重建
	while (1) {
		int rand_x, rand_y;
		do {
			//x随机数 2-55  y随机数 3-23
			srand((int)time(NULL));  //表示设置一个随机种子，每次运行都能保证随机种子不同 
			rand_x = rand() % 53 + 2;
			rand_y = rand() % 21 + 3;
		} while (rand_x % 2 != 0);//x需要是2的偶数，黑框中x和y的方块有局限性
 
		//判断生成的食物是否跟蛇身重叠
		temp_ptr = head.next;
		while (temp_ptr != NULL) {
			if (temp_ptr->x == rand_x && temp_ptr->y == rand_y) {
				//重叠，需要重新创建
				create_food_success_flag = FALSE;
				break;
			}
			temp_ptr = temp_ptr->next;
		}
 
		//food的坐标跟蛇身重叠，重新生成food_node
		if (FALSE == create_food_success_flag) {
			continue;
		}
 
		//创建成功，在黑框中打印
		create_food_success_flag = TRUE;
		food_node.x = rand_x;
		food_node.y = rand_y;
		locateAndPrint(rand_x, rand_y);
 
		break;
	}
}
 
/*初始化蛇身*/
void intSnake(int snake_len, int start_x, int start_y) {
 
 	int i;
	for (i = 0; i < snake_len; i++) {
		temp_ptr = (snake *)malloc(sizeof(snake));  //建立头节点 ，方便遍历 
		temp_ptr->x = start_x;
		temp_ptr->y = start_y;
		start_x += 2;
		//头插法
		temp_ptr->next = head.next;  //新节点的指针域指向首节点 
		head.next = temp_ptr;  //修改头结点的指针域，使其指向新节点 
		//输出初始蛇身
		locateAndPrint((head.next)->x, (head.next)->y);
	}
}
 
/*游戏结束*/
void endGame()
{
	//记录得分
	int snake_length = 0;
 
	//释放蛇身 malloc分配的资源
	snake * temp_ptr2;//这里为了思路清晰，引入第三个变量temp_ptr2，辅助变量temp_ptr2可以用head.next来替代
	temp_ptr = head.next;
	while (temp_ptr != NULL) {
		temp_ptr2 = temp_ptr->next;
		free(temp_ptr);  //释放malloc(或calloc、realloc)函数给指针变量分配的内存空间 
		temp_ptr = temp_ptr2;
		snake_length++;
	}
 
	//清屏并输出游戏失败原因
	system("cls");
 
	if (BIT_SELF == game_over_reason) {
 
		printf("蛇头与蛇身相碰，失败！\n得分：%d\n", snake_length - 9);
	}
	else if (TOUCH_WALL == game_over_reason) {
		printf("碰到墙壁，失败！\n得分：%d\n", snake_length - 9);
 
	}
 
	getchar();
}
 
/*获取键入的方向*/
void getEnteredDirection()
{
 
	if (GetAsyncKeyState(VK_OEM_PLUS))  //按“+” 
	{
		speed -= 25;
	}
	else if (GetAsyncKeyState(VK_OEM_MINUS))  //按“-” 
	{
		speed += 25;
	}
	else if (GetAsyncKeyState(VK_UP) && direction != D)
	{
		direction = U;
	}
	else if (GetAsyncKeyState(VK_DOWN) && direction != U)
	{
		direction = D;
	}
	else if (GetAsyncKeyState(VK_LEFT) && direction != R)
	{
		direction = L;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && direction != L)
	{
		direction = R;
	}
}
 
/*蛇移动*/
void snakeMove()
{
	//把原始的head.next的坐标值存起来
	int temp_x = head.next->x, temp_y = head.next->y;
 
	//判断方向，修改第一个节点的值
	if (direction == R)
	{
		head.next->x += 2;
	}
	else if (direction == L) {
		head.next->x -= 2;
	}
	else if (direction == U) {
		head.next->y -= 1;
	}
	else if (direction == D) {
		head.next->y += 1;
	}
 
	//case1.下一节点为食物： 蛇身第一节点跟食物节点重合，在蛇身第二个节点新建节点
	//case2.下一节点不为食物：在蛇身第二个节点处新建节点
	//故可合并
	snake * temp = (snake *)malloc(sizeof(snake));
	temp->x = temp_x;
	temp->y = temp_y;
	temp->next = head.next->next;
	head.next->next = temp;
 
	//判断是否撞墙
	if (head.next->x >= 58 || head.next->x <= 0 || head.next->y <= 0 || head.next->y >= 26) {
		game_over_reason = TOUCH_WALL;
		endGame();
	}
 
	//判断下一个节点是否食物
	if (
		food_node.x == head.next->x &&
		food_node.y == head.next->y
		) {
		//下一个节点是食物
		//重新新建食物
		createFood();
	}
	else {
		//下一个节点不是食物，则移动蛇身
 
		//在蛇前进的第一个位置打印方块
		locateAndPrint(head.next->x, head.next->y);
		
		temp_ptr = head.next;
		while (temp_ptr->next->next != NULL) {
 
			//判断是否咬到自己
			if (
				temp_ptr->next->next != NULL &&
				temp_ptr->next->next->x == (head.next)->x &&
				temp_ptr->next->next->y == (head.next)->y
				) {
				game_over_reason = BIT_SELF;
				endGame();
			}
 
			temp_ptr = temp_ptr->next;
		}
 
		//清除蛇尾的节点
		locateAndClear(temp_ptr->next->x, temp_ptr->next->y);
		free(temp_ptr->next);//释放蛇尾节点内存
		temp_ptr->next = NULL;
	}
}
 
/*游戏循环*/
void startGame()
{
	//初始方向，默认方向
	direction = R;
 
	while (1) {
		//判断键盘输入的方向键，但注意“移动方向为上时按下键不起作用”，左右方向同理
		getEnteredDirection();
 
		//显示蛇身
		snakeMove();//蛇身移动
 
		Sleep(speed);
	}
}
 
int main()
{
	//创建地图
	creatMap();
 
	//初始化蛇身
	intSnake(8, 4, 3);
	
	//创建食物
	createFood();
 
	//开始游戏 循环不停的判断键入的方向
	startGame();
 
	return 0;
}

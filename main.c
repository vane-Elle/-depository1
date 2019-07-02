#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
 
//�ߵ�״̬��U���� ��D���£�L:�� R����
#define U 1
#define D 2
#define L 3 
#define R 4 
 
#define EMPTY 0
#define BIT_SELF 1 //ҧ������
#define TOUCH_WALL 2 //����ǽ
 
#define TRUE 1
#define FALSE 0 
 
typedef struct SNAKE //�����һ���ڵ� 
{
	int x;
	int y;
	struct SNAKE *next;
}snake;
 
//head����ָ���ߵĵ�һ���ڵ㣬��ʶ�����ߣ����������ߵĽڵ�
snake head, *temp_ptr, food_node;
 
//direction						���뷽��
//game_over_reason				��¼��Ϸʧ��ԭ��
//speed							�ƶ��ٶ�
int direction = R, game_over_reason = EMPTY,  speed = 100;
 
void locateAndPrint(int x, int y);						//�ڹ��λ���������
void locateAndClear(int x, int y);						//�ڹ��λ���������
void creatMap();										//������ͼ
void createFood();										//����ʳ��
void intSnake(int snake_len, int start_x, int start_y); //��ʼ������
void endGame();											//������Ϸ
void getEnteredDirection();								//��ȡ����ķ���
void snakeMove();										//���ƶ�
void startGame();										//��Ϸѭ��
 
/*�ڹ��λ���������*/
void locateAndPrint(int x, int y)
{
	//��λ
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);   //��ȡ����̨�ľ��:������ָ�� 
	SetConsoleCursorPosition(hOutput, pos);  //���������ÿ���̨���λ�� 
 
	//���
	printf("��");
}
 
/*�ڹ��λ���������*/
void locateAndClear(int x, int y)
{
	//��λ
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
 
	//���
	printf(" ");
}
 
/*������ͼ*/
void creatMap()
{
	//�趨�ڴ��Ĵ�С
	system("mode con cols=100 lines=30");   //����ϵͳConsole����̨��ʾ�Ŀ�Ⱥ͸߶ȣ��߶�Ϊ30���ַ������Ϊ100���ַ�  
 
	//ע������x+2 y+1���ںڿ��о�������Լ���ģ�û�취
	int i;
	for (i = 0; i < 58; i += 2)//��ӡ���±߿�
	{
		locateAndPrint(i, 0);
		locateAndPrint(i, 26);
	}
	for (i = 1; i < 26; i++)//��ӡ���ұ߿�
	{
		locateAndPrint(0, i);
		locateAndPrint(56, i);
	}
}
 
/*����ʳ��*/
void createFood()
{
	//����ʳ��ɹ�������ѭ��
	int create_food_success_flag = TRUE;
 
	//����������ɹ����������ؽ�
	while (1) {
		int rand_x, rand_y;
		do {
			//x����� 2-55  y����� 3-23
			srand((int)time(NULL));  //��ʾ����һ��������ӣ�ÿ�����ж��ܱ�֤������Ӳ�ͬ 
			rand_x = rand() % 53 + 2;
			rand_y = rand() % 21 + 3;
		} while (rand_x % 2 != 0);//x��Ҫ��2��ż�����ڿ���x��y�ķ����о�����
 
		//�ж����ɵ�ʳ���Ƿ�������ص�
		temp_ptr = head.next;
		while (temp_ptr != NULL) {
			if (temp_ptr->x == rand_x && temp_ptr->y == rand_y) {
				//�ص�����Ҫ���´���
				create_food_success_flag = FALSE;
				break;
			}
			temp_ptr = temp_ptr->next;
		}
 
		//food������������ص�����������food_node
		if (FALSE == create_food_success_flag) {
			continue;
		}
 
		//�����ɹ����ںڿ��д�ӡ
		create_food_success_flag = TRUE;
		food_node.x = rand_x;
		food_node.y = rand_y;
		locateAndPrint(rand_x, rand_y);
 
		break;
	}
}
 
/*��ʼ������*/
void intSnake(int snake_len, int start_x, int start_y) {
 
 	int i;
	for (i = 0; i < snake_len; i++) {
		temp_ptr = (snake *)malloc(sizeof(snake));  //����ͷ�ڵ� ��������� 
		temp_ptr->x = start_x;
		temp_ptr->y = start_y;
		start_x += 2;
		//ͷ�巨
		temp_ptr->next = head.next;  //�½ڵ��ָ����ָ���׽ڵ� 
		head.next = temp_ptr;  //�޸�ͷ����ָ����ʹ��ָ���½ڵ� 
		//�����ʼ����
		locateAndPrint((head.next)->x, (head.next)->y);
	}
}
 
/*��Ϸ����*/
void endGame()
{
	//��¼�÷�
	int snake_length = 0;
 
	//�ͷ����� malloc�������Դ
	snake * temp_ptr2;//����Ϊ��˼·�������������������temp_ptr2����������temp_ptr2������head.next�����
	temp_ptr = head.next;
	while (temp_ptr != NULL) {
		temp_ptr2 = temp_ptr->next;
		free(temp_ptr);  //�ͷ�malloc(��calloc��realloc)������ָ�����������ڴ�ռ� 
		temp_ptr = temp_ptr2;
		snake_length++;
	}
 
	//�����������Ϸʧ��ԭ��
	system("cls");
 
	if (BIT_SELF == game_over_reason) {
 
		printf("��ͷ������������ʧ�ܣ�\n�÷֣�%d\n", snake_length - 9);
	}
	else if (TOUCH_WALL == game_over_reason) {
		printf("����ǽ�ڣ�ʧ�ܣ�\n�÷֣�%d\n", snake_length - 9);
 
	}
 
	getchar();
}
 
/*��ȡ����ķ���*/
void getEnteredDirection()
{
 
	if (GetAsyncKeyState(VK_OEM_PLUS))  //����+�� 
	{
		speed -= 25;
	}
	else if (GetAsyncKeyState(VK_OEM_MINUS))  //����-�� 
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
 
/*���ƶ�*/
void snakeMove()
{
	//��ԭʼ��head.next������ֵ������
	int temp_x = head.next->x, temp_y = head.next->y;
 
	//�жϷ����޸ĵ�һ���ڵ��ֵ
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
 
	//case1.��һ�ڵ�Ϊʳ� �����һ�ڵ��ʳ��ڵ��غϣ�������ڶ����ڵ��½��ڵ�
	//case2.��һ�ڵ㲻Ϊʳ�������ڶ����ڵ㴦�½��ڵ�
	//�ʿɺϲ�
	snake * temp = (snake *)malloc(sizeof(snake));
	temp->x = temp_x;
	temp->y = temp_y;
	temp->next = head.next->next;
	head.next->next = temp;
 
	//�ж��Ƿ�ײǽ
	if (head.next->x >= 58 || head.next->x <= 0 || head.next->y <= 0 || head.next->y >= 26) {
		game_over_reason = TOUCH_WALL;
		endGame();
	}
 
	//�ж���һ���ڵ��Ƿ�ʳ��
	if (
		food_node.x == head.next->x &&
		food_node.y == head.next->y
		) {
		//��һ���ڵ���ʳ��
		//�����½�ʳ��
		createFood();
	}
	else {
		//��һ���ڵ㲻��ʳ����ƶ�����
 
		//����ǰ���ĵ�һ��λ�ô�ӡ����
		locateAndPrint(head.next->x, head.next->y);
		
		temp_ptr = head.next;
		while (temp_ptr->next->next != NULL) {
 
			//�ж��Ƿ�ҧ���Լ�
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
 
		//�����β�Ľڵ�
		locateAndClear(temp_ptr->next->x, temp_ptr->next->y);
		free(temp_ptr->next);//�ͷ���β�ڵ��ڴ�
		temp_ptr->next = NULL;
	}
}
 
/*��Ϸѭ��*/
void startGame()
{
	//��ʼ����Ĭ�Ϸ���
	direction = R;
 
	while (1) {
		//�жϼ�������ķ��������ע�⡰�ƶ�����Ϊ��ʱ���¼��������á������ҷ���ͬ��
		getEnteredDirection();
 
		//��ʾ����
		snakeMove();//�����ƶ�
 
		Sleep(speed);
	}
}
 
int main()
{
	//������ͼ
	creatMap();
 
	//��ʼ������
	intSnake(8, 4, 3);
	
	//����ʳ��
	createFood();
 
	//��ʼ��Ϸ ѭ����ͣ���жϼ���ķ���
	startGame();
 
	return 0;
}

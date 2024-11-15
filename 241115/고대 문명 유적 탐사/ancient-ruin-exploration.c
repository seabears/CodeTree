#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct {
	int i;
	int j;
} Q_t;

int qfront = 0;
int qrear = 0;
int samefront = 0;
int samerear = 0;
Q_t q[100];
Q_t same[25];

int debug1 = 0;
int debug2 = 0;
void print(int arr[5][5]) {
	printf("\n");
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void rotate(int arr[5][5], int x, int y) {
	int temp[3][3];
	//3*3 복사
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			temp[i][j] = arr[x + i][y + j];
		}
	}
	
	//3*3돌려서 5*5에 붙여넣기
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			arr[x + i][y + j] = temp[2 - j][i];
		}
	}
	//print(arr);
}

/*
123		741
456	 ->	852
789		963

(0,0) (0,1) (0,2)
(1,0) (1,1) (1,2)
(2,0) (2,1) (2,2)

(0,0) (0,1) (0,1)
(0,1) (0,1) (0,1)
(0,1) (0,1) (0,1)

temp      |    바뀐거
(012, x) -> (x, 210)
(



*/
void push(int i, int j, int key) {
	Q_t temp;
	temp.i = i;
	temp.j = j;
	if (key == 0) {
		//q[qrear].i = i;
		//q[qrear++].j = j;
		q[qrear++] = (Q_t){ i,j };
	}
	else if (key == 1) {
		same[samerear++] = (Q_t){ i,j };
	}
}
Q_t pop(int key) {
	if (key == 0)
		return q[qfront++];
	if (key == 1)
		return same[samefront++];
}
void init() {
	qfront = 0;
	qrear = 0;
	samefront = 0;
	samerear = 0;
}
int bfs(int arr[5][5], int visit[5][5], int i, int j, int clr) {
	//Q_t q[100];
	int cnt = 0;
	//Q_t same[25];//현재 같은 점수짜리 저장
	init();

	int dx[4] = { -1,1,0,0 };
	int dy[4] = { 0,0,1,-1 };

	//현위치 처리
	push(i, j, 0);
	visit[i][j] = 1;
	push(i, j, 1);
	cnt++;
	//printf("* ");
	while (qfront < qrear) {
		Q_t temp = pop(0);

		for (int a = 0; a < 4; a++) {
			Q_t new = (Q_t){ temp.i + dx[a], temp.j + dy[a] };
			if (new.i >= 0 && new.i <= 4 && new.j >= 0 && new.j <= 4 && visit[new.i][new.j] == 0 && arr[new.i][new.j] == arr[i][j]) {
				push(new.i, new.j, 0);
				visit[new.i][new.j] = 1;
				push(new.i, new.j, 1);
				cnt++;
			}
		}
		//printf("%d", cnt);
	}
	
	//printf("%d ", cnt);
	if (cnt >= 3) {
		//if (debug == 1) printf("-%d-", arr[i][j]);
		//if (debug == 4) printf("|%d|", arr[i][j]);
		//printf("hi");
		if (clr == 1) {
			while (samefront < samerear) {
				//printf("hi");
				Q_t pos = pop(1);//하나씩 제거
				arr[pos.i][pos.j] = 0;
				//printf("hi");
			}
		}
		return cnt;
	}
	else return 0;
	
	

}

//유물 개수 세기, clr = 1이면 3개이상 값 0으로
int count_clear(int arr[5][5], int clr) {
	int visit[5][5] = { 0 };
	//memset(visit, 0, 100);
	int cnt = 0;
	//처리 안한 거 찾기
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (visit[i][j] == 0) {
				cnt += bfs(arr, visit, i, j, clr);
			}
		}
	}

	return cnt;
}
int main() {
	int K, M;
	scanf("%d%d", &K, &M);

	int map[5][5];
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			scanf("%d", &map[i][j]);
		}
	}
	int* list = (int*)malloc(sizeof(int) * M);
	int list_i = 0;
	for (int i = 0; i < M; i++) scanf("%d", &list[i]);

	int ans[10];
	memset(ans, 0, 40);
	int ans_i = 0;
	//[1] 탐사 진행
	int marr[5][5];	//최대일때
	for (int k = 0; k < K; k++) {	//매턴
		int max_cnt = 0;
		//회전, 열, 행 순
		for (int r = 0; r < 3; r++) {
			for (int j = 0; j < 3; j++) {
				for (int i = 0; i < 3; i++) {
					int narr[5][5];
					if (k == 0) memcpy(narr, map, 100);
					else memcpy(narr, marr, 100);
					for (int rot = 0; rot <= r; rot++) {	//무조건 1회 이상 회전
						rotate(narr, i, j);	//90도 회전
					}
					debug2++;
					int temp = count_clear(narr, 0);	//유물 개수 세기
			
					if (max_cnt < temp) {
						max_cnt = temp;
						if (debug2 == 32) print(narr);
						memcpy(marr, narr, 25 * sizeof(int));
					}
				}
			}
		}

		//printf("HI");
		//printf("===%d  ", max_cnt);
		if (max_cnt == 0) break;
		//print(marr);
		
		//[2] 연쇄획득
		int cnt = 0;
		while (1) {
			debug1++;
			//if (debug == 4) print(marr);
			int temp = count_clear(marr, 1);
			//printf(" +%d+", temp);
			//printf("hi");
			//printf("[%d]", cnt);
			if (temp == 0) break;	//획득 유물 없으면 다음턴
			cnt += temp;	//획득 유물 누적
			
			//printf("*%d* ", cnt);
			//if (cnt == 7) printf("K : %d ", k);
			if (k == 1) debug1 = 1;
			//if (debug == 4) printf("|%d|", list_i);
			//if (debug == 3) printf("? : %d ", temp);
			//리스트에서 추가
			for (int j = 0; j < 5; j++) {
				for (int i = 4; i >= 0; i--) {
					if (marr[i][j] == 0) {
						marr[i][j] = list[list_i++];
					}
				}
			}
			
			//if (debug == 1) print(marr);
			//if (debug == 1) printf("\n%d\n", cnt);
		}
		ans[ans_i++] = cnt;	//이번턴 연쇄 획득 추가
		//ans += cnt; 
		
	}

	for (int i = 0; i < K; i++) if(ans[i] !=0) printf("%d ", ans[i]);
	//printf("%d", ans);
	free(list);
	return 0;
}
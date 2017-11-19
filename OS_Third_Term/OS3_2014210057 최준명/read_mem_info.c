#define PID_MAX 32768	// 최대 PID의 크기!!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Mem_info {
	char name[100];
	int Virtual;
	int Physical;
	int Shared;
	int HWM;
	char wss[10];
};

int main(void) {
	FILE *pFile;
	
	struct Mem_info mem_info[PID_MAX];
	int wss_c[PID_MAX];
	int check[PID_MAX];

	char name[100] = { 0 };
	char temp[200];
	char temp2[200];
	char temp3[200];

	char name_s[100] = { 0 };
	char s_1[200];
	char s_2[200];
	char s_3[200];

	int i = -1, j = -1, k = 0, p = -1,l;

	printf("PID: virtual: physical: ... \n");

	while (1) {														// 시간의 0.5초 텀을 두기 위하여 for문 밖에 while문을 만들고 usleep문을 넣어 delay를 넣었습니다.
		k++;
		if (k == 131) break;
		
		for (i = 1; i < PID_MAX; i++) {								// for문을 사용하여 1번~ PID_MAX반복합니다.
			mem_info[i].HWM = 0;									
			
			sprintf(name, "/proc/%d/status", i);					// int sprintf(char *buffer, const char *format, ...)
																	// name이라는 주소에 /proc/%d/status 라는 문자열을 저장
			pFile = fopen(name, "r");								// fopen(const char * filename, const char * mode)
																	// "r": read: Open file for input operation.	
																	// name의 명령어로 열어 pFile에 저장
			
			if (pFile == 0)continue;								// status의 내용을 긁었을때 안에 내용이 없으면 넘어갑니다.
			else {
				fscanf(pFile, "%s %s", temp, temp2);					// fscanf(FILE* stream, const char* format, ...);
																		// pFile에서 문자열 %s %s을 읽어 각각 temp, temp2에 넣음
																		// %s: 공백문자를 찾을때까지 글자를 읽어옴!!
																		// ex. temp --> name		temp2 --> init
				strcpy(mem_info[i].name, temp2);						// strcpy: 문자열 복사, temp를 mem_info[i].name에 복사
																		// 어떤 Status인지 이름을 받아오기 위함!!
																		// ex. mem_info[1] --> init

				for (j = 0; j < 100; j++) {							// mem_info구조체에 필요한 내용을 넣기 위해 for을 이용하여 내용을 긁어 넣습니다 .
					fscanf(pFile, "%s", temp);						// pFile에서 문자열(%s)을 읽어 temp에 넣음!!
					if (strcmp(temp, "VmSize:") == 0) {				// stercmp 문자열 비교하는 함수, A>B --> 1, A == B --> 0, A<B --> -1
						fscanf(pFile, "%s", temp2);					// pFile에서 문자열(%s)를 읽어 temp2에 넣음!!
																	// VmSize의 값이 temp2에 들어가겠지
						
						mem_info[i].Virtual = atoi(temp2);			// atoi(const char *nptr)
					}												// nptr로 지정된 문자열을 int형으로 변환함.
					else if (strcmp(temp, "VmRSS:") == 0) {
						fscanf(pFile, "%s", temp2);
						mem_info[i].Physical = atoi(temp2);
					}
					else if (strcmp(temp, "VmHWM:") == 0) {
						fscanf(pFile, "%s", temp2);
						mem_info[i].HWM = atoi(temp2);
					}
				}
				if ((mem_info[i].HWM != wss_c[i]) && (k>2)) {		// HWM가 변한경우 check의 값을 바꿔줘서 다르게 확인합니다.
					check[i] = 2;
				}
				
				fclose(pFile);

				sprintf(name, "/proc/%d/statm", i);
				pFile = fopen(name, "r");

				fscanf(pFile, "%s", s_1);
				fscanf(pFile, "%s", s_2);
				fscanf(pFile, "%s", s_3);							// statm의 3번째 값이 shared page를 의미하기 때문에 3번 긁어내어 pFile에 넣었습니다.
				mem_info[i].Shared = atoi(s_3);						
				mem_info[i].Shared *= 4;							// share값이 page로 나오기 때문에 KB으로 바꾸기 위하여 4를 곱해주었습니다.

				fclose(pFile);
			}
			if (k > 2) {											// check[i]에 값이 있는경우(wss가 변화한경우) wss에 *로 표시를 하고 1씩 줄입니다.
				if (check[i] > 0) {
					check[i]--;
					strcpy(mem_info[i].wss, "*");
				}
				else if (check[i] == 0) {
					strcpy(mem_info[i].wss, "\0");
				}
			}
				wss_c[i] = mem_info[i].HWM;
				printf("PID: %-3d name: %-15s virtual: %-7d physical: %-7d Shared: %-7d Wss: %s \n", i, mem_info[i].name, mem_info[i].Virtual, mem_info[i].Physical, mem_info[i].Shared, mem_info[i].wss);
			}
		}	
		usleep(500 * 1000);
	}
}
#define PID_MAX 32768	// �ִ� PID�� ũ��!!
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

	while (1) {														// �ð��� 0.5�� ���� �α� ���Ͽ� for�� �ۿ� while���� ����� usleep���� �־� delay�� �־����ϴ�.
		k++;
		if (k == 131) break;
		
		for (i = 1; i < PID_MAX; i++) {								// for���� ����Ͽ� 1��~ PID_MAX�ݺ��մϴ�.
			mem_info[i].HWM = 0;									
			
			sprintf(name, "/proc/%d/status", i);					// int sprintf(char *buffer, const char *format, ...)
																	// name�̶�� �ּҿ� /proc/%d/status ��� ���ڿ��� ����
			pFile = fopen(name, "r");								// fopen(const char * filename, const char * mode)
																	// "r": read: Open file for input operation.	
																	// name�� ��ɾ�� ���� pFile�� ����
			
			if (pFile == 0)continue;								// status�� ������ �ܾ����� �ȿ� ������ ������ �Ѿ�ϴ�.
			else {
				fscanf(pFile, "%s %s", temp, temp2);					// fscanf(FILE* stream, const char* format, ...);
																		// pFile���� ���ڿ� %s %s�� �о� ���� temp, temp2�� ����
																		// %s: ���鹮�ڸ� ã�������� ���ڸ� �о��!!
																		// ex. temp --> name		temp2 --> init
				strcpy(mem_info[i].name, temp2);						// strcpy: ���ڿ� ����, temp�� mem_info[i].name�� ����
																		// � Status���� �̸��� �޾ƿ��� ����!!
																		// ex. mem_info[1] --> init

				for (j = 0; j < 100; j++) {							// mem_info����ü�� �ʿ��� ������ �ֱ� ���� for�� �̿��Ͽ� ������ �ܾ� �ֽ��ϴ� .
					fscanf(pFile, "%s", temp);						// pFile���� ���ڿ�(%s)�� �о� temp�� ����!!
					if (strcmp(temp, "VmSize:") == 0) {				// stercmp ���ڿ� ���ϴ� �Լ�, A>B --> 1, A == B --> 0, A<B --> -1
						fscanf(pFile, "%s", temp2);					// pFile���� ���ڿ�(%s)�� �о� temp2�� ����!!
																	// VmSize�� ���� temp2�� ������
						
						mem_info[i].Virtual = atoi(temp2);			// atoi(const char *nptr)
					}												// nptr�� ������ ���ڿ��� int������ ��ȯ��.
					else if (strcmp(temp, "VmRSS:") == 0) {
						fscanf(pFile, "%s", temp2);
						mem_info[i].Physical = atoi(temp2);
					}
					else if (strcmp(temp, "VmHWM:") == 0) {
						fscanf(pFile, "%s", temp2);
						mem_info[i].HWM = atoi(temp2);
					}
				}
				if ((mem_info[i].HWM != wss_c[i]) && (k>2)) {		// HWM�� ���Ѱ�� check�� ���� �ٲ��༭ �ٸ��� Ȯ���մϴ�.
					check[i] = 2;
				}
				
				fclose(pFile);

				sprintf(name, "/proc/%d/statm", i);
				pFile = fopen(name, "r");

				fscanf(pFile, "%s", s_1);
				fscanf(pFile, "%s", s_2);
				fscanf(pFile, "%s", s_3);							// statm�� 3��° ���� shared page�� �ǹ��ϱ� ������ 3�� �ܾ�� pFile�� �־����ϴ�.
				mem_info[i].Shared = atoi(s_3);						
				mem_info[i].Shared *= 4;							// share���� page�� ������ ������ KB���� �ٲٱ� ���Ͽ� 4�� �����־����ϴ�.

				fclose(pFile);
			}
			if (k > 2) {											// check[i]�� ���� �ִ°��(wss�� ��ȭ�Ѱ��) wss�� *�� ǥ�ø� �ϰ� 1�� ���Դϴ�.
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
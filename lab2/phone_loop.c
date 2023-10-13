#include <stdio.h>

int main() {
	char phone[11];
	int pos;
	scanf("%s", phone);
	int cnt = 0;
	while (scanf("%d", &pos) != EOF) {
		if (pos == -1) {
			printf("%s\n", phone);
		}

		if (pos >= 0 && pos <= 9) {
			printf("%c\n", phone[pos]);
		}
    
    	if ((pos < -1) | (pos > 9)) {
    		char err[] = "ERROR";
    		printf("%s\n", err);
    		cnt = 1;
    	}
	}
	return cnt;
}
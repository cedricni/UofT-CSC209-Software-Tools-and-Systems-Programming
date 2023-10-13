#include <stdio.h>

int main() {
	char phone[11];
	int pos;
	scanf("%s %d", phone, &pos);

	if (pos == -1) {
		printf("%s\n", phone);
		return 0;
	}

	if (pos >= 0 && pos <= 9) {
		printf("%c\n", phone[pos]);
		return 0;
	}
    
    if ((pos < -1) | (pos > 9)) {
    	char err[] = "ERROR";
    	printf("%s\n", err);
    	return 1;
    }
}
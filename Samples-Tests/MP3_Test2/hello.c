#include <time.h>

int main() {

    struct tm * ptm;
    time_t mytime;
    tzset();
    time(&mytime);
    ptm = localtime(&mytime);
    printf("%2.2d/%2.2d/%4.4d %2.2d:%2.2d\n",ptm->tm_mday, ptm->tm_mon + 1, ptm->tm_year + 1900, ptm->tm_hour,ptm->tm_min); 
    printf("Started %s\n", ctime(&mytime));
    printf("Hello World\n");
/*    time_t now;
    now = time(NULL);
    printf("Started %s\n", ctime(&now));
*/
    return 0;
    }




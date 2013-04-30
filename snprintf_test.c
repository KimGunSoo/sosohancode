#include <stdio.h>

static int g_val = 0;
void * h = NULL;
int s = 0;

void function_a(void* a)
{
    int ret;
    h = a;
    s = *((int*)a);
    ret = -1;

    char buffer[32];
    ret = snprintf(buffer, sizeof(buffer), "%d", g_val);

    printf("snprintf() ret=%d, buffer=%s, strlen() ret=%d\n", ret, buffer, strlen(buffer));
}

int main()
{
    int i = 0;

    for(i=0; i<100; i++){
        function_a((void*)(&i));
    }

    return 0;
}


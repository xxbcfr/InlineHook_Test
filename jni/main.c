#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include "hook.h"

void my_hook_function()
{
	LOGI("Hello from hook!\n");
}

// This is the original function to hook.
void my_function()
{
	LOGI("Hello, world!\n");
}

void test()
{
    my_function();
    inline_hook(my_function, my_hook_function);
    my_function();
    remove_hook(my_function);
    my_function();

    LOGI("\r\n");
    FILE * a=fopen("/data/local/tmp/testSo.so","r");
	LOGI("fopen获取的文件指针的地址值：%x\r\n",a);

	inline_hook(fopen, my_hook_function);
    a=0;
	// Now calling the function will actually call the hook function.
	a=fopen("/data/local/tmp/testSo.so","r");

	LOGI("fopen获取的文件指针的地址值：%x\r\n",a);
	remove_hook(fopen);

	
}
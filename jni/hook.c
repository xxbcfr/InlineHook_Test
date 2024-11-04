#include "hook.h"
#define SIZE_ORIG_BYTES 32

static void inline_hook_replace_inst(void *orig_func, void *hook_func) {
    int offset = ((intptr_t)hook_func - (intptr_t)orig_func) / 4;
    if (offset < -0x2000000 || offset > 0x1ffffff) {
        LOGI("超过8字节跳转能访问的区间，使用16字节跳转");
        //    
		unsigned char szLdrPCOpcodes[24] = {0xe1, 0x03, 0x3f, 0xa9, 0x40, 0x00, 0x00, 0x58, 0x00, 0x00, 0x1f, 0xd6};
        //将目的地址拷贝到跳转指令缓存位置
		uint64_t hook_proc=(uint64_t)hook_func;
        memcpy(szLdrPCOpcodes + 12,&hook_proc, 8);
        szLdrPCOpcodes[20] = 0xE0;
        szLdrPCOpcodes[21] = 0x83;
        szLdrPCOpcodes[22] = 0x5F;
        szLdrPCOpcodes[23] = 0xF8;
		memcpy(orig_func, szLdrPCOpcodes, 24);
		return;
    }
    LOGI("使用8字节跳转");
    uint32_t branch_instruction = 0x14000000 | (offset & 0x03ffffff);
    *((uint32_t*)orig_func) = branch_instruction;
}

void *get_page_addr(void *addr)
{
	return (void *)((uintptr_t)addr & ~(getpagesize() - 1));
}

unsigned char orig_bytes[SIZE_ORIG_BYTES];

void inline_hook(void *orig_func, void *hook_func)
{
	// Store the original bytes of the function.
	memcpy(orig_bytes, orig_func, SIZE_ORIG_BYTES);

    LOGI("被hook的函数的地址:%x\r\n",orig_func);
	LOGI("hook的函数的地址:%x\r\n",hook_func);
	// Make the memory page writable.

   mprotect(get_page_addr(orig_func), getpagesize(),
		 PROT_READ | PROT_WRITE | PROT_EXEC) ;
      
   

    inline_hook_replace_inst(orig_func, hook_func);

	// Make the memory page executable only.
	mprotect(get_page_addr(orig_func), getpagesize(),
		 PROT_READ | PROT_EXEC);
}

void remove_hook(void *orig_func){
	// Make the memory page writable.
	mprotect(get_page_addr(orig_func), getpagesize(),
		 PROT_READ | PROT_WRITE | PROT_EXEC);

	// Restore the original bytes of the function.
	memcpy(orig_func, orig_bytes, SIZE_ORIG_BYTES);

	// Make the memory page executable only.
	mprotect(get_page_addr(orig_func), getpagesize(),
		 PROT_READ | PROT_EXEC);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>

namespace {
    constexpr size_t HeapSize = 64_KB;
    u8 g_Heap[HeapSize] = {};
}

extern "C" {

    u32 __nx_applet_exit_mode = 2;

    u32 __nx_fs_num_sessions = 1;
    u32 __nx_fsdev_direntry_cache_size = 1;
    bool __nx_fsdev_support_cwd = false;

    extern u8 *fake_heap_start;
    extern u8 *fake_heap_end;

    void __libnx_initheap() {
        fake_heap_start = g_Heap;
        fake_heap_end = g_Heap + HeapSize;
    }

    void __appInit() {}
    void __appExit() {}

}

int main() {
    consoleInit(NULL);

    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);

    Result rc = smInitialize();
    if (R_FAILED(rc)) {
        printf("smInitialize() failed: 0x%x\n", rc);
    }

    rc = fsInitialize();
    if (R_FAILED(rc)) {
        printf("fsInitialize() failed: 0x%x\n", rc);
    }

    rc = fsdevMountSdmc();
    if (R_FAILED(rc)) {
        printf("fsdevMountSdmc() failed: 0x%x\n", rc);
    }

    while(appletMainLoop()) {
        padUpdate(&pad);

        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus)
            break;

        consoleUpdate(NULL);
    }

    fsdevUnmountAll();
    fsExit();
    smExit();

    consoleExit(NULL);

    return 0;
}
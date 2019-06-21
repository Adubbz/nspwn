#include <inttypes.h>
#include <stdio.h>
#include <switch.h>
#include <inttypes.h>

void userAppInit(void) {
    Result rc = 0;
    if (R_FAILED(rc = lrInitialize())) {
        fatalSimple(rc);
    }
}

void userAppExit(void) {
    lrExit();
}

int main(int arg, char** argv) {
    Result rc = 0;
    consoleInit(NULL);
    printf("pwn time\n");

    LrLocationResolver location_resolver;

    if (R_FAILED(rc = lrOpenLocationResolver(FsStorageId_NandSystem, &location_resolver))) {
        printf("Failed to open location resolver.\n Rc: 0x%08" PRIX32, rc);
        goto MAIN_LOOP;
    }

    if (R_FAILED(rc = lrLrRedirectProgramPath(&location_resolver, 0x010000000000100D, "@Sdcard:/nspwn.nsp"))) {
        printf("Failed to redirect program path.\n Rc: 0x%08" PRIX32, rc);
        goto MAIN_LOOP;
    }

    printf("Success\n");

    MAIN_LOOP:
    while (appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_B)
            break;

        consoleUpdate(NULL);
    }

    consoleExit(NULL);

    return 0;
}
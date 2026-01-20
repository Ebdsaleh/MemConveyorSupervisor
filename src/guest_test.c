// src/guest_test.c
#include <stdio.h>
#include <stdlib.h>

void main() {
    FILE *file;
    int lump_to_request = 0;
    unsigned char magic[5];

    printf("--- GUEST: MemConveyor Client Initialized ---\n");

    while(lump_to_request < 5) {
        printf("\n[GUEST] Requesting Lump %d...\n", lump_to_request);

        // 1. Write the Request
        file = fopen("REQ.BIN", "wb");
        if (file) {
            fwrite(&lump_to_request, sizeof(int), 1, file);
            fclose(file);
        }

        // 2. Wait for Handshake (ACK.BIN)
        while (1) {
            file = fopen("ACK.BIN", "rb");
            if (file) {
                fclose(file);
                remove("ACK.BIN");
                break;
            }
        }

        // 3. VERIFICATION: Peek at the data the Supervisor just swapped in
        printf("[GUEST] Bridge Confirmed! Peeking at Window...\n");
        file = fopen("MODEL_~1/IBM-GR~1.GGU", "rb");
        if (file) {
            // Read the first 4 bytes of the current lump
            fread(magic, 1, 4, file);
            magic[4] = '\0';
            
            // Output Hex and ASCII for clarity
            printf("[DATA] Offset %ld MB | Magic: %02X %02X %02X %02X (%s)\n", 
                   (long)lump_to_request * 16, magic[0], magic[1], magic[2], magic[3], magic);
            fclose(file);
        } else {
            // This will tell us if the 486 can't see the file
            printf("[ERROR] Guest could not open model window!\n");
        }

        lump_to_request++;
    }

    printf("\n--- GUEST: Test Sequence Complete ---\n");
}

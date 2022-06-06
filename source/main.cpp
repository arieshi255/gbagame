// --------------------------------------------------------------------
//
//  Super Simple Background Text Scroller v1.0
//
//   Not terribly unlike how we did it in the c64 days.  Although this
//   is C and on the c64 we used asm, the concept is very much similar.
//   This source is set up so you can tweak the speed and the row to
//   scroll the text on...  In actuality there are a number of ways
//   to perform a scroller like this, but I tried to break it down
//   in a simple fashion to make it easier to understand (hopefully!)
//
//   r6502 - 2004-04-02
//
//   Note: tabs == 4 on my setup
//
// --------------------------------------------------------------------

#include <stdlib.h>

#include <gba_base.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_input.h>
#include <gba_console.h>

#include <maxmod.h>

#include "Game.h"
#include "Overworld.h"

// --------------------------------------------------------------------

void vbl_handler() {
    mmVBlank();
}

int main() {
    // Set up the interrupt handlers
    irqInit();

    // Maxmod vblank handler
    irqSet(IRQ_VBLANK, vbl_handler);
    // Enable Vblank Interrupt to allow VblankIntrWait
    irqEnable(IRQ_VBLANK);

    // Allow Interrupts
    REG_IME = 1;

    // Screen mode & background to display
    SetMode( MODE_0 | BG0_ON | BG1_ON | OBJ_ON | OBJ_1D_MAP );

    // Initialize OAM memory
    initSprites();

    // Initialize console output
    consoleInit(3, 28, 1, NULL, 0, 1);

    // Music test
    //mmInitDefault((mm_addr)soundbank_bin, 15);
    //mmStart(MOD_PAGEONE, MM_PLAY_LOOP);

    Game game{};
    OverworldScene overworld{};

    game.init();
    game.run(&overworld);
}

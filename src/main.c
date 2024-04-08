#include <genesis.h>
#include <resources.h>

#define ANIM_IDLE 0
#define ANIM_DOWN 1
#define ANIM_UP 2
#define ANIM_SIDE 3

Sprite *player;
int player_x = 0;
int player_y = 0;

static void handleInput()
{
    u16 value = JOY_readJoypad(JOY_1);

    if (value & BUTTON_RIGHT)
    {
        player_x += 1;
    }
    else if (value & BUTTON_LEFT)
    {
        player_x -= 1;
    }
    if ((value & BUTTON_RIGHT) && !(value & BUTTON_UP) && !(value & BUTTON_DOWN))
    {
        SPR_setAnim(player, ANIM_SIDE);
        SPR_setHFlip(player, TRUE);
    }
    if ((value & BUTTON_LEFT) && !(value & BUTTON_UP) && !(value & BUTTON_DOWN))
    {
        SPR_setAnim(player, ANIM_SIDE);
        SPR_setHFlip(player, FALSE);
    }
    if (value & BUTTON_DOWN)
    {
        player_y += 1;
        SPR_setAnim(player, ANIM_DOWN);
    }
    else if (value & BUTTON_UP)
    {
        player_y -= 1;
        SPR_setAnim(player, ANIM_UP);
    }

    if (!(value & BUTTON_DOWN) && !(value & BUTTON_UP) && !(value & BUTTON_LEFT) && !(value & BUTTON_RIGHT))
    {
        SPR_setAnim(player, ANIM_IDLE);
    }
    SPR_setPosition(player, player_x, player_y);
}

int main()
{
    VDP_drawText("Hello Neil!!", 14, 27);

    u16 ind = TILE_USERINDEX;
    PAL_setPalette(PAL0, bg1.palette->data);
    VDP_drawImageEx(BG_B, &bg1, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);

    SPR_init();
    PAL_setPalette(PAL2, rabbit.palette->data);
    player = SPR_addSprite(&rabbit, player_x, player_y, TILE_ATTR(PAL2, FALSE, FALSE, TRUE));

    while (1)
    {
        handleInput();
        SPR_update();
        SYS_doVBlankProcess();
    }
    return (0);
}

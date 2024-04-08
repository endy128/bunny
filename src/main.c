#include <genesis.h>
#include <resources.h>

#define ANIM_IDLE 0
#define ANIM_DOWN 1
#define ANIM_UP 2
#define ANIM_SIDE 3

#define MAX_BULLETS 3

// Sprite *player;
// int player.x = 0;
// int player.y = 0;

struct Entity
{
    int x;
    int y;
    int w;
    int h;
    int velx;
    int vely;
    int health;
    Sprite *sprite;
    char name[6];
};

struct Entity player = {110, 0, 16, 16, 0, 0, 0, NULL, "PLAYER"};

struct Entity bullets[MAX_BULLETS];
u16 bulletsOnScreen = 0;

void killEntity(struct Entity *e)
{
    e->health = 0;
    SPR_setVisibility(e->sprite, HIDDEN);
}

void reviveEntity(struct Entity *e)
{
    e->health = 1;
    SPR_setVisibility(e->sprite, VISIBLE);
}

void setUpBullets()
{

    struct Entity *b = bullets;
    for (u16 i = 0; i < MAX_BULLETS; i++)
    {
        b->x = 0;
        b->y = -10;
        b->w = 8;
        b->h = 8;
        b->sprite = SPR_addSprite(&bullet, bullets[0].x, bullets[0].y, TILE_ATTR(PAL3, 0, FALSE, FALSE));
        sprintf(b->name, "Bu%d", i);
        b++;
    }
}

void shootBullet()
{
    if (bulletsOnScreen < MAX_BULLETS)
    {
        struct Entity *b;
        u16 i = 0;
        for (i = 0; i < MAX_BULLETS; i++)
        {
            b = &bullets[i];
            if (b->health == 0)
            {

                b->x = player.x + 4;
                b->y = player.y;

                reviveEntity(b);
                b->vely = -3;

                SPR_setPosition(b->sprite, b->x, b->y);
                bulletsOnScreen++;
                break;
            }
        }
    }
}

static void
handleInput()
{
    u16 value = JOY_readJoypad(JOY_1);

    if (value & BUTTON_RIGHT)
    {
        player.x += 1;
    }
    else if (value & BUTTON_LEFT)
    {
        player.x -= 1;
    }
    if ((value & BUTTON_RIGHT) && !(value & BUTTON_UP) && !(value & BUTTON_DOWN))
    {
        SPR_setAnim(player.sprite, ANIM_SIDE);
        SPR_setHFlip(player.sprite, TRUE);
    }
    if ((value & BUTTON_LEFT) && !(value & BUTTON_UP) && !(value & BUTTON_DOWN))
    {
        SPR_setAnim(player.sprite, ANIM_SIDE);
        SPR_setHFlip(player.sprite, FALSE);
    }
    if (value & BUTTON_DOWN)
    {
        player.y += 1;
        SPR_setAnim(player.sprite, ANIM_DOWN);
    }
    else if (value & BUTTON_UP)
    {
        player.y -= 1;
        SPR_setAnim(player.sprite, ANIM_UP);
    }

    if (!(value & BUTTON_DOWN) && !(value & BUTTON_UP) && !(value & BUTTON_LEFT) && !(value & BUTTON_RIGHT))
    {
        SPR_setAnim(player.sprite, ANIM_IDLE);
    }
    SPR_setPosition(player.sprite, player.x, player.y);

    if (value & BUTTON_B)
    {
        shootBullet();
    }
}

int main()
{
    VDP_drawText("Hello Neil!!", 14, 27);

    u16 ind = TILE_USERINDEX;
    PAL_setPalette(PAL0, bg1.palette->data);
    VDP_drawImageEx(BG_B, &bg1, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);

    SPR_init();
    PAL_setPalette(PAL2, rabbit.palette->data);
    player.sprite = SPR_addSprite(&rabbit, player.x, player.y, TILE_ATTR(PAL2, FALSE, FALSE, TRUE));

    PAL_setPalette(PAL3, bullet.palette->data);
    setUpBullets();

    while (1)
    {
        handleInput();
        SPR_update();
        SYS_doVBlankProcess();
    }
    return (0);
}

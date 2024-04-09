#include <genesis.h>
#include <resources.h>

#define ANIM_IDLE 0
#define ANIM_DOWN 1
#define ANIM_UP 2
#define ANIM_SIDE 3

#define MAX_BULLETS 12
#define BULLET_SPEED 3

const u16 MAX_SCREEN_HEIGHT = 240;
const u16 MAX_SCREEN_WIDTH = 320;

// Sprite *player;
// int player.x = 0;
// int player.y = 0;

typedef struct Entity
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
} entity;

entity player = {110, 0, 16, 16, 0, 0, 0, NULL, "PLAYER"};

entity bullets[MAX_BULLETS];
u16 bulletsOnScreen = 0;

void killEntity(entity *e)
{
    e->health = 0;
    SPR_setVisibility(e->sprite, HIDDEN);
}

void reviveEntity(entity *e)
{
    e->health = 1;
    SPR_setVisibility(e->sprite, VISIBLE);
}

void setUpBullets()
{
    entity *b = bullets;
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
        entity *b;
        u16 i = 0;
        for (i = 0; i < MAX_BULLETS; i++)
        {
            b = &bullets[i];
            if (b->health == 0)
            {

                b->x = player.x + 12; // half the player sprite width
                b->y = player.y + 8;

                reviveEntity(b);
                b->vely = player.vely * BULLET_SPEED;
                b->velx = player.velx * BULLET_SPEED;

                SPR_setPosition(b->sprite, b->x, b->y);
                bulletsOnScreen++;
                break;
            }
        }
    }
}

static bulletOffScreen(entity *b)
{
    if ((b->y + b->h < 0) || (b->y + b->h > MAX_SCREEN_HEIGHT))
    {
        killEntity(b);
        bulletsOnScreen--;
    }
    else if ((b->x + b->h < 0) || (b->x + b->h > MAX_SCREEN_WIDTH))
    {
        killEntity(b);
        bulletsOnScreen--;
    }
    else
    {
        SPR_setPosition(b->sprite, b->x, b->y);
    }
}

static void positionBullet()
{
    entity *b;
    for (u16 i = 0; i < MAX_BULLETS; i++)
    {
        b = &bullets[i];
        if (b->health > 0)
        {
            b->y += b->vely;
            b->x += b->velx;
            bulletOffScreen(b);
            // if (b->y + b->h < 0) // ONLY CHECKS TOP OF SCREEN
            // {
            //     killEntity(b);
            //     bulletsOnScreen--;
            // }
            // else
            // {
            //     SPR_setPosition(b->sprite, b->x, b->y);
            // }
        }
    }
}

// D-Pad
static void handleInput()
{
    u16 value = JOY_readJoypad(JOY_1);

    if (value & BUTTON_RIGHT)
    {
        player.x += 1;
        player.velx = 1;
    }
    else if (value & BUTTON_LEFT)
    {
        player.x -= 1;
        player.velx = -1;
    }
    if ((value & BUTTON_RIGHT) && !(value & BUTTON_UP) && !(value & BUTTON_DOWN))
    {
        player.velx = 1;
        player.vely = 0;
        SPR_setAnim(player.sprite, ANIM_SIDE);
        SPR_setHFlip(player.sprite, TRUE);
    }
    if ((value & BUTTON_LEFT) && !(value & BUTTON_UP) && !(value & BUTTON_DOWN))
    {
        player.velx = -1;
        player.vely = 0;
        SPR_setAnim(player.sprite, ANIM_SIDE);
        SPR_setHFlip(player.sprite, FALSE);
    }
    if (value & BUTTON_DOWN)
    {
        player.y += 1;
        player.vely = 1;
        SPR_setAnim(player.sprite, ANIM_DOWN);
    }
    else if (value & BUTTON_UP)
    {
        player.y -= 1;
        player.vely = -1;
        SPR_setAnim(player.sprite, ANIM_UP);
    }

    if (((value & BUTTON_DOWN) || (value & BUTTON_UP)) && !(value & BUTTON_RIGHT) && !(value & BUTTON_LEFT))
    {
        player.velx = 0;
    }

    if (!(value & BUTTON_DOWN) && !(value & BUTTON_UP) && !(value & BUTTON_LEFT) && !(value & BUTTON_RIGHT))
    {
        SPR_setAnim(player.sprite, ANIM_IDLE);
    }
    SPR_setPosition(player.sprite, player.x, player.y);

    // player.velx = 0;
    // player.vely = 0;

    // if (value & BUTTON_B)
    // {
    //     shootBullet();
    // }
}

// handle button presses for non D-Pad
static void joyEvent(u16 joy, u16 changed, u16 state)
{
    // if (changed & state & BUTTON_RIGHT)
    // {
    //     player.x += 1;
    // }
    // else if (changed & state & BUTTON_LEFT)
    // {
    //     player.x -= 1;
    // }
    // if ((changed & state & BUTTON_RIGHT) && !(changed & state & BUTTON_UP) && !(changed & state & BUTTON_DOWN))
    // {
    //     SPR_setAnim(player.sprite, ANIM_SIDE);
    //     SPR_setHFlip(player.sprite, TRUE);
    // }
    // if ((changed & state & BUTTON_LEFT) && !(changed & state & BUTTON_UP) && !(changed & state & BUTTON_DOWN))
    // {
    //     SPR_setAnim(player.sprite, ANIM_SIDE);
    //     SPR_setHFlip(player.sprite, FALSE);
    // }
    // if (changed & state & BUTTON_DOWN)
    // {
    //     player.y += 1;
    //     SPR_setAnim(player.sprite, ANIM_DOWN);
    // }
    // else if (changed & state & BUTTON_UP)
    // {
    //     player.y -= 1;
    //     SPR_setAnim(player.sprite, ANIM_UP);
    // }

    // if (!(changed & state & BUTTON_DOWN) && !(changed & state & BUTTON_UP) && !(changed & state & BUTTON_LEFT) && !(changed & state & BUTTON_RIGHT))
    // {
    //     SPR_setAnim(player.sprite, ANIM_IDLE);
    // }

    if (changed & state & BUTTON_B)
    {
        shootBullet();
    }
}

int main()
{
    VDP_drawText("Hello Neil!!", 14, 27);

    JOY_setEventHandler(joyEvent);

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
        positionBullet();
    }
    return (0);
}

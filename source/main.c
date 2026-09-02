/*
 * PS3 2D Port Test 0.01
 *
 * Clean-room proof of concept.
 * Nenhum codigo ou asset de Dead Cells esta incluido.
 */

#include <stdio.h>
#include <stdlib.h>
#include <io/pad.h>
#include <tiny3d.h>

#define SCREEN_W 848.0f
#define SCREEN_H 512.0f
#define FLOOR_Y  420.0f

typedef struct
{
    float x;
    float y;
    float w;
    float h;

    float vx;
    float vy;

    int grounded;

} Player;

static Player player =
{
    120.0f,
    FLOOR_Y - 52.0f,

    34.0f,
    52.0f,

    0.0f,
    0.0f,

    1
};

static int old_cross = 0;


static void draw_rect(
    float x,
    float y,
    float z,
    float w,
    float h,
    u32 color
)
{
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, z);
    tiny3d_VertexColor(color);

    tiny3d_VertexPos(x + w, y, z);
    tiny3d_VertexColor(color);

    tiny3d_VertexPos(x + w, y + h, z);
    tiny3d_VertexColor(color);

    tiny3d_VertexPos(x, y + h, z);
    tiny3d_VertexColor(color);

    tiny3d_End();
}


static void draw_scene(void)
{
    tiny3d_Project2D();

    /* fundo */

    draw_rect(
        0,
        0,
        65535,
        SCREEN_W,
        SCREEN_H,
        0x111621FF
    );


    /* camadas simples de cenario */

    draw_rect(
        35,
        220,
        64000,
        190,
        200,
        0x182234FF
    );

    draw_rect(
        260,
        170,
        64000,
        215,
        250,
        0x1C2940FF
    );

    draw_rect(
        520,
        235,
        64000,
        270,
        185,
        0x172033FF
    );


    /* chao */

    draw_rect(
        0,
        FLOOR_Y,
        1000,
        SCREEN_W,
        SCREEN_H - FLOOR_Y,
        0x252B35FF
    );

    draw_rect(
        0,
        FLOOR_Y,
        900,
        SCREEN_W,
        4,
        0x6A7080FF
    );


    /* personagem placeholder */

    draw_rect(
        player.x,
        player.y,
        200,
        player.w,
        player.h,
        0xD84B4BFF
    );


    /* cabeca */

    draw_rect(
        player.x + 7,
        player.y - 12,
        190,
        20,
        16,
        0xE87952FF
    );


    /* espada placeholder */

    draw_rect(
        player.x + player.w,
        player.y + 20,
        180,
        30,
        5,
        0xC5CEDAFF
    );
}


static void update_player(const padData *pad)
{
    const float SPEED = 3.4f;
    const float GRAVITY = 0.60f;
    const float JUMP = -10.8f;


    player.vx = 0.0f;


    if (pad->BTN_LEFT)
    {
        player.vx = -SPEED;
    }

    if (pad->BTN_RIGHT)
    {
        player.vx = SPEED;
    }


    if (
        pad->BTN_CROSS &&
        !old_cross &&
        player.grounded
    )
    {
        player.vy = JUMP;
        player.grounded = 0;
    }


    old_cross =
        pad->BTN_CROSS ? 1 : 0;


    player.vy += GRAVITY;

    player.x += player.vx;
    player.y += player.vy;


    if (player.x < 0)
    {
        player.x = 0;
    }


    if (
        player.x +
        player.w >
        SCREEN_W
    )
    {
        player.x =
            SCREEN_W -
            player.w;
    }


    if (
        player.y +
        player.h >=
        FLOOR_Y
    )
    {
        player.y =
            FLOOR_Y -
            player.h;

        player.vy = 0;

        player.grounded = 1;
    }
}


s32 main(
    s32 argc,
    const char *argv[]
)
{
    padInfo padinfo;
    padData paddata;

    int running = 1;
    int i;

    (void) argc;
    (void) argv;


    tiny3d_Init(
        1024 * 1024
    );


    ioPadInit(7);


    while (running)
    {
        ioPadGetInfo(
            &padinfo
        );


        for (
            i = 0;
            i < MAX_PADS;
            i++
        )
        {
            if (!padinfo.status[i])
            {
                continue;
            }


            ioPadGetData(
                i,
                &paddata
            );


            if (paddata.len > 0)
            {
                if (paddata.BTN_START)
                {
                    running = 0;
                    break;
                }


                update_player(
                    &paddata
                );


                break;
            }
        }


        tiny3d_Clear(
            0x000000FF,
            TINY3D_CLEAR_ALL
        );


        draw_scene();


        tiny3d_Flip();
    }


    ioPadEnd();

    return 0;
}

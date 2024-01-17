// screw you code formatter, this one needs to stay on top
#include <stdlib.h>
// now we need to have libgte.h
#include <libgte.h>
// then, these can work; the psyq library is badly written and won't include what it uses
#include <libetc.h>
#include <libgpu.h>
#include "psyx_compat.h"


#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*(a)))

#define OTSIZE 4096
#define SCREEN_Z 512
#define CUBESIZE 196

typedef struct DB {
    DRAWENV draw;
    DISPENV disp;
    OTTYPE ot[OTSIZE];
    POLY_F4 s[6];
} DB;

static SVECTOR cube_vertices[] = {
    {-CUBESIZE / 2, -CUBESIZE / 2, -CUBESIZE / 2, 0}, {CUBESIZE / 2, -CUBESIZE / 2, -CUBESIZE / 2, 0},
    {CUBESIZE / 2, CUBESIZE / 2, -CUBESIZE / 2, 0},   {-CUBESIZE / 2, CUBESIZE / 2, -CUBESIZE / 2, 0},
    {-CUBESIZE / 2, -CUBESIZE / 2, CUBESIZE / 2, 0},  {CUBESIZE / 2, -CUBESIZE / 2, CUBESIZE / 2, 0},
    {CUBESIZE / 2, CUBESIZE / 2, CUBESIZE / 2, 0},    {-CUBESIZE / 2, CUBESIZE / 2, CUBESIZE / 2, 0},
};

static int cube_indices[] = {
    0, 1, 2, 3, 1, 5, 6, 2, 5, 4, 7, 6, 4, 0, 3, 7, 4, 5, 1, 0, 6, 7, 3, 2,
};

static void init_cube(DB *db, CVECTOR *col) {
    size_t i;

    for (i = 0; i < ARRAY_SIZE(db->s); ++i) {
        SetPolyF4(&db->s[i]);
        setRGB0(&db->s[i], col[i].r, col[i].g, col[i].b);
    }
}

static void add_cube(u_long *ot, POLY_F4 *s, MATRIX *transform) {
    long p, otz, flg;
    int nclip;
    size_t i;

    SetRotMatrix(transform);
    SetTransMatrix(transform);

    for (i = 0; i < ARRAY_SIZE(cube_indices); i += 4, ++s) {
        nclip = RotAverageNclip4(&cube_vertices[cube_indices[i + 0]], &cube_vertices[cube_indices[i + 1]],
                                 &cube_vertices[cube_indices[i + 2]], &cube_vertices[cube_indices[i + 3]],
                                 (long *)&s->x0, (long *)&s->x1, (long *)&s->x3, (long *)&s->x2, &p, &otz, &flg);

        if (nclip <= 0) continue;

        if ((otz > 0) && (otz < OTSIZE)) AddPrim(&ot[otz], s);
    }
}

int main(void) {
    DB db[2];
    DB *cdb;
    SVECTOR rotation = {0};
    VECTOR translation = {0, 0, (SCREEN_Z * 3) / 2, 0};
    MATRIX transform;
    CVECTOR col[6];
    size_t i;

    PsyX_Initialise("Test1", 800, 600, 0);
    StopCallback();
    ResetCallback();
    SetVideoMode(MODE_NTSC);

    ResetGraph(0);
    InitGeom();

    SetGraphDebug(0);

    FntLoad(960, 256);
    SetDumpFnt(FntOpen(32, 32, 320, 64, 0, 512));

    SetGeomOffset(320, 240);
    SetGeomScreen(SCREEN_Z);

    SetDefDrawEnv(&db[0].draw, 0, 0, 640, 480);
    SetDefDrawEnv(&db[1].draw, 0, 0, 640, 480);
    SetDefDispEnv(&db[0].disp, 0, 0, 640, 480);
    SetDefDispEnv(&db[1].disp, 0, 0, 640, 480);

    srand(0);

    for (i = 0; i < ARRAY_SIZE(col); ++i) {
        col[i].r = rand();
        col[i].g = rand();
        col[i].b = rand();
    }

    init_cube(&db[0], col);
    init_cube(&db[1], col);

    SetDispMask(1);

    PutDrawEnv(&db[0].draw);
    PutDispEnv(&db[0].disp);

    while (1) {
        cdb = (cdb == &db[0]) ? &db[1] : &db[0];

        rotation.vy += 16;
        rotation.vz += 16;

        RotMatrix(&rotation, &transform);
        TransMatrix(&transform, &translation);

        ClearOTagR(cdb->ot, OTSIZE);

        FntPrint("Code compiled using psyq libraries\n\n");
        FntPrint("converted by psyq-obj-parser\n\n");
        FntPrint("PCSX-Redux project\n\n");
        FntPrint("https://bit.ly/pcsx-redux");

        add_cube(cdb->ot, cdb->s, &transform);

        DrawSync(0);
        VSync(0);
        SetDispMask(1);
        PsyX_EndScene();

        ClearImage(&cdb->draw.clip, 60, 120, 120);

        DrawOTag(&cdb->ot[OTSIZE - 1]);
        FntFlush(-1);
        
    }

    return 0;
}

// #include <stdlib.h>
// #include <string.h>
// #include <stdio.h>

// #include <libetc.h>
// #include <libgte.h>
// #include <libgpu.h>
// #include <PsyX/PsyX_public.h>
// #include <PsyX/PsyX_globals.h>

// #ifdef PSX
// typedef int intptr_t;
// typedef u_long OTTYPE;

// typedef short VERTTYPE;

// #define RECT16 RECT

// #else

// #if defined(_M_X64) || defined(__amd64__)
// typedef OT_TAG OTTYPE;
// #else
// typedef unsigned long long OTTYPE;
// #endif

// #endif

// #define OT_LEN 100
// #define PRIM_LEN 100

// static int db = 0;
// static DISPENV disp[2];
// static DRAWENV draw[2];
// static OTTYPE ot[2][OT_LEN];

// static char _primitiveBuffer[2][PRIM_LEN];
// static char* _primitive;

// static int _screenWidth;
// static int _screenHeight;

// static void _init(void) {
//     SetDispMask(0);
//     StopCallback();
//     ResetCallback();
//     ResetGraph(0);
//     SetDispMask(0);
//     SetGraphDebug(0);

//     // SpuInit();
//     // SpuCommonAttr commonAttr;
//     // commonAttr.mask = (SPU_COMMON_MVOLL | SPU_COMMON_MVOLR);
//     // commonAttr.mvol.left = 0x3FFF;
//     // commonAttr.mvol.right = 0x3FFF;
//     // SpuSetCommonAttr(&commonAttr);

//     SetVideoMode(MODE_NTSC);

//     _screenWidth = 320;
//     _screenHeight = 224;

//     SetDefDispEnv(&disp[0], 0, 0, _screenWidth, _screenHeight);
//     SetDefDispEnv(&disp[1], 0, _screenHeight, _screenWidth, _screenHeight);
//     SetDefDrawEnv(&draw[0], 0, _screenHeight, _screenWidth, _screenHeight);
//     SetDefDrawEnv(&draw[1], 0, 0, _screenWidth, _screenHeight);

//     setRGB0(&draw[0], 0, 255, 0);
//     setRGB0(&draw[1], 255, 0, 0);
//     draw[0].isbg = 1;
//     draw[1].isbg = 1;

//     PutDispEnv(&disp[0]);
//     PutDrawEnv(&draw[0]);
// }

// int main(int argc, char* argv[]) {
//     PsyX_Initialise("Test1", 800, 600, 0);

//     _init();
//     while (1) {
//         POLY_F3* pf3;
//         ClearOTagR((u_long*)ot[db], OT_LEN);
//         _primitive = _primitiveBuffer[db];

//         pf3 = (POLY_F3*)_primitive;
//         setPolyF3(pf3);
//         setRGB0(pf3, 255, 255, 0);
//         setXY3(pf3, 128, 0, 160, 100, 32, 100);
//         addPrim(ot[db] + (OT_LEN - 1), pf3);

//         DrawOTag((u_long*)(ot[db] + (OT_LEN - 1)));
//         DrawSync(0);
//         VSync(0);

//         db = !db;
//         PutDispEnv(&disp[db]);
//         PutDrawEnv(&draw[db]);
//         SetDispMask(1);

//         // memcpy(&prevPadbuff, padbuff, sizeof(padbuff));
//         memset(_primitiveBuffer[0], 0, sizeof(_primitiveBuffer[0]));
//         memset(_primitiveBuffer[1], 0, sizeof(_primitiveBuffer[1]));

//         PsyX_EndScene();
//     }

//     PsyX_Shutdown();
// }

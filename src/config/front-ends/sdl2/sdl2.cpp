#include "sdl2.h"
#include <rsys/adb.h>
#include <osevent/osevent.h>
#include <rsys/scrap.h>
#include <rsys/keyboard.h>
#include <OSEvent.h>
#include <ToolboxEvent.h>
#include <SegmentLdr.h>

#include "keycode_map.h"

#include <SDL.h>

using namespace Executor;

namespace
{
SDL_Window *sdlWindow;
/*SDL_Renderer *sdlRenderer;
SDL_Texture *sdlTexture;*/
SDL_Surface *sdlSurface;
}

bool SDL2VideoDriver::init()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }
    return true;
}

bool SDL2VideoDriver::setMode(int width, int height, int bpp, bool grayscale_p)
{
    printf("set_mode: %d %d %d", width, height, bpp);
    if(framebuffer_)
        delete[] framebuffer_;

    if(width)
        width_ = width;
    if(height)
        height_ = height;
    if(bpp)
        bpp_ = bpp;
    rowBytes_ = width_ * bpp_ / 8;

    maxBpp_ = 8; //32;

    sdlWindow = SDL_CreateWindow("Window",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 width_, height_,
                                 0);
    //SDL_WINDOW_FULLSCREEN_DESKTOP);

    /*sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);

    SDL_RenderSetLogicalSize(sdlRenderer, vdriver_width, vdriver_height);
    SDL_SetRenderDrawColor(sdlRenderer, 128, 128, 128, 255);
    SDL_RenderClear(sdlRenderer);
    SDL_RenderPresent(sdlRenderer);*/

    uint32_t pixelFormat;

    switch(bpp_)
    {
        case 1:
            pixelFormat = SDL_PIXELFORMAT_INDEX1LSB;
            break;
        case 4:
            pixelFormat = SDL_PIXELFORMAT_INDEX4LSB;
            break;
        case 8:
            pixelFormat = SDL_PIXELFORMAT_INDEX8;
            break;
        case 16:
            pixelFormat = SDL_PIXELFORMAT_RGB555;
            break;
        case 32:
            pixelFormat = SDL_PIXELFORMAT_BGRX8888;
            break;
        default:
            std::abort();
    }

    framebuffer_ = new uint8_t[width_ * height_ * 4];

#if 1
    uint32_t rmask, gmask, bmask, amask;
    int sdlBpp;
    SDL_PixelFormatEnumToMasks(pixelFormat, &sdlBpp, &rmask, &gmask, &bmask, &amask);

    sdlSurface = SDL_CreateRGBSurfaceFrom(
        framebuffer_,
        width_, height_,
        sdlBpp,
        rowBytes_,
        rmask, gmask, bmask, amask);
#else
    sdlSurface = SDL_CreateRGBSurfaceWithFormatFrom(
        framebuffer_,
        width_, height_,
        bpp_,
        rowBytes_,
        pixelFormat);
#endif

    return true;
}

void SDL2VideoDriver::setColors(int first_color, int num_colors, const ColorSpec *colors)
{
    SDL_Color *sdlColors = (SDL_Color *)alloca(sizeof(SDL_Color) * num_colors);
    for(int i = 0; i < num_colors; i++)
    {
        sdlColors[i].a = 255;
        sdlColors[i].r = colors[i].rgb.red >> 8;
        sdlColors[i].g = colors[i].rgb.green >> 8;
        sdlColors[i].b = colors[i].rgb.blue >> 8;
    }

    SDL_SetPaletteColors(sdlSurface->format->palette, sdlColors, first_color, num_colors);
}

void SDL2VideoDriver::updateScreenRects(int num_rects, const vdriver_rect_t *r,
                                        bool cursor_p)
{
    /*SDL_UpdateTexture(sdlTexture, nullptr, vdriver_fbuf, vdriver_row_bytes);
    SDL_RenderCopy(sdlRenderer, sdlTexture, nullptr, nullptr);
    SDL_RenderPresent(sdlRenderer);*/
    SDL_BlitSurface(sdlSurface, nullptr, SDL_GetWindowSurface(sdlWindow), nullptr);
    SDL_UpdateWindowSurface(sdlWindow);
}

static bool ConfirmQuit()
{
    const SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "Cancel" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Force Quit" },
    };
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        sdlWindow, /* .window */
        "Quit", /* .title */
        "Do you want to quit Executor?", /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        nullptr /* .colorScheme */
    };
    int buttonid;
    SDL_ShowMessageBox(&messageboxdata, &buttonid);
    return buttonid == 1;
}

static bool isModifier(unsigned char virt, uint16_t *modstore)
{
    /* Note: shift and control can be cleared if right* and left* are pressed */
    switch(virt)
    {
        case MKV_LEFTSHIFT:
        case MKV_RIGHTSHIFT:
            *modstore = shiftKey;
            break;
        case MKV_CAPS:
            *modstore = alphaLock;
            break;
        case MKV_LEFTCNTL:
        case MKV_RIGHTCNTL:
            *modstore = ControlKey;
            break;
        case MKV_CLOVER:
            *modstore = cmdKey;
            break;
        case MKV_LEFTOPTION:
        case MKV_RIGHTOPTION:
            *modstore = optionKey;
            break;
        default:
            *modstore = 0;
            return false;
    }
    return true;
}

void SDL2VideoDriver::pumpEvents()
{
    SDL_Event event;
    static uint16_t keymod = btnState;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
                LM(MouseLocation).h = event.motion.x;
                LM(MouseLocation).v = event.motion.y;

                adb_apeiron_hack(false);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                bool down_p;
                int32_t when;
                Point where;

                down_p = (event.button.state == SDL_PRESSED);
                if(down_p)
                    keymod &= ~btnState;
                else
                    keymod |= btnState;
                when = TickCount();
                where.h = event.button.x;
                where.v = event.button.y;
                ROMlib_PPostEvent(down_p ? mouseDown : mouseUp,
                                  0, (GUEST<EvQElPtr> *)0, when, where,
                                  keymod);
                adb_apeiron_hack(false);
            }
            break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
                bool down_p;
                unsigned char mkvkey;
                uint16_t mod;
                LONGINT keywhat;
                int32_t when;
                Point where;

                init_sdlk_to_mkv();
                down_p = (event.key.state == SDL_PRESSED);

                /*if(use_scan_codes)
                    mkvkey = ibm_virt_to_mac_virt[event.key.keysym.scancode];
                else*/
                {
                    auto p = sdlk_to_mkv.find(event.key.keysym.sym);
                    if(p == sdlk_to_mkv.end())
                        mkvkey = NOTAKEY;
                    else
                        mkvkey = p->second;
                }
                mkvkey = ROMlib_right_to_left_key_map(mkvkey);
                if(isModifier(mkvkey, &mod))
                {
                    if(down_p)
                        keymod |= mod;
                    else
                        keymod &= ~mod;
                }
                when = TickCount();
                where.h = LM(MouseLocation).h;
                where.v = LM(MouseLocation).v;
                keywhat = ROMlib_xlate(mkvkey, keymod, down_p);
                post_keytrans_key_events(down_p ? keyDown : keyUp,
                                         keywhat, when, where,
                                         keymod, mkvkey);
            }
            break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                //if(!we_lost_clipboard())
                sendresumeevent(false);
                //else
                //{
                //    ZeroScrap();
                //    sendresumeevent(true);
                //}
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                sendsuspendevent();
                break;
            case SDL_QUIT:
                if(ConfirmQuit())
                    ExitToShell();
                break;
        }
    }
}

/* This is really inefficient.  We should hash the cursors */
void SDL2VideoDriver::setCursor(char *cursor_data,
                               unsigned short cursor_mask[16],
                               int hotspot_x, int hotspot_y)
{
    SDL_Cursor *old_cursor, *new_cursor;

    old_cursor = SDL_GetCursor();
    new_cursor = SDL_CreateCursor((unsigned char *)cursor_data,
                                  (unsigned char *)cursor_mask,
                                  16, 16, hotspot_x, hotspot_y);
    if(new_cursor != nullptr)
    {
        SDL_SetCursor(new_cursor);
        SDL_FreeCursor(old_cursor);
    }
}

bool SDL2VideoDriver::setCursorVisible(bool show_p)
{
    return (SDL_ShowCursor(show_p));
}

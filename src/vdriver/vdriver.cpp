#include <vdriver/vdriver.h>
#include <error/error.h>

using namespace Executor;

VideoDriver *Executor::vdriver = nullptr;

VideoDriver::~VideoDriver()
{
}

bool VideoDriver::parseCommandLine(int& argc, char *argv[])
{
    return true;
}

bool VideoDriver::init()
{
    return true;
}

void VideoDriver::shutdown()
{
}

void VideoDriver::updateScreen(int top, int left, int bottom, int right, bool cursor_p)
{
    if(top < 0)
        top = 0;
    if(left < 0)
        left = 0;

    if(bottom > height())
        bottom = height();
    if(right > width())
        right = width();

    vdriver_rect_t r = {top, left, bottom, right};
    updateScreenRects(1, &r, cursor_p);
}

void VideoDriver::updateScreenRects(int num_rects, const vdriver_rect_t *r, bool cursor_p)
{
}

bool VideoDriver::isAcceptableMode(int width, int height, int bpp, bool grayscale_p, bool exact_match_p)
{
    if(bpp == 1 || bpp == 4 || bpp == 8 || bpp == 16 || bpp == 32)
        return true;
    else
        return false;
}

void VideoDriver::flushDisplay()
{
}

void VideoDriver::registerOptions()
{
}

void VideoDriver::getColors(int first_color, int num_colors,
                                struct ColorSpec *color_array)
{
    if(!isFixedCLUT())
        gui_fatal("`!vdriver_fixed_clut_p' and `VideoDriver::getColors ()' called");
    else
        gui_fatal("`VideoDriver::getColors ()' unimplemented");
}

vdriver_accel_result_t VideoDriver::accelFillRect(int top, int left, int bottom, int right, uint32_t color)
{
    return VDRIVER_ACCEL_NO_UPDATE;
}

vdriver_accel_result_t VideoDriver::accelScrollRect(int top, int left, int bottom, int right, int dx, int dy)
{
    return VDRIVER_ACCEL_NO_UPDATE;
}

void VideoDriver::accelWait()
{
}

void VideoDriver::putScrap(OSType type, LONGINT length, char *p, int scrap_cnt)
{
}

LONGINT VideoDriver::getScrap(OSType type, Handle h)
{
    return 0;
}

void VideoDriver::weOwnScrap()
{
}

void VideoDriver::setTitle(const std::string& name)
{
}

std::string VideoDriver::getTitle()
{
    return "";
}

void VideoDriver::setUseScancodes(bool val)
{
}

void VideoDriver::setCursor(char *cursor_data, uint16_t cursor_mask[16], int hotspot_x, int hotspot_y)
{
}

bool VideoDriver::setCursorVisible(bool show_p)
{
    return true;
}

bool VideoDriver::hideCursorIfIntersects(int top, int left, int bottom, int right)
{
    return setCursorVisible(false);
}

void VideoDriver::beepAtUser()
{
}

void VideoDriver::pumpEvents()
{
}

void VideoDriver::setRootlessRegion(RgnHandle rgn)
{
}
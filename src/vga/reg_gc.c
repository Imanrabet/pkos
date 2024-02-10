#include "reg_gc.h"
#include "../common/types.h"


u32 get_reg_gc(u32 index) {
    return get_reg(VGA_GRAPHICS_ADDR, VGA_GRAPHICS_DATA, index);
}

void set_graphics_controller(struct GraphicsController config) {

}

void get_graphics_controller(struct GraphicsController config) {
    config.regSetReset          = get_reg_gc(VGA_GRAPHICS_REG_SR);
    config.regEnableSetReset    = get_reg_gc(VGA_GRAPHICS_REG_ENABLE_SR);
    config.regColorCompare      = get_reg_gc(VGA_GRAPHICS_REG_COLORCOMPARE);
    config.regDataRotate        = get_reg_gc(VGA_GRAPHICS_REG_DATAROTATE);
    config.regReadMap           = get_reg_gc(VGA_GRAPHICS_REG_READMAP);
    config.regGraphicsMode      = get_reg_gc(VGA_GRAPHICS_REG_GRAPHICSMODE);
    config.regMisc              = get_reg_gc(VGA_GRAPHICS_REG_MISC);
    config.regColorDontCare     = get_reg_gc(VGA_GRAPHICS_REG_COLORDONTCARE);
    config.regBitMask           = get_reg_gc(VGA_GRAPHICS_REG_BITMASK);
}

void print_gc(struct GraphicsController gc) {
    char buffer[8];
    print("Set/Reset: 0b");
	println(itoab(gc.regSetReset, buffer));
}

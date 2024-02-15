#include "vga.h"
#include "../kernel/kernel.h"
#include "../memory/memory.h"
#include "../screen/screen.h"
#include "reg_ac.h"
#include "reg_crtc.h"
#include "reg_ext.h"
#include "reg_gc.h"
#include "reg_seq.h"
#include "vga_config.h"

#define COLOR_BLACK 0x0
#define COLOR_GREEN 0x2
#define COLOR_PURPLE 0xf


unsigned int vga_mode_var = 0;

u32 get_reg(u32 address, u32 data, u32 index) {
    /**
        Get the value of a register using ioports.
    */
    // Save the current value of the address register
    u32 saved_addr_reg = ioport_in(address);
    // Set the address register to indicate where we will read
	ioport_out(address, index);
    // Get the data from that address
	u32 result = ioport_in(data);
    // Restore the original value of the address register
	ioport_out(address, saved_addr_reg);
    // Return the result
	return result;
}

u32 set_reg(u32 address, u32 data, u32 index, u32 value) {
    /**
        Set the value of a register using ioports.
    */
    // Save the current value of the address register
	unsigned int saved_addr_reg = ioport_in(address);
    // Set the address to which we are writing
	ioport_out(address, index);
    // Set the value at that address by writing to the data port
	ioport_out(data, value);
    // Restore the original value of the address register
	ioport_out(address, saved_addr_reg);
}



void vga_info() {
	struct VGA vga;
	get_vga(vga);
	print_vga(vga);
}

void vga_enter() {
	if (vga_mode_var == 1) return;
	vga_mode_var = 1;
    println("Attempting to switch modes...");

	// Save video memory somewhere else
	// 0xb8000 to 0xbffff (32K)
	memcpy(0x0010b8000, 0xb8000, COLS*ROWS*2);

	// struct AttributeController ac;
	// get_ac(ac);
	// set_ac(ac);

	// struct ExternalGeneral ext;
	// get_ext(ext);
	// set_ext(ext);

	// u8 ioAddressSelect = ext.regMisc & 0b1;
	// struct CathodeRayTubeController crtc;
	// get_crtc(crtc, ioAddressSelect);
	// set_crtc(crtc, ioAddressSelect);

	// struct Sequencer seq;
	// get_seq(seq);
	// // Synchronous stop sequencer
	// set_reg_seq(VGA_SEQ_REG_RESET, 0x1);
	// // Turn on the Screen Disable bit
	// set_reg_seq(VGA_SEQ_REG_CLOCKING, seq.regClocking | (1 << 5));
	// // Turn off the sync reset bit
	// set_reg_seq(VGA_SEQ_REG_RESET, 0x3);

	struct GraphicsController gc;
	get_gc(gc);
	u8 newValue = gc.regMisc;
	// Set alphanumeric disable to 1 (turn off text mode)
	newValue |= 1;
	// Select mem map 0xa0000 (value 00)
	newValue &= 0b11110011;
	set_reg_gc(VGA_GC_REG_MISC, newValue);

	memset(0xa0000, 0, 400);

	vga_plot_pixel(0,0, COLOR_GREEN);
	vga_plot_pixel(2,2, COLOR_GREEN);
	// vga_clear_screen();
	// vga_plot_pixel(0, 0, COLOR_GREEN);
	// vga_plot_pixel(1, 0, COLOR_PURPLE);
    // vga_plot_pixel(1,1,COLOR_GREEN);
    // vga_plot_pixel(2,1,COLOR_GREEN);
    // vga_plot_pixel(2,2,COLOR_GREEN);
	// // draw rectangle
	// draw_rectangle(150, 10, 100, 50);
	// // draw some faces
	// draw_happy_face(10,10);
	// draw_happy_face(100,100);
	// draw_happy_face(300,150);
	// // bounds
	// vga_plot_pixel(0, 0, 15);
	// vga_plot_pixel(319, 199, COLOR_PURPLE);
	// // see some colors
	// for (int i = 0; i < 15; i++) {
	// 	for (int j = 0; j < 100; j++) {
	// 		vga_plot_pixel(i, 50+j, i);
	// 	}
	// }
	
}

void vga_exit() {
	if (vga_mode_var == 0) return;

	// // Go back to text mode
	// struct Sequencer seq;
	// get_seq(seq);
	// // Synchronous stop sequencer
	// set_reg_seq(VGA_SEQ_REG_RESET, 0x1);
	// // Turn off the Screen Disable bit
	// set_reg_seq(VGA_SEQ_REG_CLOCKING, seq.regClocking & (0b11011111));
	// // Turn off the sync reset bit
	// set_reg_seq(VGA_SEQ_REG_RESET, 0x3);

	struct GraphicsController gc;
	u8 newValue = gc.regMisc;
	// Set alphanumeric disable to 0 (turn on text mode)
	newValue &= 0;
	// Set RAM enable to 1
	newValue |= 0b10;
	// Select mem map 0xb8000
	newValue |= 0b1100;
	get_gc(gc);
	set_reg_gc(VGA_GC_REG_MISC, newValue);

	// struct AttributeController ac;
	// get_ac(ac);
	// set_ac(ac);

	// struct ExternalGeneral ext;
	// get_ext(ext);
	// set_ext(ext);

	// struct GraphicsController gc;
	// get_gc(gc);
	// gc.regMisc = 0xE;
	// set_gc(gc);

	// u8 ioAddressSelect = ext.regMisc & 0b1;
	// struct CathodeRayTubeController crtc;
	// get_crtc(crtc, ioAddressSelect);
	// set_crtc(crtc, ioAddressSelect);

	// struct Sequencer seq;
	// get_seq(seq);
	// set_seq(seq);

	// Restore text-mode video memory
	memcpy(0xb8000, 0x0010b8000, COLS*ROWS*2);

	vga_mode_var = 0;

	print_prompt();
}

void draw_rectangle(int x, int y, int width, int height) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			vga_plot_pixel(x+i, y+j, COLOR_GREEN);
		}
	}
}

void draw_happy_face(int x, int y) {
	// eye
	vga_plot_pixel(x,y,COLOR_PURPLE);
	// eye
	vga_plot_pixel(x+10,y,COLOR_PURPLE);
	// mouth
	vga_plot_pixel(x,	y+8,COLOR_PURPLE);
	vga_plot_pixel(x+1,	y+9,COLOR_PURPLE);
	vga_plot_pixel(x+2,	y+10,COLOR_PURPLE);
	vga_plot_pixel(x+3,	y+10,COLOR_PURPLE);
	vga_plot_pixel(x+4,	y+10,COLOR_PURPLE);
	vga_plot_pixel(x+5,	y+10,COLOR_PURPLE);
	vga_plot_pixel(x+6,	y+10,COLOR_PURPLE);
	vga_plot_pixel(x+7,	y+10,COLOR_PURPLE);
	vga_plot_pixel(x+8,	y+10,COLOR_PURPLE);
	vga_plot_pixel(x+9,	y+9,COLOR_PURPLE);
	vga_plot_pixel(x+10,y+8,COLOR_PURPLE);
}

void vga_clear_screen() {
    // Note: "clear_screen" name conflicted with something in screen.h
    // Now I see why namespacing is a thing
    for (int i = 0; i < 320; i++) {
        for (int j = 0; j < 200; j++) {
            vga_plot_pixel(i,j,COLOR_BLACK);
        }
    }
}

void vga_plot_pixel(int x, int y, unsigned short color) {
    unsigned short offset = x + 320 * y;
	unsigned char *PLANE0 = (unsigned char*) PLANE0_ADDRESS;
    PLANE0[offset] = color;
}

#include "early_console.hh"

#include <kernel/k_debug.hh>

namespace Kernel::EarlyConsole {

static constexpr u16 TAB_WIDTH        = 4;
static constexpr u16 CHARACTER_WIDTH  = 8;
static constexpr u16 CHARACTER_HEIGHT = 16;

static u16 row    = 0;
static u16 column = 0;

static u32* framebuffer        = nullptr;
static u16  framebuffer_width  = 0;
static u16  framebuffer_height = 0;

static u8 font[] = {
	0b00000000, // --------
	0b00000000, // --------
	0b00011000, // ---##---
	0b00111100, // --####--
	0b00111100, // --####--
	0b00111100, // --####--
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00000000, // --------
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b00100100, // --#--#--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b01101100, // -##-##--
	0b01101100, // -##-##--
	0b11111110, // #######-
	0b01101100, // -##-##--
	0b01101100, // -##-##--
	0b01101100, // -##-##--
	0b11111110, // #######-
	0b01101100, // -##-##--
	0b01101100, // -##-##--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00011000, // ---##---
	0b00011000, // ---##---
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b11000010, // ##----#-
	0b11000000, // ##------
	0b01111100, // -#####--
	0b00000110, // -----##-
	0b00000110, // -----##-
	0b10000110, // #----##-
	0b11000110, // ##---##-
	0b01111100, // -#####--
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b11000010, // ##----#-
	0b11000110, // ##---##-
	0b00001100, // ----##--
	0b00011000, // ---##---
	0b00110000, // --##----
	0b01100000, // -##-----
	0b11000110, // ##---##-
	0b10000110, // #----##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --###---
	0b00000000, // -##-##--
	0b00000000, // -##-##--
	0b00000000, // --###---
	0b00000000, // -###-##-
	0b00000000, // ##-###--
	0b00000000, // ##--##--
	0b00000000, // ##--##--
	0b00000000, // -###-##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --##----
	0b00000000, // --##----
	0b00000000, // --##----
	0b00000000, // -##-----
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00001100, // ----##--
	0b00011000, // ---##---
	0b00110000, // --##----
	0b00110000, // --##----
	0b00110000, // --##----
	0b00110000, // --##----
	0b00011000, // ---##---
	0b00001100, // ----##--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00110000, // --##----
	0b00011000, // ---##---
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00011000, // ---##---
	0b00110000, // --##----
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b01100110, // -##--##-
	0b00111100, // --####--
	0b11111111, // ########
	0b00111100, // --####--
	0b01100110, // -##--##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b01111110, // -######-
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00110000, // --##----
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b11111110, // #######-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000010, // ------#-
	0b00000110, // -----##-
	0b00001100, // ----##--
	0b00011000, // ---##---
	0b00110000, // --##----
	0b01100000, // -##-----
	0b11000000, // ##------
	0b10000000, // #-------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00111000, // --###---
	0b01101100, // -##-##--
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11010110, // ##-#-##-
	0b11010110, // ##-#-##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b01101100, // -##-##--
	0b00111000, // --###---
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00011000, // ---##---
	0b00111000, // --###---
	0b01111000, // -####---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b01111110, // -######-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b00000110, // -----##-
	0b00001100, // ----##--
	0b00011000, // ---##---
	0b00110000, // --##----
	0b01100000, // -##-----
	0b11000000, // ##------
	0b11000110, // ##---##-
	0b11111110, // #######-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b00000110, // -----##-
	0b00000110, // -----##-
	0b00111100, // --####--
	0b00000110, // -----##-
	0b00000110, // -----##-
	0b00000110, // -----##-
	0b11000110, // ##---##-
	0b01111100, // -#####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00001100, // ----##--
	0b00011100, // ---###--
	0b00111100, // --####--
	0b01101100, // -##-##--
	0b11001100, // ##--##--
	0b11111110, // #######-
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00011110, // ---####-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11111110, // #######-
	0b11000000, // ##------
	0b11000000, // ##------
	0b11000000, // ##------
	0b11111100, // ######--
	0b00000110, // -----##-
	0b00000110, // -----##-
	0b00000110, // -----##-
	0b11000110, // ##---##-
	0b01111100, // -#####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00111000, // --###---
	0b01100000, // -##-----
	0b11000000, // ##------
	0b11000000, // ##------
	0b11111100, // ######--
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b01111100, // -#####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b11111110, // #######-
	0b11000110, // ##---##-
	0b00000110, // -----##-
	0b00001100, // ----##--
	0b00011000, // ---##---
	0b00110000, // --##----
	0b00110000, // --##----
	0b00110000, // --##----
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b01111100, // -#####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b01111110, // -######-
	0b00000110, // -----##-
	0b00000110, // -----##-
	0b00000110, // -----##-
	0b00001100, // ----##--
	0b01111000, // -####---
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00110000, // --##----
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000110, // -----##-
	0b00001100, // ----##--
	0b00011000, // ---##---
	0b00110000, // --##----
	0b01100000, // -##-----
	0b00110000, // --##----
	0b00011000, // ---##---
	0b00001100, // ----##--
	0b00000110, // -----##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b01111110, // -######-
	0b00000000, // --------
	0b00000000, // --------
	0b01111110, // -######-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b01100000, // -##-----
	0b00110000, // --##----
	0b00011000, // ---##---
	0b00001100, // ----##--
	0b00000110, // -----##-
	0b00001100, // ----##--
	0b00011000, // ---##---
	0b00110000, // --##----
	0b01100000, // -##-----
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b00001100, // ----##--
	0b00011000, // ---##---
	0b00010000, // ---##---
	0b00011000, // ---##---
	0b00000000, // --------
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11011110, // ##-####-
	0b11011110, // ##-####-
	0b11011110, // ##-####-
	0b11011100, // ##-###--
	0b11000000, // ##------
	0b01111100, // -#####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00010000, // ---#----
	0b00111000, // --###---
	0b01101100, // -##-##--
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11111110, // #######-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11111100, // ######--
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01111100, // -#####--
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b11111100, // ######--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00111100, // --####--
	0b01100110, // -##--##-
	0b11000010, // ##----#-
	0b11000000, // ##------
	0b11000000, // ##------
	0b11000000, // ##------
	0b11000000, // ##------
	0b11000010, // ##----#-
	0b01100110, // -##--##-
	0b00111100, // --####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11111000, // #####---
	0b01101100, // -##-##--
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01101100, // -##-##--
	0b11111000, // #####---
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11111110, // #######-
	0b01100110, // -##--##-
	0b01100010, // -##---#-
	0b01101000, // -##-#---
	0b01111000, // -####---
	0b01101000, // -##-#---
	0b01100000, // -##-----
	0b01100010, // -##---#-
	0b01100110, // -##--##-
	0b11111110, // #######-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11111110, // #######-
	0b01100110, // -##--##-
	0b01100010, // -##---#-
	0b01101000, // -##-#---
	0b01111000, // -####---
	0b01101000, // -##-#---
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b11110000, // ####----
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00111100, // --####--
	0b01100110, // -##--##-
	0b11000010, // ##----#-
	0b11000000, // ##------
	0b11000000, // ##------
	0b11011110, // ##-####-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b01100110, // -##--##-
	0b00111010, // --###-#-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11111110, // #######-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00111100, // --####--
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00111100, // --####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00011110, // ---####-
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b01111000, // -####---
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11100110, // ###--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01101100, // -##-##--
	0b01111000, // -####---
	0b01111000, // -####---
	0b01101100, // -##-##--
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b11100110, // ###--##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11110000, // ####----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b01100010, // -##---#-
	0b01100110, // -##--##-
	0b11111110, // #######-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11000110, // ##---##-
	0b11101110, // ###-###-
	0b11111110, // #######-
	0b11111110, // #######-
	0b11010110, // ##-#-##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11000110, // ##---##-
	0b11100110, // ###--##-
	0b11110110, // ####-##-
	0b11011110, // ##-####-
	0b11001110, // ##--###-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b01111100, // -#####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11111100, // ######--
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01111100, // -#####--
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b11110000, // ####----
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11010110, // ##-#-##-
	0b11011110, // ##-####-
	0b01111100, // -#####--
	0b00001100, // ----##--
	0b00001110, // ----###-
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11111100, // ######--
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01111100, // -#####--
	0b01101100, // -##-##--
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b11100110, // ###--##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b01100000, // -##-----
	0b00111000, // --###---
	0b00001100, // ----##--
	0b00000110, // -----##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b01111100, // -#####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b01111110, // -######-
	0b01111110, // -######-
	0b01011010, // -#-##-#-
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00111100, // --####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b01111100, // -#####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b01101100, // -##-##--
	0b00111000, // --###---
	0b00010000, // ---#----
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11010110, // ##-#-##-
	0b11010110, // ##-#-##-
	0b11010110, // ##-#-##-
	0b11111110, // #######-
	0b11101110, // ###-###-
	0b01101100, // -##-##--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b01101100, // -##-##--
	0b01101100, // -##-##--
	0b00111000, // --###---
	0b00111000, // --###---
	0b01101100, // -##-##--
	0b01101100, // -##-##--
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b00111100, // --####--
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00111100, // --####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11111110, // #######-
	0b11000110, // ##---##-
	0b10000110, // #----##-
	0b00001100, // ----##--
	0b00011000, // ---##---
	0b00110000, // --##----
	0b01100000, // -##-----
	0b11000010, // ##----#-
	0b11000110, // ##---##-
	0b11111110, // #######-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00111100, // --####--
	0b00110000, // --##----
	0b00110000, // --##----
	0b00110000, // --##----
	0b00110000, // --##----
	0b00110000, // --##----
	0b00110000, // --##----
	0b00110000, // --##----
	0b00110000, // --##----
	0b00111100, // --####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b10000000, // #-------
	0b11000000, // ##------
	0b11100000, // ###-----
	0b01110000, // -###----
	0b00111000, // --###---
	0b00011100, // ---###--
	0b00001110, // ----###-
	0b00000110, // -----##-
	0b00000010, // ------#-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00111100, // --####--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00111100, // --####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00010000, // ---#----
	0b00111000, // --###---
	0b01101100, // -##-##--
	0b11000110, // ##---##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b11111111, // ########
	0b00000000, // --------
	0b00000000, // --------

	0b01100000, // -##-----
	0b01100000, // -##-----
	0b00110000, // --##----
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b01111000, // -####---
	0b00001100, // ----##--
	0b01111100, // -#####--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b01110110, // -###-##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11100000, // ###-----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b01111000, // -####---
	0b01101100, // -##-##--
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01111100, // -#####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b11000000, // ##------
	0b11000000, // ##------
	0b11000000, // ##------
	0b11000110, // ##---##-
	0b01111100, // -#####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00011100, // ---###--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00111100, // --####--
	0b01101100, // -##-##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b01110110, // -###-##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b11111110, // #######-
	0b11000000, // ##------
	0b11000000, // ##------
	0b11000110, // ##---##-
	0b01111100, // -#####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00111000, // --###---
	0b01101100, // -##-##--
	0b01100100, // -##--#--
	0b01100000, // -##-----
	0b11110000, // ####----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b11110000, // ####----
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b01110110, // -###-##-
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b01111100, // -#####--
	0b00001100, // ----##--
	0b11001100, // ##--##--
	0b01111000, // -####---
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11100000, // ###-----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b01101100, // -##-##--
	0b01110110, // -###-##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b11100110, // ###--##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00000000, // --------
	0b00111000, // --###---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00111100, // --####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000110, // -----##-
	0b00000110, // -----##-
	0b00000000, // --------
	0b00001110, // ----###-
	0b00000110, // -----##-
	0b00000110, // -----##-
	0b00000110, // -----##-
	0b00000110, // -----##-
	0b00000110, // -----##-
	0b00000110, // -----##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b00111100, // --####--
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b11100000, // ###-----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b01100110, // -##--##-
	0b01101100, // -##-##--
	0b01111000, // -####---
	0b01111000, // -####---
	0b01101100, // -##-##--
	0b01100110, // -##--##-
	0b11100110, // ###--##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00111000, // --###---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00111100, // --####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b11101100, // ###-##--
	0b11111110, // #######-
	0b11010110, // ##-#-##-
	0b11010110, // ##-#-##-
	0b11010110, // ##-#-##-
	0b11010110, // ##-#-##-
	0b11000110, // ##---##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b11011100, // ##-###--
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b01111100, // -#####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b11011100, // ##-###--
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01111100, // -#####--
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b11110000, // ####----
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b01110110, // -###-##-
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b01111100, // -#####--
	0b00001100, // ----##--
	0b00001100, // ----##--
	0b00011110, // ---####-
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b11011100, // ##-###--
	0b01100110, // -###-##-
	0b01100110, // -##--##-
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b01100000, // -##-----
	0b11110000, // ####----
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b01111100, // -#####--
	0b11000110, // ##---##-
	0b01100000, // -##-----
	0b00111000, // --###---
	0b00001100, // ----##--
	0b11000110, // ##---##-
	0b01111100, // -#####--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00010000, // ---#----
	0b00110000, // --##----
	0b00110000, // --##----
	0b11111100, // ######--
	0b00110000, // --##----
	0b00110000, // --##----
	0b00110000, // --##----
	0b00110110, // --##-##-
	0b00011100, // ---###--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b11001100, // ##--##--
	0b01110110, // -###-##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b01100110, // -##--##-
	0b00111100, // --####--
	0b00011000, // ---##---
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11010110, // ##-#-##-
	0b11010110, // ##-#-##-
	0b11010110, // ##-#-##-
	0b11111110, // #######-
	0b01101100, // -##-##--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b11000110, // ##---##-
	0b01101100, // -##-##--
	0b00111000, // --###---
	0b00111000, // --###---
	0b00111000, // --###---
	0b01101100, // -##-##--
	0b11000110, // ##---##-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b11000110, // ##---##-
	0b01111110, // -######-
	0b00000110, // -----##-
	0b00001100, // ----##--
	0b11111000, // #####---
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b11111110, // #######-
	0b11001100, // ##--##--
	0b00011000, // ---##---
	0b00110000, // --##----
	0b01100000, // -##-----
	0b11000110, // ##---##-
	0b11111110, // #######-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00001110, // ----###-
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b01110000, // -###----
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00001110, // ----###-
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00000000, // --------
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b01110000, // -###----
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00001110, // ----###-
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b00011000, // ---##---
	0b01110000, // -###----
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------

	0b00000000, // --------
	0b00000000, // --------
	0b01110110, // -###-##-
	0b11011100, // ##-###--
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
	0b00000000, // --------
};

void initialize_early_console(u32* fb_addr, u16 fb_width, u16 fb_height)
{
	framebuffer        = fb_addr;
	framebuffer_width  = fb_width;
	framebuffer_height = fb_height;
}

void put_char(char c)
{
	switch(c) {
		case ' ':
			column++;
			break;
		case '\t':
			column += TAB_WIDTH;
			break;
		case '\r':
			column = 0;
			break;
		case '\n':
			row++;
			column = 0;
			break;
		default:
			draw_char(c);
	}
}

void draw_char(char c)
{
	for(u8 i = 0; i < CHARACTER_HEIGHT; i++) {
		for(u8 j = 0; j < CHARACTER_WIDTH; j++) {
			u16 pixel_x = (CHARACTER_WIDTH * column) + j;
			u16 pixel_y = (CHARACTER_HEIGHT * row) + i;
			u8  character_pixel = font[((c - '!') * CHARACTER_HEIGHT) + i] & (0b10000000 >> j);

			framebuffer[pixel_y * framebuffer_width + pixel_x] = character_pixel ? 0xffffffff : 0x0;
		}
	}
	column++;
}

}

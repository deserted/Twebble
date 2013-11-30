#include "tweet_cell.h"
#include "consts.h"

void tweet_cell_for_tweet(char text[], char username[], const Layer *layer, GContext* ctx){

	//Painting to the graphic context

	GRect bounds = layer_get_bounds(layer);
	int text_height = bounds.size.h-(SPACING*3)-DEF_FONT;

	GRect frame1 = GRect(0, SPACING, WIDTH, text_height);
	GRect frame2 = GRect(0, SPACING*2+text_height, WIDTH, DEF_FONT);

	graphics_context_set_text_color(ctx, GColorBlack);
	graphics_draw_text(ctx, text,
         fonts_get_system_font(FONT_KEY_GOTHIC_18),
         frame1,
         GTextOverflowModeWordWrap ,
         GTextAlignmentLeft,
         NULL);
	graphics_draw_text(ctx, username,
         fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
         frame2,
         GTextOverflowModeTrailingEllipsis,
         GTextAlignmentRight,
         NULL);

}

int height_for_tweet_cell(char text[]){

	TextLayer *text_layer = text_layer_create(GRect(0, 0, WIDTH, MAX_HEIGHT));
	text_layer_set_text(text_layer, text);
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));

	GSize size = text_layer_get_content_size(text_layer);
	text_layer_destroy(text_layer);

	return (SPACING*3)+size.h+DEF_FONT;
}
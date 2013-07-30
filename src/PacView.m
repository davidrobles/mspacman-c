
#import "PacView.h"
#import "PacModel.h"
#import "Defs.h"

@implementation PacView

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		bgImage = [[NSImage alloc] initWithContentsOfFile:@"/Users/drobles/Desktop/maps/images/level1-a.png"];
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
	CGContextRef context = [[NSGraphicsContext currentContext] graphicsPort];	
	CGContextSaveGState(context);
	
	// Draw color background
	CGContextSetRGBFillColor(context, 0, 0, 0, 1);
	CGContextFillRect(context, NSMakeRect(0, 0, 100, 100));
	CGContextFillRect(context, dirtyRect);
	
	// Draw background image
	[bgImage drawInRect:NSMakeRect(0, 0, kWidth * kNodeSize, kHeight * kNodeSize) fromRect:NSZeroRect 
			  operation:NSCompositeSourceOver fraction:1.0];
	CGContextTranslateCTM(context, 0, kHeight * kNodeSize);
	CGContextScaleCTM(context, 1.0, -1.0);
//	CGContextDrawImage(context, NSMakeRect(0, 0, kWidth * kNodeSize, kHeight * kNodeSize), [bgImage CGImage]);
	
	// Draw nodes 
//	Node** the_nodes = get_game_state()->maze_state.maze->path_nodes;
//	int path_nodes_size = get_game_state()->maze_state.maze->path_nodes_size;
//	
//	for (int i = 0; i < path_nodes_size; i++)
//	{
//		CGContextBeginPath(context);
//
//		if (the_nodes[i]->adjSize == 1) {
//			CGContextSetRGBFillColor(context, 1, 0, 0, 1);			
//		} else if (the_nodes[i]->adjSize == 2) {
//			CGContextSetRGBFillColor(context, 0, 1, 0, 1);
//		} else if (the_nodes[i]->adjSize == 3) {
//			CGContextSetRGBFillColor(context, 0, 0, 1, 1);
//		} else if (the_nodes[i]->adjSize == 4) {
//			CGContextSetRGBFillColor(context, 1, 1, 1, 1);
//		}
//
//		CGContextSetRGBStrokeColor(context, 0, 0, 0, 1);
//		CGContextAddRect(context, CGRectMake(the_nodes[i]->x * kNodeSize, the_nodes[i]->y * kNodeSize,
//											 kNodeSize, kNodeSize));
//		CGContextClosePath(context);
//		CGContextDrawPath(context, kCGPathFillStroke);
//	}

	// Draw pills
	
	MazeState *the_maze_state = &get_game_state()->maze_state;
	struct Node** the_pill_nodes = the_maze_state->maze->pill_nodes;
	
	for (int i = 0; i < the_maze_state->maze->pill_nodes_size; i++)
	{
		if (the_maze_state->pills_set[i] == 1) {
			CGContextBeginPath(context);
			CGContextSetRGBFillColor(context, 1, 1, 1, 1);
			CGContextAddRect(context, CGRectMake((the_pill_nodes[i]->x * kNodeSize) - (kPillSize / 2),
												 (the_pill_nodes[i]->y * kNodeSize) - (kPillSize / 2),
												 kPillSize, kPillSize));
			CGContextClosePath(context);
			CGContextDrawPath(context, kCGPathFillStroke);
		}
	}

	// Draw power pills
	
	struct Node** the_power_nodes = the_maze_state->maze->power_nodes;
	
	for (int i = 0; i < the_maze_state->maze->power_pill_nodes_size; i++)
	{
		if (the_maze_state->powers_set[i] == 1) {
			CGContextBeginPath(context);
			CGContextSetRGBFillColor(context, 1, 1, 1, 1);
			CGContextAddRect(context, CGRectMake((the_power_nodes[i]->x * kNodeSize) - (POWER_PILL_SIZE / 2),
												 (the_power_nodes[i]->y * kNodeSize) - (POWER_PILL_SIZE / 2),
												 POWER_PILL_SIZE, POWER_PILL_SIZE));
			CGContextClosePath(context);
			CGContextDrawPath(context, kCGPathFillStroke);
		}
	}
	
	// Draw pacman
	
	CGContextBeginPath(context);
	Pacman *pacman = &get_game_state()->pacman;
	CGContextSetRGBFillColor(context, 1, 0.85, 0, 1);	
	CGContextAddRect(context, CGRectMake((pacman->current->x * kNodeSize) - (KPacmanSize / 2.0),
										 (pacman->current->y * kNodeSize) - (KPacmanSize / 2.0),
										 KPacmanSize, KPacmanSize));
	CGContextClosePath(context);
	CGContextDrawPath(context, kCGPathFillStroke);
	
	// Draw ghosts
	
	Ghost *ghosts = get_game_state()->ghosts;
	
	for (int i = 0; i < 4; i++)
	{
		CGContextBeginPath(context);
		Ghost *g = &ghosts[i];
		
		if (g->edible_time > 0) {
			CGContextSetRGBFillColor(context, kEdibleColor);
		} else if (g->type == BLINKY) {
			CGContextSetRGBFillColor(context, kBlinkyColor);
		} else if (g->type == PINKY) {
			CGContextSetRGBFillColor(context, kPinkyColor); // RED
		} else if (g->type == INKY) {
			CGContextSetRGBFillColor(context, kInkyColor); // RED			
		} else if (g->type == SUE) {
			CGContextSetRGBFillColor(context, kSueColor); // RED			
		}
					
		CGContextAddRect(context, CGRectMake((g->current->x * kNodeSize) - (GHOST_SIZE / 2.0),
											 (g->current->y * kNodeSize) - (GHOST_SIZE / 2.0),
											 GHOST_SIZE, GHOST_SIZE));
		CGContextClosePath(context);
		CGContextDrawPath(context, kCGPathFillStroke);
	}
	
	// Draw score
	CGContextTranslateCTM(context, 0, kHeight * kNodeSize);
	CGContextScaleCTM(context, 1.0, -1.0);
    CGContextSelectFont (context, "Helvetica", 1, kCGEncodingMacRoman);
    CGContextSetTextDrawingMode (context, kCGTextFillStroke);
    CGContextSetRGBFillColor(context, 1, 1, 1, 1);
	CGContextSetRGBStrokeColor(context, 1, 1, 1, 1);
	char score_str[40];
	sprintf(score_str, "Lives: %d    Score: %6d", get_game_state()->lives, get_game_state()->score);
    CGContextShowTextAtPoint(context, 150, 370, score_str, strlen(score_str)); 
	
//	if (get_goal_node() != NULL) {
//		// print goal node
//		CGContextBeginPath(context);
//		CGContextSetRGBFillColor(context, 0, 1, 1, 1);
//		CGContextSetRGBStrokeColor(context, 0, 0, 0, 1);
//		CGContextAddRect(context, CGRectMake((get_goal_node()->x * kNodeSize) - (GHOST_SIZE / 2.0),
//											 (get_goal_node()->y * kNodeSize) - (GHOST_SIZE / 2.0),
//											 GHOST_SIZE, GHOST_SIZE));
//		CGContextClosePath(context);
//		CGContextDrawPath(context, kCGPathFillStroke);		
//	}
	
	CGContextRestoreGState(context);
}

@end

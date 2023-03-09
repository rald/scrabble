interface DawgConst {
/*
	Description of the dawg data structure

The dawg is stored as an array of edges, each edge
stored in an int.  Each node is represented by the
index into this array of the first edge leaving
that node; subsequent edges leaving the same
node occupy successive locations in the array.
The last edge leaving a node is flagged by a
bit. Edges leading to terminal nodes (those which
are completed words) are marked with another bit.
The edges are labelled with character numbers from
1:26 (1=a) and occupy one 32 bit word each.  The
node with index 0 is the special node with no
edges leaving it.

Bits in format of edges in unsigned 32-bit word:

    | . . . : . . . | . . . : . . . | . . . : . . . | . . . : . . . |
     <---------------------------------------------> ! !   <------->
            Index of edge pointed to in array        L W     label

    L: this is the LAST edge out of this node
    W: this edge points to a node that completes a WORD
    
Be careful with the distinction between an edge and an edge position.
*/
	static final int  LAST =	0x80;
	static final int  WORD =	0x40;
	static final int  TERM =	0x40;
	static final int  CHAR =	0x1F;
	static final int  INDEX =	8;

	// flags to indicate how to interpret an integer
	static final int ISCHAR = 0;
	static final int ISCOUNT = 1;
	static final int ISMASK = 2;
	
	static int BLANK=27;
	static int BLANKBIT = 1<<5;
	
	// other useful constants
	static final int TILES_PER_MOVE = 7;	// max tiles in a move
	static final int BOARDSIZE = 15;
	static final int LEN=BOARDSIZE +2;	// 2 sentinels
}

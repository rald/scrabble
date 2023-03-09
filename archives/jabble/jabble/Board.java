import java.util.*;
import java.awt.*;

/**
 * Offscreen Scrabble board maintenance,
 * move generation.
**/
class Board implements DawgConst {

	// Indicate where the double/triple word/letter scores are:
	// '*' -> triple
	// '+' -> double
	static final String[] wordMultTemplate = {	// Font
	"*......*......*",
	".+...........+.",
	"..+.........+..",
	"...+.......+...",
	"....+.....+....",
	"...............",
	"...............",
	"*......+......*",
	"...............",
	"...............",
	"....+.....+....",
	"...+.......+...",
	"..+.........+..",
	".+...........+.",
	"*......*......*"
	};

	static final String[] letterMultTemplate = {
	"...+.......+...",
	".....*...*.....",
	"......+.+......",
	"+......+......+",
	"...............",
	".*...*...*...*.",
	"..+...+.+...+..",
	"...+.......+...",
	"..+...+.+...+..",
	".*...*...*...*.",
	"...............",
	"+......+......+",
	"......+.+......",
	".....*...*.....",
	"...+.......+..."
	};
	static final int HORIZONTAL = 0;
	static final int VERTICAL = 1;

	// wordMult[HORIZONTAL][y][x] = wordMult[VERTICAL][x][y] = wordMult at x,y
	// letterMult[HORIZONTAL][y][x] = letterMult[VERTICAL][x][y] = letterMult at x,y
	// They are arranged this way so that, e.g.
	// wordMult[HORIZONTAL][3] is an array of all the word multipliers in
	// horizontal row 3
	static int [][][] wordMult;
	static int [][][] letterMult;
	
	// static constructor
	static {
		wordMult = new int[2][LEN][LEN];
		letterMult = new int[2][LEN][LEN];
		multInit(wordMult, wordMultTemplate);
		multInit(letterMult, letterMultTemplate);
	}
	
	static void multInit(int[][][]mult, String[] template) {
		for(int y=0; y<BOARDSIZE; y++) {
			byte[] b = template[y].getBytes();
			for(int x=0; x<BOARDSIZE; x++) {
				int multiplier;
				switch(b[x]) {
				case '*':	multiplier=3; break;
				case '+':	multiplier = 2; break;
				default:	multiplier = 1; break;
				}
				mult[VERTICAL][x+1][y+1] = multiplier;
				mult[HORIZONTAL][y+1][x+1] = multiplier;
			}
		}
	}

	// fill[VERTICAL][x][y] = fill[HORIZONTAL][y][x] = letter at x,y
	int [][][] fill;

	// cross[VERTICAL][x][y] is the bitmask of letters we can place
	// at x,y while forming a vertical word.
	// It reflects any horizontal words which might be formed
	// perpendicular to the main word we are forming.
	
	// cross[HORIZONTAL][y][x] is the bitmask of letters we can place
	// at x,y while forming a horizontal word.
	int [][][] cross;

	// score[VERTICAL][x][y] is the points we get from a perpendicular word
	// at x,y while forming a vertical word.
	// score[HORIZONTAL][y][x] is the points we get from a perpendicular word
	// at x,y while forming a horizontal word.
	int [][][] score;

	// all the rows (horizontal and vertical)
	Row[][] row;
	static final int MIDDLEROW=8;
	
	Board() {
		fill = new int[2][LEN][LEN];
		cross = new int[2][LEN][LEN];
		score = new int[2][LEN][LEN];
		row = new Row[2][LEN];
		reset();

		for(int j=1; j<= BOARDSIZE; j++) {
			row[HORIZONTAL][j] = new Row(
				fill[HORIZONTAL][j], fill[HORIZONTAL][j-1],fill[HORIZONTAL][j+1],
				cross[HORIZONTAL][j], score[HORIZONTAL][j],
				wordMult[HORIZONTAL][j],letterMult[HORIZONTAL][j],
				true, j);
			row[VERTICAL][j] = new Row(
				fill[VERTICAL][j], fill[VERTICAL][j-1],fill[VERTICAL][j+1],
				cross[VERTICAL][j], score[VERTICAL][j],
				wordMult[VERTICAL][j],letterMult[VERTICAL][j],
				false, j);
		}
	}

	/** Reset for a new game **/
	void
	reset() {
		fillArray(fill, 0);
		fillArray(cross, -1);
		borderArray(cross, 0);
		fillArray(score, 0);
	}

	/** Set every element of 'a', (which must have dimensions 2,LEN,LEN) to 'val' **/
	static void fillArray(int[][][] a, int val) {
		for(int ori=0; ori<2; ori++)
		for(int x=0; x<LEN; x++)
		for(int y=0; y<LEN; y++) {
			a[ori][x][y] = val;
		}
	}
	
	/** Set the border elements of 'a',
	 * (which must have dimensions 2,LEN,LEN)
	 * to 'val'
	 **/
	static void borderArray(int[][][] a, int val) {
		for(int ori=0; ori<2; ori++)
		for(int j=0; j<LEN; j++) {
			a[ori][j][0] = a[ori][j][LEN-1] = a [ori][0][j] = a[ori][LEN-1][j] = val;
		}
	}
	
	/**
        * Return the best legal move with the
        * current board and 'r'.  Don't modify 'r'.
	**/
	public Move
	findMove(Rack r) {
		Move best = new Move();
		if (fill[0][MIDDLEROW][MIDDLEROW] == 0) {
			row[HORIZONTAL][MIDDLEROW].firstMove(best,r);
		} else {
			for (int j=1; j<= BOARDSIZE; j++) {
				row[HORIZONTAL][j].findMove(best,r);
				row[VERTICAL][j].findMove(best,r);
			}
		}
		return best;
	}

	/** If 'm' is legal, set m's score, return null.
	 * If 'm' is illegal, return the Problem
	 **/
	public Problem
	checkMove(Move m) {
		Problem p = m.check();
		if (p!= null)
			return p;

		int orientation= m.isHorizontal()? HORIZONTAL:VERTICAL;
		Row r = row[orientation][m.getRow()];
		return r.checkMove(m);
	}
	
	/** Place 'm' on the board **/
	public void
	place(Move move) {
		// Update fill
		for (Enumeration e = move.getTiles() ; e.hasMoreElements() ;) {
			Tile t = (Tile)e.nextElement();
 			Point p = t.getPoint();
			int ch = t.getLetter();
			Assert.assert(fill[HORIZONTAL][p.y][p.x] == 0);
			fill[HORIZONTAL][p.y][p.x] = fill[VERTICAL][p.x][p.y] = ch;
		}
		
		// Update score and cross
		for (Enumeration e = move.getTiles() ; e.hasMoreElements() ;) {
			Tile t = (Tile)e.nextElement();
			setMasks(t.getPoint());
		}
	}
	
	public void setMasks(Point p) {
		Mask mi;
		
		// left
		mi = row[HORIZONTAL][p.y].getMask(p.x, -1);
	 	score[VERTICAL][mi.col][p.y] = mi.score;
		cross[VERTICAL][mi.col][p.y] = mi.mask;
		
		// right
		mi = row[HORIZONTAL][p.y].getMask(p.x, 1);
	 	score[VERTICAL][mi.col][p.y] = mi.score;
		cross[VERTICAL][mi.col][p.y] = mi.mask;
		
		// up
		mi = row[VERTICAL][p.x].getMask(p.y, -1);
 		score[HORIZONTAL][mi.col][p.x] = mi.score;
		cross[HORIZONTAL][mi.col][p.x] = mi.mask;
		
		// down
		mi = row[VERTICAL][p.x].getMask(p.y, 1);
 		score[HORIZONTAL][mi.col][p.x] = mi.score;
		cross[HORIZONTAL][mi.col][p.x] = mi.mask;
	}
}

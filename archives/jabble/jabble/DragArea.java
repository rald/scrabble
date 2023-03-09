import java.awt.*;
import java.awt.event.*;
import java.util.*;

/**
 * Look after the rack area and the board.
 * See DragArea.txt	for more info.
 **/
/*
 * NB be careful throughout with the distinction between
 * _board_ positions 
 *	(1,1) is the top left square,
 *	(15,15) is the bottom right square
 * and _pixel_ positions
 * 	(0,0) is the top left pixel
 * 	(640,480) might be the bottom right pixel.
 * We have methods to translate between these coordinate systems.
*/
public class DragArea
extends Canvas
implements DawgConst, 
	MouseListener, MouseMotionListener, KeyListener {
	// constants
	static final int SIZE = 30;
	static final int MARGIN = 5;
	static final int NSQUARES = 15;
	
	// dimensions
	int	squaresize;
	// The dragarea contains NSQUARES*NSQUARES squares, each of size
	// squaresize.  Around the outside of the collection of squares
	// is a margin of size MARGIN, except at the top, where there
	// is a margin, a squaresize space for the rack, then another
	// margin.
	//
	// Board positions on the actual playing board have coordinates
	// between 1 and NSQUARES inclusive.
	//
	// The "rack" area includes board coordinates with y-coord of 0.
	
	// Colors
	static final Color OLDCOLOR= Color.yellow;	// old placed tiles
	static final Color NEWCOLOR= Color.green;	// new placed tiles
	static final Color RACKCOLOR = Color.red;	// new rack tiles
	
	static final Color normalC = Color.lightGray;
	static final Color tripleWordC = Color.red;
	static final Color doubleWordC = Color.pink;
	static final Color tripleLetterC = Color.blue;
	static final Color doubleLetterC = Color.green;
	
	static final Color textC = Color.black;
	
	// tiles
	private DragTile dragging;	// currently being dragged
	
	private Tile[][] square;	// Every square either has a tile, or null.
	
	public DragArea () {
		squaresize = SIZE;
		
		square = new Tile[NSQUARES+1][NSQUARES+1];
		for(int j=0; j<square.length; j++) {
			for(int k=0; k<square[j].length; k++) {
				square[j][k]=null;
			}
		}

		dragging = null;

		// event listeners
		addMouseListener(this);
		addMouseMotionListener(this);
		addKeyListener(this);
	}

	//--------------------------------------
	// add/remove/modify tiles
	//--------------------------------------
	/** Add new tile to rack. **/
	public void
	addToRack(int letter) {
		place(new Tile(freeRackPos(), letter, Tile.RACKTILE));
	}

	/** Add Enumeration<int> to rack. **/
	public void
	addToRack(Enumeration e) {
		while(e.hasMoreElements()) {
			Integer I = (Integer) e.nextElement();
			addToRack(I.intValue());
		}
	}

	/** Add new tile to board (someone else moved). **/
	public void
	addToBoard(Tile t) {
		place(t);
	}
	
	/** Add a move made by someone else **/
	public void
	addToBoard(Move m) {
		Enumeration e = m.getTiles();
		while(e.hasMoreElements()) {
			addToBoard((Tile) e.nextElement());
		}
	}
	
	/** Commit to the currently visible move. **/
	public void
	commit() {
		Graphics g = getGraphics();
		for(int x=1; x<=NSQUARES; x++) {
			for(int y=1; y<= NSQUARES; y++) {
				Tile t = square[x][y];
				if(t!=null) {
					t.commit();
					Point p = boardToPixel(t.getPoint());
					t.paint(g, p, squaresize);
				}
			}
		}
	}
	
	/** Indicate clearly what the problem is **/
	void displayProblem(Problem p) {
		System.out.println("problem: "+p);
		// todo
	}
	
	/** Return currently visible move **/
	public Move
	getMove() {
		Vector tiles = new Vector (TILES_PER_MOVE);
		for(int x=1; x<=NSQUARES; x++) {
			for(int y=1; y<= NSQUARES; y++) {
				Tile t = square[x][y];
				if (t != null && t.isMovable()) {
					tiles.addElement(t);
				}
			}
		}
		return new Move(tiles);
	}
	
	/** Rearrange our rack tiles to indicate possible move 'm' **/
	public void
	suggestMove(Move m) {
		// todo
	}
	
	//--------------------------------------
	// painting -- draw yourself
	//--------------------------------------
	public void
	paint(Graphics g) {
		for(int x=1; x<=NSQUARES; x++) {
			for(int y=0; y<= NSQUARES; y++) {
				Tile t = square[x][y];
				Point p = new Point(x,y);
				if(t==null) {
					paintSquare(g, new Point(x,y));
				} else {
					t.paint(g, boardToPixel(p), squaresize);
				}
			}
		}
	}

	//--------------------------------------
	// Dimension handling -- talk to layout manager
	//--------------------------------------
	public Dimension
	getPreferredSize(){
		return squareSizeToTotalSize(SIZE);
	}
	
	public Dimension
	getMinimumSize(){
		return squareSizeToTotalSize(SIZE);
	}
	
	/** Cope with being reshaped **/
	public void
	setBounds(Rectangle r) {
		// todo
	}
	
	//--------------------------------------
	// MouseMotionListener methods
	//--------------------------------------
	public void
	mouseDragged(MouseEvent e){
		if (dragging != null)
			dragging.dragTo(e.getPoint());
	}
	
	public void
	mouseMoved(MouseEvent e){
		// ignore
	}
	
	//--------------------------------------
	// KeyListener methods
	//--------------------------------------
	public void keyTyped(KeyEvent e) {
		// todo - use to set BLANK tiles
		switch(e.getKeyChar()){
		case 'c':	commit(); break;
		default:	getMove();
		}
	}

	public void keyPressed(KeyEvent e){
		// ignore
	}
	public void keyReleased(KeyEvent e) {
		// ignore
	}
	 
	//--------------------------------------
	// MouseListener methods
	//--------------------------------------
	public void
	mouseClicked(MouseEvent e){
		// ignore
	}
	
	public void
	mouseEntered(MouseEvent e){
		// ignore
	}
	
	public void
	mouseExited(MouseEvent e){
		// treat the same as dropping
		mouseReleased(e);
	}
	
	public void
	mousePressed(MouseEvent e){
		// We can't still be dragging when we get mouseDown,
		// because we would have dropped (somewhere) when
		// we got mouseUp.
		Assert.assert(dragging == null);
		
		Point pp = e.getPoint();
		Point bp = pixelToBoard(pp);
		
		if(!onBoard(bp))
			return;
	
		// have we clicked on a tile?
		Tile t = square[bp.x][bp.y];
		if( t != null && t.isMovable()) {
			Graphics g = getGraphics();
			Point orig =  boardToPixel(bp);
			unPlace(t);
			dragging = new DragTile(t, orig, pp, squaresize, g);
		}
	}
	
	public void
	mouseReleased(MouseEvent e){
		if (dragging != null) {
			dragging.erase();
			Tile t = new Tile(dragging);
			t.setPoint(dropPoint(e.getPoint()));
			place(t);
			dragging = null;
		}
	}

	//--------------------------------------
	// BoardToPixel methods
	//--------------------------------------
	/** Pixel position corresponding to given board position **/
	public Point
	boardToPixel(Point p) {
		int px, py;
		int	x = p.x;
		int y = p.y;
		
		// margin + extra squares
		px = MARGIN + (x-1)*squaresize;
		
		// margin, rack, margin, extra squares
		py = MARGIN + y*squaresize + ((y>0)?MARGIN:0);
		return new Point(px, py);
	}
	
	/** Convert pixel position 'pp' to a board position. **/
	public Point
	pixelToBoard(Point pp) {
		int bx, by;
		
		bx = (pp.x -MARGIN)/squaresize +1;
		if(pp.y >= 2*MARGIN+squaresize) {
			by = (pp.y -2*MARGIN - squaresize)/squaresize + 1;
		} else {
			by = 0;
		}
		return new Point (bx, by);
	}
	
	//--------------------------------------
	// PRIVATE routines
	//--------------------------------------

	/** Given a possible squaresize, return the resulting
	 * total size for the canvas
	**/
	private Dimension
	squareSizeToTotalSize (int size) {
		int x = 2 * MARGIN + NSQUARES * size;
		int y = 3 * MARGIN + (NSQUARES+1)*size;
		return new Dimension(x,y);
	}
	
	//--------------------------------------
	// Painting
	//--------------------------------------
	/** Paint Board square 'x,y' using 'g' **/
	private void
	paintSquare(Graphics g, Point bp) {
		int wordMult = Board.wordMult[1][bp.x][bp.y];
		int letterMult  = Board.letterMult[1][bp.x][bp.y];
		Color bg = normalC;
		switch(wordMult) {
		case 2:	bg = doubleWordC; break;
		case 3:	bg = tripleWordC; break;
		}
		switch(letterMult) {
		case 2:	bg = doubleLetterC; break;
		case 3:	bg = tripleLetterC; break;
		}
		Point p = boardToPixel(bp);
		g.setColor(bg);
		g.fillRect(p.x, p.y, squaresize, squaresize);
		
		g.setColor(textC);
		g.drawRoundRect(p.x, p.y, squaresize-2, squaresize -2, 2, 2);
	}
	
	private void place(Tile t) {
		Point bp = t.getPoint();
		Assert.assert(square[bp.x][bp.y] == null);
		square[bp.x][bp.y] = t;
		t.paint(getGraphics(), boardToPixel(bp), squaresize);
	}

	private void unPlace(Tile t) {
		Point bp = t.getPoint();
		Assert.assert(square[bp.x][bp.y] == t);
		square[bp.x][bp.y] = null;
		paintSquare(getGraphics(), bp);
	}
	
	/** Find closest free square to 'pixel'.
	 * 'pixel' is a pixel position.  We return a board position,
	 * which _should_ be close to pixel, and is free.
	 **/
	private Point
	dropPoint(Point pixel) {
		return closestFree(pixelToBoard(pixel));
	}

	/** Return closest 'free' square to board position 'bp' **/
	private Point
	closestFree(Point bp) {
		Point lu, ld, ru, rd;	// left/right, then up/down
		Point ul, dl, ur, dr;	// up/down, then left/right
		for(int dist=0; dist<8;dist++) {
			for(int delta=0; delta <= dist; delta++) {
				if(isFree(lu = new Point(bp.x -dist, bp.y + delta)))
					return lu;
				if(isFree( ld = new Point(bp.x -dist, bp.y - delta)))
					return ld;
				if(isFree( ru = new Point(bp.x + dist, bp.y + delta)))
					return ru;
				if(isFree( rd = new Point(bp.x + dist, bp.y - delta)))
					return rd;
				if(isFree( ul = new Point(bp.x -delta, bp.y + dist)))
					return ul;
				if(isFree( ur = new Point(bp.x +delta, bp.y + dist)))
					return ur;
				if(isFree( dl = new Point(bp.x -delta, bp.y - dist)))
					return dl;
				if(isFree( dr = new Point(bp.x +delta, bp.y - dist)))
					return dr;
			}
		}
		throw new RuntimeException("closestFree");
	}
	
	/** Return true if 'bp' is a legal space on the board, and is not occupied. **/
	private boolean
	isFree(Point bp) {
		return onBoard(bp) && square[bp.x][bp.y] == null;
	}
	
	/** Is 'bp' a legal board position (on the board or the rack?) **/
	private boolean
	onBoard(Point bp) {
		return bp.x >0 && bp.x <= NSQUARES && 
			bp.y >= 0 && bp.y <= NSQUARES;
	}
	
	private Point
	freeRackPos() {
		// Find a free spot in the rack area
		Point bp = new Point(1,0);
		for(int x=1; x<=NSQUARES; x++) {
			bp.x = x;
			if (isFree(bp))
				break;
		}
		return bp;
	}
	
	public static void test(String[] argv){
		DragArea d = new DragArea();
		TestFrame f = new TestFrame("test DragArea", d);

		d.addToRack(1);	// 'a'
		d.addToRack(2);	// 'b'
		d.addToRack(3);	// 'c'
		d.addToRack(4);	// 'd'
		d.addToRack(5);	// 'e'
		d.addToRack(6);	// 'f'
		d.addToBoard(new Tile(new Point(7,8), 2));
		d.addToBoard(new Tile(new Point(7,9), 1));
		d.addToBoard(new Tile(new Point(7,10), 4));
	}

	public static void main(String[] argv){
		test(argv);
	}	
}


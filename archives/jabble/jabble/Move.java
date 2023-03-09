import java.util.*;
import java.awt.Point;

/** A proposed or played move **/

// DragArea.java constructs a proposed Move,
// handles problems with a Move, displays
// someone else's move.

// Board.java (through Genmove.java)
// finds a Move, finds problems with a move,
// adds a move.

// Game.java needs to know if a Move involves
// moving any pieces back to the Sock, and/or
// is a pass.

class Move implements DawgConst {
	int	score;		// -1 indicates score not calculated
	Vector	tiles;	// Vector of Tiles

	public String
	toString(){
		String s =  "{" + score+":";
		Enumeration e = tiles.elements();
		while(e.hasMoreElements()){
			Tile t = (Tile)e.nextElement();
			s += t+", ";
		}
		return s+"}";
	}
	
	public void update(Vector tiles, int score) {
		this.tiles = tiles;
		this.score = score;
	}
	
	Move (Vector tiles, int score) {
		this.tiles = tiles;
		this.score = score;
	}
	
	/** Constructor, haven't determined score. **/
	Move(Vector tiles) {
		this(tiles, 0);
	}
	
	/** Blank move **/
	Move() {
		this(new Vector(0),0);
	}
	
	/** Return an enumeration of the tiles in this move **/
	public Enumeration getTiles() {
		return tiles.elements();
	}
	
	boolean isBingo() {
		return tiles.size() == 7;
	}
	
	void setScore(int score) {
		this.score = score;
	}
	
	/** todo **/
	boolean isHorizontal() {
		if (tiles.size() <2)
			return true;
		Point p1 = ((Tile)(tiles.elementAt(0))).getPoint();
		Point p2 = ((Tile)(tiles.elementAt(1))).getPoint();
		return p1.y == p2.y;
	}
	
	/** todo **/
	int getRow() {
		// todo
		int ntiles = tiles.size();
		if (ntiles == 0)
			return 0;
		else if (ntiles == 1) {
			Point p1 = ((Tile)(tiles.elementAt(0))).getPoint();
			return p1.y;
		} else {
			Point p1 = ((Tile)(tiles.elementAt(0))).getPoint();
			Point p2 = ((Tile)(tiles.elementAt(1))).getPoint();
			if(p1.y == p2.y)
				return p1.y;
			else
				return p1.x;
		}
	}
	
	/** If there's something obvious wrong with this move,
	 * return a problem, otherwise return null
	 **/
	Problem check() {
		Point first=null;
		int	ntiles;
		int	row;
		boolean	isH=false;
		
		if (tiles.size() == 0)
			return new Problem("no tiles placed");
		Enumeration e = tiles.elements();
		ntiles = 0;
		while(e.hasMoreElements()) {
			Tile t = (Tile)e.nextElement();
			Point p = t.getPoint();
			if(ntiles == 0){
				// first point
				first = p;
			} else if (ntiles == 1) {
				// second point
				if (p.x == first.x) {
					isH = false;
				} else if (p.y == first.y) {
					isH = true;
				} else {
					return new Problem("not in line", first, p);
				}
			} else {
				// subsequent points
				if(isH) {
					if (p.y != first.y) {
						return new Problem("not in line", first, p);
					}
				} else {
					if (p.x != first.x) {
						return new Problem("not in line", first, p);
					}
				}
			}
			ntiles++;
		}
		return null;
	}
}

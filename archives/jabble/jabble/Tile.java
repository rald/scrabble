import java.awt.*;

/** A Tile might have to draw itself.
 **/
class Tile {
	/** Board position.
	*	1,1 is top left.
	*	0,* means return to sock.
	**/
	Point	bp;
	int		letter;	// 1 is 'a', 1+32 is blank used as 'a'
	int		disposition;	// one of the below
	Color	bg;
	
	static Font font = new Font("helvetica", Font.PLAIN, 24);
	static FontMetrics metrics = null;
	
	static final short	RACKTILE = (short)0;
	static final short	PLACEDTILE = (short)1;
	static final short	OLDPLACEDTILE = (short)2;
	static final Color 	TEXTCOLOR = Color.black;
	
	//--------------------------------------
	// Constructors
	//--------------------------------------
	Tile(Point bp, int letter, short disposition) {
		this.bp = bp;
		this.letter = letter;
		this.disposition = disposition;
		setBG();
	}
	
	void setBG() {
		switch(disposition) {
		case RACKTILE: bg = Color.red; break;
		case PLACEDTILE: bg = Color.blue; break;
		case OLDPLACEDTILE: bg = Color.orange; break;
		default:	throw new RuntimeException("strange disposition"); 
		}
	}
	
	void commit() {
		switch(disposition) {
		case RACKTILE: disposition = OLDPLACEDTILE; break;
		case PLACEDTILE: disposition = OLDPLACEDTILE; break;
		case OLDPLACEDTILE: break;
		default:	throw new RuntimeException("strange disposition"); 
		}
		setBG();
	}
	
	Tile(Point bp, int letter) {
		this(bp, letter, PLACEDTILE);
	}
	
	Tile(int x, int y, int letter) {
		this(new Point(x,y),letter);
	}

	Tile(Tile t) {
		bp = t.bp;
		letter = t.letter;
		disposition = t.disposition;
		bg = t.bg;
	}
	
	//--------------------------------------

	public void paint(Graphics g, Point pp, int size) {
		// todo
		if(g==null)
			return;
			
		// background
		g.setColor(bg);
		g.fillRect(pp.x, pp.y, size, size);

		// text
		g.setFont(font);
		if (metrics == null)
			metrics = g.getFontMetrics();
		g.setColor(TEXTCOLOR);
		String s= Dawg.letterToString(letter);
		int dx = (size - metrics.stringWidth(s))/2;
		int dy = size - (size - metrics.getAscent())/2;
		g.drawString(s, pp.x +dx, pp.y+dy);
		
		// border
		g.drawRoundRect(pp.x, pp.y, size, size, 2, 2);
	}
	
	public boolean isMovable() {
		return disposition == RACKTILE;
	}
	
	Point getPoint(){
		return bp;
	}
	
	void setPoint(Point p) {
		bp = p;
	}
	int getLetter(){
		return letter;
	}
	
	public String toString() {
		return "("+ Dawg.letterToString(letter) +
			((bp.x >0)?
				(bp.x+","+bp.y):
				"to Sock") +
			")";
	}
	
}
